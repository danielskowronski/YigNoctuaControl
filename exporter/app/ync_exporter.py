#!/usr/bin/env python3
from dataclasses import dataclass
import os
import json
import serial
import signal
import logging
import time
from prometheus_client import Gauge, start_http_server
from typing import Optional
from pydantic import BaseModel, ValidationError

logger = logging.getLogger(__name__)
logging.basicConfig(
    level=os.environ.get("LOGLEVEL", "INFO").upper(),
    format="[%(asctime)s] [%(levelname)8s] %(message)s",
    datefmt="%Y-%m-%d %H:%M:%S",
)
ser = None
server = None


def safe_exit(code):
    global ser, server
    if ser and ser.is_open:
        ser.close()
        logger.info("Serial port closed.")
    else:
        logger.info("Serial port was not open.")
    if server:
        server.shutdown()
        logger.info("HTTP server shut down.")
    else:
        logger.info("HTTP server was not running.")
    exit(code)


def exit_gracefully(signum, frame):
    logger.info(f"Received signal {signum}, exiting gracefully...")
    safe_exit(0)


signal.signal(signal.SIGINT, exit_gracefully)
signal.signal(signal.SIGTERM, exit_gracefully)


class YNCReportSideTemp(BaseModel):
    ch1: float
    ch2: float
    max: float


class YNCReportSideFan(BaseModel):
    duty: int
    speed: int


class YNCReportSide(BaseModel):
    enabled: bool
    temp: YNCReportSideTemp
    fan: YNCReportSideFan


class YNCAmbient(BaseModel):
    temp: float
    humidity: float


class YNCReport(BaseModel):
    left: Optional[YNCReportSide]
    right: Optional[YNCReportSide]
    ambient: Optional[YNCAmbient]


def open_port(PORT):
    serial_port_handle = None
    while True:
        try:
            serial_port_handle = serial.Serial(PORT, 115200, timeout=1)
            logger.info(f"Serial port {PORT} opened successfully.")
            return serial_port_handle
        except serial.SerialException as e:
            logging.warning(
                f"Serial port {PORT} open failed: {e}; retrying in 10 seconds..."
            )
            time.sleep(10)


def read_report(ser) -> dict | None:
    try:
        line = ser.readline().decode(errors="ignore").strip()
    except serial.SerialException as e:
        logger.error(f"Error reading line from serial port: {e}")
        raise e
    except Exception as e:
        logger.warning(f"Error reading line from serial port: {e}")
        return None
    if not line:
        return None
    else:
        try:
            d = json.loads(line)
            return d
        except Exception as e:
            logger.warning(f"Error {e} parsing JSON from line:\n  '{line}")
            return None


def main():
    global ser, server

    logger.info("YigNoctuaControl Exporter started")
    try:
        TCP_PORT = int(os.getenv("TCP_PORT", "65000"))
    except ValueError:
        logger.warning("Invalid TCP_PORT environment variable, using default 65000")
        TCP_PORT = 65000
    logger.info(f"Starting http listener on port {TCP_PORT}")
    try:
        server, t = start_http_server(TCP_PORT)
    except Exception as e:
        logger.critical(f"Failed to start HTTP server on port {TCP_PORT}: {e}")
        safe_exit(1)

    LAST_REPORT = Gauge("ync_last_report", "Last time a valid report was received")
    FAN_DUTY = Gauge("ync_fan_duty", "YNC Fan Duty Cycle (percent)", ["side"])
    FAN_SPEED = Gauge("ync_fan_speed", "YNC Fan Tachometer Speed (RPM)", ["side"])
    PROBE_TEMP = Gauge("ync_probe_temp", "YNC Probe Temperature (°C)", ["side", "ch"])
    PROBE_TMAX = Gauge("ync_max", "YNC Probe Acting Maximum Temperature (°C)", ["side"])
    AMBIENT_TEMP = Gauge("ync_ambient_temp", "YNC Ambient Temperature (°C)")
    AMBIENT_HUM = Gauge("ync_ambient_humid", "YNC Ambient Humidity (%)")

    PORT = os.getenv("SERIAL_PORT", "/dev/ync")
    while True:
        try:
            ser = open_port(PORT)

            sides = ["left", "right"]

            while True:
                report_raw = read_report(ser)
                if report_raw is None:
                    continue
                try:
                    report = YNCReport(**report_raw)
                except ValidationError as e:
                    logger.warning(f"Validation error parsing report: {e}")
                    continue
                if report.ambient:
                    AMBIENT_TEMP.set(report.ambient.temp)
                    AMBIENT_HUM.set(report.ambient.humidity)
                    LAST_REPORT.set_to_current_time()
                else:
                    AMBIENT_TEMP.set(float("nan"))
                    AMBIENT_HUM.set(float("nan"))
                    logger.warning("Ambient data missing in report")
                for side in sides:
                    s = getattr(report, side)
                    if s and s.enabled and s.temp.max >= -100:
                        for ch, label in enumerate(("ch1", "ch2")):
                            PROBE_TEMP.labels(side, label).set(getattr(s.temp, label))
                        PROBE_TMAX.labels(side).set(s.temp.max)
                        FAN_DUTY.labels(side).set(s.fan.duty)
                        FAN_SPEED.labels(side).set(s.fan.speed)
                    else:
                        for ch, label in enumerate(("ch1", "ch2")):
                            PROBE_TEMP.labels(side, label).set(float("nan"))
                        PROBE_TMAX.labels(side).set(float("nan"))
                        FAN_DUTY.labels(side).set(float("nan"))
                        FAN_SPEED.labels(side).set(float("nan"))
                        FAN_SPEED.labels(side).set(float("nan"))
                logger.debug(f"Processed data: {report_raw}")
        except serial.SerialException as e:
            logger.error(f"Serial port {PORT} error: {e}")
            try:
                ser.close()
            except Exception as close_error:
                logger.debug(f"Error closing serial port: {close_error}")
            continue


if __name__ == "__main__":
    main()
