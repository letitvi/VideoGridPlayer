#pragma once

#include <string>
#include "FFmpegCommon.hpp"

namespace vgp
{
struct FFmpegPlayParam
{
    FFmpegPlayParam()
        : Scale(1.0f), StartTime(0), EndTime(0)
        , OnlyKeyFrame(false), IsPaused(false)
    {}
    float Scale; // -1.0, 1.0
    unsigned long long StartTime;
    unsigned long long EndTime; // 0 means no end
    bool OnlyKeyFrame;
    bool IsPaused;
};

class FFmpegFileReader
{
public:
    FFmpegFileReader();
    ~FFmpegFileReader();

    bool Open(const std::string& filePath,
              const std::string& subTitlePath);
    bool Play(const FFmpegPlayParam &param);
    bool Pause();
    bool Stop();
    bool Close();

    bool ShowSubtitle(const bool isShow);
};

}