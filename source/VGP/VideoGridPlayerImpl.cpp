#include "stdafx.h"
#include "VideoGridPlayerImpl.hpp"


namespace vgp
{


VideoGridPlayerImpl::VideoGridPlayerImpl(const size_t windowWidth,
    const size_t windowHeight,
    const std::string windowTitle)
    : m_manager(VgpManager::GetInstance())
{
    m_windowID = m_manager.NewWindowID();
    m_manager.NewWindow(m_windowID, windowWidth, windowHeight, windowTitle);
   
}
VideoGridPlayerImpl::VideoGridPlayerImpl(void *windowHandle)
    : m_manager(VgpManager::GetInstance())
{
    m_windowID = m_manager.NewWindowID();
    m_manager.NewWindowFrom(m_windowID, windowHandle);
}

VideoGridPlayerImpl::~VideoGridPlayerImpl()
{
}

void VideoGridPlayerImpl::NewVideo(const unsigned int videoID, const NewVideoParam &param)
{
}

void VideoGridPlayerImpl::ControlVideo(const unsigned int videoID, const ControlVideoParam &param)
{
}

void VideoGridPlayerImpl::DeleteVideo(const unsigned int videoID)
{
}


}