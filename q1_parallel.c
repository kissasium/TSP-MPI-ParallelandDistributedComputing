// kissa zahra , i21-0572 , and assignment#02
// 3 processes 
#include <stdio.h>
#include <stdbool.h>
#include <limits.h>
#include <mpi.h>

#define N 10 


int graph[N][N] = 
{
    {0, 1, 0, 0, 1, 1, 0, 0, 0, 0},
    {1, 0, 1, 0, 0, 0, 1, 0, 0, 1},
    {0, 1, 0, 1, 0, 0, 0, 1, 0, 1},
    {0, 0, 1, 0, 1, 0, 0, 0, 1, 0},
    {1, 0, 0, 1, 0, 1, 0, 0, 0, 0},
    {1, 0, 0, 0, 1, 0, 1, 0, 0, 0},
    {0, 1, 0, 0, 0, 1, 0, 1, 0, 0},
    {0, 0, 1, 0, 0, 0, 1, 0, 1, 0},
    {0, 0, 0, 1, 0, 0, 0, 1, 0, 1},
    {0, 1, 1, 0, 0, 0, 0, 0, 1, 0}
};


int queue[N * N];
int front, rear;

void initializequeue() 
{
    front = rear = -1;
}

void enqueue(int node)
{
    if (front == -1)
    {
        front = 0;
    }

    queue[++rear] = node;
}

int dequeue()
{
    return queue[front++];   // returns the last element 
}

bool queueempty()
{
    if (front > rear)
    {
        return true;
    }
    else
    {
        return false;
    }
    return false;
}

//bfs 
void BFS(int start, int end, int* distnce, int* parent) 
{
    int visited[N] = { 0 };
    initializequeue();

    for (int i = 0; i < N; ++i) 
    {
        distnce[i] = INT_MAX; 
        parent[i] = -1; 
    }

    distnce[start] = 0;
    parent[start] = start;
    visited[start] = 1;
    enqueue(start);

    while (!queueempty()) 
    {
        int current = dequeue();
        for (int i = 0; i < N; ++i) 
        {
            if (graph[current][i] && !visited[i]) 
            {
                visited[i] = 1;
                distnce[i] = distnce[current] + 1;
                parent[i] = current;
                enqueue(i);
            }
        }
    }
}

int main(int argc, char* argv[]) 
{
    int rank, size;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int starting_node = 0; 

    // Divide rows among processes
    int rows_per_process = N / size;
    int start_row = rank * rows_per_process;
    int end_row = start_row + rows_per_process;
    if (rank == size - 1) 
    {
        end_row = N;
    }

    //printf("Process %d working on rows %d to %d\n", rank, start_row, end_row - 1);

    // shortest path of each process 
    int total_cost = 0;
    for (int i = start_row; i < end_row; i++) 
    {
        int distnce[N], parent[N];
        // calculating ending node 
        BFS(i, (i + rows_per_process) % N, distnce, parent);
        int cost = 0;
        for (int j = 1; j < N; ++j) 
        {
            if (distnce[j] != INT_MAX) 
            {
                cost += distnce[j];
            }
        }
        total_cost += cost;

        //printf("Process %d cost for row %d: %d\n", rank, i, cost);
    }

    // adding them all 
    int total_shortest_path;
    MPI_Reduce(&total_cost, &total_shortest_path, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);


    if (rank == 0) 
    {
        printf("Total cost: %d\n", total_shortest_path);
    }

    MPI_Finalize();
    return 0;
}
