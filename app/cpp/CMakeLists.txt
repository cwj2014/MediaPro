#第一步 指定CMake的最低版本要求
CMAKE_MINIMUM_REQUIRED(VERSION 2.8.5)

#第二步 项目信息，能不有空格
PROJECT (MEDIAPRO)

add_subdirectory(media)
add_subdirectory(common)
