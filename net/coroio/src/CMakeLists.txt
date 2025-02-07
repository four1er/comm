cmake_minimum_required(VERSION 3.14)  # 确保CMake版本支持C++20
project(coroio)  # 项目名称

set(CMAKE_CXX_STANDARD 20)  # 设置C++标准为C++20
set(CMAKE_CXX_STANDARD_REQUIRED ON)  # 强制使用C++20
set(CMAKE_CXX_EXTENSIONS OFF)  # 禁用编译器特定的扩展

# 如果你的编译器需要额外的标志来启用协程支持（例如GCC）
if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    add_compile_options(-fcoroutines)  # GCC需要-fcoroutines标志
endif()

set(SOURCES
  base.h
  poller.h
)

add_library(coroio ${SOURCES})
