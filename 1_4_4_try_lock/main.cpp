#include <stdio.h>
#include <thread>
#include <chrono>
#include <mutex>

class Timer
{
private:
    std::chrono::time_point<std::chrono::steady_clock> time_s;
    std::chrono::time_point<std::chrono::steady_clock> time_e;
    std::chrono::duration<float> duration;
    float t;

public:
    void start()
    {
        time_s = std::chrono::high_resolution_clock::now();
    }
    void stop()
    {
        time_e = std::chrono::high_resolution_clock::now();
        duration = time_e - time_s;
        t = duration.count();
    }
    float get_duration()
    {

        return t;
    }
};

std::mutex mutex;
unsigned int items = 0;

void add_items(const char *name)
{
    unsigned int items_to_add = 0;
    while (items <= 20)
    {
        if (items_to_add)
        {
            mutex.lock();
            items += items_to_add;
            printf("%s %u items added to the list.\n", name, items_to_add);
            items_to_add = 0;
            std::this_thread::sleep_for(std::chrono::milliseconds(300));
            mutex.unlock();
        }
        else
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
            items_to_add++;
            printf("%s found some items to add.\n", name);
        }
    }
}

void add_items_try_lock(const char *name)
{
    unsigned int items_to_add = 0;
    while (items <= 20)
    {
        // if ((items_to_add > 0) && (mutex.try_lock() == true))
        // IMPORTENT : order of evaluation is matter here
        // first check (items_to_add > 0) if its true then only call try lock 
        if (items_to_add && mutex.try_lock())
        {
            items += items_to_add;
            printf("%s %u items added to the list.\n", name, items_to_add);
            items_to_add = 0;
            std::this_thread::sleep_for(std::chrono::milliseconds(300));
            mutex.unlock();
        }
        else
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
            items_to_add++;
            printf("%s found some items to add.\n", name);
        }
    }
}

void test(void (*add_items)(const char *))
{
    items = 0;
    Timer t;

    t.start();

    std::thread t1(add_items, "t1");
    std::thread t2(add_items, "t2");
    t1.join();
    t2.join();

    t.stop();

    printf("Time : %.2f s, Items : %u\n", t.get_duration(), items);
}

int main()
{
    test(add_items);
    test(add_items_try_lock);

    return 0;
}