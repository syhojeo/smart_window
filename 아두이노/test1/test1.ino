int LED =2;
int SWITCH=4;


void setup() {
     pinMode(LED , OUTPUT);
     pinMode(SWITCH , INPUT);
}

void loop() {
    
    if(digitalRead(SWITCH) == 1){
    digitalWrite(LED, HIGH);
    }
    else if(digitalRead(SWITCH) == 0){
    digitalWrite(LED,LOW);
    }
}
