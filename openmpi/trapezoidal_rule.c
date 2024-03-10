#include <stdio.h>
#include <mpi.h>

double func(double x);
double Trap(double local_a, double local_b, int local_n, double h);

int main(int argc, char** argv) {
    int rank, size, n = 1000, local_n;
    double a=0.0, b = 3.0, h, local_a, local_b;
    double local_int, total_int;
    int source;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank); 
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    h = (b-a) / n;
    local_n = n / size;

    local_a = a + rank*local_n * h;
    local_b = local_a + local_n* h;
    local_int = Trap(local_a, local_b, local_n, h);

    if (rank != 0) {
        MPI_Send(&local_int, 1, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD);
    } else {
        total_int = local_int;
        for (source = 1; source<size; source++) {
            MPI_Recv(&local_int, 1, MPI_DOUBLE, source, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            total_int += local_int;
        }
    }

    if (rank == 0) {
        printf("With n = %d trapezoids, our estimate\n", n);
        printf("of the integral from %f to %f = %.5e\n", a, b, total_int);
    }

    MPI_Finalize();
    return 0;
}

double func(double x){
    return (2*x*x + 2*x + 1);
}

double Trap(double local_a, double local_b, int local_n, double h){
    double local_result = (func(local_a) + func(local_b)) / 2.0;

    for(int i=1; i < local_n; ++i){
        double x = local_a + i*h;
        local_result += func(x);
    }

    local_result *= h;
    return local_result;
}
