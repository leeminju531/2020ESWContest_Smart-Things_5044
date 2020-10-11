#include <AtQueue.h>

#include <SoftwareSerial.h>
#include <MsTimer2.h>


SoftwareSerial Serial1(2,3); //RX, TX 연결


volatile int count = -1;
volatile boolean period = false;
AtQueue instructionQueue,responseQueue;

bool initialFlag; // 초기화시, 작동할 flag. 초기값 = true
bool atFlag;      //at 명령어를 bluetooth에 써줬을때, 작용할 flag. 초기값 = false
bool responseFlag; // at 명령어의 제대로된 응답이 들어왔을때, 작용할 flag. 초기값 = false

char* response; // AT응답 명령어 저장할 변수
unsigned int cycle = 0;

unsigned int durationCycle[5] = {0,};
boolean durationFlag[5] = {0,};
//울타리를 대체할 스위치가 눌렸을경우, 12초(10 번의 cycle) 동안 눌렸음을 전파해준다.
//RF 를 통핸 통신이기에, 싱크 문제, 인식안되는 문제를 고려해야함

volatile char str[100];
volatile int cnt = 0;



boolean fence[5] = {0,};

void setup(){
  Serial.begin(9600);
  Serial1.begin(9600);
  MsTimer2::set(300,scheduling);
  MsTimer2::start();
  pinMode(7,INPUT_PULLUP);
  pinMode(8,INPUT_PULLUP);
  pinMode(9,INPUT_PULLUP);
  pinMode(10,INPUT_PULLUP);
  pinMode(11,INPUT_PULLUP);
  
  initialFlag = true;
  atFlag = false;
  responseFlag = false;
}

void loop() {
  if( period == true ){
    period = false;

    switch(count){
      case 0:
//        detectFence();

        durationFence(); //순간적인 감지일 지라도, 12초간(10cycle) detection을 유지시켜 주어야 함.

        break;
      case 1:
        if(atFlag == true){
          int responseSize = strlen(response);
          int count = 0;
 
          if(cnt >= responseSize)
            for (int i = 0; i < responseSize ; i++)
              if(response[i] == str[cnt - responseSize + i])
                count++;
              else
                count = 0;
                
          if( count == responseSize ){ // 정상 응답한 상태
//            for(int i = 0;i<responseSize;i++){
//              Serial.print(str[cnt-responseSize+i]);
//            }
//            Serial.println(" 버퍼 정상 응답 완료");
            cycle = 0;
            count = 0;
            cnt = 0;
            responseFlag = true; 
            
          
          }else if( responseSize == 17 ){ // broadcating 명령어의 경우, 정상 action 을 취함에도 데이터가 깨져서 들어오는 상황이 발생할 수 있다.
//            for(int i = 0;i<responseSize;i++){
//              Serial.print(str[cnt-responseSize+i]);
//            }
//            Serial.println(" 버퍼 깨진 응답 완료");
            cycle = 0;                  
            count = 0;
            cnt = 0;
            responseFlag = true; 
            
          }

          if( cycle >= 3 && responseFlag == false ){ // 위 시간동안 정상응답 없을 시, 초기화.
            cycle = 0;
            initialFlag = true;
//            Serial.println(" 버퍼 응답 없음. 초기화");
 
           }       
        }

        break;
      case 2:
        if (initialFlag == true ){
          initialFlag = false;
           // Queue 초기화
          instructionQueue.reset();
          responseQueue.reset();    

          instructionQueue.pushQueue("AT");
          responseQueue.pushQueue("OK");
          instructionQueue.pushQueue("AT+ROLE0");
          responseQueue.pushQueue("OK+Set:0");
          instructionQueue.pushQueue("AT+IMME0");
          responseQueue.pushQueue("OK+Set:0");  // broadcasting mode 기본 AT 명령어
          
          atFlag = false;
          responseFlag = false;
          
        }else{ //초기 상태가 아닐때
          if( responseFlag == true ){
            responseFlag = false;
            atFlag = false;
            if ( instructionQueue.isEmpty() ){

              
            String Fence0  = String(fence[0]);
            String Fence1  = String(fence[1]);
            String Fence2  = String(fence[2]);
            String Fence3  = String(fence[3]);
            String Fence4  = String(fence[4]);
  
            String ibe0 = "AT+IBE0"+Fence0+Fence1+Fence2+Fence3+Fence4+"009";
            String ibe0_response = "OK+Set:0x"+Fence0+Fence1+Fence2+Fence3+Fence4+"009";
            char buf1[50];
            char buf2[50];
            ibe0.toCharArray(buf1,ibe0.length()+1);
            ibe0_response.toCharArray(buf2,ibe0_response.length()+1);
            
            instructionQueue.pushQueue(buf1);
            responseQueue.pushQueue(buf2);
            
            }
           
           }
        }

        break;
      case 3:
         if(atFlag == false)
          if(instructionQueue.isEmpty() == false){ // 수행할 명령어가 queue에 있을때만 써준다.
            char* instruction = instructionQueue.popQueue();
            Serial1.write(instruction);
            Serial.print(instruction);
            Serial.println(" 명령어 썻음");
            response = responseQueue.popQueue();
            Serial.print(response);
            Serial.println(" 명령어 대기");
            atFlag = true; //atFlag를 통해 명령어 써줬음을 알린다.
          }

        cycle++;
        

        break;
      default:
        initialFlag = false;
        instructionQueue.reset();
        responseQueue.reset();
        count = 0;
        break;
    }
  }
  if(Serial1.available()){
    str[cnt] = Serial1.read();
    Serial.print(str[cnt]);
    cnt++;
  }
  
  
}


void scheduling(){
  count = (count+1)%4;
  period = true;
}

//void detectFence(){
//  
//
//  
//  fence[0] = !digitalRead(7);
//  fence[1] = !digitalRead(8);
//  fence[2] = !digitalRead(9);
//  fence[3] = !digitalRead(10);
//  fence[4] = !digitalRead(11);
//
//  
//  Serial.print(fence[0]);
//  Serial.print(fence[1]);
//  Serial.print(fence[2]);
//  Serial.print(fence[3]);
//  Serial.print(fence[4]);
//  Serial.println();
//  Serial.println();
//  //회로는 pull-up 으로 구성함  -- > ! 연산을 통해 1->인식됨, 0->인식x 상태로 저장.
//}

void durationFence(){
  
  for(int i=0; i<5; i++){
    if(durationFlag[i] == false)
      fence[i] = !digitalRead(i+7);
    
    if(fence[i] == true){
      durationFlag[i] = true;
      durationCycle[i]++;
    }
      
    if( durationCycle[i] == 9 ){
      durationFlag[i] = false;
      durationCycle[i] = 0;
    }   
  }

//  Serial.print(fence[0]);
//  Serial.print(fence[1]);
//  Serial.print(fence[2]);
//  Serial.print(fence[3]);
//  Serial.print(fence[4]);
//  Serial.println();
//  Serial.println();
//       
}
