// UART Registers
#define UBRR0H_REG (*((volatile unsigned char *)0xC5))
#define UBRR0L_REG (*((volatile unsigned char *)0xC4))
#define UCSR0B_REG (*((volatile unsigned char *)0xC1))
#define UCSR0C_REG (*((volatile unsigned char *)0xC2))
#define UCSR0A_REG (*((volatile unsigned char *)0xC0))
#define UDR0_REG   (*((volatile unsigned char *)0xC6))
#define TXEN0      3
#define RXEN0      4
#define UDRE0      5
#define UCSZ00     1
#define UCSZ01     2

// EEPROM address to store incoming messages
#define EEPROM_ADDRESS 0x00  // Start at the beginning of EEPROM

void setup() {
    // UART Initialization for receiving data
    UBRR0H_REG = 0;
    UBRR0L_REG = 103;  // 9600 baud for 16 MHz clock
    UCSR0B_REG = (1 << RXEN0);  // Enable receiver
    UCSR0C_REG = (1 << UCSZ01) | (1 << UCSZ00);  // 8 data bits, 1 stop bit

    // Optionally, set up an LED or other output to indicate data reception
    // DDRB |= (1 << PB5); // Configure PB5 as output for LED
}

void loop() {
    // Variable to store the incoming character
    char receivedChar;

    // Check if data is available in the UART buffer
    if (UCSR0A_REG & (1 << RXC0)) {
        // Read the received character from UDR0
        receivedChar = UDR0_REG;

        // Store the received character in EEPROM at the current address
        eeprom_write_byte((uint8_t*)EEPROM_ADDRESS, receivedChar);

        
    }

    

void serialWriteChar(char c) {
    while (!(UCSR0A_REG & (1 << UDRE0)));  // Wait until buffer is empty
    UDR0_REG = c;  // Send character
}


