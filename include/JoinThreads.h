#ifndef JOIN_THREADS_H
#define JOIN_THREADS_H

#include <vector>
#include <thread>

class JoinThreads
{
public:
    JoinThreads() = delete;
    JoinThreads(const JoinThreads&) = delete;
    JoinThreads& operator=(const JoinThreads&) = delete;
    JoinThreads(JoinThreads&&) = delete;
    JoinThreads& operator=(JoinThreads&&) = delete;

    explicit JoinThreads(std::vector<std::thread>& t_threads)
        :
    m_threads(t_threads)
    {}

    ~JoinThreads()
    {
        for (auto &thread : m_threads)
        {
            if (thread.joinable())
            {
                thread.join();
            }
        }
    }

private:
    std::vector<std::thread>& m_threads;
};
#endif //JOIN_THREADS_H
