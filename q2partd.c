//   kissa zahra   i21-0572    assignment#02
// 3 processes or 4 ?? 

#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

#define ARRAY_SIZE 4
void alltoallvv(int* sendbuffer, int* sendcounts, int* snding_displacemnt, int* recvbuffer, int* recvcounts, int* recving_displacemnt, MPI_Comm comm) 
{
    int rank, size;
    MPI_Comm_rank(comm, &rank);
    MPI_Comm_size(comm, &size);

    // Initialize receive buffer
    for (int i = 0; i < size; i++) 
    {
        for (int j = 0; j < recvcounts[i]; j++)
        {
            recvbuffer[recving_displacemnt[i] + j] = 0;
        }
    }

    // Send and receive data using point-to-point communication
    for (int i = 0; i < size; i++) 
    {
        if (i != rank) 
        {
            MPI_Send(sendbuffer + snding_displacemnt[i], sendcounts[i], MPI_INT, i, 0, comm);
            MPI_Recv(recvbuffer + recving_displacemnt[i], recvcounts[i], MPI_INT, i, 0, comm, MPI_STATUS_IGNORE);
        }
    }

    // Copy own data
    for (int i = 0; i < recvcounts[rank]; i++) 
    {
        recvbuffer[recving_displacemnt[rank] + i] = sendbuffer[snding_displacemnt[rank] + i];
    }
}

int main(int argc, char* argv[]) 
{
   
    int sendbuffer[ARRAY_SIZE * 4] = { 1, 2, 3, 4, 5, 6, 
                                       7, 8, 9, 10, 11, 12,
                                       13, 14, 15, 16 };
    int recvbuffer[ARRAY_SIZE * 4];
    int sendcounts[4] = { 4, 4, 4, 4 };
    int recvcounts[4] = { 4, 4, 4, 4 };
    int snding_displacemnt[4] = { 0, 4, 8, 12 };
    int recving_displacemnt[4] = { 0, 4, 8, 12 };



    int rank, size;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    alltoallvv(sendbuffer, sendcounts, snding_displacemnt, recvbuffer, recvcounts, recving_displacemnt, MPI_COMM_WORLD);

    printf("Process %d received: ", rank);
    for (int i = 0; i < ARRAY_SIZE * size; i++) 
    {
        printf("%d ", recvbuffer[i]);
    }
    printf("\n");

    MPI_Finalize();
    return 0;
}
