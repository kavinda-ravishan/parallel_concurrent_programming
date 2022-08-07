#include <stdio.h>
#include <thread>
#include <atomic>

std::atomic<unsigned int> count = 0;

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

    printf("Count ( %u ) : %u\n", c * 2, count.load());
}

int main()
{
    test_1();

    return 0;
}