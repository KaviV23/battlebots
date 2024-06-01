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

// initialize variables
String command;
bool decodeAnalog = false;
char decodeMode;
// analog stick values from HC06
String X = "";
String Y = "";

void loop() {
  if (bluetooth.available()) {

    command = bluetooth.readStringUntil(']');
    
    // convert analog stick input
    // begin when string starts at X
    if (command.charAt(0) == 'X') {
      X = "";
      Y = "";
      command.remove(0,1); // remove X
      while (command.charAt(0) != 'Y') { // move x values into variable
        X.concat(command.charAt(0));
        command.remove(0,1); // remove after assigning to variable
      }
      if (command.charAt(0) == 'Y') { // same sequence as above for Y ^^
        command.remove(0,1);
        while (command.length() != 0) {
          Y.concat(command.charAt(0));
          command.remove(0,1);
        }
      Serial.println("X: " + X + " Y: " + Y);
      }
    }

// TEMP TEMP TEMP
String tempe;
    if (Y.toInt() < 225 && Y.toInt() != 177) { // Forward
      if (X.toInt() < 131) { // Left
        tempe = 355 - X.toInt();
        Serial.println("Left " + tempe);
        rotateLeft(355-X.toInt());
      } else if (X.toInt() > 224) { // Right
        tempe = X.toInt();
        Serial.println("Right " + tempe);
        rotateRight(X.toInt());
      } else {
        tempe = 355 - Y.toInt();
        Serial.println("Forward " + tempe);
        moveForward(355-Y.toInt());
      }
    } else if (Y.toInt() > 224) { // Backward
      // if (X.toInt() < 130) { // Left Back
      //   Serial.println("ReverseRight " + tempe);
      //   rotateRight(355-X.toInt());
      // } else if (X.toInt() > 224) { // Right Back
      // tempe = X.toInt();
      //   Serial.println("ReverseLeft " + tempe);
      //   rotateLeft(X.toInt());
      // } else {
      //   tempe = Y.toInt();
      //   Serial.println("Backward " + tempe);
      //   moveBackward(Y.toInt());
      // }
      tempe = Y.toInt() - 125;
      Serial.println("Backward " + tempe);
      moveBackward(Y.toInt() - 125);
    } else if (X.toInt() == 177 && Y.toInt() == 177) {
      Serial.println("Stop");
      stopMotors();
    }
  }
}