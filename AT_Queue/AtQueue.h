#ifndef Queue
#define Queue

class AtQueue {

public:
    int getQueueSize();
    void pushQueue(char* instruction);
    bool isEmpty();
    void reset();
    char* pushQueue();
    char* popQueue();
    

private:
   
    int front = 0;
    int rear = 0;
    int rows = 10;
    char* queue[10];  //포인터 배열을 통해 circular queue를 구성한다.
                       //1열 -> 지시할 명령어 저장
                       //2열 -> 응답할 명령어 저장
    
};



#endif