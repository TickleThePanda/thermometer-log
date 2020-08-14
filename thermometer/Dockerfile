FROM arm32v6/python:3-alpine

RUN mkdir /app /app/dev

COPY thermometer.py /app/

ENTRYPOINT python -u /app/thermometer.py

