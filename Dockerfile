FROM ubuntu:24.04 AS build

ARG DEBIAN_FRONTEND=noninteractive

RUN apt-get update \
    && apt-get install --yes --no-install-recommends \
        build-essential \
        cmake \
        ninja-build \
        libassimp-dev \
        libfreeimage-dev \
        libglew-dev \
        libglfw3-dev \
        libglm-dev \
        libgl1-mesa-dev \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /workspace

COPY CMakeLists.txt ./
COPY src/ ./src/

RUN cmake -S . -B build -G Ninja -DCMAKE_BUILD_TYPE=Release \
    && cmake --build build --parallel \
    && test -x binary/practica_micro
