#include <functional>
#include <iostream>
#include <string>
#include <vector>

// Concurrency headers
#include <atomic>
#include <cassert>
#include <chrono>
#include <condition_variable>
#include <deque>
#include <mutex>
#include <queue>
#include <thread>

class ThreadPool {
   public:
    using Task = std::function<void()>;

    // Create a thread pool with N worker threads
    ThreadPool(size_t n) {
        for (size_t i = 0; i < n; ++i) {
            this->add_worker();
        }
    }

    // By default create thread pool with number of threads given by the hardware concurrency.
    ThreadPool() : ThreadPool(std::thread::hardware_concurrency()) {}

    ~ThreadPool() { this->shutdown(); }

    // Return the number of worker threads
    const size_t size() const { return worker_threads_.size(); }

    // Submit a task to the thread pool
    void submit(Task task) {
        auto guard = std::lock_guard<std::mutex>{queue_mutex_};
        task_queue_.push(task);
        // Wake up a single worker thread signaling it.
        cond_var_tasks_.notify_one();
    }

    // Force ThreadPool shutdown
    void shutdown() {
        // Set the flag to false for indicating to worker threads that
        // there nor further tasks(jobs) will be submited.
        is_active_ = false;
        // Signal all threads, wake up all worker threads
        cond_var_tasks_.notify_all();
        // Wait for completion of all threads.
        for (auto& worker : worker_threads_) {
            worker.join();
        }
        // Remove all worker threads
        worker_threads_.clear();
    }

    // Block the calling thread and waits for the completion of all tasks.
    void wait_tasks() {
        auto lock = std::unique_lock<std::mutex>{queue_mutex_};
        cond_var_empty_.wait(lock, [&] { return task_queue_.empty(); });
        lock.unlock();
    }

    // Pop a task from queue => blocking the calling thread
    // if the queue is empty.
    Task pop_task() {
        auto lock = std::unique_lock<std::mutex>{queue_mutex_};
        // Block current thread if the queue is empty or until
        // the condition variable is signaled from the queue
        // thread.
        cond_var_tasks_.wait(lock, [&] { return !task_queue_.empty(); });
        auto task = task_queue_.front();
        task_queue_.pop();

        // Wake up thread that called .wait_tasks()
        if (task_queue_.empty()) {
            cond_var_empty_.notify_all();
        }

        // lock.unlock();
        return task;
    }

    // Add new worker thread
    void add_worker() {
        auto th = std::thread([this] {
            while (true) {
                // std::puts(" [INFO] Waiting task");
                auto task = pop_task();
                task();

                // Shutdown worker thread when the flag is_active_
                // is set to false and the queue is empty
                auto guard = std::lock_guard<std::mutex>{queue_mutex_};
                if (!is_active_ && task_queue_.empty()) {
                    return;
                }
            }
        });

        std::cout << " [TRACE] <add_worker> - Created worker thread ID = " << th.get_id() << std::endl;

        worker_threads_.push_back(std::move(th));

    }  // --- End of add_worker() --- //

   private:
    // Flag indicating that the thread pool is active (running)
    std::atomic<bool> is_active_ = true;
    // Task queue (message queue)
    std::queue<Task> task_queue_;
    // Worker threads
    std::deque<std::thread> worker_threads_;
    // Mutex for protecting the queue
    std::mutex queue_mutex_;
    // Condition variable for signaling that
    // there are tasks in the queue
    std::condition_variable cond_var_tasks_;

    // Condition variable for signaling that the task queue is empty
    std::condition_variable cond_var_empty_;
};

int main(int argc, char** argv) {
    using namespace std::chrono_literals;

    if (argc < 3) {
        std::cerr << " Error: invalid command line switch." << std::endl;
        return EXIT_FAILURE;
    }

    auto cmd = std::string(argv[1]);
    auto size = std::stoi(argv[2]);
    assert(size > 0);

    if (cmd == "serial") {
        for (int i = 0; i < size; i++) {
            std::this_thread::sleep_for(std::chrono::seconds(2));
            std::cout << " Task id = " << i << " thread id = " << std::this_thread::get_id() << " completed OK. \n";
        }
        return EXIT_FAILURE;
    }

    if (cmd == "parallel") {
        // Get thread pool size from environemnt variable
        const char* thsize = std::getenv("THREAD_POOL_SIZE");

        // std::cout << " thsize = " << thsize << std::endl;

        int n_workers = thsize != nullptr ? std::stoi(thsize) : std::thread::hardware_concurrency();

        ThreadPool thp(n_workers);
        // Mutex used for protecting std::cout from data race
        std::mutex cout_mutex;

        // Run
        for (int i = 0; i < size; i++) {
            thp.submit([=, &cout_mutex] {
                std::this_thread::sleep_for(std::chrono::seconds(2));

                {
                    std::lock_guard<std::mutex> guard(cout_mutex);
                    std::cout << " Task id = " << i << " thread id = " << std::this_thread::get_id()
                              << " completed OK. \n";
                }
            });
        }
        std::cout << " [TRACE] <main> Waiting tasks execution ..."
                  << "\n";
    }

    return 0;
}
