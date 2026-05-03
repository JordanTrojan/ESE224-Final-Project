#include "TradeStack.h"
#include <string>
#include <iostream>
using namespace std;

TradeStack::TradeStack() {
    top = nullptr;
    size = 0;
}
TradeStack::~TradeStack() {
    while (top != nullptr) {
        StackNode* temp = top;
        top = top->next;
        delete temp;
    }
}

void TradeStack::push(const TradeRecord& record) {
    StackNode* newNode = new StackNode{record, top};
    top = newNode;
    size++;
}

// Remove and return the top record.
// Precondition: !isEmpty()
TradeRecord TradeStack::pop() { 
    if (isEmpty()) {
        cout << "Error: stack is empty" << endl;
        return TradeRecord(); // Return default record if stack is empty
    }
    StackNode* temp = top;
    top = top->next;
    TradeRecord record = temp->data;
    delete temp;
    size--;
    return record;

}

// Return the top record without removing it.
// Precondition: !isEmpty()
TradeRecord TradeStack::peek() const {
    if (isEmpty()) {
        cout << "Error: stack is empty" << endl;
        return TradeRecord();            
    } 
    StackNode* temp = top;
    return temp->data;
}

bool TradeStack::isEmpty() const {
    return size == 0;
}
int  TradeStack::getSize() const {
    return size;
}

void TradeStack::printAll() const {
    StackNode* current = top;
    while (current != nullptr) {
        cout << "Action: " << current->data.action
             << " | Ticker: " << current->data.ticker
             << " | Date: "   << current->data.date
             << " | Shares: " << current->data.shares
             << " | Price: $" << current->data.price
             << " | Total: $" << current->data.totalCost
             << endl;
        current = current->next;
    }
}

