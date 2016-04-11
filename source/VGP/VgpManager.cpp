#include "stdafx.h"
#include "VgpManager.hpp"

#include <algorithm>
#include <chrono>
#include <thread>


namespace vgp
{

VgpManager& VgpManager::GetInstance()
{
    static VgpManager instance;
    return instance;
}

VgpManager::VgpManager()
    : m_isThreadRunning(true)
    , m_refreshRate(kDefaultRefreshRate)
    , m_renderThread(std::thread(&VgpManager::ThreadMain, this))
{

}


void VgpManager::NewWindow(const unsigned int windowID,
                          const size_t windowWidth,
                          const size_t windowHeight,
                          const std::string windowTitle)
{
    VGPLOG_INFO() << "NewWindow, windowID=" << windowID;
    VgpControlPtr ctl = std::make_shared<VgpControl>();
    ctl->f = std::bind(&VgpManager::NewWindowInternal, this, 
                      windowID, windowWidth, windowHeight, windowTitle);
    std::unique_lock<std::mutex> lk(ctl->m);
    PushMessage(std::ref(ctl));
    ctl->cv.wait(lk);
}

void VgpManager::NewWindowFrom(const unsigned int windowID,
                              void *windowHandle)
{
    VGPLOG_INFO() << "NewWindowForm, windowID=" << windowID;
    VgpControlPtr p = std::make_shared<VgpControl>();
    p->f = std::bind(&VgpManager::NewWindowFromInternal, this,
                       windowID, windowHandle);
    std::unique_lock<std::mutex> lk(p->m);
    PushMessage(std::ref(p));
    p->cv.wait(lk);
}


void VgpManager::DeleteWindow(const unsigned int windowID)
{
    VgpControlPtr p = std::make_shared<VgpControl>();
    p->f = std::bind(&VgpManager::DeleteWindowInternal, this,
                       windowID);
    std::unique_lock<std::mutex> lk(p->m);
    PushMessage(std::ref(p));
    p->cv.wait(lk);
}

void VgpManager::NewVideo(const unsigned int windowID, const unsigned int videoID,
                          const NewVideoParam& param)
{
    VgpControlPtr p = std::make_shared<VgpControl>();
    p->f = std::bind(&VgpManager::NewVideoInternal, this,
                     windowID, videoID, param);
    std::unique_lock<std::mutex> lk(p->m);
    PushMessage(std::ref(p));
    p->cv.wait(lk);
}

void VgpManager::PushMessage(VgpControlPtr& ptr)
{
    std::unique_lock<std::mutex> lk(m_controlQueueMutex);
    m_controlQueue.push_back(ptr);
}

void VgpManager::ExecuteMessage()
{
    std::unique_lock<std::mutex> lk(m_controlQueueMutex);
    std::for_each(m_controlQueue.begin(), m_controlQueue.end(), [](VgpControlPtr &p) 
    {
        std::unique_lock<std::mutex> lk(p->m);
        p->f();
        p->cv.notify_one();
    });
    m_controlQueue.clear();
}

void VgpManager::NewWindowInternal(const unsigned int windowID,
                           const size_t windowWidth,
                           const size_t windowHeight,
                           const std::string windowTitle)
{
    WindowContextPtr window = std::make_shared<WindowContext>(windowID, windowWidth, windowHeight, windowTitle);
    m_windows[windowID] = window;
}

void VgpManager::NewWindowFromInternal(const unsigned int windowID,
                                       void *windowHandle)
{
    WindowContextPtr window = std::make_shared<WindowContext>(windowID, windowHandle);
    m_windows[windowID] = window;;
}


void VgpManager::DeleteWindowInternal(const unsigned int windowID)
{
    m_windows.erase(windowID);
}

void VgpManager::NewVideoInternal(const unsigned int windowID, const unsigned int videoID,
                      const NewVideoParam& param)
{
    //m_windows[windowID]->
}

void VgpManager::Initialize()
{
   
    VGPLOG_INFO() << "===== VgpManager() START =====";
    VGPLOG_DDEBUG("Start Logger {},{},{}", 1, 2, 3);

    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        VGPLOG_FATAL() << "SDL_Init() failed : " << SDL_GetError();
    }
    if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1")) {
        VGPLOG_FATAL() << "Unable to Init hinting: " << SDL_GetError();
    }

    // Initialize image loading for PNGs
    /*
    if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) {
    }

    // Initialize ttf font
    if (TTF_Init() == -1) {
    BLOG(fatal) << "Unable to init SDL_ttf: " << TTF_GetError();
    }
    */
    SDL_LogSetAllPriority(SDL_LOG_PRIORITY_WARN);

    PrintDeviceInformation();
}

void VgpManager::PrintDeviceInformation()
{
    VGPLOG_DEBUG() << "platform :: " << SDL_GetPlatform();
    VGPLOG_DEBUG() << "L1 cache size :: " << SDL_GetCPUCacheLineSize() << "kbytes";
    VGPLOG_DEBUG() << "cpu num :: " << SDL_GetCPUCount();
    VGPLOG_DEBUG() << "memory size :: " << SDL_GetSystemRAM() << "MB";
    VGPLOG_DEBUG() << "3D Now :: " << SDL_Has3DNow();
    VGPLOG_DEBUG() << "AVX :: " << SDL_HasAVX();
    VGPLOG_DEBUG() << "AltiVec :: " << SDL_HasAltiVec();
    VGPLOG_DEBUG() << "MMX :: " << SDL_HasMMX();
    VGPLOG_DEBUG() << "RDTSC :: " << SDL_HasRDTSC();
    VGPLOG_DEBUG() << "SSE :: " << SDL_HasSSE();
    VGPLOG_DEBUG() << "SSE2 :: " << SDL_HasSSE2();
    VGPLOG_DEBUG() << "SSE3 :: " << SDL_HasSSE3();
    VGPLOG_DEBUG() << "SSE41 :: " << SDL_HasSSE41();
    VGPLOG_DEBUG() << "SSE42 :: " << SDL_HasSSE42();
}


VgpManager::~VgpManager()
{
    VGPLOG_INFO() << "===== VgpManager() END =====";
    m_isThreadRunning = false;
    m_renderThread.join();

    spdlog::drop_all();
    SDL_Quit();
    
}

void VgpManager::ThreadMain()
{
    using namespace std::chrono_literals;
    Initialize();

    SDL_Event e;
    while (m_isThreadRunning)
    {
        auto start = std::chrono::steady_clock::now();
        std::chrono::duration<double, std::milli> timeToSleep((double)1000 / m_refreshRate);

        // message from Application
        ExecuteMessage();

        // handle event
        while (SDL_PollEvent(&e) != 0)
        {
            HandleEvent(e);
        }

        // update scene
        Update();

        // render window
        Render();

        // sleep thread for refresh rate
        std::chrono::duration<double, std::milli> elapsed = std::chrono::steady_clock::now() - start;
        std::chrono::duration<double, std::milli> sleepTime = timeToSleep - elapsed;
        std::chrono::milliseconds ms(long long(sleepTime.count()));
        if (ms.count() > 0)
        {
            std::this_thread::sleep_for(ms);
        }
        
    }
}

void VgpManager::HandleEvent(SDL_Event &e)
{
    while (SDL_PollEvent(&e) != 0)
    {
        if (e.type == SDL_QUIT)
        {
            m_isThreadRunning = true;
            break;
        }
    }
}


void VgpManager::Update()
{
    for (auto w : m_windows)
        w.second->Update();
}

void VgpManager::Render()
{
    for (auto w : m_windows)
        w.second->Update();
}

}