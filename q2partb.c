//   kissa zahra   i21-0572    assignment#02
// 3 processes 
#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

int main(int argc, char* argv[])
{
    MPI_Init(&argc, &argv);

    int size;
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int rankid;
    MPI_Comm_rank(MPI_COMM_WORLD, &rankid);

    int counts[3] = { 2, 2, 3 };
    int displacements[3] = { 0, 1, 2 };  // fixed it (|<>|)
    int buffer[9] = { 0 };
    int* my_values;
    int my_values_count;

    switch (rankid)   // for each process how many values it will send 
   {
    case 0:
    {
        my_values_count = 1;
        my_values = (int*)malloc(sizeof(int) * my_values_count);
        *my_values = 100;
        printf("Value sent by process %d: %d.\n", rankid, *my_values);
        break;
    }
    case 1:
    {
        my_values_count = 1;
        my_values = (int*)malloc(sizeof(int) * my_values_count);
        *my_values = 101;
        printf("Value sent by process %d: %d.\n", rankid, *my_values);
        break;
    }
    case 2:
    {
        my_values_count = 2;   // values that it will send 
        my_values = (int*)malloc(sizeof(int) * my_values_count);
        my_values[0] = 102;
        my_values[1] = 103;
        printf("Values sent by process %d: %d and %d.\n", rankid, my_values[0], my_values[1]);
        break;
    }
   }

    for (int i = 0; i < size; i++) 
    {
        if (i != rankid) 
        {
            MPI_Send(my_values, my_values_count, MPI_INT, i, 0, MPI_COMM_WORLD);
        }
    }

    for (int i = 0; i < size; i++) 
    {
        if (i != rankid) 
        { 
            // start to recieve from that buffer 
            MPI_Recv(&buffer[displacements[i]], counts[i], MPI_INT, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        }
    }

    printf("Values %d:", rankid);
    for (int i = 0; i < 7; i++)
    {
        printf(" %d", buffer[i]);
    }
    printf("\n");
    free(my_values);

    MPI_Finalize();

    return 0;
}
