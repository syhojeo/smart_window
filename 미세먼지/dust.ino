/* Grove - Dust Sensor Demo v1.0
Code: Dust_2
 */
#include<SoftwareSerial.h>

SoftwareSerial btSerial(2,3); // Rx(Txd), Tx(Rxd) 핀설정

int motorain1=7;
int motorain2=4;
int en=10;
int led = 12;
int pin = 8;
int sta = 1;
unsigned long duration;
unsigned long starttime;
unsigned long sampletime_ms = 5000;//sampe 30s ;
unsigned long lowpulseoccupancy = 0;
float ratio = 0;
float concentration = 0;
float ugm3 =0;
int x;


void setup() {
  Serial.begin(9600);
  btSerial.begin(9600); 
  pinMode(pin, INPUT);
  pinMode(13,OUTPUT);
  pinMode(led, OUTPUT);
  pinMode(motorain1, OUTPUT);
  pinMode(motorain2,OUTPUT);
  pinMode(en, OUTPUT);
  starttime = millis();//get the current time;
  
}

void loop() {
  duration = pulseIn(pin, LOW);
  lowpulseoccupancy = lowpulseoccupancy + duration;

  if ((millis() - starttime) > sampletime_ms) // 5초마다 센서에서 받아오도록 설정
  {
    ratio = lowpulseoccupancy / (sampletime_ms * 10.0); 
    concentration = 1.1 * pow(ratio, 3) - 3.8 * pow(ratio, 2) + 520 * ratio + 0.62; //들어오는 센서값 세제곱 미터당 마이크로그램 값으로 가공
    ugm3 = concentration *100 /13000;

    lowpulseoccupancy = 0;   //루프가 끝나기전에 초기값은 초기화
    starttime = millis();
  }
 
  x = (int)(ugm3);   //미세먼지 농도 정수로 형변환

  Serial.println(x);
  btSerial.println(x);   // 미세먼지 농도 블루투스 송신

  if(btSerial.available()>0){ //수신 가능한 상태인지 확인
      
  if(btSerial.read()=='1' && sta == 1){   // 창문을 열어야 될 경우
    analogWrite(en,150);   // PWM을 이용한 모터 제어
    digitalWrite(motorain1,HIGH);  //창문 열기
    digitalWrite(motorain2,LOW);
    delay(1700);
    digitalWrite(motorain1,LOW);
  
    sta = 0;
   }

 
 else if(btSerial.read()=='0' && sta == 0){  //창문을 닫아야 될경우 
    analogWrite(en,150); // PWM을 이용한 모터 제어
    digitalWrite(motorain1,LOW);  // 창문 닫기
    digitalWrite(motorain2,HIGH); 
    delay(1700);
    digitalWrite(motorain2,LOW);
    
    sta = 1;
   }
  
  }
  
}
