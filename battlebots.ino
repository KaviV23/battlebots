#include <SoftwareSerial.h>
#include <stdbool.h>
#include <string.h>

#define ENA 5
#define IN1 6
#define IN2 7
#define ENB 9
#define IN3 10
#define IN4 11

int defaultSpeed = 150; // adjust as you see fit

// can also change the forward and backwards speed with different variables

#define BLUETOOTH_TX 2
#define BLUETOOTH_RX 3

SoftwareSerial bluetooth(BLUETOOTH_TX, BLUETOOTH_RX);

void setupMotors() {
  pinMode(ENA, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(ENB, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
}

void moveForward(int speed) {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  analogWrite(ENA, speed);

  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  analogWrite(ENB, speed);  
}

void moveBackward(int speed) {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  analogWrite(ENA, speed);

  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
  analogWrite(ENB, speed);  
}

void rotateLeft(int speed) {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  analogWrite(ENA, speed);

  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  analogWrite(ENB, speed);  
}

void rotateRight(int speed) {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  analogWrite(ENA, speed);

  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
  analogWrite(ENB, speed);  
}

void stopMotors() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
}

void setup() {
  Serial.begin(9600);
  bluetooth.begin(9600);

  setupMotors();
}

bool speedChange = false;
String newSpeed = "";

void loop() {
  if (bluetooth.available()) {
    char command = bluetooth.read();

    if(command == '}') {
      speedChange = false;
      defaultSpeed = newSpeed.toInt();
      newSpeed = "";
    }

    if(speedChange == true) {
      newSpeed.concat(command);
    }
    
    if(command == '{') {
      speedChange = true;
    }
    
    switch (command) {
        case 'F':
          moveForward(defaultSpeed);
          break;
        case 'B':
          moveBackward(defaultSpeed);
          break;
        case 'L':
          rotateLeft(defaultSpeed);
          break;
        case 'R':
          rotateRight(defaultSpeed);
          break;
        case 'S':
          stopMotors();
          break;
          default:
          break;
      }
  }
}
