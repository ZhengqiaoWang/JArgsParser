cmake_minimum_required (VERSION 2.8)
# 项目信息
project (JArgsParser)

aux_source_directory(src DIR_SRCS)

add_executable(JArgsParser ${DIR_SRCS})
