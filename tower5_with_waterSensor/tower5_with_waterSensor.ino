#include <SoftwareSerial.h>
#include <MsTimer2.h>
#include <AtQueue.h>

SoftwareSerial Serial1(2,3); //RX, TX 연결

volatile int count = -1;
volatile boolean period = false;
AtQueue instructionQueue,responseQueue;

bool initialFlag; // 초기화시, 작동할 flag. 초기값 = true
bool atFlag;      //at 명령어를 bluetooth에 써줬을때, 작용할 flag. 초기값 = false
bool responseFlag; // at 명령어의 제대로된 응답이 들어왔을때, 작용할 flag. 초기값 = false

char* response; // AT응답 명령어 저장할 변수
char* bluetoothFairingResponse = "OK+CONN";
char* bluetoothFairingQuit = "OK+LOST";
unsigned int cycle = 0;

volatile char str[100];
volatile int cnt = 0;

int water_level;
int water_percent;

void setup() {
  Serial.begin(9600);
  Serial1.begin(9600);
  MsTimer2::set(300,scheduling);
  MsTimer2::start();
  initialFlag = true;
  atFlag = false;
  responseFlag = false;

}

void loop() {
  

  if( period == true ){
    period = false;

    switch(count){
      case 0: // sensing
        detectWater();

        break;
        
      case 1: // 응답 완료 확인
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

            cycle = 0;
            count = 0;
            cnt = 0;
            responseFlag = true; 
            
          
          }else if( responseSize == 17 ){ // broadcating 명령어의 경우, 정상 action 을 취함에도 데이터가 깨져서 들어오는 상황이 발생할 수 있다.

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

      case 2 : // 판단
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
            if (instructionQueue.isEmpty() ){
              String waterPercent = String(water_percent);
              if(waterPercent.length() == 1){ // 1~9%인 한자리를 나타내는 경우 -> 001~009%로 써준다.
                waterPercent = "00"+waterPercent;
              }else if(waterPercent.length() == 2){ // 10%~99%인 두자리를 나타내는 경우 -> 010~099%로 써준다.
                waterPercent = "0"+waterPercent;
              }
                    
              String ibe0 = "AT+IBE0"+waterPercent.substring(0,3)+"00009";
              String ibe0_response = "OK+Set:0x"+waterPercent.substring(0,3)+"00009";
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
      case 3 : // at 명령어
        if(atFlag == false)
          if(instructionQueue.isEmpty() == false){ // 수행할 명령어가 queue에 있을때만 써준다.
            char* instruction = instructionQueue.popQueue();
            Serial1.write(instruction);

            response = responseQueue.popQueue();
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

  
  
//  if (Serial1.available()) {
//    str[cnt] = Serial1.read();
////    Serial.print(str[cnt]);
//    cnt++;
//  }
  serialEvent();


  checkFairing();
  
}

void checkFairing(){
  

    
if( str[cnt-1] == 'N' && str[cnt-2] == 'N' && str[cnt-3] == 'O' &&str[cnt-4] == 'C' && str[cnt-5] == '+' && str[cnt-6] == 'K' && str[cnt-7] == 'O'){
  

  while(1){
    serialEvent();
    if(str[cnt-1] =='T' && str[cnt-2] =='S' && str[cnt-3] =='O' && str[cnt-4] =='L' && str[cnt-5] =='+' && str[cnt-6] =='K' && str[cnt-7] =='O' ) {
      Serial.print(str[cnt-8]);
      cnt = 0;
      initialFlag = true;
      break;
    }
  }

    //데이터 받아오기
    //시리얼 통신으로 데이터 써주기
    //끊길때까지 기다리기
    //초기화해주기
  }//end of if 문

}



void detectWater(){
  water_level = analogRead(A0);
  water_percent = map(water_level,0,500,0,100);
  
  if(water_percent > 100)
    water_percent = 100;

}

// uno 의 경우 시리얼 이벤트 제공 x << 이는 사용자 정의 함수로서 사용함
void serialEvent(){
  if (Serial1.available()) {
    str[cnt] = Serial1.read();
//    Serial.print(str[cnt]);
    cnt++;
  }
  
}

void scheduling(){
  count = (count+1)%4;
  period = true;
}
