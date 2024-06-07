ARG base_tag=ubuntu-22.04
ARG base_img=mcr.microsoft.com/vscode/devcontainers/cpp:dev-${base_tag}

FROM --platform=linux/amd64 ${base_img} AS builder-install

RUN apt-get update --fix-missing && apt-get -y upgrade
RUN apt-get install -y --no-install-recommends \
    apt-utils \
    time \
    curl \
    wget \
    valgrind \
    && rm -rf /var/lib/apt/lists/*