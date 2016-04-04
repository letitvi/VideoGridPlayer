#pragma once


#include "IRenderer.hpp"
namespace vgp
{

class VideoContext : public IRenderer
{
public:
    VideoContext();
    ~VideoContext();

    void HandleEvent(SDL_Event &e);
    void Update();
    void Render();
};

}