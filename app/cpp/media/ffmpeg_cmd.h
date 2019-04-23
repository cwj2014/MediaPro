//
// Created by yu.cai on 2019/4/17.
//

#ifndef MEDIAPRO_FFMPEG_CMDL_H
#define MEDIAPRO_FFMPEG_CMDL_H

#include <vector>
#include <string>

/**
 * 参数请参考ffmpeg
 * @param params: 参数数组
 * @return:参考ffmpeg退出
 */
int executeCmd(std::vector<std::string>);

#endif //MEDIAPRO_FFMPEG_CMDL_H
