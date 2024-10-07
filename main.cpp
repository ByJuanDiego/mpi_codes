#include <iostream>
#include <mpi.h>
using namespace std;

int main(int argc, char *argv[]) {
    int rank, size, data = 0;

    const int err = MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (err != MPI_SUCCESS) {
        MPI_Abort(MPI_COMM_WORLD, err);
    }

    if (rank == 2) {
        data = 20;
    }

    printf("Peekaboo! from %d/%d with data=%d\n", rank, size - 1, data);

    MPI_Finalize();
}
