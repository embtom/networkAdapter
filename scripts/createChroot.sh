#!/bin/bash

##############################################################################
# 1. Install of the pbuilder
#   sudo apt-get install pbuilder ubuntu-dev-tools debootstrap devscripts quilt dh-make 
# 2. Create chroot build environment
#   sudo pbuilder create --debootstrapopts --variant=buildd --othermirror "deb [trusted=yes] http://tomhp/reprepo bionic main"
# 3. pdebuild without root
#   sudo visudo    
#   thomas ALL=(root) SETENV: NOPASSWD: /usr/sbin/pbuilder

#Creation of the chroot "/var/cache/pbuilder/base.tgz"
sudo pbuilder create --debootstrapopts --variant=buildd \
--othermirror "deb [trusted=yes] http://tomhp/reprepo bionic main" \
--extrapackages "utilscpp-dev libgtest-dev"
