#pragma once

#include <memory>
#include "FFmpegCommon.hpp"

struct AVCodecContextHelper
{
public:
    AVCodecContextHelper(AVCodec *codec)
        : m_avCodecContext(avcodec_alloc_context3(codec))
    {}

    ~AVCodecContextHelper()
    {avcodec_free_context(&m_avCodecContext);}

    AVCodecContext* get() { return m_avCodecContext; }

private:
    AVCodecContext*    m_avCodecContext;
};

class AVFrameHelper
{
public:
    AVFrameHelper()
        : m_avFrame(av_frame_alloc())
    {}

    ~AVFrameHelper() 
    { av_frame_free(&m_avFrame);}

    AVFrame* get() { return m_avFrame; }

private:
    AVFrame*    m_avFrame;
};