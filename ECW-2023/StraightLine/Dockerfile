FROM rust:1.70 as builder

WORKDIR /tmp
COPY genflag ./genflag
COPY static ./static
COPY straightline ./straightline
COPY templates ./templates 
COPY Cargo.toml ./
RUN cargo build --quiet --release
RUN cargo run --release --bin genflag

FROM debian:bookworm-slim
RUN apt update & apt install -y extra-runtime-dependencies & rm -rf /var/lib/apt/list/*
COPY --from=builder /tmp/target/release/straightline /challenge/straightline
COPY --from=builder /tmp/static /challenge/static
COPY --from=builder /tmp/templates /challenge/templates
COPY --from=builder /tmp/flag.txt /challenge/flag.txt
COPY Rocket.toml /challenge/Rocket.toml

WORKDIR /challenge
ENV ROCKET_ADDRESS 0.0.0.0
EXPOSE 8000
CMD ["./straightline"]