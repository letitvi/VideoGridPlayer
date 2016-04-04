#pragma once

#include <memory>
#include "FFmpegHelper.hpp"
#include "MediaDataType.hpp"

namespace vgp
{
typedef std::unique_ptr<AVCodecContextHelper> AVCodecContextHelperPtr;
typedef std::unique_ptr<AVFrameHelper> AVFrameHelperPtr;

typedef std::shared_ptr<DecodedVideoData> DecodedVideoDataPtr;
typedef std::shared_ptr<DecodedAudioData> DecodedAudioDataPtr;
typedef std::shared_ptr<DecodedSubtitle> DecodedSubtitlePtr;

class FFmpegVideoDecoder
{
public:
    FFmpegVideoDecoder();
    ~FFmpegVideoDecoder();

    DecodedVideoDataPtr DecodeVideo();

private:
    AVCodecContextHelperPtr m_avCodecContext;
    AVFrameHelperPtr m_avFrame;
};

}