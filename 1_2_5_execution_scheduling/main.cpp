#include <stdio.h>
#include <thread>
#include <chrono>

bool is_working = true;

void work(const char *name)
{
    unsigned int work_count = 0;
    while (is_working)
    {
        work_count++;
    }
    printf("%s done, count: %u.\n", name, work_count);
}

int main()
{
    std::thread work1(work, "work1");
    std::thread work2(work, "work2");

    printf("Work started.\n");

    std::this_thread::sleep_for(std::chrono::seconds(1));
    is_working = false;

    work1.join();
    work2.join();

    return 0;
}