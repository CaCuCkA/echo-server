#ifndef THREAD_SAFE_QUEUE_H
#define THREAD_SAFE_QUEUE_H

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif // defined(_MSC_VER) && (_MSC_VER >= 1200)

#include <mutex>
#include <condition_variable>
#include <thread>
#include <deque>

template<class T>
class ThreadSafeQueue
{
public:
    ThreadSafeQueue() = default;
    ~ThreadSafeQueue() = default;
    ThreadSafeQueue(const ThreadSafeQueue&) = delete;
    ThreadSafeQueue& operator=(const ThreadSafeQueue&) = delete;
    ThreadSafeQueue(ThreadSafeQueue&&) = delete;
    ThreadSafeQueue& operator=(ThreadSafeQueue&&) = delete;

    void push(T& val)
    {
        {
            std::unique_lock<std::mutex> lock(m_mutex);
            m_buffer.push_front(std::move(val));
        }
        m_not_empty.notify_one();
    }

    void pop(T& val)
    {
        std::unique_lock<std::mutex> lock(m_mutex);
        while (m_buffer.empty())
        {
            m_not_empty.wait(lock);
        }
        val = std::move(m_buffer.back());
        m_buffer.pop_back();
    }

private:
    std::deque<T> m_buffer;
    std::condition_variable m_not_empty;
    mutable std::mutex m_mutex;
};

#endif //THREAD_SAFE_QUEUE_H
