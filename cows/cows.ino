#include <MsTimer2.h>
#include <AtQueue.h>
#include <cow.h>
#include <Wire.h>

#define BROADCASTING 1
#define SCANNING 0
#define STABLE 1
#define UNSTABLE 0

#define BLUETOOTHSTABLE 1
#define BLUETOOTHERROR 0

const int MPU=0x68;  //MPU 6050 의 I2C 기본 주소
int AcX,AcY;

int redpin = 46; 
int greenpin =45;
int bluepin =44; 


int temperaturePin = 0;

char str[3000];
int cnt = 0;

char* instruction; // AT 명령어 저장할 변수
char* response; // AT응답 명령어 저장할 변수


volatile int count = -1;
volatile boolean period = false;
Cow cow;
AtQueue instructionQueue,responseQueue;

unsigned int cycle = 0;

bool initialFlag; // 초기화시, 작동할 flag. 초기값 = true
bool atFlag;      //at 명령어를 bluetooth에 써줬을때, 작용할 flag. 초기값 = false
bool responseFlag; // at 명령어의 제대로된 응답이 들어왔을때, 작용할 flag. 초기값 = false
bool modeFlag; //0->Scanning Mode, 1->BroadCasting Mode

bool analysis_str_for_rssi; //scanning 을 완료했을시, str 문자열을 분석하여 rssi값을 가져와야 한다.
int numForAnalysis;

int closeTower = 0;

unsigned long pre;
unsigned long cur;
void setup() {
  Wire.begin();      //Wire 라이브러리 초기화
  Wire.beginTransmission(MPU); //MPU로 데이터 전송 시작
  Wire.write(0x6B);  // PWR_MGMT_1 register
  Wire.write(0);     //MPU-6050 시작 모드로
  Wire.endTransmission(true); 
  Serial.begin(9600);
  Serial1.begin(9600);
  pinMode(redpin, OUTPUT);
  pinMode(bluepin, OUTPUT);
  pinMode(greenpin, OUTPUT);
  MsTimer2::set(100,scheduling);
  MsTimer2::start();
  color(0,0,255); //scanning 이전 기본 LED 색 : BLUE
  initialFlag = true;
  atFlag = false;
  responseFlag = false;
  modeFlag = SCANNING;
  analysis_str_for_rssi = false;
  
}

