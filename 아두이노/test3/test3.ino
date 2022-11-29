#define sw1 7
int x = 0;
void setup() {
  Serial.begin(9600);
  pinMode(sw1,INPUT);
  
}

void loop() {
  x=digitalRead(sw1);

  Serial.println(x);
}
