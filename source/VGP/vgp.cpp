#include "stdafx.h"

#include <vgp/vgp.hpp>

#include "VideoGridPlayerImpl.hpp"


namespace vgp
{

VideoGridPlayer::VideoGridPlayer(const size_t windowWidth,
                                 const size_t windowHeight,
                                 const std::string windowTitle)
{
    m_impl = new VideoGridPlayerImpl(windowWidth, windowHeight, windowTitle);
}

VideoGridPlayer::VideoGridPlayer(void *windowHandle)
{
    m_impl = new VideoGridPlayerImpl(windowHandle);
}

VideoGridPlayer::~VideoGridPlayer()
{
    delete m_impl;
}

void VideoGridPlayer::NewVideo(const unsigned int videoID, const NewVideoParam &param)
{
    m_impl->NewVideo(videoID, param);
}

void VideoGridPlayer::ControlVideo(const unsigned int videoID, const ControlVideoParam &param)
{
    m_impl->ControlVideo(videoID, param);
}

void VideoGridPlayer::DeleteVideo(const unsigned int videoID)
{
    m_impl->DeleteVideo(videoID);
}

}