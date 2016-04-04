#include "stdafx.h"

#include "WindowContext.hpp"


namespace vgp
{



WindowContext::WindowContext(const unsigned int windowID,
              void *windowHandle)
    : m_windowID(windowID)
{
    m_sdlWindow = SDL_CreateWindowFrom(windowHandle);
    m_sdlRenderer = SDL_CreateRenderer(m_sdlWindow, -1, SDL_RENDERER_ACCELERATED);
}

WindowContext::WindowContext(const unsigned int windowID,
                             const size_t windowWidth,
                             const size_t windowHeight,
                             const std::string windowTitle)
    : m_windowID(windowID)
{
    m_sdlWindow = SDL_CreateWindow(
        windowTitle.c_str(),
        SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
        windowWidth, windowHeight,
        SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
    m_sdlRenderer = SDL_CreateRenderer(m_sdlWindow, -1, SDL_RENDERER_ACCELERATED);
}

WindowContext::~WindowContext()
{
    SDL_DestroyRenderer(m_sdlRenderer);
    SDL_DestroyWindow(m_sdlWindow);
}

void WindowContext::HandleEvent(SDL_Event &e)
{
  
}

void WindowContext::Update()
{

}

void WindowContext::Render()
{
    // if (!mMinimized)
    {
        // Clear screen
        SDL_SetRenderDrawColor(m_sdlRenderer, 0x00, 0x00, 0x00, SDL_ALPHA_OPAQUE);
        SDL_RenderClear(m_sdlRenderer);

        // Update screen
        SDL_RenderPresent(m_sdlRenderer);
    }
}

}