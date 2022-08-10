#include <thread>
#include <mutex>
#include <condition_variable>

int work_count = 10; // id-0 : {2, 4, 6, 8, 10}, // id-1 : {1, 3, 5, 7, 9}
std::mutex mutex;
std::condition_variable condition_variable;

void work(int id, int no_threads)
{
    int failled_to_access_work_count = 0;
    while (work_count > 0)
    {
        std::unique_lock<std::mutex> lock(mutex); // pick up the lock
        // Important: place wait function inside the while loop
        // In certain operating environments the condition variable could have spurious wakeup 
        // (wakeup from sleeping state when it is not supposed to)
        // By placing inside of a while loop if a spurious wakeup occurs, 
        // the thread will see that it's still not time to continue on and it will go back to sleeping.
        while ((id != work_count % no_threads) && (work_count > 0)) // is it not your turn?
        {
            failled_to_access_work_count++; // it's not your turn.
            condition_variable.wait(lock); // release the lock and wait
        }
        if (work_count > 0)
        {
            work_count--; // it's your turn. (critical section)
            lock.unlock();
            // Important: only wakeup one thread and if it doesn’t wakeup the one who is have the next turn program will stuck. 
            // condition_variable.notify_one(); // only notify one waiting thread
            condition_variable.notify_all(); // notify all waiting thread
        }
    }
    printf("Thread %d failled to access work %u times.\n", id, failled_to_access_work_count);
}

int main()
{
    const int no_threads = 5;
    std::thread threads[no_threads];
    for (int i = 0; i < no_threads; i++)
    {
        threads[i] = std::thread(work, i, no_threads);
    }
    for (std::thread &t : threads)
    {
        t.join();
    }

    printf("Work count : %i\n", work_count);
}