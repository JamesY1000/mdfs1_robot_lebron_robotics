#include <Arduino.h>
#include <L298NX2.h>

// Pin setup
const int ENA = 5;   // PWM pin for speed control
const int IN1 = 6;   // Direction pin 1
const int IN2 = 7;   // Direction pin 2

int encoderPin1 = 3; // A phase
int encoderPin2 = 4; // B phase
volatile int lastEncoded = 0;
volatile long encoderValue = 0;
volatile long correctEncoderValue = 0;
long lastencoderValue = 0;
  
int lastMSB = 0;
int lastLSB = 0;

void updateEncoder();

void setup() {

  // Set up serial port display
  Serial.begin (9600);

  // Set motor control pins as outputs
  pinMode(ENA, OUTPUT); 
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);

  // Set up encoder pins as inputs
  pinMode(encoderPin1, INPUT);
  pinMode(encoderPin2, INPUT);
  digitalWrite(encoderPin1, HIGH); //turn pullup resistor on
  digitalWrite(encoderPin2, HIGH); //turn pullup resistor on
  attachInterrupt(0, updateEncoder, CHANGE);
  attachInterrupt(1, updateEncoder, CHANGE);


  // Start of program
  
    // Write encoder values to terminal
    correctEncoderValue = encoderValue/4;
    Serial.println("Encoder value: " + String(correctEncoderValue));
    delay(1000);
  
    // Move forward
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);
    analogWrite(ENA, 200); // Speed: 0–255
    delay(2000);           // Run for 2 seconds
  
    // Stop
    analogWrite(ENA, 0);
    delay(1000);
  
    // Write encoder values to terminal
    correctEncoderValue = encoderValue/4;
    Serial.println("Encoder value: " + String(correctEncoderValue));
    delay(1000);
  
    // Move backward
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, HIGH);
    analogWrite(ENA, 200);
    delay(2000);
  
    // Stop again
    analogWrite(ENA, 0);
    delay(1000);
  
    // Write encoder values to terminal
    correctEncoderValue = encoderValue/4;
    Serial.println("Encoder value: " + String(correctEncoderValue));
    delay(1000);


}

void loop() {



}

void updateEncoder(){
  int MSB = digitalRead(encoderPin1); //MSB = most significant bit
  int LSB = digitalRead(encoderPin2); //LSB = least significant bit
  
  int encoded = (MSB << 1) |LSB; //converting the 2 pin value to single number
  int sum  = (lastEncoded << 2) | encoded; //adding it to the previous encoded value
  
  if(sum == 0b1101 || sum == 0b0100 || sum == 0b0010 || sum == 0b1011) encoderValue ++;
  if(sum == 0b1110 || sum == 0b0111 || sum == 0b0001 || sum == 0b1000) encoderValue --;
  
  lastEncoded = encoded; //store this value for next time
}