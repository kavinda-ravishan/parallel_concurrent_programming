#include <cstdio>
#include <future>
#include <cmath>

unsigned long long recursive_sum(unsigned long long low, unsigned long long high, unsigned int max_depth, unsigned long long depth = 0)
{
    if (depth >= max_depth)
    {
        unsigned long long sum = 0;
        printf("CAL -> Low : %lli - high : %lli\n", low, high);
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
    unsigned int no_threads = 8;
    unsigned int max_depth = (unsigned int)log2(no_threads);
    printf("Sum : %lli\n", recursive_sum(1, 100, max_depth));

    return 0;
}