#include <Joystick.h>
#include <PCF8575.h>


// The digital pins 2 - 20 are grounded when they are pressed.
// Pin 10, A10, Red = UP
// Pin 15, D15, Yellow = RIGHT
// Pin 16, D16, Orange = DOWN
// Pin 14, D14, Green = LEFT

// Pin 9, A9 = Button 1
// Pin 8, A8 = Button 2
// Pin 7, D7 = Button 3
// Pin 3, D3 = Button 4
// Pin 2, D2 = Button 5
// Pin 4, A6 = Button 6

// Pin 20, A2 = Select Button 1
// Pin 19, A1 = Start Button 2

// Pin 5, D5 = Other Button
// Pin 6, A7 = Other Button
// Pin 18, A0 = Other Button
// Pin 21, A3 = Other Button

//PCF8675 - I/O Expander
uint8_t address = 0x20;

PCF8575 PCF(address, &Wire);

uint16_t value = 0xFFFF;
//LED PINS - outputs
const uint8_t pin0 = 0;
const uint8_t pin1 = 1;
const uint8_t pin2 = 2;
const uint8_t pin3 = 3;
const uint8_t pin4 = 4;
const uint8_t pin5 = 5;
const uint8_t pin6 = 6;
const uint8_t pin7 = 7;
const uint8_t pin8 = 8;
const uint8_t pin9 = 9;
//BUTTON PINS - 6 bits, inputs, must be high
const uint8_t pin10 = 10;
const uint8_t pin11 = 11;
const uint8_t pin12 = 12;
const uint8_t pin13 = 13;
const uint8_t pin14 = 14;
const uint8_t pin15 = 15;

Joystick_ Joystick(JOYSTICK_DEFAULT_REPORT_ID,JOYSTICK_TYPE_GAMEPAD,
  8, 0,                  // Button Count, Hat Switch Count
  true, true, false,     // X and Y, but no Z Axis
  false, false, false,   // No Rx, Ry, or Rz
  false, false,          // No rudder or throttle
  false, false, false);  // No accelerator, brake, or steering



// Last state of the buttons
int lastButtonState[16] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
int buttonMap[16] = {10,15,16,14,9,8,7,3,2,4,20,19,5,6,18,21};

// ButtonMap = 0, Pin 10 = UP
// ButtonMap = 1, Pin 15 = RIGHT
// ButtonMap = 2, Pin 16 = DOWN
// ButtonMap = 3, Pin 14 = LEFT

// ButtonMap = 4, Pin 9 = Button 1
// ButtonMap = 5, Pin 8 = Button 2
// ButtonMap = 6, Pin 7 = Button 3
// ButtonMap = 7, Pin 3 = Button 4
// ButtonMap = 8, Pin 2 = Button 5
// ButtonMap = 9, Pin 4 = Button 6

// ButtonMap = 10, Pin 20 = Select Button 1
// ButtonMap = 11, Pin 19 = Start Button 2

// ButtonMap = 12, Pin 5 = Other Button
// ButtonMap = 13, Pin 6 = Other Button
// ButtonMap = 14, Pin 18 = Other Button
// ButtonMap = 15, Pin 21 = Other Button

void printHex(uint16_t x)
{
  if (x < 0x1000) Serial.print('0');
  if (x < 0x100)  Serial.print('0');
  if (x < 0x10)   Serial.print('0');
  Serial.println(x, HEX);
}

void setup() 
{
  Serial.begin(115200);
  Serial.println(__FILE__);
  Serial.print("PCF8575_LIB_VERSION:\t");
  Serial.println(PCF8575_LIB_VERSION);
  Serial.println();

  Wire.begin();

  while(1){
    if (!PCF.setAddress(address))
    {
        Serial.print("could not initialize... ");
        Serial.println(address,HEX);
        ++address;
    }
    else
    {
        Serial.print("Initialized... ");
        Serial.println(address,HEX);
        break;
    }
  }
  // Initialize Button Pins
  pinMode(2, INPUT_PULLUP);
  pinMode(3, INPUT_PULLUP);
  pinMode(4, INPUT_PULLUP);
  pinMode(5, INPUT_PULLUP);
  pinMode(6, INPUT_PULLUP);
  pinMode(7, INPUT_PULLUP);
  pinMode(8, INPUT_PULLUP);
  pinMode(9, INPUT_PULLUP);
  pinMode(10, INPUT_PULLUP);
  pinMode(14, INPUT_PULLUP);
  pinMode(15, INPUT_PULLUP);
  pinMode(16, INPUT_PULLUP);
  pinMode(18, INPUT_PULLUP);
  pinMode(19, INPUT_PULLUP);
  pinMode(20, INPUT_PULLUP);
  pinMode(21, INPUT_PULLUP);

  // Initialize Joystick Library
  Joystick.begin();
  Joystick.setXAxisRange(-1, 1);
  Joystick.setYAxisRange(-1, 1);
}

void loop() {
  
  PCF.write(pin3,LOW);  
  while(1)
  {
    int index;
    value = PCF.read16();
    value = value >> 6;
    if ((value & 0xff) != 0xFF)
    {
        for(index=0;(index<6);index++)
        {
          if (!(value & 0x01))
          {
              Serial.print("Button pressed ");
              Serial.println(index+10);
          }
          value = value >> 1;
        }
    }
  }

  // Read pin values
  for (int index = 0; index < 16; index++)
  {
    int currentButtonState = !digitalRead(buttonMap[index]);
    if (currentButtonState != lastButtonState[index])
    {
      switch (index) {
        case 0: // UP
          if (currentButtonState == 1) {
            Joystick.setYAxis(-1);
          } else {
            Joystick.setYAxis(0);
          }
          break;
        case 1: // RIGHT
          if (currentButtonState == 1) {
            Joystick.setXAxis(1);
          } else {
            Joystick.setXAxis(0);
          }
          break;
        case 2: // DOWN
          if (currentButtonState == 1) {
            Joystick.setYAxis(1);
          } else {
            Joystick.setYAxis(0);
          }
          break;
        case 3: // LEFT
          if (currentButtonState == 1) {
            Joystick.setXAxis(-1);
          } else {
            Joystick.setXAxis(0);
          }
          break;
        case 4: // Black Button 1
          Joystick.setButton(0, currentButtonState);
          break;
        case 5: // Black Button 2
          Joystick.setButton(1, currentButtonState);
          break;
        case 6: // Black Button 3
          Joystick.setButton(2, currentButtonState);
          break;
        case 7: // Black Button 4
          Joystick.setButton(3, currentButtonState);
          break;
        case 8: // Black Button 5
          Joystick.setButton(4, currentButtonState);
          break;
        case 9: // Black Button 6
          Joystick.setButton(5, currentButtonState);
          break;
        case 10: // Select Button
          Joystick.setButton(6, currentButtonState);
          break;
        case 11: // Start Button
          Joystick.setButton(7, currentButtonState);
          break;
        case 12: // Other Button 1
          Joystick.setButton(8, currentButtonState);
          break;
        case 13: // Other Button 2
          Joystick.setButton(9, currentButtonState);
          break;
        case 14: // Other Button 3
          Joystick.setButton(10, currentButtonState);
          break;
        case 15: // Other Button 4
          Joystick.setButton(11, currentButtonState);
          break;
      }
      lastButtonState[index] = currentButtonState;
    }
  }

  delay(10);
}

