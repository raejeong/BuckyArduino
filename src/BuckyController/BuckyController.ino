#include "Arduino.h"
#include "digitalWriteFast.h"

#define ENCODER1_INTERRUPT 0
#define ENCODER1_PIN_A 2
#define ENCODER1_PIN_B 4
#define ENCODER1_REVERSED

#define ENCODER2_INTERRUPT 1
#define ENCODER2_PIN_A 3
#define ENCODER2_PIN_B 7
#define ENCODER2_REVERSED

#define ENCODER3_INTERRUPT 2
#define ENCODER3_PIN_A 21
#define ENCODER3_PIN_B 8
#define ENCODER3_REVERSED

#define PWM1_PIN 5
#define DIR1_PIN 6

#define PWM2_PIN 10
#define DIR2_PIN 8

#define PWM3_PIN 11
#define DIR3_PIN 12

volatile bool encoder1Set;
volatile long encoder1Ticks = 0;
volatile long encoder1TicksOld = 0;
float encoder1Speed = 0;

volatile bool encoder2Set;
volatile long encoder2Ticks = 0;
volatile long encoder2TicksOld = 0;
float encoder2Speed = 0;

volatile bool encoder3Set;
volatile long encoder3Ticks = 0;
volatile long encoder3TicksOld = 0;
float encoder3Speed = 0;

unsigned long tPeriod = 0;
unsigned long tOld = 0;
unsigned long tCurrent = 0;

unsigned int dirc[3] = {0,0,0};
unsigned int pwm[3] = {0,0,0};
unsigned int dumpVar = 0;


void HandleEncoder1InterruptA()
{
  encoder1Set = digitalReadFast(ENCODER1_PIN_B);
  #ifdef ENCODER1_REVERSED
    encoder1Ticks -= encoder1Set ? -1 : +1;
  #else
    encoder1Ticks += encoder1Set ? -1 : +1;
  #endif
}

void HandleEncoder2InterruptA()
{
  encoder2Set = digitalReadFast(ENCODER2_PIN_B);
  #ifdef ENCODER2_REVERSED
    encoder2Ticks -= encoder2Set ? -1 : +1;
  #else
    encoder2Ticks += encoder2Set ? -1 : +1;
  #endif
}

void HandleEncoder3InterruptA()
{
  encoder3Set = digitalReadFast(ENCODER3_PIN_B);
  #ifdef ENCODER3_REVERSED
    encoder3Ticks -= encoder3Set ? -1 : +1;
  #else
    encoder3Ticks += encoder3Set ? -1 : +1;
  #endif
}

void setup() 
{
  	pinMode(ENCODER1_PIN_A, INPUT);
  	digitalWrite(ENCODER1_PIN_A, LOW);
  	pinMode(ENCODER1_PIN_B, INPUT);
  	digitalWrite(ENCODER1_PIN_B, LOW);
  	attachInterrupt(ENCODER1_INTERRUPT, HandleEncoder1InterruptA, RISING);
    pinMode(PWM1_PIN, OUTPUT);
    pinMode(DIR1_PIN, OUTPUT);

    pinMode(ENCODER2_PIN_A, INPUT);
    digitalWrite(ENCODER2_PIN_A, LOW);
    pinMode(ENCODER2_PIN_B, INPUT);
    digitalWrite(ENCODER2_PIN_B, LOW);
    attachInterrupt(ENCODER2_INTERRUPT, HandleEncoder2InterruptA, RISING);
    pinMode(PWM2_PIN, OUTPUT);
    pinMode(DIR2_PIN, OUTPUT);

    pinMode(ENCODER3_PIN_A, INPUT);
    digitalWrite(ENCODER3_PIN_A, LOW);
    pinMode(ENCODER3_PIN_B, INPUT);
    digitalWrite(ENCODER3_PIN_B, LOW);
    attachInterrupt(ENCODER3_INTERRUPT, HandleEncoder3InterruptA, RISING);
    pinMode(PWM3_PIN, OUTPUT);
    pinMode(DIR3_PIN, OUTPUT);

   	Serial.begin(115200);

    tOld = millis();
}

void loop() {
    tCurrent = millis();
    tPeriod = tCurrent - tOld;
    tOld = tCurrent;

    encoder1Speed = (encoder1Ticks - encoder1TicksOld)*1000*2*3.14/(tPeriod*7.0);
    encoder1TicksOld = encoder1Ticks;

    encoder2Speed = (encoder2Ticks - encoder2TicksOld)*1000*2*3.14/(tPeriod*7.0);
    encoder2TicksOld = encoder2Ticks;

    encoder3Speed = (encoder3Ticks - encoder3TicksOld)*1000*2*3.14/(tPeriod*7.0);
    encoder3TicksOld = encoder3Ticks;

    while(Serial.available() < 6);
    for(int i = 0; i < 3; i++){
      dirc[i] = Serial.read();
    }

    for(int i = 0; i < 3; i++){
      pwm[i] = Serial.read();
    }

    Serial.print(encoder1Speed);
    Serial.print(", ");
    Serial.print(encoder2Speed);
    Serial.print(", ");
    Serial.println(encoder3Speed);

    digitalWrite(DIR1_PIN, dirc[0]);
    analogWrite(PWM1_PIN, pwm[0]);

    digitalWrite(DIR2_PIN, dirc[1]);
    analogWrite(PWM2_PIN, pwm[1]);

    digitalWrite(DIR3_PIN, dirc[2]);
    analogWrite(PWM3_PIN, pwm[2]);

    delay(2);
}
