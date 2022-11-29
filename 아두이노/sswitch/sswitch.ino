int btnPin = 2;
int count = 0;
void setup() {
    pinMode(btnPin,INPUT);
    attachInterrupt(0,pushBtn,RISING);
    Serial.begin(9600);
}

void loop() {
  int data = digitalRead(btnPin);
  if(data == 1){
    count ++;
  }
}
   
 void pushBtn(){
  Serial.println("on");
  }
  
