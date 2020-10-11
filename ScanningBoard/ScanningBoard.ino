#include <SoftwareSerial.h>
#include <MsTimer2.h>
#include <AtQueue.h>

#define FIRST 0
#define SECOND 1
#define THIRD 2
#define FORTH 3
#define FIVE 4

#define TOWER 5   //타워(SLAVE) 갯수

String cow1 = "A810871B6C67";
String cow2 = "A810871B6C57";
String cow3 = "B452A912ACF5";

String tower1 = "A810871C1DD5";
String tower2 = "A810871D101D";
String tower3 = "A810871D13C4";
String tower4 = "A810871BFBC4";
String tower5 = "1862E43DE485";

int tower[TOWER] = {0,0,0,0,0}; 

bool cow1Read;
bool cow2Read;
bool cow3Read;
bool waterRead;
int cowReadNum;

String waterPercent;

volatile int count = -1;
volatile boolean period = false; //스케줄링을 위한 변수.

unsigned int cycle = 0; // 혹시모를 블루투스가 멈추는 상황을 대비한 초기화 주기 체크를 위한 변수

char str[3000];   //disi 명령어를 통해 읽어들인 문자열을 저장
int cnt;//reading data 배열에 저장을 위한 변수

bool initialFlag; // 초기화시, 작동할 flag. 초기값 = true
bool atFlag;      //at 명령어를 bluetooth에 써줬을때, 작용할 flag. 초기값 = false
bool responseFlag; // at 명령어의 제대로된 응답이 들어왔을때, 작용할 flag. 초기값 = false

char* response; // AT응답 명령어 저장할 변수
bool extractDataAtStr; //scanning 을 완료했을시, str 문자열을 분석하기 위함
int numForAnalysis;

AtQueue instructionQueue,responseQueue;

bool fence[20] = {0,};

int waterData = 0; // 이 값을 기반으로 tower5 에서 물의 50%,70%,100%를 써 줄 것.


typedef struct{
  int tower = -1;
  float temperature = -1;
}COW;

COW cow[3];// n 번 index - > n+1 번 cow 의 정보를 저장

void setup() {
  Serial.begin(9600);
  Serial1.begin(9600);
  MsTimer2::set(300,scheduling);
  MsTimer2::start();
  initialFlag = true;
  atFlag = false;
  responseFlag = false;
  cow1Read=false;
  cow2Read=false;
  cow3Read=false;
  waterRead = false;
  cowReadNum = 0;
 

}

void loop() {

  if( period == true ){
  period = false;

  switch(count){
    case 0:
      if( atFlag == true ){
        int responseSize = strlen(response);
        int count = 0;

        if( cnt >= responseSize )
          for (int i = 0; i < responseSize ; i++)
            if(response[i] == str[cnt - responseSize + i])
              count++;
            else
              count = 0;

        if(count == responseSize){

          
          if(response == "OK+DISCE"){
            extractDataAtStr = true;
            numForAnalysis = cnt;
            }
          cycle = 0;
          count = 0;
          cnt = 0;
          responseFlag = true; 

        }

        if( cycle >= 6 && responseFlag == false ){ //10은 임의의 값. 위 시간동안 정상응답 없을 시, 초기화.
          cycle = 0;
          initialFlag = true;
//            Serial.println("bluetooth don't response to AT instruction, started initializing");
         }

        
        
      }
//        Serial.println("buffer checking Section completed");
      break;

    case 1:
     if( initialFlag == true){
      initialFlag = false;
      
      // Queue 초기화
      instructionQueue.reset();
      responseQueue.reset();    

      //bluetooth Scanning Mode 설정 queue에 저장.
      instructionQueue.pushQueue("AT");
      responseQueue.pushQueue("OK");
      instructionQueue.pushQueue("AT+IMME1");
      responseQueue.pushQueue("OK+Set:1");
      instructionQueue.pushQueue("AT+ROLE1");
      responseQueue.pushQueue("OK+Set:1");
      
      atFlag = false;
      responseFlag = false;
        
     }else{
      if( responseFlag == true){
        responseFlag = false;
        atFlag = false;
        if(extractDataAtStr == true){
          extractDataAtStr = false;

          extractData();

          communicateToDrone();

          communicateToDisplay();
          
          cowReadNum = 0;
          cow1Read = false;
          cow2Read = false;
          cow3Read = false;

          //분석하고 통신을 위한 print를 해준다.
          
          
        }

        if(instructionQueue.isEmpty()){
          instructionQueue.pushQueue("AT+DISI?");
          responseQueue.pushQueue("OK+DISCE");
        }
        
      }
          

      
      
     }



  
//      Serial.println("Judgment Section Completion");
    case 2: 
      break;
      
    case 3:  //at 명령어 (가능할 시) 수행
      if(atFlag == false) // atFlag =false -> at명령어 안쓴 상태(이미 썻는데 또 쓰기 방지)
        if(instructionQueue.isEmpty() == false){ // 수행할 명령어가 queue에 있을때만 써준다.
          char* instruction = instructionQueue.popQueue();
          Serial1.write(instruction);
//            Serial.print(instruction);
//            Serial.println(" commanded to bluetooth");
          response = responseQueue.popQueue();
//            Serial.print(response);
//            Serial.println(" 명령어 대기");
          atFlag = true; //atFlag를 통해 명령어 써줬음을 알린다.
          Serial.print(instruction);
          Serial.println("명령어 지시");
        }
      cycle++; // case 맨 마지막 cycle 증가 시키기(응답 x 경우 check를 위함)

      break;
  }
}

checkFairing();
  
  
}


