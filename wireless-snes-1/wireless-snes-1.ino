// Based very much on NintendoSpy Firmware for Arduino

#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

#define DEBUG 1

#define PIN_READ(pin)  (PIND&(1<<(pin)))

#define WAIT_FALLING_EDGE(pin) while( !PIN_READ(pin) ); while( PIN_READ(pin) );

#define DATA_OUT_PIN  PORTB2 // 10
#define DATA_OUT_PORT DDB2 // 10

#define SNES_LATCH      3 // white
#define SNES_DATA       4 // red
#define SNES_CLOCK      6 // yellow

#define SNES_BITCOUNT  16

#define SET_DATA_OUT_LOW  PORTB &=~ (1<<PORTB2); // digitalWrite (10, LOW);
#define SET_DATA_OUT_HIGH PORTB |=(1<< PORTB2);  // digitalWrite (10, HIGH);

RF24 radio(7, 8); // CE, CSN

const byte thisAddress[6] = "SNES1";
const byte otherAddress[6] = "SNES2";

#define SPLIT '\n'  // Use a new-line character to split up the controller state packets.

// Declare some space to store the bits we read from a controller.
unsigned char thisController[SNES_BITCOUNT];
unsigned char otherController[SNES_BITCOUNT];

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

  radio.begin();
  radio.openWritingPipe(thisAddress);
  radio.setPALevel(RF24_PA_MIN);
  radio.stopListening();
  
  Serial.begin(115200);
}

// Read SNES controller data
void read_shiftRegister(unsigned char bits)
{
  unsigned char *rawDataPtr = thisController;
  //radio.read(&otherController, sizeof(otherController));
  
  WAIT_FALLING_EDGE(SNES_LATCH);

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
  while(--bits > 0);

  radio.write(&thisController, sizeof(thisController));
}

// Sends a packet of controller data over the Arduino serial interface.
// Mostly for debugging.
inline void sendRawData()
{
  unsigned char i = 0;
  for(i; i < SNES_BITCOUNT; i++) {
    if (thisController[i]) {
      Serial.print(" This controller ");
      Serial.print(name[i]);
      Serial.print(SPLIT);
    }
    if (otherController[i]) {
      Serial.print(" Other controller ");
      Serial.print(name[i]);
      Serial.print(SPLIT);
    }
  }
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
