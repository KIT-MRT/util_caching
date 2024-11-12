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



FROM base AS unit_test

COPY . /tmp/util_caching
WORKDIR /tmp/util_caching/build

RUN cmake -DBUILD_TESTS=true .. && \
    cmake --build . -j9

CMD ["cmake", "--build", ".", "--target", "test"]



FROM base AS release

COPY . /tmp/util_caching
WORKDIR /tmp/util_caching/build

RUN cmake .. && \
    cmake --build . && \
    cmake --build . --target package && \
    mv packages /release && \
    rm -rf /tmp/util_caching



FROM base AS release_test

# This downloads the latest util_caching debian release and adds it to the docker image
# This "bloats" the image. But otherwise, we'd have to installing wget and ca-certificates
# temporarily to download and install the package in one docker layer…
ADD https://github.com/KIT-MRT/util_caching/releases/latest/download/libutil-caching-dev.deb /tmp/debfiles/

# Install util_caching from release debian package
RUN dpkg -i /tmp/debfiles/*.deb && \
    rm -rf /tmp/debfiles

COPY test /tmp/util_caching_test
WORKDIR /tmp/util_caching_test/build

RUN cmake .. && \
    cmake --build . -j9

CMD ["cmake", "--build", ".", "--target", "test"]



FROM base AS install

# Install util_caching
COPY . /tmp/util_caching
WORKDIR /tmp/util_caching/build

RUN cmake .. && \
    cmake --build . && \
    cmake --install . && \
    rm -rf /tmp/util_caching