void checkFairing(){
  if(Serial.available()){ // 이는 시리얼 통신에서 데이터를 받아오는 상황으로
                          //파이썬 기반 디스플레이에서 수위 충전 버튼을 눌러 데이터를 전해줄 때 유일하게 발생된다.
    char data = Serial.read();
    Serial.println("버퍼 비우기 시작");
    
    deleteBuffer();
    Serial.println("버퍼 비우기 완료");
    
    instructionQueue.reset();
    responseQueue.reset();
    instructionQueue.pushQueue("AT+CON1862E43DE485"); // TOWER5 과 CONNECTION -> 정상 응답어 :OK+CONNAOK+CONN
    instructionQueue.pushQueue("AT"); // 페어링을 끊을때 사용 ->정상 응답어 : OK+LOST
    
    
      
      
     
    char* instruction = instructionQueue.popQueue();
    Serial1.write(instruction); //페어링 연결의 경우,  TOWER5는 계속해서 명령어를 반복중인 상태에서 페어링 연결을 써주면,
                                //응답 반환어가 데이터가 깨져서 정상응답어와 거의 일치하지 않는다. -> 명령어 길이로서 이를 판단한다.
                                //하지만, OK+LOST 의 경우에는 깨지지 않고 데이터를 반환하므로, 이 명령어를 체크할 수 있도록 한다.
    cnt = 0;
    
    while(1){
     if (Serial1.available()) {
      str[cnt] = Serial1.read();
      cnt++;
     }

     if( cnt >= 15){ // 'OK+CONNAOK+CONN 로 응답하는 경우 -> 페어링 된 상태( 이 상태는 깨질 확률 높아 크기로)
      cnt = 0;
      Serial1.write(data); // 수위센서 동작을 위한 data 를 써준다. data = 1 -> 50% 동작하라, 2->70%, 3->100%
      delay(50);
      //확인을 위해 작성
      Serial.print("블루투스 페어링을 통해 전달한 값 : ");
      Serial.println(data);
      
      char* instruction = instructionQueue.popQueue();
      Serial1.write(instruction);
     }

     if(str[cnt-1] =='T' && str[cnt-2] =='S' && str[cnt-3] =='O' && str[cnt-4] =='L' && str[cnt-5] =='+' && str[cnt-6] =='K' && str[cnt-7] =='O' ){
      initialFlag = true;
      cnt = 0;
      break;
     }
    }    
   } 
}


