#define IN1 5  // Motor driver pin for left motor (movement motor)
#define IN2 18 // Motor driver pin for left motor (movement motor)
#define IN3 19 // Motor driver pin for right motor (movement motor)
#define IN4 21 // Motor driver pin for right motor (movement motor)

#define ARM_IN1 32  // Motor driver pin for the arm motor (arm control)
#define ARM_IN2 35  // Motor driver pin for the arm motor (arm control)
#define MOTION_SENSOR_PIN 34  // GPIO pin for motion sensor

const unsigned long COMM_TIMEOUT = 5000; // Communication timeout in milliseconds
unsigned long lastCommTime = 0;

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

  // Set motion sensor pin as input
  pinMode(MOTION_SENSOR_PIN, INPUT);
}

void loop() {
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

  // Check for communication timeout
  if (millis() - lastCommTime > COMM_TIMEOUT) {
    stopMovement();  // Stop all movement if communication timeout occurs
  }

  // Check if the motion sensor detects an object
  if (digitalRead(MOTION_SENSOR_PIN) == HIGH) {
    stopMovement();  // Stop moving when the motion sensor detects the object
    pickUpGarbage();  // Use the motor to pick up the garbage
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
