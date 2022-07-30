#include <stdlib.h>
#include <chrono>
#include <thread>

void cpu_waster()
{
    printf("CPU Waster Process ID: %d\n", getpid());
    printf("CPU Waster Thread ID: %d\n", std::this_thread::get_id());
    while (true)
        ;
}

int main()
{
    printf("CPU Waster Process ID: %d\n", getpid());
    printf("CPU Waster Thread ID: %d\n", std::this_thread::get_id());

    std::thread thread1(cpu_waster);
    std::thread thread2(cpu_waster);

    while (true)
    {
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    return 0;
}