//
// Created by yu.cai on 2019/4/12.
//

#ifndef FFMPEG_UTIL_H_H_
#define FFMPEG_UTIL_H_H_
/**
 * 合成两个mp4文件
 * @param inAudioFile：音频输入源
 * @param inVideoFile：视频输入源
 * @param outputFile：合成输出
 * @return： 0成功，非0失败
 */
int MergeTwoFile(const char* inAudioFile, const char* inVideoFile, const char* outputFile);

/**
 * 解码视频并写YUV420p文件
 * @param inFile
 * @param outFile
 * @return
 */
int DecodeVideoAndWriteYUV420(const char* inFile, const char* outFile);

#endif //TESTCPP_MERGE_MP4_UTIL_H
