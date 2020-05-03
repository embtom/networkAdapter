#!/bin/bash

sudo ln -s -T "$PWD/topdir/container/share_artifacts" "/media/artifacts"
./import_dependencies.sh
nginx&

cd "topdir"
"./scripts/200_pbuilder_debmirror_localhost.sh"
"./scripts/300_pbuilder_build_netadapter.sh"
