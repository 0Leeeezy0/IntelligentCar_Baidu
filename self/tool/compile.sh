#! /bin/bash
# 该脚本用于自动编译LazyBug-AUTONOMOUS工程

rm -r ../build
mkdir ../build
cd ../build
cmake ..
make -j6

