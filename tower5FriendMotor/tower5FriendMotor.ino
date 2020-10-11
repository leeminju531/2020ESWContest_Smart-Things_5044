char data = 0;
#define DIR 12
#define PWM 10

void setup() {
  Serial.begin(9600);
  pinMode(DIR, OUTPUT);
  pinMode(PWM, OUTPUT);
  
  pinMode(5,OUTPUT);
  pinMode(6,OUTPUT);
  pinMode(7,OUTPUT);
}

void loop() {
  if(Serial.available()){
    data = (char)Serial.read();
    Serial.print(data);
    //char to int
  }

  if (data == '1')
   waterCompliment50();
  else if (data == '2')
    waterCompliment70();
  else if (data == '3')
   waterCompliment100();

}


void waterCompliment50(){
  mOpen();
  for(int i = 0; i<9 ; i++){
    pump();
  }
  mClose();
  data = NULL;
}

void waterCompliment70(){
  mOpen();
  for(int i = 0; i<12 ; i++){
    pump();
  }
  mClose();
  data = NULL;
}

void waterCompliment100(){

  
  mOpen();
  for(int i = 0; i<18 ; i++){
    pump();
  }
  mClose();
  data = NULL;
}

void mOpen(){
  
  digitalWrite(DIR, HIGH);
  analogWrite(PWM, 250);
  delay(30000);
  analogWrite(PWM, 0);
}

void pump(){
  digitalWrite(DIR, HIGH);
  analogWrite(PWM, 250);
  delay(5000);
  digitalWrite(DIR, LOW);
  analogWrite(PWM, 250);
  delay(5000);
}


void mClose(){
  digitalWrite(DIR, LOW);
  analogWrite(PWM, 250);
  delay(25000);
  analogWrite(PWM, 0);
}
