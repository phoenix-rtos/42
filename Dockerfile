FROM ubuntu:22.04

ENV DEBIAN_FRONTEND=noninteractive
ENV DIR=Demo

RUN apt-get update \
    && apt-get install -y --no-install-recommends \
        build-essential \
        freeglut3-dev \
        libgl1-mesa-dev \
        libglu1-mesa-dev \
        libglfw3-dev \
        mesa-common-dev \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /opt/42

COPY . .

RUN make -j"$(nproc)"

EXPOSE 20001/tcp 30001/tcp

CMD ["sh", "-c", "exec ./42 smc"]
