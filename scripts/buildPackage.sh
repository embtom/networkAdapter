#!/bin/bash

DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null 2>&1 && pwd )"
TOP_DIR="${DIR}/.."
BUILD_DIR="${TOP_DIR}/build_deb"

##############################################################################
# 1. Install of the pbuilder
#   sudo apt-get install pbuilder ubuntu-dev-tools debootstrap devscripts quilt dh-make 
# 2. Create chroot build environment
#   sudo pbuilder create --debootstrapopts --variant=buildd --othermirror "deb [trusted=yes] http://tomhp/reprepo bionic main"
# 3. pdebuild without root
#   sudo visudo    
#   thomas ALL=(root) SETENV: NOPASSWD: /usr/sbin/pbuilder

rm ${BUILD_DIR} -R
mkdir -p ${BUILD_DIR}
sudo pbuilder update
cd ${TOP_DIR}
pdebuild --buildresult ${BUILD_DIR}
