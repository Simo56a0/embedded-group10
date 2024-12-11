// Define register addresses for PORTB and DDRB
#define PORTB_REG (*((volatile unsigned char *)0x25))
#define DDRB_REG  (*((volatile unsigned char *)0x24))
#define PB5 5
#define PB4 4

// Define register addresses for PORTD and DDRD
#define PORTD_REG (*((volatile unsigned char *)0x2B))
#define DDRD_REG  (*((volatile unsigned char *)0x2A))
#define PD7 7  // Output pin (Trigger for ultrasonic)
#define PD6 6  // Input pin (Echo from ultrasonic)

// UART Registers
#define UBRR0H_REG (*((volatile unsigned char *)0xC5))
#define UBRR0L_REG (*((volatile unsigned char *)0xC4))
#define UCSR0B_REG (*((volatile unsigned char *)0xC1))
#define UCSR0C_REG (*((volatile unsigned char *)0xC2))
#define UCSR0A_REG (*((volatile unsigned char *)0xC0))
#define UDR0_REG   (*((volatile unsigned char *)0xC6))
#define TXEN0      3
#define UDRE0      5
#define UCSZ00     1
#define UCSZ01     2

void setup() {
  // UART Initialization
  UBRR0H_REG = 0;
  UBRR0L_REG = 103;  // 9600 baud for 16 MHz clock
  UCSR0B_REG = (1 << TXEN0);  // Enable transmitter
  UCSR0C_REG = (1 << UCSZ01) | (1 << UCSZ00);  // 8 data bits, 1 stop bit

  // Initialize Serial communication
  Serial.begin(9600);
}

void loop() {
  long duration, cm;

  // Trigger ultrasonic pulse
  PORTD_REG &= ~(1 << PD7);  // Clear PD7
  delayMicroseconds(2);      // Wait 2 µs
  PORTD_REG |= (1 << PD7);   // Set PD7
  delayMicroseconds(10);     // Wait 10 µs
  PORTD_REG &= ~(1 << PD7);  // Clear PD7

  // Measure echo pulse width
  duration = pulseIn(PD6, HIGH);  // Read from PD6 (echo)

  // Calculate distance
  cm = microsecondsToCentimeters(duration);

  // Control LEDs based on distance
  if (cm > 20 && cm <= 40) {
    Serial.write("A");  // Signal for medium distance
    
  } else if (cm > 0 && cm <= 20) {
    Serial.write("B");  // Signal for close distance
   
  }
 

  delay(100);  // Wait for next measurement
}

// Convert microseconds to centimeters
long microsecondsToCentimeters(long microseconds) {
  return microseconds / 29 / 2;  // Convert microseconds to cm
}

// Send emergency information to Serial
void sendEmergencyInfo(long cm) {
  serialPrintString("Emergency vehicle detected at ");
  serialPrintNumber(cm);
  serialPrintString(" cm\n");
}

// UART Transmission Functions
void serialWriteChar(char c) {
  while (!(UCSR0A_REG & (1 << UDRE0)));  // Wait until buffer is empty
  UDR0_REG = c;  // Send character
}

void serialPrintString(const char* str) {
  while (*str) {
    serialWriteChar(*str++);  // Send characters one by one
  }
}

void serialPrintNumber(long number) {
  char buffer[10];
  ltoa(number, buffer, 10);  // Convert number to string
  serialPrintString(buffer);  // Send string
}
