cmake_minimum_required (VERSION 2.8)
# 项目信息
project (JArgsParser)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

aux_source_directory(src DIR_SRCS)
include_directories(JArgsParser interface)
include_directories(JArgsParser include)

add_executable(JArgsParser ${DIR_SRCS})

add_subdirectory(ut)