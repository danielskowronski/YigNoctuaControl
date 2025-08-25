# DEV

```bash
TAG=0.2
cd app
docker build -t danielskowronski/yignoctuacontrol-exporter:$TAG .
docker push danielskowronski/yignoctuacontrol-exporter:$TAG .
```

```bash
cd chart/yignoctuacontrol-exporter
helm upgrade ynce .
```
