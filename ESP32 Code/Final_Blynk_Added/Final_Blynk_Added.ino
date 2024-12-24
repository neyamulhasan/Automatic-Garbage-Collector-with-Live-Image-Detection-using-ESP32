#include <NewPing.h>
#include <WiFi.h>
#include <BlynkSimpleEsp32.h>
#include <DHT.h>

#define TRIG_PIN_1 12  // Trigger pin for the first ultrasonic sensor
#define ECHO_PIN_1 13  // Echo pin for the first ultrasonic sensor
#define TRIG_PIN_2 14  // Trigger pin for the second ultrasonic sensor
#define ECHO_PIN_2 27  // Echo pin for the second ultrasonic sensor

#define IN1 5          // Motor driver pin for left motor (movement motor)
#define IN2 18         // Motor driver pin for left motor (movement motor)
#define IN3 19         // Motor driver pin for right motor (movement motor)
#define IN4 21         // Motor driver pin for right motor (movement motor)

#define ARM_IN1 2      // Motor driver pin for the arm motor (arm control)
#define ARM_IN2 4      // Motor driver pin for the arm motor (arm control)
#define ARM_IN3 22
#define ARM_IN4 23

#define SMOKE_PIN 34   // Smoke sensor pin
#define DHTPIN 32      // Pin where DHT11 is connected
#define DHTTYPE DHT11  // Define DHT type

// Blynk and WiFi credentials
char auth[] = "tjrK_lvec_3p8UbjadunctxhL8qfPUNl";  // From Blynk app email
char ssid[] = "Salim_Hall_5G";       // Your WiFi SSID
char pass[] = "132069@MNU";   // Your WiFi Password

// Define Blynk virtual pins
#define VPIN_TEMP V1   // Virtual Pin for Temperature
#define VPIN_HUMID V2  // Virtual Pin for Humidity
#define VPIN_SMOKE V3  // Virtual Pin for Smoke

// Set up ultrasonic sensor parameters
NewPing sonar1(TRIG_PIN_1, ECHO_PIN_1, 200); // First ultrasonic sensor
NewPing sonar2(TRIG_PIN_2, ECHO_PIN_2, 200); // Second ultrasonic sensor

// DHT11 sensor setup
DHT dht(DHTPIN, DHTTYPE);

const unsigned long COMM_TIMEOUT = 3000; // Communication timeout in milliseconds
unsigned long lastCommTime = 0;

BlynkTimer timer;

void setup() {
  Serial.begin(9600); // Start serial communication

  // Blynk setup
  Blynk.begin(auth, ssid, pass);

  // Set motor pins as outputs (for movement)
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  // Set motor pins as outputs for the arm motor
  pinMode(ARM_IN1, OUTPUT);
  pinMode(ARM_IN2, OUTPUT);
  pinMode(ARM_IN3, OUTPUT);
  pinMode(ARM_IN4, OUTPUT);

  // Smoke sensor setup
  pinMode(SMOKE_PIN, INPUT);

  // Start DHT11 sensor
  dht.begin();

  // Setup a timer to read sensor data every 5 seconds
  timer.setInterval(5000L, sendSensorData);
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
  if (distance2 > 0 && distance2 < 10) {
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

  // Run Blynk and timer
  Blynk.run();
  timer.run();
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
  // Step 1: Arm motor forward for 2 seconds (ARM_IN3 and ARM_IN4)
  digitalWrite(ARM_IN3, HIGH);
  digitalWrite(ARM_IN4, LOW);
  delay(1500);  // Move arm for 2 seconds

  // Step 2: Arm motor forward for 2 seconds (ARM_IN1 and ARM_IN2)
  digitalWrite(ARM_IN1, HIGH);
  digitalWrite(ARM_IN2, LOW);
  delay(1200);  // Adjust the delay as needed for your motor to pick up

  // Step 3: Arm motor reverse for 2 seconds (ARM_IN1 and ARM_IN2)
  digitalWrite(ARM_IN1, LOW);
  digitalWrite(ARM_IN2, HIGH);
  delay(1200);  // Reverse arm motor for 2 seconds

  digitalWrite(ARM_IN3, LOW);
  digitalWrite(ARM_IN4, HIGH);
  delay(1500);  // Move arm for 2 seconds

  // Stop the arm motor
  digitalWrite(ARM_IN1, LOW);
  digitalWrite(ARM_IN2, LOW);
  digitalWrite(ARM_IN3, LOW);
  digitalWrite(ARM_IN4, LOW);

  moveForward();
}

// Function to return to the initial state after detecting an object in the box
void returnToInitialState() {
  stopMovement();  // Stop all movement
  delay(1000);     // Delay for a moment
  moveForward();   // Move forward to start the operation again
}

// Function to send sensor data to the Blynk app
void sendSensorData() {
  // Read temperature and humidity
  float temp = dht.readTemperature();
  float humid = dht.readHumidity();

  // Read smoke sensor data
  int smokeDetected = digitalRead(SMOKE_PIN);

  // Send data to Blynk
  Blynk.virtualWrite(VPIN_TEMP, temp);
  Blynk.virtualWrite(VPIN_HUMID, humid);
  Blynk.virtualWrite(VPIN_SMOKE, smokeDetected ? 1 : 0); // 1 = Smoke detected, 0 = No smoke

  // Print to Serial Monitor for debugging
  Serial.print("Temperature: ");
  Serial.print(temp);
  Serial.print(" °C, Humidity: ");
  Serial.print(humid);
  Serial.print(" %, Smoke Detected: ");
  Serial.println(smokeDetected ? "Yes" : "No");
}
