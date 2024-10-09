#include <iostream>
#include <mpi/mpi.h>

#define N 400

using namespace std;

void print_matrix(double matrix[][N]) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            cout << matrix[i][j] << " ";
        }
        cout << endl;
    }
}

/*
 * Paralelice la siguiente funcion con MPI.
 * Cada proceso debe tener N/p filas de la matriz original.
 * El proceso maestro debe almacenar la matriz resultante.
*/
void normalization_sequential(double A[][N]) {
    double sum[N];
    double fact = 0.0, max = 0.0;
    int i, j;

    for (i = 0; i < N; i++) {
        sum[i] = 0.0;
        for (j = 0; j < N; j++)
            sum[i] += A[i][j] * A[i][j];
    }

    fact = 1.0 / sum[0];
    for (i = 0; i < N; i++) sum[i] *= fact;

    max = 0.0;
    for (i = 0; i < N; i++) {
        if (sum[i] > max) max = sum[i];
    }

    for (i = 0; i < N; i++) {
        for (j = 0; j < N; j++)
            A[i][j] *= max;
    }
}

void normalization_parallel(double A[][N], int rank, int size) {
    cout << "Parallel" << endl;



    double A_result[N][N];

    if (rank == 0) {
        for (int i = 0; i < N; ++i) {
            for (int j = 0; j < N; ++j) {
                A[i][j] = i * j + 1;
            }
        }
    }

    const int k = N / size;

    double A_partition[k][N];
    double sum[k];
    double fact = 0.0;
    double max_local = 0.0;
    double max_global = 0.0;

    if (rank == 0) {
        // print_matrix(A);
    }

    MPI_Scatter(&A[0][0], k * N, MPI_DOUBLE, &A_partition[0][0], k * N, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    for (int i = 0; i < k; i++) {
        sum[i] = 0.0;
        for (int j = 0; j < N; j++) {
            sum[i] += A_partition[i][j] * A_partition[i][j];
        }
    }

    if (rank == 0) {
        fact = 1 / sum[0];
    }

    MPI_Bcast(&fact, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    for (int i = 0; i < k; i++) {
        sum[i] *= fact;
    }

    for (int i = 0; i < k; i++) {
        max_local = max_local > sum[i] ? max_local : sum[i];
    }

    // Reduce & Broadcast
    MPI_Allreduce(&max_local, &max_global, 1, MPI_DOUBLE, MPI_MAX, MPI_COMM_WORLD);

    for (int i = 0; i < k; i++) {
        for (int j = 0; j < N; j++) {
            A_partition[i][j] = A_partition[i][j] * max_global;
        }
    }

    MPI_Gather(&A_partition[0][0], k * N, MPI_DOUBLE, &A_result[0][0], k * N, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        // print_matrix(A_result);
    }

    MPI_Finalize();
}

int main(int argc, char *argv[]) {
    double A[N][N];

#ifdef PARALLEL

    int rank, int size;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    normalization_parallel(A, rank, size);

#else

    normalization_sequential(A);

#endif

    return 0;
}
