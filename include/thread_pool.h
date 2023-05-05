#pragma once

#include <vector>
#include <queue>
#include <memory>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <future>
#include <functional>

class ThreadPool
{
public:
    ThreadPool(int threads);
    template <class F, class... Args>
    void enqueue(F &&function, Args &&...args);
    ~ThreadPool();

private:
    std::vector<std::thread> _workers;
    std::queue<std::function<void()>> _tasks;
    std::condition_variable _condition;
    std::mutex _queue_mutex;
    bool _stop = false;
};
