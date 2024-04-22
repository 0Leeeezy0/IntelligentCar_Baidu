#CMake版本
cmake_minimum_required(VERSION 3.5)

#工程名字
project(IMAGE_COLLECT.RUN)

#OpenCV
find_package(OpenCV REQUIRED)    
include_directories( ${OpenCV_INCLUDE_DIRS} )

#编译时所需源文件
add_executable(${PROJECT_NAME}  ./src/ImageCollect.cpp)

#OpenCV与工程链接
target_link_libraries(${PROJECT_NAME} PRIVATE ${OpenCV_LIBS})

