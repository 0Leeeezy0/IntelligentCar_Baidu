#CMake版本
cmake_minimum_required(VERSION 3.5)

#工程名字
project(DEMO.RUN)

#OpenCV
#用find_package方法引入OpenCV库
#也可以用dir的方法
#但是find_package方法可移植性好
find_package(OpenCV REQUIRED)    
include_directories( ${OpenCV_INCLUDE_DIRS} )

#ONNX
#开发端必须最好和EB上的路径一致
include_directories(/usr/local/include/onnx)
link_directories(/usr/local/lib/onnx)
link_libraries(/usr/local/lib/onnx/libonnxruntime.so)

#PPNC
link_directories(./lib/model)
link_libraries(./lib/model/libppnc_runtime.so)

#开发端

find_package(PkgConfig)
pkg_search_module(GLIB REQUIRED glib-2.0)
include_directories(${GLIB_INCLUDE_DIRS})

#引入头文件
include_directories(./include)
include_directories(./include/model)

#编译时所需源文件
add_executable(${PROJECT_NAME}  ./src/main.cpp
                                ./src/path_across.cpp
                                ./src/path_circle.cpp
                                ./src/path_side_search.cpp
                                ./src/path_AI.cpp
                                ./lib/libimage_process.cpp
                                ./lib/libdata_process.cpp
                                ./lib/libmodel_onnx.cpp
                                ./lib/libuart_wz.cpp
                                ./lib/libuart_eb.cpp)

#OpenCV与工程链接
target_link_libraries(${PROJECT_NAME} PRIVATE ${OpenCV_LIBS})
