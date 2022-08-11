#include <thread>
#include <mutex>
#include <array>
#include <condition_variable>
#include <boost/thread/barrier.hpp>

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

class Barrier
{
private:
    unsigned int num_threads;
    unsigned int wait_threads;
    std::mutex m_;
    std::condition_variable cv_;

public:
    Barrier(unsigned int num_threads)
        : num_threads(num_threads)
    {
        wait_threads = 0;
        num_threads = 0;
    }

    void wait()
    {
        std::unique_lock<std::mutex> lck(m_);
        wait_threads++;
        cv_.notify_all();
        while (wait_threads < num_threads)
        {
            cv_.wait(lck);
        }
    }
};

// Barrier barrier(10); // 10 : number of threads
boost::barrier barrier(10);

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
    barrier.wait();
    scoped_lock<std::mutex> lock(mutex);
    result *= 2;
    printf("process 1 doubled the result.\n");
}

void process_2()
{
    cpu_work(1);
    // scoped_lock need to unlock before waiting
    { // mutex lock
        scoped_lock<std::mutex> lock(mutex);
        result += 3;
    } // mutex unlock
    printf("process 2 added 3 to the result.\n");
    barrier.wait();
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