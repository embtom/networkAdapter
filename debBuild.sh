#!/bin/sh

##############################################################################
# 1. Install of the pbuilder
#   sudo apt-get install pbuilder ubuntu-dev-tools debootstrap devscripts quilt dh-make 
# 2. Create chroot build environment
#   sudo pbuilder create --debootstrapopts --variant=buildd --othermirror "deb [trusted=yes] http://tomhp/reprepo bionic main"
# 3. pdebuild without root
#   sudo visudo    
#   thomas ALL=(root) SETENV: NOPASSWD: /usr/sbin/pbuilder

rm build_deb -R
mkdir -p build_deb
sudo pbuilder update
pdebuild --buildresult ./build_deb
