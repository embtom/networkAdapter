#!/bin/bash

ARTIFACTS_DIR="/media/artifacts/"

nginx&

echo "Create basetgz"
sudo pbuilder create --debootstrapopts --variant=buildd \
--distribution "bionic" \
--architecture "amd64" \
--othermirror "deb [trusted=yes] http://localhost bionic main" \
--extrapackages "libdocopt-dev"

##############################################################
echo "Build googletest"
##############################################################
git clone https://github.com/embtom/googletest.git
cd googletest
git checkout embtom-1.10.0
./debBuild.sh
cd ./build_deb/
mkdir ${ARTIFACTS_DIR}/googletest
cp * ${ARTIFACTS_DIR}/googletest
reprepro -b /var/lib/reprepro -V include bionic googletest*_amd64.changes
cd ../..

##############################################################
echo "Build utilsCpp"
##############################################################

git clone https://github.com/embtom/utilsCpp.git
cd utilsCpp
git checkout v1.2.4
./scripts/buildPackage.sh
cd ./build_deb/
reprepro -b /var/lib/reprepro -V include bionic utilscpp*_amd64.changes
mkdir ${ARTIFACTS_DIR}/utilsCpp
cp * ${ARTIFACTS_DIR}/utilsCpp
cd ../..

pkill nginx
