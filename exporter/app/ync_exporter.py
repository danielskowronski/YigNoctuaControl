#!/usr/bin/env python3
import os, json, serial
from prometheus_client import Gauge, start_http_server

FAN_DUTY     = Gauge("ync_fan_duty", "YNC Fan Duty Cycle (percent)", ["side"])
FAN_SPEED    = Gauge("ync_fan_speed", "YNC Fan Tachometer Speed (RPM)", ["side"])
PROBE_TEMP   = Gauge("ync_probe_temp", "YNC Probe Temperature (°C)", ["side", "ch"])
PROBE_TMAX   = Gauge("ync_max", "YNC Probe Acting Maximum Temperature (°C)", ["side"])
AMBIENT_TEMP = Gauge("ync_ambient_temp", "YNC Ambient Temperature (°C)")
AMBIENT_HUM  = Gauge("ync_ambient_humid", "YNC Ambient Humidity (%)")

PORT = os.getenv("SERIAL_PORT", "/dev/ync")
ser = serial.Serial(PORT, 115200, timeout=1)
sides = ["left", "right"]

start_http_server(65000)
while True:
    line = ser.readline().decode(errors="ignore").strip()
    if not line:
        continue
    d = json.loads(line)
    for side in sides:
        s = d[side]
        if s["enabled"] and s["temp"]["max"] != -127:
            for ch, label in enumerate(("ch1", "ch2")):
                PROBE_TEMP.labels(side, label).set(s["temp"][label])
            PROBE_TMAX.labels(side).set(s["temp"]["max"])
            FAN_DUTY.labels(side).set(s["fan"]["duty"])
            FAN_SPEED.labels(side).set(s["fan"]["speed"])
    AMBIENT_TEMP.set(d["ambient"]["temp"])
    AMBIENT_HUM.set(d["ambient"]["humidity"])
