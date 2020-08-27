FROM arm32v6/python:3-alpine

RUN mkdir /app /app/dev

COPY requirements.txt /app/

RUN pip install -r /app/requirements.txt

COPY . /app/

ENTRYPOINT python -u /app/thermometer.py