void loop() {
  
  if(period == true){
    period = false;
    switch(count){
      case 0:
        
        cow.setTemperature(Temperature());
//        Serial.println("temperature measurement Section Completed");
        break;
        
      case 1:
        mpuGet();
        cow.inputImu(abs(AcX),abs(AcY));
//        Serial.println("imu measurement Section Completed");
        break;
        
      case 2:// buffer 확인 ms 체크할 것.(20ms 가정)
      
        if(atFlag == true )//AT 명령어를 써주었다면
        {
          int responseSize = strlen(response);
          int atSize = strlen(instruction);
          int count = 0;
          bool bluetoothState = BLUETOOTHSTABLE;
          
          if(cnt >= responseSize)
            for (int i = 0; i < responseSize ; i++){
              if(response[i] == str[cnt - responseSize + i]){ //완료 응답 명령어가 기대응답과 일치하는지 확인한다.
                count++;
                bluetoothState = BLUETOOTHSTABLE;
              }else if(instruction[i] == str[cnt - responseSize + i]){  // 완료한 명령어가 입력한 명령어와 일치하는 상황
                count++;                                               // 이 상태의 경우, hw error로 회생 불가함  << 온도 문제 추정  
                bluetoothState = BLUETOOTHERROR;
              }else{
                count = 0;
                bluetoothState = BLUETOOTHSTABLE;
              }
            }
                
          if(count == responseSize){ // 응답 했음

            if ( bluetoothState = BLUETOOTHERROR ){  //이 상태의 경우 하드웨어적 bluetooth의 완전한 error.
                                                     //sw 로 해결 불가 - > 육안으로 이러한 상태를 파악하기 위해 3색 LED 로 구분지어준다.                                
             color(250,250,0);  
             while(1){
                                         
             }
            }
            
            if(response == "OK+DISCE"){ // Scanning 완료 응답 명령어
              analysis_str_for_rssi = true;
              numForAnalysis = cnt;
              }
            cycle = 0;
            count = 0;
            cnt = 0;
            responseFlag = true; 
            
         }else if( modeFlag == BROADCASTING )               // "OK+Set:0x23940005" 의 응답을 보이는 부분에서만 유일하게 한자리가 깨지는 상황이 빈번하게 발생함.(buffer 문제 추정)
          if( responseSize == 17 ){                        //그러나, Bluetooth는 실제로 명령어의 역할을 제대로 수행했기 때문에, 역할에는 영향을 주지 않았음.
//            Serial.println("broken but, responsed is ok ");//위 상황에 대해서는 응답 크기를 통해, 실제 기대응답과 같지 않아도 응답Flag 를 true로 취급해 주기 위함.
//            for(int i = 0;i<responseSize;i++){
//              Serial.print(str[cnt-responseSize+i]);
//            }
            cycle = 0;                  
            count = 0;
            cnt = 0;
            responseFlag = true; 
            
          }
         
  

           if( cycle >= 5 && responseFlag == false ){ //4.5sec(0.9*5sec) 시간동안 정상응답 없을 시, 초기화.
            cycle = 0;
            initialFlag = true;
//            Serial.println("bluetooth don't response to AT instruction, started initializing");

           }
           
        }
//        Serial.println("buffer checking Section completed");
        break;
        
      case 3:// 판단 
        
        if( initialFlag == true){
          initialFlag = false;
          
          // Queue 초기화
          instructionQueue.reset();
          responseQueue.reset();    
          color(0,0,255); //scanning 이전 기본 LED 색 : BLUE
          //bluetooth Scanning Mode 설정 queue에 저장.
          instructionQueue.pushQueue("AT");
          responseQueue.pushQueue("OK");
          instructionQueue.pushQueue("AT+IMME1");
          responseQueue.pushQueue("OK+Set:1");
          instructionQueue.pushQueue("AT+ROLE1");
          responseQueue.pushQueue("OK+Set:1");
          
          
          modeFlag = SCANNING;
     
          atFlag = false;
          responseFlag = false;
         
        }else{
          if( responseFlag == true){
            
            responseFlag = false;
            atFlag = false;


            if(analysis_str_for_rssi == true){
//              Serial.println("Search All Towers");
              analysis_str_for_rssi = false;
              closeTower = Analysis(numForAnalysis);
              if( closeTower == -1 ){ // 모든 tower 탐색 실패시, 재 수행.
                color(255,0,0); // scanning 실패 시, LED 색 : RED
//                Serial.println("All Tower Recognition Failures");

                instructionQueue.reset();
                responseQueue.reset();
                instructionQueue.pushQueue("AT");
                responseQueue.pushQueue("OK");
                initialFlag = true;
                
              }else{
                color(0,255,0); // scanning 성공시, LED 색 : GREEN
//                Serial.println("All Tower Recognition Successfully Recognized");
                modeFlag = BROADCASTING;
              }
            }
              
            if (instructionQueue.isEmpty() ){
              
              switch(modeFlag){
                case SCANNING:
                  if( cow.getState() == STABLE ){//Scnning -> Broadcast mode 변환
                    color(0,0,255); //scanning 이전 기본 LED 색 : BLUE
                    instructionQueue.reset();
                    responseQueue.reset();
                    instructionQueue.pushQueue("AT+DISI?");
                    responseQueue.pushQueue("OK+DISCE");
                    instructionQueue.pushQueue("AT+ROLE0");
                    responseQueue.pushQueue("OK+Set:0");
                    instructionQueue.pushQueue("AT+IMME0");
                    responseQueue.pushQueue("OK+Set:0");
                    modeFlag = BROADCASTING;
       
                  }else{ // cow.getState == UNSTABLE
                    instructionQueue.pushQueue("AT");
                    responseQueue.pushQueue("OK");
                  }
                  break;
                  
                case BROADCASTING:
                  if( cow.getState() == STABLE ){
                    color(100,100,100); // broadcasting mode 일때, rgb 색 
                    
                    String temperature = String(cow.getTemperature());
                    
                    String ibe0 = "AT+IBE0"+temperature.substring(0,2)+temperature.substring(3,5)+"000"+String(closeTower);
                    String ibe0_response = "OK+Set:0x"+temperature.substring(0,2)+temperature.substring(3,5)+"000"+String(closeTower);
                    char buf1[100];
                    char buf2[100];
                    ibe0.toCharArray(buf1,ibe0.length()+1);
                    ibe0_response.toCharArray(buf2,ibe0_response.length()+1);
                    
                    
                    instructionQueue.pushQueue(buf1);
                    responseQueue.pushQueue(buf2);
                    
                  }else{ // cow.getState == UNSTABLE
                    color(0,0,255); //scanning 이전 기본 LED 색 : BLUE
                    instructionQueue.pushQueue("AT+IMME1");
                    responseQueue.pushQueue("OK+Set:1");
                    instructionQueue.pushQueue("AT+ROLE1");
                    responseQueue.pushQueue("OK+Set:1");
                    
                    modeFlag = SCANNING;
                    
                  }
  
                  break;
              }
            
          }
         }
          
        }
        
//        Serial.println("Judgment Section Completion");
      case 4:
   
        break;
        
      case 5:// at명령어 수행(10ms 가정)
      
        if(atFlag == false) // atFlag =false -> at명령어 안쓴 상태(이미 썻는데 또 쓰기 방지)
          if(instructionQueue.isEmpty() == false){ // 수행할 명령어가 queue에 있을때만 써준다.
            instruction = instructionQueue.popQueue();
            Serial1.write(instruction);
//            Serial.print(instruction);
//            Serial.println(" commanded to bluetooth");
            response = responseQueue.popQueue();
//            Serial.print(response);
//            Serial.println(" 명령어 대기");
            atFlag = true; //atFlag를 통해 명령어 써줬음을 알린다.
          }
        cycle++; // case 맨 마지막 cycle 증가 시키기(응답 x 경우 check를 위함)
        
//        Serial.println("command writing section completed ");
//        Serial.println();

      case 6:
      case 7:
      case 8:
        break;
        
      default :
//        Serial.println("default 실행, ms 재측정바람 ");
        initialFlag = false;
        instructionQueue.reset();
        responseQueue.reset();
        modeFlag = SCANNING;
        count = 0;
        break;
  
      
    }
    
  }
 
 
}
void serialEvent1(){
  str[cnt] = Serial1.read();
//  Serial.print(str[cnt]);
  cnt++;
}

