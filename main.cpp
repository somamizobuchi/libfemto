#include "femto.hpp"
#include <iostream>

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
        std::this_thread::sleep_for(std::chrono::seconds(1));
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

int main()
{
    TestWorker worker;
    worker.initialize();
    std::this_thread::sleep_for(std::chrono::seconds(1));
    worker.pause();
    std::this_thread::sleep_for(std::chrono::seconds(1));
    worker.resume();
    std::this_thread::sleep_for(std::chrono::seconds(1));
    worker.shutdown();
    return 0;
}