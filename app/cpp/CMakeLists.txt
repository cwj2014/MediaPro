#第一步 指定CMake的最低版本要求
CMAKE_MINIMUM_REQUIRED(VERSION 2.8.5)

#第二步 项目信息，能不有空格
PROJECT (MEDIAPRO)

add_subdirectory(media)
add_subdirectory(common)

set(JNILoad_LIB_SRCS JNI/jni_load.cpp)
ADD_LIBRARY(JNILoad SHARED ${JNILoad_LIB_SRCS})
#注意jni_load.cpp中引用了ffmpegtool的函数实现，所以需要加入ffmpegtool这个库
SET(JNILOAD_LIBS log
                 ffmpegtool
                 nativetool)
TARGET_LINK_LIBRARIES(JNILoad ${JNILOAD_LIBS})
