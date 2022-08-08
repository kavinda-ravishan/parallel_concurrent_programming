
#include <stdio.h>
#include <thread>
#include <mutex>

static int s_Count = 1000000;

void work_livelock(std::mutex &m1, std::mutex &m2)
{
    while (s_Count > 0)
    {
        m1.lock();
        if (!m2.try_lock())
        {
            m1.unlock();
            continue;
        }
        if (s_Count)
        {
            s_Count--;
        }
        m2.unlock();
        m1.unlock();
    }
}

void work_livelock_fixed(std::mutex &m1, std::mutex &m2)
{
    while (s_Count > 0)
    {
        m1.lock();
        if (!m2.try_lock())
        {
            m1.unlock();
            // reschedule the execution of threads, allowing other threads to run.
            std::this_thread::yield();
            continue;
        }
        if (s_Count)
        {
            s_Count--;
        }
        m2.unlock();
        m1.unlock();
    }
}

int main()
{
    std::mutex m1, m2;

    auto fun = work_livelock_fixed;

    std::thread t1(fun, std::ref(m1), std::ref(m2));
    std::thread t2(fun, std::ref(m2), std::ref(m1));
    std::thread t3(fun, std::ref(m1), std::ref(m2));
    std::thread t4(fun, std::ref(m2), std::ref(m1));

    t1.join();
    t2.join();
    t3.join();
    t4.join();

    printf("Count : %d\n", s_Count);

    return 0;
}