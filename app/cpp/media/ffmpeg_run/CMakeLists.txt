#当前目录下的使用相对路径
set(FFMPEG_DIR ffmpeg-4.0.2)
INCLUDE_DIRECTORIES(${FFMPEG_DIR})

set(FFMPEG_SRCS ${FFMPEG_DIR}/fftools/cmdutils.c
        ${FFMPEG_DIR}/fftools/ffmpeg.c
        ${FFMPEG_DIR}/fftools/ffmpeg_cuvid.c
        ${FFMPEG_DIR}/fftools/ffmpeg_filter.c
        ${FFMPEG_DIR}/fftools/ffmpeg_hw.c
        ${FFMPEG_DIR}/fftools/ffmpeg_opt.c)

ADD_LIBRARY(ffmpeg ${FFMPEG_SRCS})
