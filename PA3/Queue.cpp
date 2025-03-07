#include "Queue.h"
#include <iostream>

#include "Map.h"

// Constructor to initialize an empty queue
Queue::Queue() {
    front = -1;
    rear = -1;
}

// Adds a province to the end of the queue
void Queue::enqueue(int province) {
    if((rear+1) %MAX_SIZE == front) {
        return;
    }
    if(front == -1)
        front = 0;

    rear = (rear+1) % MAX_SIZE;
    data[rear] = province;

}

// Removes and returns the front province from the queue
int Queue::dequeue() {
    if(isEmpty()) {
        return -1;
    }
    if(front == rear) {
        const int province = data[front];
        rear = -1;
        front = -1;
        return province;
    }
    const int province = data[front];
    front = (front+1) % MAX_SIZE;
    return province;

}

// Returns the front province without removing it
int Queue::peek() const {
    if(!isEmpty()) {
        return data[front];
    }
    return -1;
}

// Checks if the queue is empty
bool Queue::isEmpty() const {
    return front == -1;
}

// Add a priority neighboring province in a way that will be dequeued and explored before other non-priority neighbors
void Queue::enqueuePriority(int province) {
    if((rear+1) %MAX_SIZE == front) {
        return;
    }
    if(isEmpty()) {
        front = rear = 0;
        data[front] = province;
    }
    else {
        front = (front-1+MAX_SIZE)%MAX_SIZE;
        data[front] = province;
    }
}