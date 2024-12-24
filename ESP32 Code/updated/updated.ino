#include <Arduino.h>

// Pin Definitions
const int motorIn1 = 5;
const int motorIn2 = 18;
const int motorIn3 = 19;
const int motorIn4 = 21;
const int armMotorPin = 16;
const int metalArmMotorPin = 25;  // For metal objects
const int frontMotionSensorPin = 33;
const int conveyorMotionSensorPin = 32;
const int metalDetectorPin = 20;

char command;
unsigned long lastCommunication = 0;
const unsigned long communicationTimeout = 3000;  // 3 seconds

void setup() {
    Serial.begin(9600);
    
    pinMode(motorIn1, OUTPUT);
    pinMode(motorIn2, OUTPUT);
    pinMode(motorIn3, OUTPUT);
    pinMode(motorIn4, OUTPUT);
    pinMode(armMotorPin, OUTPUT);
    pinMode(metalArmMotorPin, OUTPUT);
    pinMode(frontMotionSensorPin, INPUT);
    pinMode(conveyorMotionSensorPin, INPUT);
    pinMode(metalDetectorPin, INPUT);
    
    stopAllMotors();  // Ensure all motors are stopped initially
}

void loop() {
    if (Serial.available() > 0) {
        command = Serial.read();
        lastCommunication = millis();  // Update last communication time

        if (command == 'F') {
            moveForward();
        } else if (command == 'R') {
            rotate();
        }
    }

    // Front motion sensor detects object nearby
    if (digitalRead(frontMotionSensorPin) == HIGH) {
        stopAllMotors();
        pickObject();
    }

    // Check for timeout in communication
    if (millis() - lastCommunication > communicationTimeout) {
        stopAllMotors();
    }
}

// Function to pick object
void pickObject() {
    digitalWrite(armMotorPin, HIGH);  // Pick object with standard arm
    delay(2000);  // Simulate arm picking time
    digitalWrite(armMotorPin, LOW);

    // Drop object on conveyor belt
    if (digitalRead(conveyorMotionSensorPin) == LOW) {
        moveBackward(2000);  // Move back and retry detection
    } else {
        // Check if object is metal
        if (digitalRead(metalDetectorPin) == HIGH) {
            digitalWrite(metalArmMotorPin, HIGH);  // Use metal arm for metal objects
            delay(2000);
            digitalWrite(metalArmMotorPin, LOW);
        }
    }
}

// Movement functions
void moveForward() {
    digitalWrite(motorIn1, HIGH);
    digitalWrite(motorIn2, LOW);
    digitalWrite(motorIn3, HIGH);
    digitalWrite(motorIn4, LOW);
}

void rotate() {
    digitalWrite(motorIn1, LOW);
    digitalWrite(motorIn2, HIGH);
    digitalWrite(motorIn3, HIGH);
    digitalWrite(motorIn4, LOW);
}

void moveBackward(int duration) {
    digitalWrite(motorIn1, LOW);
    digitalWrite(motorIn2, HIGH);
    digitalWrite(motorIn3, LOW);
    digitalWrite(motorIn4, HIGH);
    delay(duration);
    stopAllMotors();
}

void stopAllMotors() {
    digitalWrite(motorIn1, LOW);
    digitalWrite(motorIn2, LOW);
    digitalWrite(motorIn3, LOW);
    digitalWrite(motorIn4, LOW);
    digitalWrite(armMotorPin, LOW);
    digitalWrite(metalArmMotorPin, LOW);
}
