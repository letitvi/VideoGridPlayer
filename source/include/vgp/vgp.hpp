#pragma once

#ifndef VGP_API_HPP
#define VGP_API_HPP

#include <string>
#include <vgp/export.hpp>

namespace vgp
{

struct NewVideoParam
{
    NewVideoParam()
    {}

    NewVideoParam(std::string& filePath)
        : VideoFilePath(filePath)
    {}

    std::string VideoFilePath;
    std::string SubtitleFilePath;
};

struct ControlVideoParam
{

};



class VideoGridPlayerImpl;

class VGP_API VideoGridPlayer
{
public:
    VideoGridPlayer(const size_t windowWidth, 
                    const size_t windowHeight,
                    const std::string windowTitle);

    VideoGridPlayer(void *windowHandle);

    ~VideoGridPlayer();

    void NewVideo(const unsigned int videoID, const NewVideoParam &param);
    void ControlVideo(const unsigned int videoID, const ControlVideoParam &param);
    void DeleteVideo(const unsigned int videoID);

private:
    VideoGridPlayer(const VideoGridPlayer&) = delete;
    VideoGridPlayer& operator=(const VideoGridPlayer&) = delete;

private:
    VideoGridPlayerImpl* m_impl;
};

}

#endif