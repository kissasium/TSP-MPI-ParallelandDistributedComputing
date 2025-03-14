//   kissa zahra   i21-0572    assignment#02
// 4 processes 
#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <string.h>

void allgatherr(void* sendbuffer, int sendcount, MPI_Datatype sendtype, void* recvbuffer, int recvcount, MPI_Datatype recvtype, MPI_Comm comm) 
{
    int rankid, size;
    MPI_Comm_rank(comm, &rankid);
    MPI_Comm_size(comm, &size);

    MPI_Aint displs[size]; // displacement to determine where their data will be placed in the recieve buffer 

    MPI_Type_extent(recvtype, &displs[0]);
    displs[0] = 0;   // process 0 data will be at the begining of the buffer 

    for (int i = 1; i < size; i++) 
    {
        // no of bytes taken by the one instance 
        MPI_Type_extent(recvtype, &displs[i]);
        displs[i] *= recvcount;
        displs[i] += displs[i - 1];   // each process's data is placed in the correct position 
        // immediately after the previous process 
    }


    
    for (int i = 0; i < size; i++) 
    {
        if (i != rankid)   // you can not send to yourself 
        {
            MPI_Send(sendbuffer, sendcount, sendtype, i, 0, comm);
        }
    }

    for (int i = 0; i < size; i++) 
    {
        if (i != rankid) 
        {
            // recieve from all other processes 
            MPI_Recv((char*)recvbuffer + displs[i], recvcount, recvtype, i, 0, comm, MPI_STATUS_IGNORE);
        }
        else 
        {
            // current process needs to copy its own data from the send buffer (sendbuffer) to the recieve buffer (recvbuffer) 
            MPI_Type_extent(sendtype, &displs[i]);
            displs[i] *= sendcount;   // datatype = 4(int), 4 element send so 4*4 = 16, total size of data that we sent 
            // starting at the displacement displs[i]
            memcpy((char*)recvbuffer + displs[i] * rankid, sendbuffer, sendcount * size);
            
        }
    }
}

int main(int argc, char* argv[]) 
{
    int rankid, size;
    MPI_Init(&argc, &argv);
    // taking current rankid of the process 
    MPI_Comm_rank(MPI_COMM_WORLD, &rankid);
    MPI_Comm_size(MPI_COMM_WORLD, &size);  

    int sendbuffer = rankid;
    int recvbuffer[size];

    allgatherr(&sendbuffer, 1, MPI_INT, recvbuffer, 1, MPI_INT, MPI_COMM_WORLD);

    if (rankid == 0) 
    {
        MPI_Aint displs[size];  

        MPI_Type_extent(MPI_INT, &displs[0]);
        displs[0] = 0;

        for (int i = 1; i < size; i++) 
        {
            MPI_Type_extent(MPI_INT, &displs[i]);
            displs[i] *= 1; // since we are sending and receiving 1 integer at a time
            displs[i] += displs[i - 1];
        }
        printf("Received data :) :\n");
        for (int i = 0; i < size; i++) 
        {
            printf("%d ", recvbuffer[i]);
        }
        printf("\n");
    }

    MPI_Finalize();
    return 0;
}
