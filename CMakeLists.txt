cmake_minimum_required(VERSION 3.15)
project(annotate_muduo)

# 允许调试
enable_testing()

# 添加子目录，使得子目录中的cmakeLists得以生效
add_subdirectory(base)
# add_subdirectory(net)

set(CMAKE_CXX_STANDARD 11)

add_executable(annotate_muduo main.cpp base/Singleton.h)