// Register and pin definitions
#define DDRB 0x24   // Data Direction Register for PORTB
#define PORTB 0x25  // PORTB Register

// Traffic light LEDs for both sides
#define PB0 0  // Red light for Side 1
#define PB1 1  // Yellow light for Side 1
#define PB2 2  // Green light for Side 1
#define PB3 3  // Red light for Side 2
#define PB4 4  // Yellow light for Side 2
#define PB5 5  // Green light for Side 2

// Global variable to track the last received command
char lastCommand = 'C';   // Default: Normal traffic light mode
unsigned long lastCommandTime = 0;  // Time of last command received

void setup() {
  // Initialize Serial communication
  Serial.begin(9600);

  // Set PB0 - PB5 as outputs (traffic light LEDs)
  *((volatile unsigned char *)DDRB) |= 0x3F;  // 0x3F = 00111111 (PB0 to PB5 as output)
}

void loop() {
  // Check if data is available from the Master
  if (Serial.available() > 0) {
    lastCommand = Serial.read();  // Read the latest command
    lastCommandTime = millis();   // Update last command received time

    // Print received command to the serial monitor
    Serial.print("Received command: ");
    Serial.println(lastCommand);
  }

  // Switch based on the last command
  switch (lastCommand) {
    case 'A':  // Emergency for Side 1
      emergencyModeSide1();
      Serial.write('Emergency vehicle at lane 1');
      break;

    case 'B':  // Emergency for Side 2
      emergencyModeSide2();
      Serial.write('Emergency vehicle at lane ');
      break;

    default:  // Default to normal traffic light sequence
      normalTrafficLights();
      break;
  }

  // Check if 5 seconds have passed since the last command
  if (millis() - lastCommandTime <= 5000) {
    lastCommand = 'C';  // Return to normal mode
  }
}

void emergencyModeSide1() {
  *((volatile unsigned char *)PORTB) = (1 << PB2) | (1 << PB3);  // Side 1 Green, Side 2 Red
  delay(1000);  // 1-second delay
}

void emergencyModeSide2() {
  *((volatile unsigned char *)PORTB) = (1 << PB0) | (1 << PB5);  // Side 1 Red, Side 2 Green
  delay(1000);  // 1-second delay
}

void normalTrafficLights() {
  // Side 1: Red, Side 2: Green
  *((volatile unsigned char *)PORTB) = (1 << PB0) | (1 << PB5);
  delay(3000);  // 3-second delay

  // Side 1: Yellow, Side 2: Yellow
  *((volatile unsigned char *)PORTB) = (1 << PB1) | (1 << PB4);
  delay(1000);  // 1-second delay

  // Side 1: Green, Side 2: Red
  *((volatile unsigned char *)PORTB) = (1 << PB2) | (1 << PB3);
  delay(3000);  // 3-second delay

  // Side 1: Yellow, Side 2: Yellow
  *((volatile unsigned char *)PORTB) = (1 << PB1) | (1 << PB4);
  delay(1000);  // 1-second delay
}

