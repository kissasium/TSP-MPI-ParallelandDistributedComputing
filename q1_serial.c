// kissa zahra , i21-0572 , and assignment#02
#include <stdio.h>
#include <limits.h>
#include <stdbool.h>

#define N 10 
int queue[N * N];
int front = -1, rear = -1;

// adjacency matrix
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

// bfs
void BFS(int start, int* distnce, int* parent) 
{

    int visited[N] = { 0 };

    for (int i = 0; i < N; ++i) 
    {
        distnce[i] = INT_MAX; // max
        parent[i] = -1; // no paremt 
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

int main() 
{
    int starting_node = 0; 
    int total_cost = 0;
    int distnce[N], parent[N];

    BFS(starting_node, distnce, parent);


    printf("Shortest Path from node %d:\n", starting_node);
    for (int i = 0; i < N; ++i) 
    {
        if (distnce[i] != INT_MAX) 
        {
            total_cost += distnce[i];
            printf("Node %d -> ", i);
        }
    }
    printf("Node %d\n", starting_node);

    // adding cost of the last edge to starting node 
    total_cost += graph[parent[N - 1]][starting_node];

    printf("Total cost: %d\n", total_cost);

    return 0;
}
