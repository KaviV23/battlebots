#include <SoftwareSerial.h>
#include <stdbool.h>
#include <string.h>

#define ENA 5
#define IN1 6
#define IN2 7
#define ENB 9
#define IN3 10
#define IN4 11

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

// initialize variables
String command;
String latestMove;
int speed = 100;
static int maxSpeed = 255;

void loop() {
  if (bluetooth.available()) {
    command = bluetooth.readStringUntil(']');

    if (command.charAt(0) == '{') {
      command.remove(0,1);
      String speedStr = "";
      while (command.charAt(0) != '}') {
        speedStr += command.charAt(0);
        command.remove(0,1);
      }
      speed = speedStr.toInt();
    } 
    latestMove = command;

    if (latestMove == String('F')) {
      moveForward(speed);
      bluetooth.println("FORWARD");
    } else if (latestMove == String('B')) {
      moveBackward(speed);
      bluetooth.println("BACKWARD");
    } else if (latestMove == String('L')) {
      rotateLeft(maxSpeed);
      bluetooth.println("LEFT");
    } else if (latestMove == String('R')) {
      rotateRight(maxSpeed);
      bluetooth.println("RIGHT");
    } else if (latestMove == String('S')) {
      stopMotors();
      bluetooth.println("STOP");
    }
  
  }
}