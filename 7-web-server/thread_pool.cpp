#include <iostream>
#include <vector>
#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <atomic>
#include <functional>
#include <chrono>


class ThreadPool {
private:
    using Task = std::function<void()>;
    std::vector<std::thread> workers;
    std::queue<Task> tasks;

    std::mutex queue_mutex;
    std::condition_variable condition;
    std::atomic<bool> stop;

public:
    ThreadPool(size_t _threads) : stop(false) {
        for (int i = 0; i < _threads; ++i) {
            workers.emplace_back([this]() {
                while (true) {
                    std::function<void()> task;

                    {
                        std::unique_lock<std::mutex> lock(this->queue_mutex);
                        this->condition.wait(lock, [this]() {
                            return this->stop || !this->tasks.empty();
                        });

                        if (this->stop && this->tasks.empty()) {
                            return;
                        }

                        task = std::move(this->tasks.front());
                        this->tasks.pop();
                    }

                    task();
                }
            });
        }
    }

    void enqueue(std::function<void()> task) {
        {
            std::unique_lock<std::mutex> lock(queue_mutex);
            if (stop) throw std::runtime_error("Called enqueue on stopped pool.");
            tasks.emplace(std::move(task));
        }
        condition.notify_one();
    }

    ~ThreadPool() {
        {
            std::unique_lock<std::mutex> lock(queue_mutex);
            stop = true;
            lock.unlock();
        }

        condition.notify_all();
        for (std::thread &worker: workers) {
            worker.join();
        }
    }
};

int main() {
    std::cout << "Hello from thread pool\n";
    ThreadPool threadPool{2};
    threadPool.enqueue([] {
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
        std::cout << "First task\n";
    });
    threadPool.enqueue([] {
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
        std::cout << "Second task\n";
    });
    threadPool.enqueue([] {
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
        std::cout << "Third task\n";
    });
    threadPool.enqueue([] {
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
        std::cout << "Fourth task\n";
    });
    return 0;
}