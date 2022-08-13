#include <iostream>
#include <future>
#include <cmath>

unsigned long long sum(unsigned long long low, unsigned long long high)
{

    unsigned long long sum = 0;
    for (unsigned long long i = low; i <= high; i++)
    {
        sum += i;
    }
    return sum;
}

unsigned long long recursive_sum(unsigned long long low, unsigned long long high, unsigned int max_depth, unsigned long long depth = 0)
{
    if (depth >= max_depth)
    {
        unsigned long long sum = 0;
        for (unsigned long long i = low; i <= high; i++)
        {
            sum += i;
        }
        return sum;
    }
    else
    {
        auto mid = (low + high) / 2;
        auto sum_1 = std::async(std::launch::async, recursive_sum, low, mid - 1, max_depth, depth + 1);
        auto sum_2 = recursive_sum(mid, high, max_depth, depth + 1);
        return sum_1.get() + sum_2;
    }
}
int main()
{
    const int num_eval_runs = 10;
    const int sum_values = 100000000;
    unsigned int no_threads = 4;
    unsigned int max_depth = (unsigned int)log2(no_threads);

    std::chrono::duration<double> sequential_time(0);
    auto sequential_result = sum(0, sum_values); // warm up run
    for (int i = 0; i < num_eval_runs; i++)
    {
        auto start = std::chrono::high_resolution_clock::now();
        sum(0, sum_values);
        auto end = std::chrono::high_resolution_clock::now();
        sequential_time += end - start;
    }
    sequential_time /= num_eval_runs;

    std::chrono::duration<double> parallel_time(0);
    auto parallel_result = recursive_sum(0, sum_values, max_depth); // warm up run
    for (int i = 0; i < num_eval_runs; i++)
    {
        auto start = std::chrono::high_resolution_clock::now();
        recursive_sum(0, sum_values, max_depth);
        auto end = std::chrono::high_resolution_clock::now();
        parallel_time += end - start;
    }
    parallel_time /= num_eval_runs;

    if (sequential_result != parallel_result)
        std::cout << "Error : result mismatch!" << std::endl;

    std::cout << "Hardware concurrency : " << std::thread::hardware_concurrency()<< std::endl;
    std::cout << "Sequential Sum       : " << sequential_time.count() * 1000 << "ms" << std::endl;
    std::cout << "Parallel Sum         : " << parallel_time.count() * 1000 << "ms" << std::endl;
    std::cout << "Speedup              : " << sequential_time / parallel_time << std::endl;
    std::cout << "Efficiency           : " << 100 * (sequential_time / parallel_time) / std::thread::hardware_concurrency() << "%" << std::endl;

    return 0;
}