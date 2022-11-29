#include <LiquidCrystal.h>

#define sensor 2
#define coldLED 10
#define hotLED 9
#define swup 8
#define swdown 13

int temp = 20; //희망온도 초기화
float rdata(int port); //데이터를 받아와 온도로 변환후 저장
float averageT();  //  여러 센서의 변환된 온도의 평균값
void averagePrint(); // 평균온도 시리얼 모니터 출력
void portPrint(); // 각 센서의 온도 시리얼 모니터 출력
void lcdPrint(int t); //lcd에 평균온도값 출력
void LED(int t); // 제어온도에 따른 led
int lastbuttonstate;
int lastbuttonstate2;

LiquidCrystal lcd(12,11,5,4,3,20); //rs, e ,d4,d5,d6,d7 와 연결되는 포트 활성화





void setup()
{
    pinMode(swup,INPUT_PULLUP); //스위치up
    pinMode(swdown, INPUT_PULLUP); // 스위치 down
   
    pinMode(coldLED, OUTPUT); // led핀 설정
    pinMode(hotLED, OUTPUT); // led핀 설정
    lcd.begin(16,2); //lcd 크기 설정
    Serial.begin(9600);  //시리얼 통신 시작
    lastbuttonstate = digitalRead(swup);
    lastbuttonstate2 = digitalRead(swdown);
}

void loop()
{ 
  
    averagePrint(); // 평균온도 시리얼 모니터 출력
    portPrint(); // 각 센서 온도 시리얼 모니터 출력
 
  
  if(lastbuttonstate == digitalRead(swup)){
  }
  else if (lastbuttonstate != digitalRead(swup)){
    ++temp;
  }

  if(lastbuttonstate2 == digitalRead(swdown)){
  }
  else if (lastbuttonstate2 != digitalRead(swdown)){
    --temp;
  }
  Serial.println(temp);


  
  lastbuttonstate = digitalRead(swup);
  lcdPrint(temp); //lcd에 센서 평균 온도값 출력

  LED(temp); //제어 온도에 따른 LED
  
   delay(300);
  lcd.clear(); //lcd화면 초기화

 
}











float rdata(int port){ //데이터를 받아와 온도로 변환후 저장
  
  float rdata = analogRead(port);
  float Temp = (500*rdata)/1024; //데이터를 온도로 변환하는 공식
  return Temp;

}

float averageT(){  // 여러 센서의 변환된 온도의 평균값
  float average=0;
  float sum=0;
  for(int i=0; i<sensor; i++){
    sum += rdata(i);
  }
  average = sum /sensor;
  return average;
}

void averagePrint(){
    Serial.print("평균온도: ");
    Serial.print(averageT());
    Serial.println(" C");
}

void portPrint(){ //각센서의 온도출력
  for(int i=0 ; i <sensor; i++){
    Serial.print(i+1);
    Serial.print("번 센서온도 ");
    Serial.print(rdata(i));
    Serial.print("C ");
    
  }
  Serial.println();
}

void lcdPrint(int t){
    lcd.display(); //lcd화면 활성화
    lcd.setCursor(0,0);
    lcd.print("Temp:");
    
    //lcd.print(rdata(3));
     
    lcd.print(averageT()); // 온도 평균값 lcd표현
    lcd.print(" C");
    lcd.setCursor(0,1);
    lcd.print("control:");
    lcd.print(t);
    lcd.print(" C");
}

void LED(int t){
   if(averageT() <= t ){
    digitalWrite(hotLED,HIGH);
    digitalWrite(coldLED,LOW);
   }
   else{
    digitalWrite(coldLED,HIGH);
    digitalWrite(hotLED,LOW);
   }
}
