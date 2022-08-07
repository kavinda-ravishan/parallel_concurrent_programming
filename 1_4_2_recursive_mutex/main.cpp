#include <stdio.h>
#include <thread>
#include <mutex>

unsigned int count_1 = 0;
unsigned int count_2 = 0;
std::recursive_mutex mutex;

void increment_1()
{

    mutex.lock();
    count_1++;
    mutex.unlock();
}

// function lock 2 times (recursive_mutex needed)
void increment_2()
{
    mutex.lock();
    count_2++;
    increment_1();
    mutex.unlock();
}

void loop(unsigned int count)
{
    for (unsigned int i = 0; i < count; i++)
    {
        increment_1();
        increment_2();
    }
}

int main()
{
    unsigned int loop_count = 1000000;

    std::thread t1(loop, loop_count);
    std::thread t2(loop, loop_count);

    t1.join();
    t2.join();

    printf("Count 1 ( %u ) : %u\n", loop_count * 2, count_1);
    printf("Count 2 ( %u ) : %u\n", loop_count * 2, count_2);

    return 0;
}