#pragma once

#include <SDL.h>
#include <memory>
#include <string>

#include "IRenderer.hpp"

namespace vgp
{

class WindowContext : public IRenderer
{
public:
    WindowContext(const unsigned int windowID,
                  void *windowHandle);
    WindowContext(const unsigned int windowID,
                  const size_t windowWidth,
                  const size_t windowHeight,
                  const std::string windowTitle);
    ~WindowContext();

    void HandleEvent(SDL_Event &e);
    void Update();
    void Render();

    unsigned int GetWindowID() const { return m_windowID; }

private:
    SDL_Window *m_sdlWindow;
    SDL_Renderer *m_sdlRenderer;
    unsigned int m_windowID;
};

typedef std::shared_ptr<WindowContext> WindowContextPtr;

}