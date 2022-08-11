/*
Barrier : Release when certain number of threads are waiting
Latch   : Release when count value reaches zero
*/

#include <thread>
#include <mutex>
#include <array>
#include <condition_variable>
#include <boost/thread/latch.hpp>

unsigned int result = 1;
std::mutex mutex;

class Latch
{
private:
    unsigned int count;
    std::mutex m_;
    std::condition_variable cv_;

public:
    Latch(unsigned int count) : count(count) {}

    void count_down()
    {
        std::unique_lock<std::mutex> lck(m_);
        count--;
        cv_.notify_all();
    }

    void wait()
    {
        std::unique_lock<std::mutex> lck(m_);
        while (count)
        {
            cv_.wait(lck);
        }
    }
};

// Latch latch(5); // 5 : num operation need befor allow next
boost::latch latch(5);

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
    latch.wait();
    std::scoped_lock<std::mutex> lock(mutex);
    result *= 2;
    printf("process 1 doubled the result.\n");
}

void process_2()
{
    cpu_work(1);
    {
        std::scoped_lock<std::mutex> lock(mutex);
        result += 3;
    }
    printf("process 2 added 3 to the result.\n");
    latch.count_down();
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

    printf("Final result : %i", result);

    return 0;
}