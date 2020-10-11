#include "AtQueue.h"

int AtQueue::getQueueSize(){
    return (rear-front+rows)%rows;
}

bool AtQueue::isEmpty(){

    return (front == rear);
}

void AtQueue::reset(){
    front = 0;
    rear = 0;
}

void AtQueue::pushQueue(char *instruction){//항상 수행할 AT명령어와 그에 대응하는
                                                      //응답 명령어를 같이 써준다.
    rear = (rear+1) % rows;
    
    queue[rear] = instruction;

    //rear 인덱스 1 증가시키고, 그 위치에 명령어 저장.
}

char* AtQueue::popQueue(){
    if(isEmpty() == false){
        front = (front + 1) % rows;
        return queue[front];
    }
    else
        return -1;
        
}

