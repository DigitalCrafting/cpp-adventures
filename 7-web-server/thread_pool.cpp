#include <iostream>
#include <vector>
#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <atomic>
#include <functional>


class ThreadPool {
private:
    using Task = std::function<void()>;
    std::vector<std::thread> workers;
    std::queue<Task> tasks;

    std::mutex queue_mutex;
    std::condition_variable condition;
    std::atomic<bool> stop;

public:
    ThreadPool(size_t _threads): stop(false) {
        for (int i = 0; i < _threads; ++i) {
            // TODO
            std::cout << "Initializing thread " << i << " of ThreadPool\n";
        }
    }

    ~ThreadPool() {
        {
            std::unique_lock<std::mutex> lock(queue_mutex);
            stop = true;
            lock.unlock();
        }

        condition.notify_all();
        for (std::thread &worker : workers) {
            worker.join();
        }
    }
};

int main() {
    std::cout << "Hello from thread pool\n";
    ThreadPool threadPool{2};
    return 0;
}