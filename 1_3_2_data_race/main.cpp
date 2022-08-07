#include <stdio.h>
#include <thread>
#include <chrono>

unsigned int count = 0;

class Timer
{
private:
    std::chrono::time_point<std::chrono::steady_clock> time_s;
    std::chrono::time_point<std::chrono::steady_clock> time_e;
    std::chrono::duration<float> duration;

public:
    void start()
    {
        time_s = std::chrono::high_resolution_clock::now();
    }
    void stop()
    {
        time_e = std::chrono::high_resolution_clock::now();
    }
    float get_duration()
    {
        duration = time_e - time_s;
        return duration.count() * 1000;
    }
};

void increment(unsigned int c)
{
    Timer t;

    t.start();
    for (unsigned int i = 0; i < c; i++)
    {
        count++;
    }
    t.stop();

    printf("Thread duration : %f\n", t.get_duration());
}

int main()
{
    unsigned int c = 1000000;

    std::thread t1(increment, c);
    std::thread t2(increment, c);

    t1.join();
    t2.join();

    printf("Count ( %u ) : %u\n", c * 2, count);

    return 0;
}