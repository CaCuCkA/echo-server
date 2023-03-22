#ifndef THREAD_SAFE_QUEUE_H
#define THREAD_SAFE_QUEUE_H

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

    void Enque(T&& val)
    {
        {
            std::unique_lock lock(m_mutex);
            m_buffer.push_front(std::move(val));
        }
        m_not_empty.notify_one();
    }

    bool TryDeque(T& val)
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        if (m_buffer.empty())
        {
            return false;
        }
        val = std::move(m_buffer.back());
        m_buffer.pop_back();
        return true;
    }

private:
    std::deque<T> m_buffer;
    std::condition_variable m_not_empty;
    mutable std::mutex m_mutex;
};
#endif //THREAD_SAFE_QUEUE_H
