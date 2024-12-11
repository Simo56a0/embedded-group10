void setup() {
  // Initialize Serial communication
  Serial.begin(9600);

}

void loop() {
  if (Serial.available() > 0) {
    char command = Serial.read();  // Read command from sensor

    // Print received command to the serial monitor
    Serial.print("Received command: ");
    Serial.println(command);

    // Process the command using a switch statement
    switch (command) {
      case 'A':  // Emergency for Side 1
        Serial.write('A');  // Signal: Side 1 Green, Side 2 Red
        break;

      case 'B':  // Emergency for Side 2
        Serial.write('B');  // Signal: Side 2 Green, Side 1 Red
        break;

      default:  // Normal traffic sequence
        Serial.write('C');  // Signal: Normal operation
        break;
    }
  }

  // Small delay for stability
  delay(100);  
}
