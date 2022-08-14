#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <future>
#include <thread>

void disp_mat(int** m, int R, int C)
{
    printf("\n");
    for(int i=0; i<R; i++)
    {
        printf("|");
        for(int j=0; j<C; j++)
        {
            printf("%5i", m[i][j]);
        }
        printf(" |\n");
    }
    printf("\n");
}

void free_2d_arr(int** m, int R)
{
    for(int i=0; i<R; i++)
    {
        free(m[i]);
    }
    free(m);
}

int** alloc_fill_2d_arr(int R, int C, bool zeros=false)
{
    int** m = (int**)malloc(R * sizeof(int*));

    if(m == NULL) exit(EXIT_FAILURE);

    for(int i=0; i<R; i++)
    {
        m[i] = (int*)malloc(C * sizeof(int));

        if(m[i] == NULL) exit(EXIT_FAILURE);
        
        for (int j=0; j<C; j++)
        {
            m[i][j]  = zeros? 0 : (rand() % 10 + 1); 
        }
    }

    return m;
}

void add_mat_el(int* a, int* b, int* c)
{
    *c += (*a) * (*b);
}

int** mat_mul_s(int** m1, int m1_r, int m1_c, int** m2, int m2_r, int m2_c)
{
    int** m = alloc_fill_2d_arr(m1_r, m2_c, true);

    for(int i=0; i<m1_r; i++)
    {
        for(int j=0; j<m2_c; j++)
        {
            for(int k=0; k<m1_r; k++)
            {
                add_mat_el(&m1[i][k], &m2[k][j], &m[i][j]);
                // m[i][j] += m1[i][k] * m2[k][j];
            }
        }
    }

    return m;
}

int** mat_mul_p(int** m1, int m1_r, int m1_c, int** m2, int m2_r, int m2_c)
{
    int** m = alloc_fill_2d_arr(m1_r, m2_c, true);

    for(int i=0; i<m1_r; i++)
    {
        for(int j=0; j<m2_c; j++)
        {
            for(int k=0; k<m1_r; k++)
            {
                std::future<void> empty = std::async(std::launch::async, add_mat_el, &m1[i][k], &m2[k][j], &m[i][j]);
            }
        }
    }

    return m;
}


void parallel_worker(
    int** m1, int m1_r, int m1_c, 
    int** m2, int m2_r, int m2_c,
    int** m, int start_r, int end_r
    )
{
    for(int i=start_r; i<end_r; i++)
    {
        for(int j=0; j<m2_c; j++)
        {
            for(int k=0; k<m1_r; k++)
            {
                m[i][j] += m1[i][k] * m2[k][j];
            }
        }
    }
}

int** parallel_matrix_multiply(int** m1, int m1_r, int m1_c, int** m2, int m2_r, int m2_c)
{
    int** m = alloc_fill_2d_arr(m1_r, m2_c, true);

    int num_workers = std::thread::hardware_concurrency();
    int chunk_size = ceil((float)m1_r/num_workers);

    std::thread threads[num_workers];
    for(int i=0; i<num_workers; i++)
    {
        int start_r = std::min(i*chunk_size, m1_r);
        int end_r = std::min((i+1)*chunk_size, m1_r);

        threads[i] = std::thread(parallel_worker, m1, m1_r, m1_c, m2, m2_r, m2_c, m, start_r, end_r);
    }

    for(auto& t : threads)
    {
        t.join();
    }

    return m;
}

int main()
{
    const int M1_R = 5;
    const int M1_C = 5;
    const int M2_R = M1_C;
    const int M2_C = 5;

    const int M_R = M1_R;
    const int M_C = M2_C;

    int** m1 = alloc_fill_2d_arr(M1_R, M1_C);
    int** m2 = alloc_fill_2d_arr(M2_R, M2_C);
    
    int** m = mat_mul_s(m1, M1_R, M1_C, m2, M2_R, M2_C);
    int** m_p = mat_mul_p(m1, M1_R, M1_C, m2, M2_R, M2_C);

    disp_mat(m, M_R, M_C);
    disp_mat(m_p, M_R, M_C);

    free_2d_arr(m1, M1_R);
    free_2d_arr(m2, M1_R);
    free_2d_arr(m, M_R);
    free_2d_arr(m_p, M_R);
    
    return 0;
}