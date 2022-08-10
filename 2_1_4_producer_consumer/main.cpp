#include <stdio.h>
#include <thread>
#include <queue>
#include <mutex>
#include <condition_variable>

class ProducerConsumer
{
private:
    std::queue<int> products;
    std::mutex mutex;
    std::condition_variable produced; // this is only a place to threads to wait

public:
    void produce(int i)
    {
        std::unique_lock<std::mutex> mutex_lock(mutex);
        products.push(i);
        mutex_lock.unlock();
        produced.notify_one();
    }

    int consume()
    {
        std::unique_lock<std::mutex> mutex_lock(mutex);
        while (products.empty())
        {
            // release lock and wait
            produced.wait(mutex_lock); // wait until notify
            // when notify, this will lock the mutext and check the condition (while loop condision)
        }

        int num = products.front();
        products.pop();
        return num;
    }
};

void producer_task(ProducerConsumer *product_line)
{
    for (int i = 0; i < 100000; i++)
    {
        product_line->produce(i);
    }
    product_line->produce(-1);
    printf("Producer done producing products!\n");
}

void consumer_task(ProducerConsumer *product_line)
{
    int num_products = 0;
    while (true)
    {
        int product = product_line->consume();
        if (product == -1)
        {
            printf("Consumer took %i number of products.\n", num_products);
            product_line->produce(-1);
            return;
        }
        else
        {
            num_products++;
        }
    }
}

int main()
{
    ProducerConsumer *product_line = new ProducerConsumer();

    std::thread t1(producer_task, product_line);
    std::thread t2(consumer_task, product_line);
    std::thread t3(consumer_task, product_line);

    t1.join();
    t2.join();
    t3.join();

    delete product_line;

    return 0;
}