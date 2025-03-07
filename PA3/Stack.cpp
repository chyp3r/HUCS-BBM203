#include "Stack.h"
#include <iostream>

// Constructor to initialize an empty stack
Stack::Stack() {
    top = -1;
}

// Adds a province to the top of the stack
void Stack::push(const int province) {
    const int size = getSize();
    if(size<MAX_SIZE) {
        top++;
        data[top] = province;
    }
}

// Removes and returns the top province from the stack
int Stack::pop() {
    if(top>=0) {
        const int item = data[top];
        top--;
        return item;
    }
    return -1;
}

// Returns the top province without removing it
int Stack::peek() const {
    return top != -1 ? data[top]: -1;
}

// Checks if the stack is empty
bool Stack::isEmpty() const {
    return getSize() == 0;
}

// Function to get the current size of the stack
int Stack::getSize() const {
    return top+1;
}
