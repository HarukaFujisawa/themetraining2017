int vib_pin=7;
int led_pin=13;
void setup() {
    Serial.begin(9600);

  pinMode(vib_pin,INPUT);
  pinMode(led_pin,OUTPUT);
}

void loop() {
  int val;
  val=digitalRead(vib_pin);

  Serial.println(val);
  
  if(val==1)
  {
    digitalWrite(led_pin,HIGH);
 //   delay(1000);
 //   digitalWrite(led_pin,LOW);
   // delay(1000);
   }
   else
   digitalWrite(led_pin,LOW);
   delay(10);
}
