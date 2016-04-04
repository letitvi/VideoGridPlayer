#pragma once

namespace vgp
{

enum VideoPixelFormat
{
    kUnknownPixelFormat,
    RGB555, /**< RGB 555 16-bit colorspace */
    RGB565, /**< RGB 565 16-bit colorspace */
    BGR24,	/**< BGR 24-bit colorspace */
    BGR32,	/**< BGR4 32-bit colorspace */
    BGRA,	/**< BGRA 32-bit colorspace */
    UYVY,	/**< UYVY 16-bit colorspace, packed YUV, 4:2:2, U->Y->V->Y */
    YUYV,	/**< YUYV 16-bit colorspace, packed YUV, 4:2:2, Y->U->Y->V */
    YV12,	/**< YV12 12 bit colorspace, planar YUV, 4:2:0, planes order: Y->V->U */
    I420	/**< I420 12 bit colrospace, planar YUV, 4:2:0, planes order: Y->U->V */
};

enum MediaType
{
    MEDIA_UNKNOWN,
    MEDIA_VIDEO,
    MEDIA_AUDIO,
    MEDIA_SUBTITLE
};

struct DecodedMediaData
{
    MediaType Type;
    unsigned long long PlayTimeSec;
    unsigned long long TimeStampMsec;
};

struct DecodedVideoData : DecodedMediaData
{
    static const unsigned int kMaxVideoBufferArray = 4;
    unsigned int VideoWidth;
    unsigned int VideoHeight;
    VideoPixelFormat PixelFormat;

    unsigned int UsedBufferCount;
    unsigned int LineSize[kMaxVideoBufferArray];
    unsigned int DataSize[kMaxVideoBufferArray];
    unsigned char * DataPtr[kMaxVideoBufferArray];

    DecodedVideoData()
        : VideoWidth(0), VideoHeight(0)
    {
        Type = MEDIA_VIDEO;
        UsedBufferCount = 0;
        for (size_t i = 0; i < kMaxVideoBufferArray; i++) {
            LineSize[i] = 0;
            DataSize[i] = 0;
            DataPtr[i] = nullptr;
        }
    }
};

struct DecodedAudioData : DecodedMediaData
{
    DecodedAudioData()
    {
        Type = MEDIA_AUDIO;
    }

};

struct DecodedSubtitle : DecodedMediaData
{
    DecodedSubtitle()
    {
        Type = MEDIA_SUBTITLE;
    }
};
}