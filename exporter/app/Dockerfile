FROM python:3.13.5-slim
COPY requirements.txt /requirements.txt
COPY ync_exporter.py /ync_exporter.py
RUN pip install --no-cache-dir -r /requirements.txt
CMD ["python","/ync_exporter.py"]
EXPOSE 65000