#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define MAX_VERTICES 100

typedef struct Node
{
    int vertex;
    struct Node *next;
} Node;

typedef struct Graph
{
    int numVertices;
    Node *adjList[MAX_VERTICES];
    bool visited[MAX_VERTICES];
} Graph;

typedef struct Queue
{
    int items[MAX_VERTICES];
    int front, rear;
} Queue;

void bfs(Graph *graph, int startVertex);
int dequeue(Queue *queue);
void enqueue(Queue *queue, int value);
bool isEmpty(Queue *queue);
Queue *createQueue();

Queue *createQueue()
{
    Queue *queue = (Queue *)malloc(sizeof(Queue));
    queue->front = -1;
    queue->rear = -1;
    return queue;
}

bool isEmpty(Queue *queue)
{
    return queue->front == -1;
}

void enqueue(Queue *queue, int value)
{
    if (queue->rear == MAX_VERTICES - 1)
    {
        printf("Fila cheia!\n");
        return;
    }

    if (queue->front == -1)
        queue->front = 0;

    queue->items[++queue->rear] = value;
}

int dequeue(Queue *queue)
{
    if (isEmpty(queue))
    {
        printf("Fila vazia!\n");
        return -1;
    }

    int value = queue->items[queue->front];
    if (queue->front >= queue->rear)
    {
        queue->front = queue->rear = -1;
    }
    else
    {
        queue->front++;
    }

    return value;
}

void bfs(Graph *graph, int startVertex)
{
    Queue *queue = createQueue();

    graph->visited[startVertex] = true;
    enqueue(queue, startVertex);

    printf("Ordem de visita: ");

    while (!isEmpty(queue))
    {
        int currentVertex = dequeue(queue);
        printf("%d ", currentVertex);

        Node *temp = graph->adjList[currentVertex];
        while (temp)
        {
            int adjVertex = temp->vertex;

            if (!graph->visited[adjVertex])
            {
                graph->visited[adjVertex] = true;
                enqueue(queue, adjVertex);
            }

            temp = temp->next;
        }
    }

    printf("\n");

    free(queue);
}
