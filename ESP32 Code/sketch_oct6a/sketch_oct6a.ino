#include <ESP32Servo.h>   // ESP32Servo library for controlling servos
#include <NewPing.h>

#define TRIG_PIN_1 12      // Trigger pin for the first ultrasonic sensor
#define ECHO_PIN_1 13      // Echo pin for the first ultrasonic sensor
#define TRIG_PIN_2 14      // Trigger pin for the second ultrasonic sensor
#define ECHO_PIN_2 27      // Echo pin for the second ultrasonic sensor

#define IN1 5              // Motor driver pin for left motor (movement motor)
#define IN2 18             // Motor driver pin for left motor (movement motor)
#define IN3 19             // Motor driver pin for right motor (movement motor)
#define IN4 21             // Motor driver pin for right motor (movement motor)

#define GEAR_IN1 2         // Gear motor pin for lifting the arm
#define GEAR_IN2 4         // Gear motor pin for lifting the arm

#define SERVO_RIGHT_PIN 22  // Right servo pin
#define SERVO_LEFT_PIN 23   // Left servo pin

Servo rightServo;
Servo leftServo;

NewPing sonar1(TRIG_PIN_1, ECHO_PIN_1, 200);  // First ultrasonic sensor
NewPing sonar2(TRIG_PIN_2, ECHO_PIN_2, 200);  // Second ultrasonic sensor

unsigned long lastCommTime = 0;
const unsigned long COMM_TIMEOUT = 3000; // Communication timeout in milliseconds

void setup() {
  Serial.begin(9600);

  // Motor setup
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  
  // Gear motor setup
  pinMode(GEAR_IN1, OUTPUT);
  pinMode(GEAR_IN2, OUTPUT);

  // Servo setup
  rightServo.attach(SERVO_RIGHT_PIN);
  leftServo.attach(SERVO_LEFT_PIN);

  // Set both servos to middle position (90 degrees)
  rightServo.write(90);
  leftServo.write(90);

  delay(1000);  // Small delay to stabilize servos at starting position

  // Move the right servo to 120 degrees and left servo to 60 degrees
  for (int pos = 90; pos <= 150; pos++) {
    rightServo.write(pos);
    leftServo.write(180 - pos);  // Left servo mirror movement (180 - pos)
    delay(15);  // Adjust this value to control the speed of servo movement
  }

  // Start the ultrasonic sensors and wheel motors after the servos finish
  moveForward();
}

void loop() {
  // Check for communication timeout
  if (millis() - lastCommTime > COMM_TIMEOUT) {
    stopMovement();  // Stop all movement if communication timeout occurs
  }

  // Read distances from ultrasonic sensors
  int distance1 = sonar1.ping_cm(); // Distance from the first ultrasonic sensor
  int distance2 = sonar2.ping_cm(); // Distance from the second ultrasonic sensor

  // Check if the first ultrasonic sensor detects an object (e.g., within 25 cm)
  if (distance1 > 0 && distance1 < 15) {
    stopMovement();  // Stop moving when the first ultrasonic sensor detects the object
    pickUpGarbage(); // Use the servos and gear motor to pick up and drop the garbage
  }

  // Check if the second ultrasonic sensor detects an object in the box (e.g., within 25 cm)
  if (distance2 > 0 && distance2 < 15) {
    returnToInitialState();  // Call function to restart the operation
  }

  // Process commands from the terminal (optional feature)
  if (Serial.available()) {
    char command = Serial.read();  // Read the command
    lastCommTime = millis();  // Update last communication time
    
    if (command == 'F') {
      moveForward();
    } else if (command == 'R') {
      rotateAndSearch();
    } else{
      // Heartbeat signal received, no action needed
    }
  }
}

// Function to move the garbage collector forward
void moveForward() {
  digitalWrite(IN1, HIGH);  // Left motor forward
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);  // Right motor forward
  digitalWrite(IN4, LOW);
}

// Function to rotate and search for objects
void rotateAndSearch() {
  digitalWrite(IN1, HIGH);  // Rotate left motor
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);   // Rotate right motor in reverse
  digitalWrite(IN4, HIGH);
}

// Function to stop all movement
void stopMovement() {
  digitalWrite(IN1, LOW);  // Stop left motor
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);  // Stop right motor
  digitalWrite(IN4, LOW);
}

// Function to pick up garbage using servos and gear motor
void pickUpGarbage() {
  // Step 1: Close the servos to pick up the object (reverse previous movement)
  for (int pos = 150; pos >= 90; pos--) {
    rightServo.write(pos);
    leftServo.write(180 - pos);
    delay(15);
  }

  // Step 2: Use the gear motor to lift the arm for 2 seconds
  digitalWrite(GEAR_IN1, HIGH);
  digitalWrite(GEAR_IN2, LOW);
  delay(1500);  // Lift the arm for 2 seconds

  // Step 3: Return the arm to its original position
  digitalWrite(GEAR_IN1, LOW);
  digitalWrite(GEAR_IN2, HIGH);
  delay(1500);  // Lower the arm back`

  // Stop the gear motor
  digitalWrite(GEAR_IN1, LOW);
  digitalWrite(GEAR_IN2, LOW);

  // Step 4: Re-open the servos to release the object
  for (int pos = 90; pos <= 150; pos++) {
    rightServo.write(pos);
    leftServo.write(180 - pos);
    delay(15);
  }

  returnToInitialState();  // Call function to restart the operation
}

// Function to return to initial state after detecting object in the box
void returnToInitialState() {
  stopMovement();
  delay(1000);
  moveForward();
}
