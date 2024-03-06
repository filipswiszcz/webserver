#include "factory.h"

#pragma mark - Factory
#pragma region Factory {

    Factory::Factory(const size_t num) {

        for (size_t i = 0; i < num; i++) {
            workers.emplace_back([this] {
                while (true) {
                    std::function<void()> t;

                    {
                        std::unique_lock<std::mutex> guard(tasks_guard);

                        cond.wait(guard, [this] {return !tasks.empty() || stop;});

                        if (stop && tasks.empty()) return;

                        t = std::move(tasks.front()); tasks.pop();
                    }

                    t();
                }
            });
        }
    }

    Factory::~Factory() {

        {
            std::unique_lock<std::mutex> guard(tasks_guard); stop = true;
        }

        cond.notify_all();

        for (auto& worker : workers) {worker.join();}
    }

    void Factory::enqueue(std::function<void()> task) {

        {
            std::unique_lock<std::mutex> guard(tasks_guard);
            tasks.emplace(std::move(task));
        }

        cond.notify_all();
    }


#pragma endregion Factory }