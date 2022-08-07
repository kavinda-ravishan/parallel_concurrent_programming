#include <stdio.h>
#include <thread>
#include <chrono>

void work_1()
{
    printf("work 1 started.\n");
    std::this_thread::sleep_for(std::chrono::seconds(3));
    printf("work 1 Done.\n");
}

int main()
{
    printf("Create new thread for work 1\n");
    std::thread t1(work_1);

    printf("Is thread alive : %s\n", t1.joinable() ? "True" : "False");

    printf("Main thread is doing some work.\n");
    std::this_thread::sleep_for(std::chrono::seconds(1));
    printf("Is thread alive : %s\n", t1.joinable() ? "True" : "False");

    printf("Main thread waiting for work 1.\n");
    t1.join();

    printf("Is thread alive : %s\n", t1.joinable() ? "True" : "False");

    printf("end\n");

    return 0;
}