#include <future>

int thread_work(int a, int b)
{
    printf("Thread is working....\n");
    std::this_thread::sleep_for(std::chrono::seconds(3));
    return a + b;
}

int main()
{
    printf("start.\n");
    std::future<int> result = std::async(std::launch::async, thread_work, 2, 5);
    printf("main thread waiting for result.\n"); // main thread can do other work
    printf("Result : %i\n", result.get()); // wait for result

    return 0;
}