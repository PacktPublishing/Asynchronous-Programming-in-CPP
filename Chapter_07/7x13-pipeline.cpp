#include <future>
#include <iostream>
#include <syncstream>
#include <vector>

using namespace std::chrono_literals;

#define sync_cout std::osyncstream(std::cout)

template <typename Func>
class Task {
   public:
    // Constructor only accepting a function.
    // No dependencies with previous tasks.
    Task(int id, Func& func) : id_(id), func_(func), has_dependency_(false) {
        sync_cout << "Task " << id << " constructed without dependencies.\n";
        fut_ = std::async(std::launch::deferred, [this](){ (*this)(); });
    }

    // Constructor accepting a variable number of std::future<void>
    // defining dependencies with previous tasks.
    template <typename... Futures>
    Task(int id, Func& func, Futures&&... futures) : id_(id), func_(func), has_dependency_(true) {
        sync_cout << "Task " << id << " constructed with dependencies.\n";
        fut_ = std::async(std::launch::deferred, [this](){ (*this)(); });
        add_dependencies(std::forward<Futures>(futures)...);
    }

    // Return future for this task to create dependency chain.
    std::shared_future<void> get_dependency() {
        sync_cout << "Getting future from task " << id_ << std::endl;
        return fut_.share();
    }

    // Run the task waiting for dependant tasks.
    void operator()() {
        sync_cout << "Starting task " << id_ << std::endl;

        // Wait for each future to be completed
        wait_completion();

        // Run task function.
        sync_cout << "Running task " << id_ << std::endl;
        func_();
    }

    // Start asynchronous task
    void start() {
        fut_.get();
    }

   private:
    // Add futures to the futures vector using variadic templates.
    template <typename... Futures>
    void add_dependencies(Futures&&... futures) {
        (deps_.push_back(futures), ...);
    }

    // Wait for tasks that depends on to be completed.
    void wait_completion() {
        sync_cout << "Waiting completion for task " << id_ << std::endl;
        if (!deps_.empty()) {
            for (auto& fut : deps_) {
                if (fut.valid()) {
                    sync_cout << "Fut valid so getting value in task " << id_ << std::endl;
                    fut.get();
                }
            }
        }
    }

   private:
    int id_;        
    Func& func_;
    std::future<void> fut_;
    std::vector<std::shared_future<void>> deps_;
    bool has_dependency_;
};

int main() {
    // Tasks to run in the different pipeline steps
    auto sleep1s = []() { std::this_thread::sleep_for(1s); };
    auto sleep2s = []() { std::this_thread::sleep_for(2s); };

    // Tasks creation.
    Task task1(1, sleep1s);
    Task task2(2, sleep2s, task1.get_dependency());
    Task task3(3, sleep1s, task2.get_dependency());
    Task task4(4, sleep2s, task2.get_dependency());
    Task task5(5, sleep2s, task3.get_dependency(), task4.get_dependency());    

    // Starting the pipeline
    // Launch last tasks. It will recursivelly call their dependency tasks by
    // using the dependency future and trigger the deferred asynchronous tasks.
    sync_cout << "Starting the pipeline..." << std::endl;
    task5.start();

    sync_cout << "All done!" << std::endl;
    return 0;
}