void deleteBuffer(){
  if(atFlag == true){
      instructionQueue.reset();
      responseQueue.reset();
      unsigned long pre = millis();
      while(1){
        unsigned long cur = millis();
        if (Serial1.available()) {
          str[cnt] = Serial1.read();
          cnt++;
        } // 버퍼를 비운다.
        if(cur-pre > 5000)
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
  count = (count+1)%4; 
  period = true;
}

void extractData(){
  
  //rssi값 char ->String ->int 형 변환을 위한 변수
  //trans1 : 온도정보를 변환
  //trans2 : 위치정보를 변환
  String trans1 = "      ";
  String trans2 = "   ";
  
  for( int i=0;i<numForAnalysis;i++){
   if(str[i] == '7' && str[i-1] == '6' && str[i-2] == 'C' && str[i-3] == '6' && str[i-4] =='B'){ // cow1 로부터 위치,온도정보 scan
      trans1.setCharAt(0,str[i-55]);
      trans1.setCharAt(1,str[i-54]);
      trans1.setCharAt(2,'.');
      trans1.setCharAt(3,str[i-53]);
      trans1.setCharAt(4,str[i-52]);
      trans2.setCharAt(0,str[i-48]);
      
      cow1Read = true;
      cowReadNum++;
      cow[FIRST].temperature = trans1.toFloat();
      cow[FIRST].tower = trans2.toInt();
    }
    else if(str[i] == '7' && str[i-1] == '5' && str[i-2] == 'C' && str[i-3] == '6' && str[i-4] =='B'){// cow2 로부터 위치,온도정보 scan
      trans1.setCharAt(0,str[i-55]);
      trans1.setCharAt(1,str[i-54]);
      trans1.setCharAt(2,'.');
      trans1.setCharAt(3,str[i-53]);
      trans1.setCharAt(4,str[i-52]);
      trans2.setCharAt(0,str[i-48]);
      
      cow2Read = true;
      cowReadNum++;      
      cow[SECOND].temperature = trans1.toFloat();
      cow[SECOND].tower = trans2.toInt();

    }
    else if(str[i] == '5' && str[i-1] == 'F' && str[i-2] == 'C' && str[i-3] == 'A' && str[i-4] =='2'){ // cow3 로부터 위치,온도정보 scan
      trans1.setCharAt(0,str[i-55]);
      trans1.setCharAt(1,str[i-54]);
      trans1.setCharAt(2,'.');
      trans1.setCharAt(3,str[i-53]);
      trans1.setCharAt(4,str[i-52]);
      trans2.setCharAt(0,str[i-48]);

      cow3Read = true;
      cowReadNum++;
      cow[THIRD].temperature = trans1.toFloat();
      cow[THIRD].tower = trans2.toInt();

    }else if(str[i] == '5' && str[i-1] == 'D' && str[i-2] == 'D' && str[i-3] == '1' && str[i-4] =='C'){ //tower1 로 부터 울타리 충격 정보 scan
      trans1.setCharAt(0,str[i-55]);
      trans1.setCharAt(1,str[i-54]);
      trans1.setCharAt(2,str[i-53]);
      trans1.setCharAt(3,str[i-52]);
      trans1.setCharAt(4,str[i-51]);
  
      fence[0] = trans1.substring(0,1).toInt();
      fence[1] = trans1.substring(1,2).toInt();
      fence[2] = trans1.substring(2,3).toInt();
      fence[3] = trans1.substring(3,4).toInt();
      fence[4] = trans1.substring(4,5).toInt();

    }else if(str[i] == 'D' && str[i-1] == '1' && str[i-2] == '0' && str[i-3] == '1' && str[i-4] =='D'){ //tower2 로 부터 울타리 충격 정보 scan
      trans1.setCharAt(0,str[i-55]);
      trans1.setCharAt(1,str[i-54]);
      trans1.setCharAt(2,str[i-53]);
      trans1.setCharAt(3,str[i-52]);
      trans1.setCharAt(4,str[i-51]);
  
      fence[5] = trans1.substring(0,1).toInt();
      fence[6] = trans1.substring(1,2).toInt();
      fence[7] = trans1.substring(2,3).toInt();
      fence[8] = trans1.substring(3,4).toInt();
      fence[9] = trans1.substring(4,5).toInt();

    }else if(str[i] == '4' && str[i-1] == 'C' && str[i-2] == '3' && str[i-3] == '1' && str[i-4] =='D'){ //tower3 로 부터 울타리 충격 정보 scan
      trans1.setCharAt(0,str[i-55]);
      trans1.setCharAt(1,str[i-54]);
      trans1.setCharAt(2,str[i-53]);
      trans1.setCharAt(3,str[i-52]);
      trans1.setCharAt(4,str[i-51]);
      
  
      fence[10] = trans1.substring(0,1).toInt();
      fence[11] = trans1.substring(1,2).toInt();
      fence[12] = trans1.substring(2,3).toInt();
      fence[13] = trans1.substring(3,4).toInt();
      fence[14] = trans1.substring(4,5).toInt();

    }else if(str[i] == '4' && str[i-1] == 'C' && str[i-2] == 'B' && str[i-3] == 'F' && str[i-4] =='B'){ //tower4 로 부터 울타리 충격 정보 scan
      trans1.setCharAt(0,str[i-55]);
      trans1.setCharAt(1,str[i-54]);
      trans1.setCharAt(2,str[i-53]);
      trans1.setCharAt(3,str[i-52]);
      trans1.setCharAt(4,str[i-51]);
  
      fence[15] = trans1.substring(0,1).toInt();
      fence[16] = trans1.substring(1,2).toInt();
      fence[17] = trans1.substring(2,3).toInt();
      fence[18] = trans1.substring(3,4).toInt();
      fence[19] = trans1.substring(4,5).toInt();

    }else if(str[i] == '5' && str[i-1] == '8' && str[i-2] == '4' && str[i-3] == 'E' && str[i-4] =='D'){// tower5 로 부터 수위정보 scan
      waterRead = true;
      trans1.setCharAt(0,str[i-55]);
      trans1.setCharAt(1,str[i-54]);
      trans1.setCharAt(2,str[i-53]);

      waterPercent = trans1.substring(0,3);
      
    }
  }

}

void communicateToDrone(){
                                 
  printCowSwarm();
  
  
}

void communicateToDisplay(){
  
  printCowData();
  
  printWater();
  
  printFence();
    
  Serial.println();

}


void printFence(){//tower1,2,3,4 로 인식한 울타리 정보를 출력할 것.
  for(int i = 0 ; i<20 ; i++){
    Serial.print(fence[i]);
    fence[i] = 0;
  }


}

void printWater(){
  if(waterRead ==true ){
    Serial.print(waterPercent);
  }else{
    Serial.print("-01");
  }
}

void printCowData(){
  if(cow1Read == true){
//    Serial.print("close tower of Cow1 : ");
    Serial.print(cow[FIRST].tower);
//    Serial.print("Temperature of Cow1 : ");
    Serial.print(cow[FIRST].temperature);

  }else if(cow1Read == false){
    Serial.print("000.00");
  }

  
  if(cow2Read == true){
//    Serial.print("close tower of Cow2 : ");
    Serial.print(cow[SECOND].tower);
//    Serial.print("Temperature of Cow2 : ");
    Serial.print(cow[SECOND].temperature);
  }else if(cow2Read == false){
    Serial.print("000.00");
  }
  
  
  if(cow3Read == true){
//    Serial.print("close tower of Cow3 : ");
    Serial.print(cow[THIRD].tower);
//    Serial.print("Temperature of Cow3 : ");
    Serial.print(cow[THIRD].temperature);
  }else if(cow3Read == false){
    Serial.print("000.00");
  }

}


void printCowSwarm(){
  if(cowReadNum >= 2){ // 전체 소는 3마리이기 때문에, 2마리 이상 scanning 됐을때, 군집을 형성을 스캔할 가능성이 있다.

    if(cow1Read == true){
      tower[cow[FIRST].tower - 1]++;
    }
    if(cow2Read == true){
      tower[cow[SECOND].tower - 1]++;
    }
    if(cow3Read == true){
      tower[cow[THIRD].tower - 1]++;
    }
    //tower 배열에는 index 에 대응하는 소의 마릿수가 저장된다.
    //예를 들어, 1번tower에 소2마리, 5번 tower에 소 1마리  ->tower = {2,0,0,0,1} 이 저장됨.


    //FindTower[0] -> 군집을 형성하는 tower위치 값을 저장,
    //FindTower[1] -> tower 근처에 위치해있는 소의 마리 수
    int FindTower[2] = {-1,0};             
                                        
    for(int i=0 ; i<TOWER ; i++){
      if(tower[i] > FindTower[1]){
        FindTower[0] = i;
        FindTower[1] = tower[i];
      }
    }
    FindTower[0]++; // index는 0부터 시작하기 때문에, +1을 해줌으로써, tower의 위치가 저장된다(tower의 경우 1부터 시작하기 때문)
    
    if(FindTower[1] >= 2){ //tower에 근처에 위치한 소가 2마리 이상일때 군집으로 취급한다.(데모버전의 경우, 전체 소의 수가 3마리.)
//      Serial.print("드론이 가야할 위치 : ");
      Serial.print(FindTower[0]);
    }else
//      Serial.println("인식된 소가 군집을 형성하고 있지 않습니다.");
      Serial.print("0");


      
  }else     //cowReadNum > 2 가 아닌경우는 군집 형성여부 알 수 없음.
//    Serial.println("2마리 이상 소가 scan 되지 않았습니다.");
    Serial.print("0");
  
    


  tower[FIRST] = 0; 
  tower[SECOND] = 0; 
  tower[THIRD] = 0;
  tower[FORTH] = 0; 
  tower[FIVE] = 0; 
}
