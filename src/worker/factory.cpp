#include "factory.h"

#pragma mark - Factory
#pragma region Factory {

namespace worker {

    Factory::Factory(const size_t num) {

        this -> running = true;

        for (size_t i = 0; i < num; i++) {
            this -> workers.emplace_back([this] {
                while (true) {
                    std::function<void()> t;

                    {
                        std::unique_lock<std::mutex> guard(this -> tasks_guard);

                        this -> cond.wait(guard, [this] {return !tasks.empty() || !running;});

                        if (running && this -> tasks.empty()) return;

                        t = std::move(this -> tasks.front()); this -> tasks.pop(); guard.unlock();
                    }

                    t();
                }
            });
        }
    }

    Factory::~Factory() {

        {
            std::unique_lock<std::mutex> guard(this -> tasks_guard); this -> running = false;
        }

        this -> cond.notify_all();

        for (auto& worker : this -> workers) {worker.join();}
    }

    void Factory::enqueue(std::function<void()> task) {

        {
            std::unique_lock<std::mutex> guard(this -> tasks_guard);
            this -> tasks.emplace(std::move(task));
        }

        this -> cond.notify_all();
    }

} // namespace worker

#pragma endregion Factory }