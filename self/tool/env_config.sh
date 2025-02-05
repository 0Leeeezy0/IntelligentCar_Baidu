#! /bin/bash
# 该脚本用于自动配置开发环境

read step

if [ $step = "help" ];
then
    echo "该脚本用于配置开发环境以及边缘计算环境"
    echo "install  apt自动安装程序依赖库"
    echo "onnx_linux_*  拷贝onnx_linux_*到 /usr/local/lib 和 /usr/local/inclued"
fi

if [ $step = "install" ];
then
    # 安装依赖库
    sudo apt install cmake  # cmake
    sudo apt install make   # make
    sudo apt install gcc    # gcc编译器
    sudo apt install g++    # g++编译器
    sudo apt install pkg-config # pkg-config
fi

if [ $step = "onnx_linux_x64" ];
then
    # x86-64架构
    # 将工程内的ONNX库复制至用户空间
    # 复制库
    sudo mkdir /usr/local/lib/onnx
    cd ./lib/onnx_linux_x64
    sudo cp -r . /usr/local/lib/onnx
    echo "库文件拷贝完成"
    # 复制.pc文件   
    # sudo cp onnx.pc /usr/local/lib/pkgconfig
    # 复制头文件
    sudo mkdir /usr/local/include/onnx
    cd ../../include/onnx_linux_x64
    sudo cp -r . /usr/local/include/onnx
    echo "头文件拷贝完成"
fi

if [ $step = "onnx_linux_aarch64" ];
then
    # AARCH64架构
    # 将工程内的ONNX库复制至用户空间
    # 复制库
    sudo mkdir /usr/local/lib/onnx
    cd ./lib/onnx_linux_aarch64
    sudo cp -r . /usr/local/lib/onnx
    echo "库文件拷贝完成"
    # 复制.pc文件   
    # sudo cp onnx.pc /usr/local/lib/pkgconfig
    # 复制头文件
    sudo mkdir /usr/local/include/onnx
    cd ../../include/onnx_linux_aarch64
    sudo cp -r . /usr/local/include/onnx
    echo "头文件拷贝完成"
fi

