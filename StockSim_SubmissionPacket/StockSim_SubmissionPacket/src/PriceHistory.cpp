#include "PriceHistory.h"
#include "CSVParser.h"
#include <iostream>
using namespace std;

//PriceHistory========================================================================
PriceHistory::PriceHistory() : head(nullptr), tail(nullptr), size(0) {}

PriceHistory::~PriceHistory() {
    while (head) {
        PriceNode* temp = head;
        head = head->next;
        delete temp;
    }
}

// Appends a new PriceNode to the tail (most-recent end)
void PriceHistory::append(const string& date, double open, double high,
                          double low, double close, long volume) {
    PriceNode* newNode = new PriceNode(date, open, high, low, close, volume); // Assuming close is the price
    if (!head) {
        head = tail = newNode;
    } else {
        newNode->prev = tail;
        tail->next = newNode;
        tail = newNode;
    }
    size++;
}

PriceNode* PriceHistory::findByDate(const string& date) const {
    PriceNode* current = head;
    while (current) {
        if (current->date == date) {
            return current;
        }
        current = current->next;
    }
    return nullptr;
}

void PriceHistory::printRange(const string& startDate, const string& endDate) const {
    PriceNode* current = head;
    while (current) {
        if (CSVParser::dateInRange(current->date, startDate, endDate)) {
            cout << current->date << ": " << current->open << ": " << current->high << ": " << current->low << ": " << current->close << ": " << current->volume << ": " << endl;
        }
        if (current->date > endDate) break;
        current = current->next;
    }
}
    // Returns the head (oldest) node pointer — read only
PriceNode* PriceHistory::getHead() const {
    return head;
}

    // Returns the tail (newest) node pointer — read only
PriceNode* PriceHistory::getTail() const {
    return tail;
}

int PriceHistory::getSize() const {
    return size;
}

//Iterator=========================================================================================
PriceHistory::Iterator::Iterator(PriceNode* node) {
    current=node;
}

PriceHistory::Iterator& PriceHistory::Iterator::operator++() {
    current=current->next;
    return *this;
}

PriceNode& PriceHistory::Iterator::operator*() {
    return *current;
}

bool PriceHistory::Iterator::operator!= (const Iterator& other) const {
    return current != other.current;
}

PriceHistory::Iterator PriceHistory::begin() const {
    return Iterator(head);
}

PriceHistory::Iterator PriceHistory::end() const {
    return Iterator(nullptr);
}

//ReverseIterator=========================================================================================

PriceHistory::ReverseIterator::ReverseIterator(PriceNode* node) {
    current=node;
}

PriceHistory::ReverseIterator& PriceHistory::ReverseIterator::operator++() {
    current=current->prev;
    return *this;
}

PriceNode& PriceHistory::ReverseIterator::operator*() {
    return *current;
}

bool PriceHistory::ReverseIterator::operator!= (const ReverseIterator& other) const {
    return current != other.current;
}

PriceHistory::ReverseIterator PriceHistory::rbegin() const {
    return ReverseIterator(tail);
}

PriceHistory::ReverseIterator PriceHistory::rend() const {
    return ReverseIterator(nullptr);
}
