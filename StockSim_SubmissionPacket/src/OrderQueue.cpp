#include "OrderQueue.h"
#include <string>
#include <iostream>

using namespace std;

OrderQueue::OrderQueue() {
    front = nullptr;
    back = nullptr;
    size = 0;
}
OrderQueue::~OrderQueue() {
    QueueNode* temp;
    while (!isEmpty()) {
        temp = front;
        front = front->next;
        delete temp;
    }
}

// Add an order to the back of the queue.
void OrderQueue::enqueue(const Order& order) {
    QueueNode* newNode = new QueueNode{order, nullptr};
        if (isEmpty()) {
            front = newNode;
            back = newNode;
        } else {
            back->next = newNode;
            back = newNode;
        }
        size++;
}

// Remove and return the order at the front.
// Precondition: !isEmpty()
Order OrderQueue::dequeue() {
    if (isEmpty()) {
        return Order();
    }
    QueueNode* temp = front;
    front = front->next;
    Order order = temp->data;
    delete temp;
    size--;
    return order;
}

// Return the front order without removing it.
// Precondition: !isEmpty()
Order OrderQueue::peek() const {
    if(isEmpty()){
        return Order();
    }
    return front->data;
}

bool OrderQueue::isEmpty() const {
    return size == 0;
}

int OrderQueue::getSize() const {
    return size;
}

// Print all pending orders front to back.
void OrderQueue::printAll() const {
    QueueNode* current = front;
    while (current != nullptr) {
        cout << "Side: "   << current->data.side
             << " | Ticker: " << current->data.ticker
             << " | Type: "   << current->data.type
             << " | Shares: " << current->data.shares
             << " | Target: $"<< current->data.targetPrice
             << " | Date: "   << current->data.submittedDate
             << endl;
        current = current->next;
    }
}