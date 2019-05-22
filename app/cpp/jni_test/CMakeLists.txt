INCLUDE_DIRECTORIES(../)
SET(JNITEST_LIB_SRCS jnitest_impl.cpp
                        jni_load.cpp)
ADD_LIBRARY(JniTest SHARED ${JNITEST_LIB_SRCS})
TARGET_LINK_LIBRARIES(JniTest log android)
