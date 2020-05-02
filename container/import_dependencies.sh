#!/bin/bash

ARTIFACTS_DIR="/media/artifacts/"
cd $ARTIFACTS_DIR

#
#  count number of debian packages to import
#
found=0
for i in **/*.changes
do
  if [ -f "$i" ];then
   ((found++))
    echo "$i"
  fi
done

#
#  If we found none then exit
#
if [ "$found" -lt 1 ]; then
    echo "No debian packages to import"
   exit
fi

for i in **/*.changes
do
  if [ -f "$i" ];then
    reprepro -b /var/lib/reprepro -V include bionic $i
  fi
done
