#include <algorithm>
#include <cassert>
#include <cmath>
#include <cstdlib>
#include <iostream>
#include <mpi.h>
#include <vector>

using namespace std;

double bucket_sort(vector<float> &a, int m)
{
    double comm_time = 0.0;

    int world_size, world_rank;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
    assert(m % world_size == 0);

    vector<vector<float>> buckets(m);

    int mp        = m / world_size;
    int bix_start = world_rank * mp;
    int bix_end   = (world_rank + 1) * mp;
    if (world_rank == 0)
    {
        double t0 = MPI_Wtime();
        for (auto ai : a)
        {
            assert(0 <= ai and ai < 1);
            int bix = ai * m;
            int dst = bix / mp;
            if (dst == 0)
                buckets[bix].push_back(ai);
            else
                MPI_Send(&ai, 1, MPI_FLOAT, dst, 0, MPI_COMM_WORLD);
        }
        float x = -1;
        for (int p = 1; p < world_size; ++p)
            MPI_Send(&x, 1, MPI_FLOAT, p, 0, MPI_COMM_WORLD);
        double t1 = MPI_Wtime();
        comm_time += t1 - t0;
    }
    else
    {
        while (true)
        {
            float ai;
            MPI_Recv(
                &ai, 1, MPI_FLOAT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

            if (ai == -1)
                break;
            int bix = ai * m;
            assert(bix_start <= bix and bix < bix_end);
            buckets[bix].push_back(ai);
        }
    }

    vector<float> local_a;
    for (int bix = bix_start; bix < bix_end; ++bix)
    {
        sort(buckets[bix].begin(), buckets[bix].end());
        local_a.insert(local_a.end(), buckets[bix].begin(), buckets[bix].end());
    }

    if (world_rank == 0)
    {
        int len = local_a.size();
        for (int i = 0; i < len; ++i)
            a[i] = local_a[i];

        double t0 = MPI_Wtime();
        for (int p = 1; p < world_size; ++p)
        {
            int plen;

            MPI_Recv(
                &plen, 1, MPI_INT, p, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            MPI_Recv(a.data() + len,
                     plen,
                     MPI_FLOAT,
                     p,
                     0,
                     MPI_COMM_WORLD,
                     MPI_STATUS_IGNORE);
            len += plen;
        }
        double t1 = MPI_Wtime();
        comm_time += t1 - t0;
    }
    else
    {
        int len = local_a.size();

        MPI_Send(&len, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
        MPI_Send(local_a.data(), len, MPI_FLOAT, 0, 0, MPI_COMM_WORLD);
    }
    return comm_time;
}

int main(int argc, char **argv)
{
    int world_size, world_rank;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

    int           n = 1 << 19;
    vector<float> a;
    if (world_rank == 0)
        for (int i = 0; i < n; ++i)
            a.push_back(rand() / (RAND_MAX + 1.0));

    double t1        = MPI_Wtime();
    double comm_time = bucket_sort(a, world_size * (int)sqrt(n));
    double t2        = MPI_Wtime();
    double comp_time = (t2 - t1) - comm_time;
    assert(comp_time >= 0.0);

    if (world_rank == 0)
    {
        printf("TIME: %f\n", t2 - t1);
        printf("COMP: %f\n", comp_time);
        printf("COMM: %f\n", comm_time);
    }

    MPI_Finalize();
    return 0;
}
