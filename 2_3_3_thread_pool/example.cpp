#include <stdio.h>
#include <thread>
#include <mutex>
#include <condition_variable>

std::condition_variable cv;
std::mutex m;

bool stay = true;

void print_name()
{
    printf("hello.\n");
}

void (*function)();

void thread_1_work()
{
    std::unique_lock<std::mutex> lck(m);
    stay = false;
    function = print_name;
    lck.unlock();
    cv.notify_all();
}

void thread_2_work()
{
    std::unique_lock<std::mutex> lck(m);
    while (stay)
    {
        cv.wait(lck);
    }
    function();
    lck.unlock();
}

int main()
{
    std::thread t2(thread_2_work);
    std::thread t1(thread_1_work);

    t1.join();
    t2.join();

    return 0;
}