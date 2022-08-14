#include <stdio.h>
#include <thread>
#include <shared_mutex>
#include <chrono>
#include <array>

const int DAYS_PER_WEEK = 7;
char WEEKDAYS[DAYS_PER_WEEK][10] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};
int today = 0;
std::shared_mutex mutex;

void calender_reader(const int id)
{
    for (int i = 0; i < 7; i++)
    {
        // shared read mode - lock and multiple threads can read at same time
        // today is a shared memory
        mutex.lock_shared(); // readers can read same time (all reader threads can access this memory at same time)
        printf("Reader-%i sees today is %s\n", id, WEEKDAYS[today]);
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        mutex.unlock_shared();
    }
}

void calender_writer(const int id)
{
    for (int i = 0; i < 7; i++)
    {
        // exclusive write mode - only one thread can lock and write (other threads cannot read as well)
        // today is a shared memory
        mutex.lock(); // only one can write (cannot read by others threads)
        today = (today + 1) % DAYS_PER_WEEK;
        printf("Writer-%i update date to %s\n", id, WEEKDAYS[today]);
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        mutex.unlock();
    }
}

int main()
{
    std::array<std::thread, 10> readers;
    for (int i = 0; i < readers.size(); i++)
    {
        readers[i] = std::thread(calender_reader, i);
    }

    std::array<std::thread, 2> writers;
    for (int i = 0; i < writers.size(); i++)
    {
        writers[i] = std::thread(calender_writer, i);
    }

    for (int i = 0; i < readers.size(); i++)
    {
        readers[i].join();
    }

    for (int i = 0; i < writers.size(); i++)
    {
        writers[i].join();
    }

    return 0;
}