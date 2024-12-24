#include <NewPing.h>

#define TRIG_PIN_1 12  // Trigger pin for the first ultrasonic sensor
#define ECHO_PIN_1 13   // Echo pin for the first ultrasonic sensor
#define TRIG_PIN_2 14   // Trigger pin for the second ultrasonic sensor
#define ECHO_PIN_2 27   // Echo pin for the second ultrasonic sensor

#define IN1 5           // Motor driver pin for left motor (movement motor)
#define IN2 18          // Motor driver pin for left motor (movement motor)
#define IN3 19          // Motor driver pin for right motor (movement motor)
#define IN4 21          // Motor driver pin for right motor (movement motor)

#define ARM_IN1 2      // Motor driver pin for the arm motor (arm control)
#define ARM_IN2 4      // Motor driver pin for the arm motor (arm control)

const unsigned long COMM_TIMEOUT = 3000; // Communication timeout in milliseconds
unsigned long lastCommTime = 0;

// Set up ultrasonic sensor parameters
NewPing sonar1(TRIG_PIN_1, ECHO_PIN_1, 200); // First ultrasonic sensor
NewPing sonar2(TRIG_PIN_2, ECHO_PIN_2, 200); // Second ultrasonic sensor

void setup() {
  Serial.begin(9600); // Start serial communication
  
  // Set motor pins as outputs (for movement)
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  // Set motor pins as outputs for the arm motor
  pinMode(ARM_IN1, OUTPUT);
  pinMode(ARM_IN2, OUTPUT);
}

void loop() {
  // Check for communication timeout
  if (millis() - lastCommTime > COMM_TIMEOUT) {
    stopMovement();  // Stop all movement if communication timeout occurs
  }

  // Read distances from ultrasonic sensors
  int distance1 = sonar1.ping_cm(); // Distance from the first ultrasonic sensor
  int distance2 = sonar2.ping_cm(); // Distance from the second ultrasonic sensor

  // Check if the first ultrasonic sensor detects an object (e.g., within 10 cm)
  if (distance1 > 0 && distance1 < 10) {
    stopMovement();  // Stop moving when the first ultrasonic sensor detects the object
    pickUpGarbage(); // Use the motor to pick up the garbage
  }

  // Check if the second ultrasonic sensor detects an object in the box (e.g., within 10 cm)
  if (distance1 > 0 && distance1 < 10) {
    returnToInitialState(); // Call function to restart the operation
  }

  // Process commands from Raspberry Pi if available
  if (Serial.available()) {
    char command = Serial.read();  // Read the command from the Raspberry Pi
    lastCommTime = millis();  // Update last communication time
    
    if (command == 'F') {
      moveForward();
    } else if (command == 'R') {
      rotateAndSearch();
    } else if (command == 'H') {
      // Heartbeat signal received, no action needed
    }
  }
}

// Function to move the garbage collector forward
void moveForward() {
  digitalWrite(IN1, HIGH);  // Left motor forward
  digitalWrite(IN2, LOW);   // Left motor forward
  digitalWrite(IN3, HIGH);  // Right motor forward
  digitalWrite(IN4, LOW);   // Right motor forward
}

// Function to rotate the body and search
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

// Function to control the arm motor and pick up garbage
void pickUpGarbage() {
  // Rotate arm motor to pick up garbage
  digitalWrite(ARM_IN1, HIGH);  // Arm motor forward (e.g., arm moving down)
  digitalWrite(ARM_IN2, LOW);
  delay(1000);  // Adjust the delay as needed for your motor to pick up

  // Reverse arm motor to return to initial position
  digitalWrite(ARM_IN1, LOW);
  digitalWrite(ARM_IN2, HIGH);  // Arm motor reverse (e.g., arm moving up)
  delay(1000);

  // Stop the arm motor
  digitalWrite(ARM_IN1, LOW);
  digitalWrite(ARM_IN2, LOW);

  // Once no object is detected, continue moving forward
  moveForward();
}

// Function to return to the initial state after detecting an object in the box
void returnToInitialState() {
  stopMovement();  // Stop all movement
  delay(1000);     // Delay for a moment
  moveForward();   // Move forward to start the operation again
}
