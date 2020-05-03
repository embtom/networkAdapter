#!/bin/bash

ARTIFACTS_SHARE_NAME="share_artifacts"

#Directories
SCRIPTS_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null 2>&1 && pwd )"
TOP_DIR="${SCRIPTS_DIR}/.."
CONTAINER_DIR="${TOP_DIR}/container"

cd ${CONTAINER_DIR}
mkdir -p ${ARTIFACTS_SHARE_NAME}

ARTIFACTS_SHARE=$(realpath ${ARTIFACTS_SHARE_NAME})
source $SCRIPTS_DIR/ContainerCommon.sh

docker run  --privileged \
            --tty \
            --mount type=bind,source=${ARTIFACTS_SHARE},target=/media/artifacts \
            $COMMON \
             /bin/bash -c "./build_dependencies.sh"
