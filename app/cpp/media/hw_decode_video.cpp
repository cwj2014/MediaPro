//
// Created by yu.cai on 2019/5/7.
//
#include "ffmpeg_util.h"
#include "global.h"
#include <cstdio>
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>
#include <stdio.h>
#include "yuv420p_util.h"
#include "codecimpl.h"



static enum AVPixelFormat mediacodec_hwaccel_get_format(AVCodecContext *avctx,
                                                        const enum AVPixelFormat *pix_fmts)
{
    const enum AVPixelFormat *p = NULL;
    int ret = -1;

    for (p = pix_fmts; *p != AV_PIX_FMT_NONE; p++) {
        AVMediaCodecContext *mediacodec_ctx = NULL;
        const AVPixFmtDescriptor *desc = av_pix_fmt_desc_get(*p);

        if (!(desc->flags & AV_PIX_FMT_FLAG_HWACCEL)) {

            break;
        }

        if (*p != AV_PIX_FMT_MEDIACODEC) {
            continue;
        }

        mediacodec_ctx = av_mediacodec_alloc_context();
        if (!mediacodec_ctx) {
            fprintf(stderr, "Failed to allocate hwaccel ctx\n");
            continue;
        }
        ret = av_mediacodec_default_init(avctx, mediacodec_ctx, nullptr);
        avctx->hwaccel_context = mediacodec_ctx;
        break;
    }

    return *p;
}

int DecodeVideoAndWriteYUV420(const char* inFile, const char* outFile){
    AVFormatContext * avformatctx = nullptr;
    int video_stream_index = -1;
    AVCodec* avCodec = nullptr;
    AVCodecContext *avCodecContext;
    AVPacket* avPacket = av_packet_alloc();
    av_init_packet(avPacket);
    int ret = 0;
    FILE * oFile = nullptr;
    auto callback = [&](AVCodecContext *ctx, const AVFrame* frame){
        if(ctx->codec_type==AVMEDIA_TYPE_VIDEO){
            AVFrame* avFrame = av_frame_clone(frame);
            int num =  1; //分子
            int den = avformatctx->streams[video_stream_index]->r_frame_rate.num;//分母
            avFrame->pts = av_rescale_q(frame->pts, avformatctx->streams[video_stream_index]->time_base, AVRational{num, den});
            printf("pts:%ld 转成 %ld\n", frame->pts, avFrame->pts);
            if(oFile != nullptr)
                WriteYUV420ToFile(avFrame, oFile);
        }
    };
    oFile = fopen(outFile, "wb");
    if(oFile == nullptr){
        return -1;
    }
    if(avformat_open_input(&avformatctx, inFile, nullptr, nullptr)<0){
        return -1;
    }
    if(avformat_find_stream_info(avformatctx, nullptr)<0){
        goto failed;
    }
    for(int i=0; i<avformatctx->nb_streams; i++){
        AVStream* stream = avformatctx->streams[i];
        if(stream->codecpar->codec_type == AVMEDIA_TYPE_VIDEO){
            video_stream_index = i;
            break;
        }
    }
    if(video_stream_index == -1){
        goto failed;
    }

    if(avformatctx->streams[video_stream_index]->codecpar->codec_id == AV_CODEC_ID_H264){
        avCodec = avcodec_find_decoder_by_name("h264_mediacodec");
    }
    if(avCodec == nullptr){
        avCodec = avcodec_find_decoder(avformatctx->streams[video_stream_index]->codecpar->codec_id);
    }
    if(avCodec == nullptr){
        goto failed;
    }
    avCodecContext = avcodec_alloc_context3(avCodec);
    if(avCodecContext == nullptr){
        goto failed;
    }

    if(avcodec_parameters_to_context(avCodecContext, avformatctx->streams[video_stream_index]->codecpar)<0)
        goto failed;
    avCodecContext->get_format = mediacodec_hwaccel_get_format;

    if(avcodec_open2(avCodecContext, avCodec, nullptr)<0){
        goto failed;
    }

    while(av_read_frame(avformatctx, avPacket) >= 0){
        if(avPacket->stream_index == video_stream_index){
            decode(avCodecContext, avPacket, callback);
        }
        av_packet_unref(avPacket);
    }
failed:
    avformat_close_input(&avformatctx);
    avcodec_free_context(&avCodecContext);
    fclose(oFile);
    av_packet_free(&avPacket);
    return ret < 0;
}

