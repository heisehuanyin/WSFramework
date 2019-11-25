#!/bin/bash

run_home=`pwd`

if [ ! -d ~/.WSFramework ]; then
mkdir ~/.WSFramework
fi

if [ ! -d ~/.WSFramework/plugins ]; then
mkdir ~/.WSFramework/plugins
fi

find ${run_home} -type f -name "*.dylib" | xargs -n1 -I{} cp {} ~/.WSFramework/plugins/
