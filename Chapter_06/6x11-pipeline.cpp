#include <future>
#include <iostream>
#include <syncstream>
#include <vector>

using namespace std::literals;

#define sync_cout std::osyncstream(std::cout)

template <typename Func>
class Task {
   public:
    // Constructor only accepting a function.
    // No dependencies with previous tasks.
    Task(int id, Func& func) : id_(id), func_(func), has_dependency_(false) {
        sync_cout << "Task " << id << " constructed without dependencies." << std::endl;
        fut_ = prom_.get_future().share();
    }

    // Constructor accepting a variable number of std::future<void>
    // defining dependencies with previous tasks.
    template <typename... Futures>
    Task(int id, Func& func, Futures&&... futures) : id_(id), func_(func), has_dependency_(true) {
        sync_cout << "Task " << id << " constructed with dependencies." << std::endl;
        fut_ = prom_.get_future().share();
        add_dependencies(std::forward<Futures>(futures)...);
    }

    // Return future for this task to create dependency chain.
    std::shared_future<void> get_dependency() {
        sync_cout << "Getting future from task " << id_ << std::endl;
        return fut_;
    }

    // Run the task waiting for dependant tasks.
    void operator()() {
        sync_cout << "Running task " << id_ << std::endl;

        // Wait for each future to be completed
        wait_completion();

        // Run task function.
        func_();

        // Signal dependent tasks about completion.
        sync_cout << "Signaling completion of task " << id_ << std::endl;
        prom_.set_value();
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

    std::promise<void> prom_;                     // Signal task completion
    std::shared_future<void> fut_;                // Own future to share with dependant tasks
    std::vector<std::shared_future<void>> deps_;  // Wait for tasks that this task depends on for completion

    bool has_dependency_;
};

int main() {
    // Tasks to run in the different pipeline steps
    auto sleep1s = []() { std::this_thread::sleep_for(1s); };
    auto sleep2s = []() { std::this_thread::sleep_for(2s); };

    // Tasks creation.
    Task task1(1, sleep1s);
    Task task2(2, sleep1s, task1.get_dependency());
    Task task3(3, sleep1s, task2.get_dependency());
    Task task4(4, sleep2s, task3.get_dependency());
    Task task5(5, sleep2s, task3.get_dependency(), task4.get_dependency());    

    // Starting the pipeline
    // Launch all tasks. They will be waiting for their dependency promise to be triggered.
    // As Task #1 (tasks[0]) has no dependencies, it will start straight away.
    // Other tasks will be waiting until other previous tasks finish.
    sync_cout << "Starting the pipeline..." << std::endl;
    task1();
    task2();
    task3();
    task4();
    task5();

    // Wait for pipeline to complete by checking the future of last task.
    sync_cout << "Waiting for the pipeline to finish..." << std::endl;    
    auto finish_pipeline_fut = task5.get_dependency();
    finish_pipeline_fut.get();

    sync_cout << "All done!" << std::endl;
    return 0;
}
