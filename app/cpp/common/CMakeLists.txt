INCLUDE_DIRECTORIES( ../)
SET(NATIVETOOL_LIB_SRCS native_tool_impl.cpp
                        jni_load.cpp)
ADD_LIBRARY(nativetool SHARED ${NATIVETOOL_LIB_SRCS})
TARGET_LINK_LIBRARIES(nativetool log android)
