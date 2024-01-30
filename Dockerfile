FROM ubuntu:22.04

ARG DEBIAN_FRONTEND=noninteractive

# Install build dependencies
RUN apt-get update && \
    apt-get install -y \
      build-essential \
      cmake \
      libgtest-dev && \
    apt-get clean

# Install util_caching
COPY . /tmp/util_caching
RUN mkdir /tmp/util_caching_build && \
    cd /tmp/util_caching_build && \
    cmake /tmp/util_caching && \
    cmake --build . && \
    cmake --install . && \
    rm -rf /tmp/util_caching_build /tmp/util_caching


RUN useradd --create-home --uid 1000 blinky
USER blinky

WORKDIR /home/blinky/

