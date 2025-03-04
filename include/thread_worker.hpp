#include "i_process.hpp"
#include <mutex>
#include <condition_variable>

namespace femto
{
    /// @brief Interface for a process of execution
    /// @tparam TConfig the configuration type
    template <typename TConfig>
    class ThreadWorker : public ILifecycle, public IConfigurable<TConfig>
    {
    private:
        std::thread m_thread;
        std::atomic<bool> m_quit;
        std::atomic<LifecycleState> m_state{LifecycleState::UNINITIALIZED};
        std::condition_variable m_cv;
        std::mutex m_mutex;
        std::optional<TConfig> m_config_requested;

    public:
        virtual result_t initialize() override
        {
            if (m_state.load(std::memory_order_acquire) != LifecycleState::UNINITIALIZED)
                return std::unexpected("ThreadWorker already initialized");

            m_thread = std::thread(&ThreadWorker::_thread_function, this);
            return result_t();
        };

        virtual result_t pause() override
        {
            // return if already paused
            if (m_state.load(std::memory_order_acquire) == LifecycleState::PAUSED)
                return result_t();

            // return unexpected if not running
            if (m_state.load(std::memory_order_acquire) != LifecycleState::RUNNING)
                return std::unexpected("Pause called invalid state");

            m_state.store(LifecycleState::PAUSED, std::memory_order_release);
            return result_t();
        };

        virtual result_t resume() override
        {
            // return if already running
            if (m_state.load(std::memory_order_acquire) == LifecycleState::RUNNING)
                return result_t();

            // return unexpected if not paused
            if (m_state.load(std::memory_order_acquire) != LifecycleState::PAUSED)
                return std::unexpected("Resume called invalid state");

            m_state.store(LifecycleState::RESUME, std::memory_order_release);
            m_cv.notify_one();

            return result_t();
        };

        virtual result_t shutdown() override
        {
            m_quit.store(true, std::memory_order_release);
            if (m_thread.joinable())
                m_thread.join();
            return result_t();
        };

        virtual result_t configure(const TConfig &config) override
        {
            std::lock_guard<std::mutex> lock(m_mutex);
            if (m_config_requested.has_value())
                return std::unexpected("Unprocessed configuration");
            m_config_requested.emplace(config);
            return result_t();
        };

        virtual ~ThreadWorker()
        {
            if (m_thread.joinable())
                m_thread.join();
        };

    protected:
        /// @brief Main loop of the worker
        virtual void main_loop() = 0;

        /// @brief Called upon initialization in the worker thread
        virtual void on_initialize() = 0;

        /// @brief Called upon pausing the worker in the worker thread
        virtual void on_pause() = 0;

        /// @brief Called upon resuming the worker in the worker thread
        virtual void on_resume() = 0;

        /// @brief Called upon shutting
        virtual void on_shutdown() = 0;

        /// @brief Called upon configuration
        /// @param config the configuration
        virtual void on_configuration(const TConfig &config) = 0;

    private:
        void _thread_function()
        {
            _initialize();

            while (!m_quit)
                _main_loop();

            _shutdown();
        }

        void _main_loop()
        {
            switch (m_state.load(std::memory_order_acquire))
            {
            case LifecycleState::PAUSED:
                _pause();
                break;
            case LifecycleState::RESUME:
                _resume();
                break;
            case LifecycleState::RUNNING:
                main_loop();
                break;
            default:
                break;
            }
        };

        void _initialize()
        {
            on_initialize();
            m_state.store(LifecycleState::RUNNING, std::memory_order_release);
        };

        void _pause()
        {
            on_pause();
            m_state.store(LifecycleState::PAUSED, std::memory_order_release);
            do
            {
                std::unique_lock<std::mutex> lock(m_mutex);
                auto status = m_cv.wait_for(lock, std::chrono::milliseconds(100), [this]()
                                            { return (m_state.load(std::memory_order_acquire) == LifecycleState::RESUME) || (m_quit.load(std::memory_order_acquire)); });
                if (status)
                    break;
                _configure();
            } while (m_state.load(std::memory_order_acquire) == LifecycleState::PAUSED);
        };

        void _resume()
        {
            on_resume();
            m_state.store(LifecycleState::RUNNING, std::memory_order_release);
        };

        void _configure()
        {
            std::lock_guard<std::mutex> lock(m_mutex);
            if (m_config_requested.has_value())
            {
                on_configuration(m_config_requested.value());
                m_config_requested.reset();
            }
        };

        void _shutdown()
        {
            on_shutdown();
            m_state.store(LifecycleState::SHUTDOWN, std::memory_order_release);
        };
    };

} // namespace femto