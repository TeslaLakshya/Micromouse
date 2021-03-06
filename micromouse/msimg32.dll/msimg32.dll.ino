#define DIR_PIN1          2
#define STEP_PIN1         3
//#define ENABLE_PIN       4

#define STEP_HIGH1        PORTD |=  0b00001000;
#define STEP_LOW1         PORTD &= ~0b00001000;

#define TIMER1_INTERRUPTS_ON    TIMSK1 |=  (1 << OCIE1A);
#define TIMER1_INTERRUPTS_OFF   TIMSK1 &= ~(1 << OCIE1A);

unsigned int c0;
String rx = "";
volatile int count1 = 0;
/*int trigPin = 8;
int echoPin = 12;
long duration, distance;*/
int x;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(STEP_PIN1,   OUTPUT);
  pinMode(DIR_PIN1,    OUTPUT);
  //pinMode(ENABLE_PIN, OUTPUT);

  noInterrupts();
  TCCR1A = 0;
  TCCR1B = 0;
  TCNT1  = 0;
  OCR1A = 1000;                             
  TCCR1B |= (1 << WGM12);
  TCCR1B |= ((1 << CS11) | (1 << CS10));
  interrupts();
  TIMER1_INTERRUPTS_ON

  c0 = 1600; // was 2000 * sqrt( 2 * angle / accel )
}
volatile unsigned long n1 = 0;
volatile unsigned int speedToReach1 = 100, currentSpeed1 = 1600;
volatile float d1 = 1600;
ISR(TIMER1_COMPA_vect)
{
   //Serial.println(flag);
     if(currentSpeed1 != speedToReach1){
      count1++;
      //Serial.println(count1);
     }
      //Serial.println(count1);
      STEP_HIGH1
      STEP_LOW1
      //Serial.println(micros());
      /*if(speedToReach == currentSpeed){
          n = 0;
      }*/
      //Ramp up phase   
      if(speedToReach1 < currentSpeed1){
          n1++;
          count1 = 0;
          d1 = d1 - (2 * d1) / (4 * n1 + 1);
          //d1copy = d1;
          //Serial.println(d);
      }
      //Ramp down phase
      //Serial.println(speedToReach + currentSpeed);
      if(speedToReach1 > currentSpeed1){
          n1--;
          count1 = 0;
          //Serial.println(d);
          d1 = (d1 * (4 * n1 + 1)) / (4 * n1 + 1 - 2);
          //d1copy = d1; 
           //Serial.println(d); 
      }
      currentSpeed1 = d1;
      //Serial.println(millis());
      //Serial.print(" ");
      //Serial.println(22000000/(currentSpeed * 400));
      //yet to be coded
}

void loop() {
  // put your main code here, to run repeatedly:
  //Serial.println(0);
  if(Serial.available() > 0){
     char r = Serial.read();
     //Serial.println(0);
     if(r == '\n'){
        speedToReach1 = rx.toInt();
        //speedToReach2 = speedToReach1;
        //Serial.print(speedToReach1);
        rx = "";
      }
      else{
         rx = rx + r;
        }
  }
  /*digitalWrite(trigPin, LOW);
  delayMicroseconds(5);
  //send pulse
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  //receive pulse
  duration = pulseIn(echoPin, HIGH);
  //find distance
  distance = (duration/2) * 0.0343;
  x = map(distance, 2, 400, 1600, 1);
  if(!(x < 0))
    speedToReach = x;
  if(x < 4)
    TIMER1_INTERRUPTS_OFF
  else
    TIMER1_INTERRUPTS_ON*/
  //Serial.println(n);
  //speedToReach = 1600;
}
