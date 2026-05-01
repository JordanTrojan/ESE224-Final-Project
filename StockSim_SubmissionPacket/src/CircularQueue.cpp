#include "CircularQueue.h"

CircularQueue::CircularQueue(int capacity){
    buffer = new double[capacity];
    this->capacity = capacity;
    count = 0;
    tail = 0;
    head = 0;
}
CircularQueue::~CircularQueue(){
    delete[] buffer;
}
void CircularQueue::enqueue(double value){
    if(isFull()){
        buffer[tail] = value;
        head = (head + 1) % capacity;
        tail = (tail + 1) % capacity;
    }else{
        buffer[tail] = value;
        tail = (tail + 1) % capacity;
        count++;
    }
}

double CircularQueue::dequeue(){
    if(isEmpty()){
        return 0.0;
    }
    double value = buffer[head];
    head = (head + 1) % capacity;
    count--;
    return value;
}
double CircularQueue::getAverage() const{
    if(isEmpty()){
        return 0.0;
    }
    double sum = 0.0;
    for(int i = 0; i < count; i++){
        int index = (head+i) % capacity;
        sum += buffer[index];
    }
    return sum / count;
}

double CircularQueue::peek() const{
    if(isEmpty()){
        return 0.0;
    }
    return buffer[head];
}    
bool CircularQueue::isFull() const{
    return count == capacity;
}
bool CircularQueue::isEmpty() const{
    return count == 0;
}
int CircularQueue::getCount() const{
    return count;
}
int CircularQueue::getCapacity() const{
    return capacity;
}

