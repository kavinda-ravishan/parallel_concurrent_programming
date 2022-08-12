#include <stdio.h>
#include <boost/asio.hpp>

void work(int id)
{
    printf("Work id : %d, Thread id : %d\n", id, std::this_thread::get_id());
}

int main()
{
    boost::asio::thread_pool pool(4);
    for (int i = 0; i < 100; i++)
    {
        boost::asio::post(pool, [i]()
                          { work(i); });
    }

    pool.join();

    return 0;
}