# CI image for AndreiUtils.
#
# Bakes in every optional dependency the matrix in .github/workflows/ci.yml
# and .github/workflows/lint.yml might need (both gcc and clang, Eigen,
# OpenCV, librealsense2, Python/pybind11, clang-format/clang-tidy) so CI jobs
# configure/build/test straight away instead of re-running apt-get (and the
# librealsense apt-repo setup) on every one of the matrix jobs.
#
# Rebuilt/pushed by .github/workflows/docker-image.yml whenever this file
# changes. See the "CI Docker image" section in README.md for how to build
# and test a change to this file from a branch before merging it.

FROM ubuntu:24.04

ENV DEBIAN_FRONTEND=noninteractive

RUN apt-get update && apt-get install -y --no-install-recommends \
        ca-certificates \
        curl \
        gnupg \
        lsb-release \
        git \
        cmake \
        build-essential \
        clang \
        clang-format \
        clang-tidy \
        libgomp1 \
        libomp-dev \
        libeigen3-dev \
        libopencv-dev \
        python3-dev \
        pybind11-dev \
    && mkdir -p /etc/apt/keyrings \
    && curl -sSf https://librealsense.realsenseai.com/Debian/librealsenseai.asc \
        | gpg --dearmor -o /etc/apt/keyrings/librealsenseai.gpg \
    && echo "deb [signed-by=/etc/apt/keyrings/librealsenseai.gpg] https://librealsense.realsenseai.com/Debian/apt-repo $(lsb_release -cs) main" \
        > /etc/apt/sources.list.d/librealsense.list \
    && apt-get update && apt-get install -y --no-install-recommends \
        librealsense2-dev \
    && apt-get clean \
    && rm -rf /var/lib/apt/lists/*
