# CMake版本要求
cmake_minimum_required(VERSION 3.5)

# 工程名字
project(SWAN.RUN)

#启动对C++14标准的支持
set(CMAKE_CXX_STANDARD 14)

# 设置搜索路径
# 库搜索路径
# set( LIB_PATH "/usr/local/lib")
# set( ONNX_INCLUDE_PATH "/usr/local/include/onnx")
# 头文件搜索路径
set( SWAN_INCLUDE_PATH "./include" )
set( JSON_INCLUDE_PATH "./include/json" )
set( INCLUDE_PATH "./src/include" )
# 源文件搜索路径
set( SWAN_LIB_FOLDER_PATH "./lib/swan" )
set( SRC_FOLDER_PATH "./src/src")
set( LIB_FOLDER_PATH "./src/lib")

# 源文件列表
list(APPEND SRC_PATH
    ${SRC_FOLDER_PATH}/demo.cpp
    ${LIB_FOLDER_PATH}/point_process.cpp
    ${LIB_FOLDER_PATH}/auto_pk.cpp
    ${LIB_FOLDER_PATH}/uart.cpp
    ${LIB_FOLDER_PATH}/img_process.cpp
    ${SWAN_LIB_FOLDER_PATH}/camera.cpp
    ${SWAN_LIB_FOLDER_PATH}/control.cpp
    ${SWAN_LIB_FOLDER_PATH}/image.cpp
    # ${SWAN_LIB_FOLDER_PATH}/model.cpp
    ${SWAN_LIB_FOLDER_PATH}/uart.cpp
)

# 查找包
find_package(PkgConfig)
# OpenCV
find_package(OpenCV REQUIRED) 
# ONNX
# 技术力有限，暂时无法解决
# 1.在pkg-config文件夹下加入.pc文件，用pkg_search_module 编译出错
# 2.在cmake文件夹下加入.cmake文件，用find_package 暂时不会用
# pkg_search_module(ONNX REQUIRED onnx)   
# link_directories(${ONNX_LIBRARY_DIRS})  

# ONNX调用链接库
# link_directories( ${LIB_PATH}/onnx )
# link_libraries( ${LIB_PATH}/onnx/libonnxruntime.so.1.17.1 )

# PTHREAD调用链接库
link_libraries(pthread)

# 引入头文件
include_directories( ${INCLUDE_PATH} )
include_directories( ${SWAN_INCLUDE_PATH} )
include_directories( ${JSON_INCLUDE_PATH} )
include_directories( ${PACKAGE_INCLUDE_PATH} )
include_directories( ${OpenCV_INCLUDE_DIRS} )
include_directories( ${ONNX_INCLUDE_PATH} )

# 生成可执行目标文件
add_executable( ${PROJECT_NAME} ${SRC_PATH} )

# 链接
# OpenCV
target_link_libraries( ${PROJECT_NAME} PRIVATE ${OpenCV_LIBS} )
