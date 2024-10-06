#include <gtest/gtest.h>
#include <functional>

class AsyncTaskScheduler {
   public:
    virtual int runTask(std::function<int()> task) = 0;
};

class MockTaskScheduler : public AsyncTaskScheduler {
   public:
    int runTask(std::function<int()> task) override {
        // Directly run the task for testing
        return task();
    }
};

TEST(AsyncTests, DependencyInjection) {
    MockTaskScheduler scheduler;

    auto task = []() -> int {
        return 42;
    };

    int result = scheduler.runTask(task);
    EXPECT_EQ(result, 42);
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}