void scheduling(){
  count = (count+1)%9;
//  Serial.println("Dd");
  period = true;
}

int Analysis(int num){
  int rssi[5] = {-1,-1,-1,-1,-1}; // 각 tower에 대응하는 rssi 값을 배열에 저장 ex) n번인덱스 의값 -> (n+1)번 tower의 rssi 값.
  String trans = "   ";
  
  for( int i = 0; i<num ; i++ ){ // 각 tower uuid 에 대응하는 rssi 값을 추출한다.
    if(str[i] == '5' && str[i-1] == 'D' && str[i-2] == 'D' && str[i-3] == '1' && str[i-4] =='C'){
      trans.setCharAt(0,str[i+4]);
      trans.setCharAt(1,str[i+5]);
      rssi[0] = trans.toInt();
      Serial.println("Tower1 Recognition Success ");
    }   
    else if(str[i] == 'D' && str[i-1] == '1' && str[i-2] == '0' && str[i-3] == '1' && str[i-4] =='D'){
      trans.setCharAt(0,str[i+4]);
      trans.setCharAt(1,str[i+5]);
      rssi[1] = trans.toInt();
      Serial.println("Tower2 Recognition Success ");
    }
    else if(str[i] == '4' && str[i-1] == 'C' && str[i-2] == '3' && str[i-3] == '1' && str[i-4] =='D'){
      trans.setCharAt(0,str[i+4]);
      trans.setCharAt(1,str[i+5]);
      rssi[2] = trans.toInt();
      Serial.println("Tower3 Recognition Success ");
    } 
    else if(str[i] == '4' && str[i-1] == 'C' && str[i-2] == 'B' && str[i-3] == 'F' && str[i-4] =='B'){
      trans.setCharAt(0,str[i+4]);
      trans.setCharAt(1,str[i+5]);
      rssi[3] = trans.toInt();
      Serial.println("Tower4 Recognition Success ");

    } 
    else if(str[i] == '5' && str[i-1] == '8' && str[i-2] == '4' && str[i-3] == 'E' && str[i-4] =='D'){
      trans.setCharAt(0,str[i+4]);
      trans.setCharAt(1,str[i+5]);
      rssi[4] = trans.toInt();
      Serial.println("Tower5 Recognition Success ");
    } 
  }

  int close_tower[2] = {-1,200};//rssi 배열중 가장 근접한 tower에 대한 정보
                              //[0] - > tower위치를 저장
                              //[1] - > 근접 타워의 rssi 값을 저장
                              
  for(int i=0; i<5;i++){ //rssi값 가장 작은 tower를 찾는다.
    if(close_tower[1]>=rssi[i]){
      close_tower[0] = i;
      close_tower[1] = rssi[i];
    }
  }
  close_tower[0] = close_tower[0] + 1; // 가장 근접한(rssi가 가장 작은) tower의 위치를 의미한다.

  for(int i=0 ; i<5 ; i++) // 만약, rssi[5] 값 중 여전히 -1인 것이 존재한다면, 모든 tower 를 scanning 하지 못함을 의미한다.
    if(rssi[i] == -1)
      return -1; 
    
  

  return close_tower[0];
}





void color(int R,int G,int B){
  analogWrite(redpin,R);
  analogWrite(greenpin,G);
  analogWrite(bluepin,B);
}

float Temperature(){
  float value = analogRead(temperaturePin)/3.3; 
  return value;
}


//mpu센싱값 갱신


void mpuGet(){
  Wire.beginTransmission(MPU);//MPU6050 호출
  Wire.write(0x3B);//AcX 레지스터 위치 요청
  Wire.endTransmission(false);
  Wire.requestFrom(MPU,14,true);//14byte의 데이터를 요청

  AcX=Wire.read()<<8|Wire.read();//두개의 나뉘어진 바이트를 하나로 이어붙임
  AcY=Wire.read()<<8|Wire.read();
}
