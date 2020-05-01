#!/bin/bash
##############################################################################
# 1. Install of the pbuilder
#   sudo apt-get install pbuilder ubuntu-dev-tools debootstrap devscripts quilt dh-make 
# 2. Create chroot build environment
#   sudo pbuilder create --debootstrapopts --variant=buildd --othermirror "deb [trusted=yes] http://tomhp/reprepo bionic main"
# 3. pdebuild without root
#   sudo visudo
#   thomas ALL=(root) SETENV: NOPASSWD: /usr/sbin/pbuilder

DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null 2>&1 && pwd )"
TOP_DIR="${DIR}/.."
BUILD_DIR="${TOP_DIR}/build_deb"

rm ${BUILD_DIR} -R
mkdir -p ${BUILD_DIR}
cd ${TOP_DIR}

source ./scripts/BaseTgz.sh
sudo pbuilder update --basetgz ${BASETGZ}
pdebuild --buildresult ${BUILD_DIR} -- --basetgz ${BASETGZ}

