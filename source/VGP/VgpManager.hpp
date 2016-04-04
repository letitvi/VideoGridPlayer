#pragma once

#include <memory>
#include <string>
#include <list>
#include <memory>
#include <thread>
#include <atomic>
#include <mutex> 
#include <functional>
#include <condition_variable>

#include <SDL.h>
#include <spdlog/spdlog.h>
#include "WindowContext.hpp"
#include "ThreadPool.hpp"


namespace vgp
{

struct VgpControl
{
    std::mutex m;
    std::condition_variable cv;
    std::function <void()> f;
};

typedef std::shared_ptr<VgpControl> VgpControlPtr;

class VgpManager : public IRenderer
{
public:
    static VgpManager& GetInstance();
    std::shared_ptr<spdlog::logger> GetLogger()
    {
        return m_logger;
    }

    unsigned int NewWindowID()
    {
        return m_lastWindowID++;
    }

    void NewWindow(
        const unsigned int windowID,
        const size_t windowWidth,
        const size_t windowHeight,
        const std::string windowTitle);

    void NewWindowFrom(
        const unsigned int windowID,
        void *windowHandle);

    void DeleteWindow(const unsigned int windowID);

private:
    void PushMessage(VgpControlPtr& ptr);
    void ExecuteMessage();

    void HandleEvent(SDL_Event &e);
    void Render();
    void Update();

private:
    void NewWindowInternal(
        const unsigned int windowID,
        const size_t windowWidth,
        const size_t windowHeight,
        const std::string windowTitle);

    void NewWindowFromInternal(
        const unsigned int windowID,
        void *windowHandle);

    void DeleteWindowInternal(const unsigned int windowID);
    

private:
    void Initialize();
    void PrintDeviceInformation();
    std::shared_ptr<spdlog::logger> m_logger;
    

    std::list<WindowContextPtr> m_windows;
    std::atomic<unsigned int> m_lastWindowID;

    std::vector<VgpControlPtr> m_controlQueue;
    std::mutex m_controlQueueMutex;

    unsigned int m_refreshRate;
    enum {kDefaultRefreshRate = 30};

private:
    ThreadPool m_readerThreads;
    ThreadPool m_decoderThreads;

    std::thread m_renderThread;
    void ThreadMain();
    std::atomic<bool> m_isThreadRunning;
private:
    VgpManager();
    ~VgpManager();
    VgpManager(const VgpManager&) = delete;
    VgpManager& operator=(const VgpManager&) = delete;
};

}