#define _GNU_SOURCE
#include <thread>
#include <set>
#include <pthread.h>
#include <sched.h>
#include <mach/mach.h>

namespace femto::utils
{
    bool set_thread_affinity(const pthread_t &thread, const std::set<uint32_t> &cpus)
    {
#ifdef __linux__
        cpu_set_t cpu_set;
        CPU_ZERO(&cpu_set);

        for (auto &cpu : cpus)
        {
            if (cpu < std::thread::hardware_concurrency())
            {
                CPU_SET(cpu, &cpu_set);
            }
        }

        return (pthread_setaffinity_np(thread, sizeof(cpu_set_t), &cpu_set) == 0);
#else
        return false;
#endif
    }

    bool set_thread_priority(std::thread &th, int32_t policy, int32_t priority)
    {
        struct sched_param sch_params;
        memset(&sch_params, 0, sizeof(struct sched_param));
        sch_params.sched_priority = priority;
        return (pthread_setschedparam(th.native_handle(), policy, &sch_params) == 0);
    }
} // namespace femto
