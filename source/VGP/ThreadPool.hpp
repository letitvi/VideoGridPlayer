#pragma once

#include <memory>
#include <thread>
#include <functional>

#define ASIO_STANDALONE
#include <asio.hpp>

namespace vgp
{

typedef std::unique_ptr<asio::io_service::work> asio_work_ptr;
class ThreadPool
{
public:
    ThreadPool(size_t threadsNum)
        : m_service()
        , m_work(new asio_work_ptr::element_type(m_service))
        , m_strand(m_service)
    {
        auto f = std::bind(static_cast<size_t(asio::io_service::*)()>(&asio::io_service::run), &m_service);
        for (size_t i = 0; i < threadsNum; i++)
        {
            m_threads.push_back(std::thread(f));
        }
    }

    ~ThreadPool() 
    {
        m_work.reset();
        for (size_t i = 0 ; i < m_threads.size() ; ++i)
            m_threads[i].join();
        m_service.stop();
    }

    asio::io_service& GetService()
    {
        return m_service;
    }

    template<class F>
    void enqueue(F f)
    {
        m_service.post(f);
    }

    template<class F>
    void order_enqueue(F f)
    {
        m_strand.post(f);
    }

   
private:
    asio::io_service m_service;
    asio_work_ptr m_work;
    asio::io_service::strand m_strand;
    std::vector<std::thread> m_threads;


};

}