#include <thread>
#include <mutex>
#include <array>

unsigned int result = 1;
std::mutex mutex;

template <class mutex>
class scoped_lock
{
private:
    mutex &m;

public:
    scoped_lock(mutex &m) : m(m)
    {
        m.lock();
    }

    ~scoped_lock()
    {
        m.unlock();
    }
};

void cpu_work(unsigned long workUnits)
{
    unsigned long x = 0;
    for (unsigned long i = 0; i < workUnits * 1000000; i++)
    {
        x++;
    }
}

void process_1()
{
    cpu_work(1);
    scoped_lock<std::mutex> lock(mutex);
    result *= 2;
    printf("process 1 doubled the result.\n");
}

void process_2()
{
    cpu_work(1);
    scoped_lock<std::mutex> lock(mutex);
    result += 3;
    printf("process 2 added 3 to the result.\n");
}

int main()
{
    std::thread threads[10];
    for (int i = 0; i < 10; i += 2)
    {
        threads[i] = std::thread(process_1);
        threads[i + 1] = std::thread(process_2);
    }
    for (auto &t : threads)
    {
        t.join();
    }

    printf("Final result : %i\n", result);

    return 0;
}