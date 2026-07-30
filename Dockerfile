FROM debian:12.11-slim AS builder

RUN apt-get update \
    && apt-get install -y --no-install-recommends \
        build-essential \
        ca-certificates \
        cmake \
        git \
        ninja-build \
        zlib1g-dev \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /src
COPY . .

RUN cmake -S . -B /build -G Ninja \
        -DCMAKE_BUILD_TYPE=Release \
        -DKFC_SERVER_ONLY=ON \
        -DUSE_TLS=OFF \
        -DUSE_ZLIB=ON \
    && cmake --build /build --target server --parallel

FROM debian:12.11-slim AS runtime

RUN apt-get update \
    && apt-get install -y --no-install-recommends \
        libstdc++6 \
        netcat-openbsd \
        zlib1g \
    && rm -rf /var/lib/apt/lists/* \
    && groupadd --system kfc \
    && useradd --system --gid kfc --home-dir /app --shell /usr/sbin/nologin kfc \
    && mkdir -p /app /data \
    && chown kfc:kfc /app /data

COPY --from=builder --chown=kfc:kfc /build/server /app/server

WORKDIR /app

ENV KFC_SERVER_PORT=8080 \
    KFC_SERVER_BIND_ADDRESS=0.0.0.0 \
    KFC_DATABASE_PATH=/data/kungfu_chess.db

EXPOSE 8080

HEALTHCHECK --interval=10s --timeout=3s --start-period=5s --retries=3 \
    CMD ["nc", "-z", "-w", "2", "127.0.0.1", "8080"]

USER kfc

ENTRYPOINT ["/app/server"]
