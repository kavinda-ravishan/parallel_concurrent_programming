#include <stdio.h>
#include <thread>
#include <mutex>
#include <tuple>

static int s_Count = 5000;

void work_deadLock(std::mutex &mutex_1, std::mutex &mutex_2)
{
    while (s_Count > 0)
    {
        mutex_1.lock();
        mutex_2.lock();

        if (s_Count)
            s_Count--;

        mutex_2.unlock();
        mutex_1.unlock();
    }
}

void work_scopeLock(std::mutex &mutex_1, std::mutex &mutex_2)
{
    while (s_Count > 0)
    {
        // /std:c++17
        std::scoped_lock lock(mutex_1, mutex_2); // unlock automatically

        if (s_Count)
            s_Count--;
    }
}

int main()
{
    std::mutex mutex_1, mutex_2;
    std::thread t1(work_scopeLock, std::ref(mutex_1), std::ref(mutex_2)); // 1, 2
    std::thread t2(work_scopeLock, std::ref(mutex_2), std::ref(mutex_1)); // 2, 1

    t1.join();
    t2.join();

    printf("Done\n");

    return 0;
}