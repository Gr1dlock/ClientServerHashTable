#include "thread_pool.h"

ThreadPool::ThreadPool(int threads)
{
    for (int i = 0; i < threads; i++)
    {
        _workers.emplace_back(
            [this]
            {
                while (true)
                {
                    std::function<void()> task;

                    {
                        std::unique_lock<std::mutex> lock(this->_queue_mutex);
                        this->_condition.wait(lock, [this]
                                              { return this->_stop || !this->_tasks.empty(); });
                        if (this->_stop && this->_tasks.empty())
                        {
                            return;
                        }
                        task = std::move(this->_tasks.front());
                        this->_tasks.pop();
                    }

                    task();
                }
            });
    }
}

ThreadPool::~ThreadPool()
{
    {
        std::unique_lock<std::mutex> lock(_queue_mutex);
        _stop = true;
    }
    _condition.notify_all();
    for (std::thread &worker : _workers)
    {
        worker.join();
    }
}

template <class F, class... Args>
void ThreadPool::enqueue(F &&function, Args &&...args)
{
    auto task = std::make_shared<std::packaged_task<void()>>(
        std::bind(std::forward<F>(function), std::forward<Args>(args)...));
    {
        std::unique_lock<std::mutex> lock(_queue_mutex);
        _tasks.emplace([task]
                      { (*task)(); });
    }
    _condition.notify_one();
}