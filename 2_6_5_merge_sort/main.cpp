#include <iostream>
#include <cstring>
#include <thread>
#include <cmath>

void disp_arr(int* arr, const int size)
{
    std::cout<<std::endl<<"| ";
    for(int i = 0; i < size; i++)
    {
        std::cout<<arr[i]<<" | ";
    }
    std::cout<<std::endl;
}

void disp_arr(int* arr, int from, int to)
{
    std::cout<<std::endl<<"| ";
    for(int i = from; i <= to; i++)
    {
        std::cout<<arr[i]<<" | ";
    }
    std::cout<<std::endl;
}

// [left, ... , mid] and [mid+1, ... , right]
void merge(int* arr, int left, int mid, int right)
{
    int size = right - left + 1;
    int temp_arr[size];

    int index_left = left;
    int index_right = mid+1;

    for(int i=0; i<size; i++)
    {
        if(index_left > mid) temp_arr[i] = arr[index_right++];
        else if (index_right > right) temp_arr[i] = arr[index_left++];
        else
        {
            if(arr[index_left] < arr[index_right]) temp_arr[i] = arr[index_left++];
            else temp_arr[i] = arr[index_right++];
        }
    }

    memcpy(arr+left, temp_arr, size*sizeof(int));
}

void merge_sort_fun(int* arr, int left, int right)
{
    if(left < right)
    {
        int mid = (left + right)/2;
        merge_sort_fun(arr, left, mid);
        merge_sort_fun(arr, mid+1, right);
        merge(arr, left, mid, right);
    }
}

void merge_sort_fun_parallel(int* arr, int left, int right, int depth=0)
{
    if(depth >= std::log2(std::thread::hardware_concurrency()) || left > right)
    {
        std::cout<<left <<" | " <<right<< " : sort\n";
        merge_sort_fun(arr, left, right);
    }
    else
    {
        int mid = (left + right)/2;
        depth++;
        std::thread thread(merge_sort_fun_parallel, arr, left, mid, depth);
        merge_sort_fun_parallel(arr, mid+1, right, depth);
        thread.join();
        merge(arr, left, mid, right);
    }
}

void merge_sort(int* arr, const int size, bool parallel=false)
{
    if(parallel) merge_sort_fun_parallel(arr, 0, size-1);
    else merge_sort_fun(arr, 0, size-1);
}

bool is_sorted(int* arr, const int size)
{
    if(size==1) return true;
    for(int i=0; i<size-1; i++)
    {
        if(arr[i] > arr[i+1]) return false;
    }
    return true;
}

int main()
{
    const int size = 100;
    int arr[size];

    for(int i=0; i<size; i++)
    {
        arr[i] = rand()%1000;
    }
    
    std::cout<< std::endl;
    std::cout<<(is_sorted(arr, size)? "True" : "False")<<std::endl;
    disp_arr(arr, size);
    merge_sort(arr, size, true);
    std::cout<<(is_sorted(arr, size)? "True" : "False")<<std::endl;
    disp_arr(arr, size);

    return 0;
}