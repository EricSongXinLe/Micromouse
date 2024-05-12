#ifndef QUEUE_H
#define QUEUE_H

#include <stdlib.h> // Add this line to include the stdlib.h header file

struct Coord {
    int x;
    int y;
};

// Definition of QueueNode struct
struct QueueNode {
    struct Coord data;
    struct QueueNode* next;
};

// Definition of Queue struct
struct Queue {
    struct QueueNode* front;
    struct QueueNode* rear;
};

// Function to create a new QueueNode
struct QueueNode* createQueueNode(struct Coord coord) {
    struct QueueNode* newNode = (struct QueueNode*)malloc(sizeof(struct QueueNode));
    if (newNode == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(1);
    }
    newNode->data = coord;
    newNode->next = NULL;
    return newNode;
}

// Function to create an empty Queue
struct Queue* createQueue() {
    struct Queue* queue = (struct Queue*)malloc(sizeof(struct Queue));
    if (queue == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(1);
    }
    queue->front = queue->rear = NULL;
    return queue;
}

// Function to check if the queue is empty
int isEmpty(struct Queue* queue) {
    return (queue->front == NULL);
}

// Function to enqueue an element to the rear of the queue
void push_back(struct Queue* queue, struct Coord coord) {
    struct QueueNode* newNode = createQueueNode(coord);
    if (isEmpty(queue)) {
        queue->front = queue->rear = newNode;
    } else {
        queue->rear->next = newNode;
        queue->rear = newNode;
    }
}

// Function to dequeue an element from the front of the queue
struct Coord pop(struct Queue* queue) {
    if (isEmpty(queue)) {
        fprintf(stderr, "Queue is empty\n");
        exit(1);
    }
    struct QueueNode* temp = queue->front;
    struct Coord data = temp->data;
    queue->front = queue->front->next;
    free(temp);
    return data;
}

#endif