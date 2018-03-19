// Based very much on NintendoSpy Firmware for Arduino

#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

#define DEBUG 0

#define PIN_READ(pin)  (PIND&(1<<(pin)))

#define WAIT_FALLING_EDGE(pin) while( !PIN_READ(pin) ); while( PIN_READ(pin) );


#define DATA_OUT_PIN  PORTB2 // 10
#define DATA_OUT_PORT DDB2 // 10

#define SNES_LATCH      3 // white
#define SNES_DATA       4 // red
#define SNES_CLOCK      6 // yellow

#define SNES_BITCOUNT  16

#define SET_DATA_OUT_LOW  PORTB &=~(1<<DATA_OUT_PIN);
#define SET_DATA_OUT_HIGH PORTB |= (1<<DATA_OUT_PIN);

#define ZERO  '\0'  // Use a byte value of 0x00 to represent a bit with value 0.
#define ONE    '1'  // Use an ASCII one to represent a bit with value 1.  This makes Arduino debugging easier.
#define SPLIT '\n'  // Use a new-line character to split up the controller state packets.

// Declare some space to store the bits we read from a controller.
unsigned char rawData[128];

// names of buttons
const char* name[] = {
          " B",
          " Y",
          " Select",
          " Start",
          " Up",
          " Down",
          " Left",
          " Right",
          " A",
          " X",
          " L",
          " R",
          "dummy1",
          "dummy2",
          "dummy3",
          "dummy4"};

// General initialization, just sets all pins to input and starts serial communication.
void setup()
{
  PORTD = 0x00;
  DDRD  = 0x00;
  DDRC  = 0x00;

  DDRB |= (1<<DATA_OUT_PORT); // pin 10 is in output mode
  SET_DATA_OUT_HIGH;

  Serial.begin(115200);
}

// Reading SNES controller data.
void read_shiftRegister(unsigned char bits)
{
  unsigned char *rawDataPtr = rawData;

  WAIT_FALLING_EDGE(SNES_LATCH) ;

  do {
      WAIT_FALLING_EDGE(SNES_CLOCK);

      *rawDataPtr = !PIN_READ(SNES_DATA);
      if (*rawDataPtr) {
        SET_DATA_OUT_LOW;
      }
      else {
        SET_DATA_OUT_HIGH;
      }
      ++rawDataPtr;
  }
  while( --bits > 0 );
}

// Sends a packet of controller data over the Arduino serial interface.
// Mostly for debugging.
inline void sendRawData()
{
  unsigned char i = 0
  for(i; i < SNES_BITCOUNT; i++) {
    if (rawData[i]) {
      Serial.print(name[i]);
    }
  }
  Serial.write(SPLIT);
}

void loop()
{
  noInterrupts();
  read_shiftRegister(SNES_BITCOUNT);
  interrupts();
  if (DEBUG) {
    sendRawData();
  }
}
