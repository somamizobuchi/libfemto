#include <gtest/gtest.h>
#include "femto.hpp"

class TestWorker : public femto::ThreadWorker<int>
{

public:
    TestWorker()
    {
        std::cout << "ctor\n";
    };

    virtual ~TestWorker() = default;

    virtual femto::result_t configure(const int &config) override
    {
        std::cout << "config\n";
        return femto::result_t();
    }

    int get_configuration() const override
    {
        return -1;
    }

protected:
    void main_loop() override
    {
        std::cout << "main\n";
        // std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    void on_initialize() override
    {
        std::cout << "init\n";
    }

    void on_pause() override
    {
        std::cout << "pause\n";
    }

    void on_resume() override
    {
        std::cout << "resume\n";
    }

    void on_shutdown() override
    {
        std::cout << "shutdown\n";
    }

    void on_configuration(const int &config) override
    {
        std::cout << "config\n";
    }
};

TEST(ThreadTest, Test1)
{
    femto::result_t result = femto::result_t();
    EXPECT_EQ(result.has_value(), true);
}

TEST(ThreadTest, Test2)
{
    TestWorker worker;
    worker.initialize();
    // std::cout << "pause\n";
    // worker.pause();
    // std::cout << "resume\n";
    // worker.resume();
    worker.shutdown();
}

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}