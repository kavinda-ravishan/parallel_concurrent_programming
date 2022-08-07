#include <stdio.h>
#include <thread>
#include <chrono>

void daemon_thread_work()
{
    while (true)
    {
        printf("Daemon thread working.....\n");
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
}

int main()
{
    std::thread thread(daemon_thread_work);
    thread.detach();

    for (int i = 0; i < 3; i++)
    {
        printf("Main thread working.....\n");
        std::this_thread::sleep_for(std::chrono::microseconds(600));
    }
    printf("Main thread Done.\n");

    printf("end\n");

    return 0;
}