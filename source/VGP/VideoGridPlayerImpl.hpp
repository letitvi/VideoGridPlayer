#pragma once

#include <string>
#include <memory>
#include <SDL.h>

#include <vgp/vgp.hpp>
#include "VgpManager.hpp"


namespace vgp
{

class VideoGridPlayerImpl
{
public:
    VideoGridPlayerImpl(const size_t windowWidth, 
                        const size_t windowHeight,
                        const std::string windowTitle);
    VideoGridPlayerImpl(void *windowHandle);

    ~VideoGridPlayerImpl();

    void NewVideo(const unsigned int videoID, const NewVideoParam &param);
    void ControlVideo(const unsigned int videoID, const ControlVideoParam &param);
    void DeleteVideo(const unsigned int videoID);

private:
    VgpManager &m_manager;
    int m_windowID;
};

}