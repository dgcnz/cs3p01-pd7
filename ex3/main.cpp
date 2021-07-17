#include <algorithm>
#include <assert.h>
#include <omp.h>
#include <random>
#include <stdio.h>
#include <string>
#include <vector>

using namespace std;

const int N = 524288; // 2^19
vector<int> arr, arr2;

void printArray()
{
    for (auto &i : arr)
    {
        printf("%d ", i);
    }
    printf("\n");
}

template <typename Compare>
void merge(int start, int n)
{
    const Compare comp;
    int step = n / 2;
    while (step > 0)
    {
#pragma omp parallel for
        for (int i = 0; i < n; i += step * 2)
        {
            for (int j = start + i; j < min(N - step, start + i + step); ++j)
            {
                if (comp(arr[j], arr[j + step]))
                {
                    swap(arr[j], arr[j + step]);
                }
            }
        }
        step /= 2;
    }
}

void merge_up(int start, int n)
{
    merge<greater<int>>(start, n);
}

void merge_down(int start, int n)
{
    merge<less<int>>(start, n);
}

void measure()
{
    double start, end;

    start = omp_get_wtime();

    // do merges
    for (int s = 2; s <= N; s *= 2)
    {
        for (int i = 0; i < N; i += s * 2)
        {
            merge_up(i, s);
            merge_down(i + s, s);
        }
    }

    end = omp_get_wtime();

    printf("%f\n", (end - start) * 1000.0);
}

int main(int argc, char **argv)
{
    random_device rd;
    uniform_int_distribution<int> dist(0, 100);
    arr.resize(N);
    arr2.resize(N);
    for (int i = 0; i < N; ++i)
    {
        arr[i] = arr2[i] = dist(rd);
    }

    for (int &p : vector<int>{1, 2, 4}) // {1, 2, 4, 8, 16}
    {
        omp_set_num_threads(p);
        arr = arr2;
        measure();
    }
}