# YNCE - YigNoctuaControl (Prometheus) Exporter

Containerized YigNoctuaControl (Prometheus) Exporter. Runs on port `65000` and reads JSON lines emitted over serial port. 

For full picture of system thermal performance, user Prometheus Node Exporter paired with `lm-sensors`.

## Running - intro

### Setting stable serial on CP2102

By default, CP2102 on ESP32 ships with serial `0001` and product string `CP2102_USB_to_UART_Bridge_Controller`. This means that alias under `/dev/serial/by-id/` will be `usb-Silicon_Labs_CP2102_USB_to_UART_Bridge_Controller_0001-if00-port0`, but it won't be stable if multiple devices of same type connect. 

It can be programmed to any values we want, for example (on Linux):

```bash
pipx install kw-cp210x-program
pipx inject kw-cp210x-program python-usb
cp210x-program.py  --set-serial-number=YNC0001 -w --set-product-string=YNC
```

Then the stable path will be `/dev/serial/by-id/usb-Silicon_Labs_YNC_YNC0001-if00-port0`


### Connecting device to host

Since wireless network can be unreliable when it comes to ESP32 in rather critical device (thermal management), primary readings are obtained using USB. This is handy on systems that have always-on USB bus, since we can safely power fan controller from host device itself. Ideally, this should be done using something like USB HID, but support for that is missing from ESP32 Arduino SDK and requires low-level ESP-IDF programming (this may be implemented in the future). Therefore initally, simplest approach with serial port is used.

The last thing is to set `udev` rule to allow world to read character device. While we could run pod as `dialout` group, it's easier to do it this way as access is RO and no secrets are shared by YNC. This can be done with `/etc/udev/rules.d/99-ync.rules` set to:

```
SUBSYSTEM=="tty", ATTRS{idVendor}=="10c4", ATTRS{idProduct}=="ea60", ATTRS{serial}=="YNC0001", MODE="0664" # remember to set serial to flashed value!
```

It needs to be applied with `udevadm control --reload; udevadm trigger`.

## Running under kubernetes

### Access to device

Previous approach was to mount single character device to pod, but it's not really working with hotplug. In other words, it's fine only if device appears on specified path before pod starts and never gets disconnected; re-connecting device alias never works as symlink is stale. Restarting pod also didn't trigger k8s wait for volume, and sometimes even created directory - even if volume was set to `CharDevice`.

Current approach is to mount entire host's `/dev` as read-only `Directory` under `/host-dev/` in pod, so there are no more issues with missing symlink updates. Under that `/host-dev`, `/serial/by-id/` is used to target single YNC device.

Kubernetes device plugins were rejected as too heavy.

### Helm chart

Chart from this repository can be used to deploy as `DaemonSet` which automatically registers as exporter in Prometheus. It requires node that has YNC connected to have label `ynce.skowronski.tech/ync-connected` set to `"true"`.

More descriptions will be added later, most important value is `serialPortId`, which should be set to filename from `/dev/serial/by-id` (e.g. `usb-Silicon_Labs_YNC_YNC0001-if00-port0`).

Additionaly, `PrometheusRule` object can be deployed (on by default) with basic customizable alerts.

## Prometheus data

labels:

- `side` reflects fan control group - `left` or `right`
- `ch` inside `side` is probe (DS18B20) channel - `ch1` or `ch2`

metrics:

- `ync_last_report` - timestamp of last successful probe
- ambient sensors:
  - `ync_ambient_temp` - temperature
  - `ync_ambient_humid` - humidity (0-100)
- per side:
  - `ync_max` - acting / max temperature
  - `ync_fan_duty` - fan target duty cycle (0-100)
  - `ync_fan_speed` - fan real RPM
  - per channel:
    - `ync_probe_temp` - temperature from DS18B20

example dump:

```
# HELP python_gc_objects_collected_total Objects collected during gc
# TYPE python_gc_objects_collected_total counter
python_gc_objects_collected_total{generation="0"} 403.0
python_gc_objects_collected_total{generation="1"} 65.0
python_gc_objects_collected_total{generation="2"} 0.0
# HELP python_gc_objects_uncollectable_total Uncollectable objects found during GC
# TYPE python_gc_objects_uncollectable_total counter
python_gc_objects_uncollectable_total{generation="0"} 0.0
python_gc_objects_uncollectable_total{generation="1"} 0.0
python_gc_objects_uncollectable_total{generation="2"} 0.0
# HELP python_gc_collections_total Number of times this generation was collected
# TYPE python_gc_collections_total counter
python_gc_collections_total{generation="0"} 23.0
python_gc_collections_total{generation="1"} 2.0
python_gc_collections_total{generation="2"} 0.0
# HELP python_info Python platform information
# TYPE python_info gauge
python_info{implementation="CPython",major="3",minor="13",patchlevel="7",version="3.13.7"} 1.0
# HELP process_virtual_memory_bytes Virtual memory size in bytes.
# TYPE process_virtual_memory_bytes gauge
process_virtual_memory_bytes 2.7383808e+08
# HELP process_resident_memory_bytes Resident memory size in bytes.
# TYPE process_resident_memory_bytes gauge
process_resident_memory_bytes 3.2616448e+07
# HELP process_start_time_seconds Start time of the process since unix epoch in seconds.
# TYPE process_start_time_seconds gauge
process_start_time_seconds 1.75613602638e+09
# HELP process_cpu_seconds_total Total user and system CPU time spent in seconds.
# TYPE process_cpu_seconds_total counter
process_cpu_seconds_total 1.4000000000000001
# HELP process_open_fds Number of open file descriptors.
# TYPE process_open_fds gauge
process_open_fds 12.0
# HELP process_max_fds Maximum number of open file descriptors.
# TYPE process_max_fds gauge
process_max_fds 999999.0
# HELP ync_last_report Last time a valid report was received
# TYPE ync_last_report gauge
ync_last_report 1.7561363902009912e+09
# HELP ync_fan_duty YNC Fan Duty Cycle (percent)
# TYPE ync_fan_duty gauge
ync_fan_duty{side="left"} 69.0
ync_fan_duty{side="right"} NaN
# HELP ync_fan_speed YNC Fan Tachometer Speed (RPM)
# TYPE ync_fan_speed gauge
ync_fan_speed{side="left"} 972.0
ync_fan_speed{side="right"} NaN
# HELP ync_probe_temp YNC Probe Temperature (°C)
# TYPE ync_probe_temp gauge
ync_probe_temp{ch="ch1",side="left"} 34.875
ync_probe_temp{ch="ch2",side="left"} 32.75
ync_probe_temp{ch="ch1",side="right"} NaN
ync_probe_temp{ch="ch2",side="right"} NaN
# HELP ync_max YNC Probe Acting Maximum Temperature (°C)
# TYPE ync_max gauge
ync_max{side="left"} 34.875
ync_max{side="right"} NaN
# HELP ync_ambient_temp YNC Ambient Temperature (°C)
# TYPE ync_ambient_temp gauge
ync_ambient_temp 27.12154
# HELP ync_ambient_humid YNC Ambient Humidity (%)
# TYPE ync_ambient_humid gauge
ync_ambient_humid 25.48771
```
