//   kissa zahra   i21-0572    assignment#02
// 3 processes 
#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <string.h>


void alltoall(void* sendbuf, int sendcount, MPI_Datatype sendtype, void* recvbuf, int recvcount, MPI_Datatype recvtype, MPI_Comm comm) 
{
    int rank, size;
    MPI_Comm_rank(comm, &rank);
    MPI_Comm_size(comm, &size);

    int send_size, recv_size;
    MPI_Type_size(sendtype, &send_size);
    MPI_Type_size(recvtype, &recv_size);

    // sending 
    for (int i = 0; i < size; i++) 
    {
        if (i != rank) 
        {
            MPI_Send(sendbuf + i * sendcount * send_size, sendcount, sendtype, i, 0, comm);
        }
    }

    // recieve 
    for (int i = 0; i < size; i++) 
    {
        if (i != rank) 
        {
            MPI_Recv(recvbuf + i * recvcount * recv_size, recvcount, recvtype, i, 0, comm, MPI_STATUS_IGNORE);
        }
        else 
        {
            // copy its own elements 
            memcpy(recvbuf + i * recvcount * recv_size, sendbuf + i * sendcount * send_size, sendcount * send_size);
        }
    }
}

int main(int argc, char* argv[]) 
{
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int send_data[size]; // Each process sends its own rank
    int recv_data[size]; // Array to store received data

    for (int i = 0; i < size; i++) 
    {
        send_data[i] = rank;
    }

    alltoall(send_data, 1, MPI_INT, recv_data, 1, MPI_INT, MPI_COMM_WORLD);

    printf("Rank %d received: ", rank);
    for (int i = 0; i < size; i++) 
    {
        printf("%d ", recv_data[i]);
    }
    printf("\n");

    MPI_Finalize();
    return 0;
}
