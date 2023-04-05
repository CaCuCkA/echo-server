#ifndef STATIC_THREADPOOL_H
#define STATIC_THREADPOOL_H

#include "ThreadSafeQueue.h"
#include "JoinThreads.h"
#include "FunctionWrapper.h"

#include <atomic>
#include <thread>
#include <future>

class StaticThreadPool
{
public:
    StaticThreadPool(const StaticThreadPool&) = delete;
    StaticThreadPool& operator=(const StaticThreadPool&) = delete;
    StaticThreadPool(StaticThreadPool&&) = delete;
    StaticThreadPool& operator=(StaticThreadPool&&) = delete;

    StaticThreadPool()
        :
    m_done(false), m_joiner(m_threads)
    {
        const uint16_t threadCount = std::thread::hardware_concurrency();
        try
        {
            for (uint16_t i = 0; i < threadCount; ++i)
            {
                m_threads.emplace_back(&StaticThreadPool::WorkerThread, this);
            }
        }
        catch (...)
        {
            m_done = true;
            throw;
        }
    }

    ~StaticThreadPool()
    {
        m_done = true;
    }

    template<typename FunctionType>
    std::future<typename  std::result_of<FunctionType()>::type>
    Submit(FunctionType function)
    {
        typedef typename std::result_of<FunctionType()>::type resultType;
        std::packaged_task<resultType()> task(std::move(function));
        std::future<resultType> result(task.get_future());
        m_work_queue.Enque(std::move(task));
        return result;
    }

private:
    void WorkerThread()
    {
        while(!m_done)
        {
            FunctionWrapper task;
            if (m_work_queue.TryDeque(task))
            {
                task();
            }
            else
            {
                std::this_thread::yield();
            }
        }
    }
private:
    std::atomic_bool m_done;
    ThreadSafeQueue<FunctionWrapper> m_work_queue;
    std::vector<std::thread> m_threads;
    JoinThreads m_joiner;
};
#endif //STATIC_THREADPOOL_H
