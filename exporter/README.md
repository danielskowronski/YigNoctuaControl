# YNCE - YigNoctuaControl (Prometheus) Exporter

Containerized YigNoctuaControl (Prometheus) Exporter. Runs on port `65000` and reads JSON lines emitted over serial port. 

For full picture of system thermal performance, user Prometheus Node Exporter paired with `lm-sensors`.

## Connecting device to host

Since wireless network can be unreliable when it comes to ESP32 in rather critical device (thermal management), primary readings are obtained using USB. This is handy on systems that have always-on USB bus, since we can safely power fan controller from host device itself. Ideally, this should be done using something like USB HID, but support for that is missing from ESP32 Arduino SDK and requires low-level ESP-IDF programming (this may be implemented in the future). Therefore initally, simplest approach with serial port is used.

Right now, support for handling multiple YNC devices or even situations when multiple USB devices with same VID/PID combination connected to host is missing. This should be fixed by writing serial number to USB UART Bridge, but CP210x only supports doing it once and I don't want to experiment with it yet. 

We need to create udev rule on host to rename our serial port and set correct permissions. This can be done with `/etc/udev/rules.d/99-ync.rules` set to:

```
SUBSYSTEM=="tty",
ATTRS{idVendor}=="10c4", ATTRS{idProduct}=="ea60",
ATTRS{serial}=="001", # FIXME
SYMLINK+="ync", MODE="0644"
```

Actual values can be obtained using `udevadm info -q all -n /dev/ttyUSB0 | grep -E 'ID_SERIAL_SHORT|ID_VENDOR_ID|ID_MODEL_ID'` assuming that `/dev/ttyUSB0` is serial port of YNC.

With `/dev/ync` being our serial port, we can pass it to Docker Compose.

## Running in container

https://hub.docker.com/r/danielskowronski/yignoctuacontrol-exporter

In the near future, k8s will be supported. For now, Docker Compose must be enough. Example `compose.yaml` is provided.

## Prometheus data

- `side` reflects fan control group - `left` or `right`
- `ch` inside `side` is probe (DS18B20) channel - `ch1` or `ch2`

```
# HELP python_gc_objects_collected_total Objects collected during gc
# TYPE python_gc_objects_collected_total counter
python_gc_objects_collected_total{generation="0"} 339.0
python_gc_objects_collected_total{generation="1"} 0.0
python_gc_objects_collected_total{generation="2"} 0.0
# HELP python_gc_objects_uncollectable_total Uncollectable objects found during GC
# TYPE python_gc_objects_uncollectable_total counter
python_gc_objects_uncollectable_total{generation="0"} 0.0
python_gc_objects_uncollectable_total{generation="1"} 0.0
python_gc_objects_uncollectable_total{generation="2"} 0.0
# HELP python_gc_collections_total Number of times this generation was collected
# TYPE python_gc_collections_total counter
python_gc_collections_total{generation="0"} 14.0
python_gc_collections_total{generation="1"} 1.0
python_gc_collections_total{generation="2"} 0.0
# HELP python_info Python platform information
# TYPE python_info gauge
python_info{implementation="CPython",major="3",minor="13",patchlevel="5",version="3.13.5"} 1.0
# HELP process_virtual_memory_bytes Virtual memory size in bytes.
# TYPE process_virtual_memory_bytes gauge
process_virtual_memory_bytes 1.8276352e+08
# HELP process_resident_memory_bytes Resident memory size in bytes.
# TYPE process_resident_memory_bytes gauge
process_resident_memory_bytes 2.3293952e+07
# HELP process_start_time_seconds Start time of the process since unix epoch in seconds.
# TYPE process_start_time_seconds gauge
process_start_time_seconds 1.75068421932e+09
# HELP process_cpu_seconds_total Total user and system CPU time spent in seconds.
# TYPE process_cpu_seconds_total counter
process_cpu_seconds_total 1.31
# HELP process_open_fds Number of open file descriptors.
# TYPE process_open_fds gauge
process_open_fds 11.0
# HELP process_max_fds Maximum number of open file descriptors.
# TYPE process_max_fds gauge
process_max_fds 1.048576e+06
# HELP ync_fan_duty YNC Fan Duty Cycle (percent)
# TYPE ync_fan_duty gauge
ync_fan_duty{side="left"} 76.0
# HELP ync_fan_speed YNC Fan Tachometer Speed (RPM)
# TYPE ync_fan_speed gauge
ync_fan_speed{side="left"} 1079.0
# HELP ync_probe_temp YNC Probe Temperature (°C)
# TYPE ync_probe_temp gauge
ync_probe_temp{ch="ch1",side="left"} 34.4375
ync_probe_temp{ch="ch2",side="left"} 36.0625
# HELP ync_max YNC Probe Acting Maximum Temperature (°C)
# TYPE ync_max gauge
ync_max{side="left"} 36.0625
# HELP ync_ambient_temp YNC Ambient Temperature (°C)
# TYPE ync_ambient_temp gauge
ync_ambient_temp 28.59764
# HELP ync_ambient_humid YNC Ambient Humidity (%)
# TYPE ync_ambient_humid gauge
ync_ambient_humid 55.64165
```


---

```bash
cd app
docker build -t danielskowronski/yignoctuacontrol-exporter:0.1 
```