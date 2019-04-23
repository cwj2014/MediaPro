//
// Created by yu.cai on 2019/4/12.
//
extern "C"{
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libavfilter/buffersink.h>
#include <libavfilter/buffersrc.h>
#include <libavutil/time.h>
#include <libavutil/opt.h>
#include <libavutil/imgutils.h>
}
#include <cstdio>
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>
#include <stdio.h>
#include <memory>
#include "merge_mp4_util.h"

struct MediaInfo{
    AVFormatContext* inAVFormatCtx;
    AVStream* outStream;
    int stream_index;
};

std::shared_ptr<MediaInfo> InitStream(AVFormatContext* outFormatCtx, const char* filepath, AVMediaType type){
    AVFormatContext* ofctx = nullptr;
    if(avformat_open_input(&ofctx, filepath, nullptr, nullptr)<0){
        return nullptr;
    }
    avformat_find_stream_info(ofctx, nullptr);
    int stream_index = -1;
    AVStream* avStream = avformat_new_stream(outFormatCtx, nullptr);
    for(int i=0; i<ofctx->nb_streams; i++){
        AVStream* stream = ofctx->streams[i];
        if(stream->codecpar->codec_type == type){
            avcodec_parameters_copy(avStream->codecpar, stream->codecpar);
            stream_index = i;
            break;
        }
    }
    auto ptr = std::make_shared<MediaInfo>();
    ptr->stream_index = stream_index;
    ptr->inAVFormatCtx = ofctx;
    ptr->outStream = avStream;
    return ptr;
}

bool ReadPacket(std::shared_ptr<MediaInfo> mediaInfo, AVPacket* pkt){
    bool readpkt = false;
    while(true){//直到读取到视频数据为至
        if(av_read_frame(mediaInfo->inAVFormatCtx, pkt)<0){
            break;
        }
        if(pkt->stream_index == mediaInfo->stream_index){
            readpkt = true;
            break;
        }else{
            av_packet_unref(pkt);
        }
    }
    return readpkt;
}


int MergeTwoFile(const char* inAudioFile, const char* inVideoFile, const char* outputFile){
    AVFormatContext* outFormatCtx;
    if(avformat_alloc_output_context2(&outFormatCtx, nullptr, nullptr, outputFile)){
        return -1;
    }

    std::shared_ptr<MediaInfo> audioMediaInfo = InitStream(outFormatCtx, inAudioFile, AVMEDIA_TYPE_AUDIO);
    std::shared_ptr<MediaInfo> videoMediaInfo = InitStream(outFormatCtx, inVideoFile, AVMEDIA_TYPE_VIDEO);

    if(avio_open(&outFormatCtx->pb, outputFile, AVIO_FLAG_WRITE)<0){
        return -1;
    }

    avformat_write_header(outFormatCtx, nullptr);

    AVPacket* pkt = av_packet_alloc();
    av_init_packet(pkt);

    int64_t last_video_pts = 0;
    int64_t last_audio_pts = 0;

    AVStream* inAudioSt = audioMediaInfo->inAVFormatCtx->streams[audioMediaInfo->stream_index];
    AVStream* inVideoSt = videoMediaInfo->inAVFormatCtx->streams[videoMediaInfo->stream_index];
    while(1){
        AVStream* in = nullptr;
        AVStream* out = nullptr;
        if(av_compare_ts(last_video_pts, inVideoSt->time_base, last_audio_pts, inAudioSt->time_base)<=0){//先读取视频
            if(!ReadPacket(videoMediaInfo, pkt)){
                break;
            }
            last_video_pts = pkt->pts;
            out = videoMediaInfo->outStream;
            in = inVideoSt;
        }else{//先读取音频
            if(!ReadPacket(audioMediaInfo, pkt)){
                break;
            }
            last_audio_pts = pkt->pts;
            out = audioMediaInfo->outStream;
            in = inAudioSt;
        }
        int64_t pts = pkt->pts;
        int64_t dts = pkt->dts;
        int64_t dur = pkt->duration;
        pkt->pts = av_rescale_q_rnd(pkt->pts, in->time_base, out->time_base, (AVRounding)(AV_ROUND_NEAR_INF|AV_ROUND_PASS_MINMAX));
        pkt->dts = av_rescale_q_rnd(pkt->dts, in->time_base, out->time_base, (AVRounding)(AV_ROUND_NEAR_INF|AV_ROUND_PASS_MINMAX));
        pkt->duration = av_rescale_q(pkt->duration, in->time_base, out->time_base);
        pkt->pos = -1;
        pkt->stream_index = out->index;
        av_interleaved_write_frame(outFormatCtx, pkt);
        av_packet_unref(pkt);
    }


    av_write_trailer(outFormatCtx);
    avio_close(outFormatCtx->pb);
    avformat_free_context(outFormatCtx);

    av_packet_free(&pkt);

    avformat_close_input(&audioMediaInfo->inAVFormatCtx);
    avformat_close_input(&videoMediaInfo->inAVFormatCtx);

    return 0;
}
