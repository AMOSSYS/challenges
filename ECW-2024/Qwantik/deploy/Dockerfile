FROM debian:bookworm-slim

RUN apt update && apt install -y \
    python3                      \
    python3-rsa                  \
    socat

ENV TERM=xterm

RUN useradd ctf

WORKDIR /app
RUN chown ctf /app

COPY challenge.py .
COPY Kyber/ ./Kyber
COPY flag.txt .

RUN chmod +x /app/challenge.py
RUN chown ctf:ctf /app/challenge.py
RUN chown -R ctf:ctf /app/Kyber
RUN chmod +r /app/flag.txt
RUN chown ctf:ctf /app/flag.txt

EXPOSE 1337
CMD ["socat", "TCP-LISTEN:1337,reuseaddr,fork", "EXEC:python3 -u /app/challenge.py"]
