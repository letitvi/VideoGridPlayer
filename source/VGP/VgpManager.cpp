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
    , m_readerThreads(4)
    , m_decoderThreads(4)
    , m_renderThread(std::thread(&VgpManager::ThreadMain, this))
{

}


void VgpManager::NewWindow(const unsigned int windowID,
                          const size_t windowWidth,
                          const size_t windowHeight,
                          const std::string windowTitle)
{
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
    m_windows.push_back(window);
}

void VgpManager::NewWindowFromInternal(const unsigned int windowID,
                                       void *windowHandle)
{
    WindowContextPtr window = std::make_shared<WindowContext>(windowID, windowHandle);
    m_windows.push_back(window);
}


void VgpManager::DeleteWindowInternal(const unsigned int windowID)
{
    m_windows.remove_if([windowID](WindowContextPtr p) { return p->GetWindowID() == windowID; });
}

void VgpManager::Initialize()
{
    //auto console = spdlog::stdout_logger_mt("console");

    std::vector<spdlog::sink_ptr> sinks;
    sinks.push_back(std::make_shared<spdlog::sinks::stdout_sink_mt>());
    sinks.push_back(std::make_shared<spdlog::sinks::rotating_file_sink_mt>("vgp", "log", 1048576 * 5, 3));
    m_logger = std::make_shared<spdlog::logger>("vgp", begin(sinks), end(sinks));
    spdlog::register_logger(m_logger);

    spdlog::set_level(spdlog::level::trace);
    spdlog::set_pattern("*** [%H:%M:%S %z] [thread %t] %v ***");

    m_logger->info("============================ VgpManager() START");

    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        m_logger->critical("SDL_Init() failed : ") << SDL_GetError();
    }
    if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1")) {
        m_logger->critical("Unable to Init hinting: ") << SDL_GetError();
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
    if (m_logger->level() <= spdlog::level::debug) {
        m_logger->debug()
            << "platform :: " << SDL_GetPlatform() << "\r\n"
            << "L1 cache size :: " << SDL_GetCPUCacheLineSize() << "kbytes" << "\r\n"
            << "cpu num :: " << SDL_GetCPUCount() << "\r\n"
            << "memory size :: " << SDL_GetSystemRAM() << "MB" << "\r\n"
            << "3D Now :: " << SDL_Has3DNow() << "\r\n"
            << "AVX :: " << SDL_HasAVX() << "\r\n"
            << "AltiVec :: " << SDL_HasAltiVec() << "\r\n"
            << "MMX :: " << SDL_HasMMX() << "\r\n"
            << "RDTSC :: " << SDL_HasRDTSC() << "\r\n"
            << "SSE :: " << SDL_HasSSE() << "\r\n"
            << "SSE2 :: " << SDL_HasSSE2() << "\r\n"
            << "SSE3 :: " << SDL_HasSSE3() << "\r\n"
            << "SSE41 :: " << SDL_HasSSE41() << "\r\n"
            << "SSE42 :: " << SDL_HasSSE42() << "\r\n";
    }
}


VgpManager::~VgpManager()
{
    m_logger->info("-------------------------------- VgpManager() END");
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
        w->Update();
}

void VgpManager::Render()
{
    for (auto w : m_windows)
        w->Render();
}

}