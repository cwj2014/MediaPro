#第一步 指定CMake的最低版本要求
CMAKE_MINIMUM_REQUIRED(VERSION 2.8.5)

#第二步 项目信息，能不有空格
PROJECT (MEDIAPRO)

#第三步 指定生成目标
set(FFMPEG_DIR ffmepg_install)

INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/${FFMPEG_DIR}/${ANDROID_ABI}/include)

#ADD_EXECUTABLE(merge merge_2mp4_output_mp4.cpp)

add_library(avcodec SHARED IMPORTED)
set_target_properties(avcodec
        PROPERTIES IMPORTED_LOCATION
        ${PROJECT_SOURCE_DIR}/../libs/${ANDROID_ABI}/libavcodec.so)

add_library(avdevice SHARED IMPORTED)
set_target_properties(avdevice
        PROPERTIES IMPORTED_LOCATION
        ${PROJECT_SOURCE_DIR}/../libs/${ANDROID_ABI}/libavdevice.so)

add_library(avfilter SHARED IMPORTED)
set_target_properties(avfilter
        PROPERTIES IMPORTED_LOCATION
        ${PROJECT_SOURCE_DIR}/../libs/${ANDROID_ABI}/libavfilter.so)

add_library(avformat SHARED IMPORTED)
set_target_properties(avformat
        PROPERTIES IMPORTED_LOCATION
        ${PROJECT_SOURCE_DIR}/../libs/${ANDROID_ABI}/libavformat.so)

add_library(avutil SHARED IMPORTED)
set_target_properties(avutil PROPERTIES IMPORTED_LOCATION
        ${PROJECT_SOURCE_DIR}/../libs/${ANDROID_ABI}/libavutil.so)

add_library(postproc SHARED IMPORTED)
set_target_properties(postproc
        PROPERTIES IMPORTED_LOCATION
        ${PROJECT_SOURCE_DIR}/../libs/${ANDROID_ABI}/libpostproc.so)

add_library(swresample SHARED IMPORTED)
set_target_properties(swresample
        PROPERTIES IMPORTED_LOCATION
        ${PROJECT_SOURCE_DIR}/../libs/${ANDROID_ABI}/libswresample.so)

add_library(swscale SHARED IMPORTED)
set_target_properties(swscale
        PROPERTIES IMPORTED_LOCATION
        ${PROJECT_SOURCE_DIR}/../libs/${ANDROID_ABI}/libswscale.so)

SET(LIBS avutil
        avcodec
        swresample
        swscale
        postproc
        avformat
        avfilter
        z
        m)

SET(FFMPEGTOOL_LIB_SRCS ffmpeg_version.cpp)
ADD_LIBRARY(ffmpegtool SHARED ${FFMPEGTOOL_LIB_SRCS})

TARGET_LINK_LIBRARIES(ffmpegtool ${LIBS})
