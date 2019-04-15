//
// Created by yu.cai on 2019/4/12.
//

#ifndef TESTCPP_MERGE_MP4_UTIL_H
#define TESTCPP_MERGE_MP4_UTIL_H
/**
 * 合成两个mp4文件
 * @param inAudioFile：音频输入源
 * @param inVideoFile：视频输入源
 * @param outputFile：合成输出
 * @return： 0成功，非0失败
 */
int MergeTwoFile(const char* inAudioFile, const char* inVideoFile, const char* outputFile);

#endif //TESTCPP_MERGE_MP4_UTIL_H
