#pragma once

#include <thread>
#include <set>
#include "i_process.hpp"
#include "types.hpp"
#include "sched.h"
#include "pthread.h"
#include <cstdio>
#include "utils.hpp"
#include <iostream>

namespace femto
{
    struct ThreadConfiguration
    {
        std::set<uint32_t> affinity{}; // Affinity set (cpu cores)
        int32_t priority{};            // Thread priority
    };

    template <typename TConfig>
    class ThreadProcess : public IConfigurable<ThreadConfiguration>
    {
    private:
        ThreadConfiguration m_config;
        std::thread m_thread;

    public:
        ThreadProcess(uptr_t<IProcess<TConfig>> process, const ThreadConfiguration &config) : {}
        ~ThreadProcess();

        virtual void configure(const ThreadConfiguration &config) override
        {
            if (!utils::set_thread_affinity(m_thread.native_handle(), config.affinity))
            {
                std::cerr << "Failed to set thread affinity\n";
            }

            if (!utils::set_thread_priority(m_thread, SCHED_OTHER, config.priority))
            {
                std::cerr << "Failed to set thread priority\n";
            }
        }
    };

} // namespace femto
