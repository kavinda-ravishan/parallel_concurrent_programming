#include <stdio.h>
#include <mutex>
#include <condition_variable>
#include <thread>

class Semaphore
{
public:
    Semaphore(unsigned long init_count) : count_(init_count) {}

    void acquire()
    {
        std::unique_lock<std::mutex> lck(m_);
        while (!count_)
        {
            cv_.wait(lck);
        }
        count_--;
    }

    void release()
    {
        std::unique_lock<std::mutex> lck(m_);
        count_++;
        lck.unlock();
        cv_.notify_one();
    }

private:
    std::mutex m_;
    std::condition_variable cv_;
    unsigned long count_;
};

Semaphore port(4);

void task(int id)
{
    srand(id);
    std::this_thread::sleep_for(std::chrono::milliseconds(rand() % 2000 + 1000));
}

void connection(int id)
{
    port.acquire();
    printf("Connection %i is working...\n", id);
    task(id);
    printf("Connection %i is disconnected...\n", id);
    port.release();
}

int main()
{
    std::thread threads[10];

    for (int i = 0; i < 10; i++)
    {
        threads[i] = std::thread(connection, i);
    }
    for (std::thread &t : threads)
    {
        t.join();
    }

    return 0;
}