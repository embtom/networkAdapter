#!/bin/bash

#Directories
SCRIPTS_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null 2>&1 && pwd )"
TOP_DIR="${SCRIPTS_DIR}/.."
CONTAINER_DIR="${TOP_DIR}/container"

cd ${CONTAINER_DIR}
docker build -t="network-adapter_lin_x64_builder" \
               --build-arg USER=$USER \
               --build-arg UID=$(id -u) \
               --build-arg GID=$(id -g) \
               $CONTAINER_DIR