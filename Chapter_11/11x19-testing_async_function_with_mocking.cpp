#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <functional>

class AsyncTaskScheduler {
   public:
    virtual int runTask(std::function<int()> task) = 0;
};

class MockTaskScheduler : public AsyncTaskScheduler {
   public:
    MOCK_METHOD(int, runTask, (std::function<int()> task), (override));
};

TEST(AsyncTests, MockMethod) {
    using namespace testing;

    MockTaskScheduler scheduler;

    auto task = []() -> int {
        return 42;
    };

    EXPECT_CALL(scheduler, runTask(_)).WillOnce(
        Invoke(task)
    );

    auto result = scheduler.runTask(task);
    EXPECT_EQ(result, 42);
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
