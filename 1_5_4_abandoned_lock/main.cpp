#include <stdio.h>
#include <thread>
#include <mutex>

namespace kvn
{
    class scoped_lock
    {
        std::mutex &m;

    public:
        scoped_lock(std::mutex &m) : m(m)
        {
            m.lock();
        }
        ~scoped_lock()
        {
            m.unlock();
        }
    };
}

static int s_count = 5000;

void work_abandoned_lock(std::mutex &mutex, const char *name)
{
    while (s_count > 0)
    {
        mutex.lock();
        if (s_count)
        {
            s_count--;
        }
        if (s_count == 10)
        {
            printf("%s stoped working.\n", name);
            return;
        }
        mutex.unlock();
    }
}

void work_abandoned_lock_fixed(std::mutex &mutex, const char *name)
{
    while (s_count > 0)
    {
        // std::scoped_lock lock(mutex);
        kvn::scoped_lock lock(mutex);
        if (s_count)
        {
            s_count--;
        }
        if (s_count == 10)
        {
            printf("%s stoped working.\n", name);
            return;
            // scoped_lock, unlock automatically due to out of scope
        }
    }
}

int main()
{
    std::mutex mutex;
    std::thread t1(work_abandoned_lock_fixed, std::ref<std::mutex>(mutex), "t1");
    std::thread t2(work_abandoned_lock_fixed, std::ref<std::mutex>(mutex), "t2");
    t1.join();
    t2.join();

    printf("Count : %d\n", s_count);

    return 0;
}