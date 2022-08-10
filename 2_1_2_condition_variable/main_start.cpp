#include <thread>
#include <mutex>

int work_count = 10; // id-0 : {2, 4, 6, 8, 10}, // id-1 : {1, 3, 5, 7, 9}
std::mutex mutex;

void work(int id)
{
    int failled_to_access_work_count = 0;
    while (work_count > 0)
    {
        std::unique_lock<std::mutex> lock(mutex);       // pick up the lock
        if ((id == work_count % 2) && (work_count > 0)) // is it your turn?
        {
            work_count--; // it's your turn.
        }
        else
        {
            failled_to_access_work_count++; // it's not your turn.
        }
    }
    printf("Thread %d failled to access work %u times.\n", id, failled_to_access_work_count);
}

int main()
{
    std::thread threads[2];
    for (int i = 0; i < 2; i++)
    {
        threads[i] = std::thread(work, i);
    }
    for (std::thread &t : threads)
    {
        t.join();
    }

    printf("Work count : %i\n", work_count);
}