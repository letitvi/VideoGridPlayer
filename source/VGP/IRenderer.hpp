#pragma once

#include <SDL.h>
namespace vgp
{


struct IRenderer
{
    virtual void HandleEvent(SDL_Event &e)=0;
    virtual void Update()=0;
    virtual void Render()=0;
};

}