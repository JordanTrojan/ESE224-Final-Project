#include "CircularQueue.h"

CircularQueue(int capacity){
    buffer = new double[capacity];
    this->capacity = capacity;
    count = 0;
    tail = 0;
    head = 0;
}
~CircularQueue(){
    delete[] buffer;
}