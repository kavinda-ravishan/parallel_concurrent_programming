#include <stdio.h>
#include <thread>
#include <mutex>
#include <array>

static int s_Count = 5000;

namespace kvn
{
    class scoped_lock
    {
    private:
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

} // namespace kvn

void work(std::mutex &mutex, unsigned int id)
{
    int count = 0;
    while (s_Count > 0)
    {
        kvn::scoped_lock lock(mutex);
        if (s_Count)
        {
            s_Count--;
            count++;
        }
    }

    printf("Thread = %u, count : %d\n", id, count);
}

int main()
{
    std::mutex mutex;

    std::array<std::thread, 10> threads;
    for (int i = 0; i < threads.size(); i++)
    {
        threads[i] = std::thread(work, std::ref(mutex), i);
    }

    for (int i = 0; i < threads.size(); i++)
    {
        threads[i].join();
    }

    return 0;
}