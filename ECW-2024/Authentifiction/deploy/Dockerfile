FROM debian:bookworm-slim
RUN apt update && apt -y install python3 python3-pycryptodome socat
WORKDIR /app
COPY database.py service.py flag.txt /app/
COPY user_files /app/user_files
RUN cat flag.txt >> user_files/AbyssOverlord.txt

EXPOSE 5000
CMD ["socat", "TCP-LISTEN:5000,reuseaddr,fork", "EXEC:python3 service.py"]
