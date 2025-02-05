# CMake版本要求
cmake_minimum_required(VERSION 3.5)

# 工程名字
project(SWAN_HSV_TOOL.RUN)

#启动对C++14标准的支持
set(CMAKE_CXX_STANDARD 14)

# 设置搜索路径
# 源文件搜索路径
set( SRC_FOLDER_PATH "./src")

# 源文件列表
list(APPEND SRC_PATH
    ${SRC_FOLDER_PATH}/swan_HSV_tool.cpp
)

# 查找包
find_package(PkgConfig)
# OpenCV
find_package(OpenCV REQUIRED) 

# PTHREAD调用链接库
link_libraries(pthread)

# 引入头文件
include_directories( ${OpenCV_INCLUDE_DIRS} )
# 生成可执行目标文件
add_executable( ${PROJECT_NAME} ${SRC_PATH} )

# 链接
# OpenCV
target_link_libraries( ${PROJECT_NAME} PRIVATE ${OpenCV_LIBS} )
