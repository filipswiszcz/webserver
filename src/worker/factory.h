#ifndef POOL_H
#define POOL_H

#include <vector>
#include <queue>

#include <functional>

#include <thread>
#include <mutex>
#include <condition_variable>

namespace worker {

    class Factory {

        public:

            Factory(const size_t num);

            ~Factory();

            void enqueue(std::function<void()> task);

        private:

            std::vector<std::thread> workers;
            std::queue<std::function<void()>> tasks;
            std::mutex tasks_guard;
            std::condition_variable cond;

            bool running;

    };

} // namespace worker

#endif