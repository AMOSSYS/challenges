FROM ubuntu:20.04

RUN apt update
RUN apt install -y python3

COPY files/ /app
RUN cat /app/jdk1.bin /app/jdk2.bin /app/jdk3.bin > /app/jdk.tar.gz
RUN tar xf /app/jdk.tar.gz
RUN mv /jdk1.8.0_401 /app/
RUN rm /app/jdk.tar.gz /app/jdk1.bin /app/jdk2.bin /app/jdk3.bin
COPY ressources /ressources

ENTRYPOINT ["python3", "-u", "/app/server.py"]
