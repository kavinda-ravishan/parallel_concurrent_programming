#include <stdio.h>
#include <thread>
#include <mutex>

unsigned int count = 0;
std::mutex mutex;

void increment_noMutex(unsigned int c)
{
    for (unsigned int i = 0; i < c; i++)
    {
        count++;
    }
}
void test_1()
{
    count = 0;
    unsigned int c = 1000000;

    std::thread t1(increment_noMutex, c);
    std::thread t2(increment_noMutex, c);

    t1.join();
    t2.join();

    printf("Count ( %u ) : %u\n", c * 2, count);
}

void increment_Mutex(unsigned int c)
{
    for (unsigned int i = 0; i < c; i++)
    {
        mutex.lock();
        count++;
        mutex.unlock();
    }
}
void test_2()
{
    count = 0;
    unsigned int c = 1000000;

    std::thread t1(increment_Mutex, c);
    std::thread t2(increment_Mutex, c);

    t1.join();
    t2.join();

    printf("Count ( %u ) : %u\n", c * 2, count);
}

int main()
{
    test_1();
    test_2();

    return 0;
}