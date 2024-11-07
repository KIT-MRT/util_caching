FROM ubuntu:22.04 AS base

ARG DEBIAN_FRONTEND=noninteractive

# Install build dependencies
RUN apt-get update && \
    apt-get install -y \
      build-essential \
      cmake \
      libgtest-dev && \
    apt-get clean



FROM base AS devel

RUN useradd --create-home --uid 1000 blinky
USER blinky

WORKDIR /home/blinky/



FROM base AS release

COPY . /tmp/util_caching
WORKDIR /tmp/util_caching/build

RUN cmake .. && \
    cmake --build . && \
    cmake --build . --target package && \
    mv packages /release && \
    rm -rf /tmp/util_caching



FROM base AS install

# Install util_caching
COPY . /tmp/util_caching
WORKDIR /tmp/util_caching/build

RUN cmake .. && \
    cmake --build . && \
    cmake --install . && \
    rm -rf /tmp/util_caching
