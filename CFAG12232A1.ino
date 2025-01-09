//==============================================================================
//
//  CRYSTALFONTZ CFAG12232A1 Family EXAMPLE FIRMWARE
//  The controller is an Wuxi i-core AiP31520
//    https://www.crystalfontz.com/controllers/Wuxi/AiP31520/
//
//  Code written for Seeeduino 3.3v (important!), an open-source 3.3v capable Arduino clone.
//    https://www.seeedstudio.com/Seeeduino-V4.2-p-2517.html
//    https://github.com/SeeedDocument/SeeeduinoV4/raw/master/resources/Seeeduino_v4.2_sch.pdf
//
//==============================================================================
//This is free and unencumbered software released into the public domain.
//
//Anyone is free to copy, modify, publish, use, compile, sell, or
//distribute this software, either in source code form or as a compiled
//binary, for any purpose, commercial or non-commercial, and by any
//means.
//
//In jurisdictions that recognize copyright laws, the author or authors
//of this software dedicate any and all copyright interest in the
//software to the public domain. We make this dedication for the benefit
//of the public at large and to the detriment of our heirs and
//successors. We intend this dedication to be an overt act of
//relinquishment in perpetuity of all present and future rights to this
//software under copyright law.
//
//THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
//EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
//MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
//IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR
//OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
//ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
//OTHER DEALINGS IN THE SOFTWARE.
//
//For more information, please refer to <http://unlicense.org/>
//==============================================================================
// LCD parallel 6800 pins and control lines on Seeeduino:
// Seeduino    |  Port  | LCD
// ------------+--------+-------------
// #0-7/D0-7   |  PD0-7 | DB0-DB7 (data bus lines)
// #8/D8       |  PB0   | A0  (H: Data, L: Command)
// #9/D9       |  PB1   | CS1 (column select 1/chip enable IC1)
// #10/D10     |  PB2   | CS2 (column select 2/chip enable IC2)
// #11/D11     |  PB3   | R/W (read/write)
// #12/D12     |  PB4   | RES (H -> L: Reset)
//
// you also need to control the contrast with a variable resistor,
// please refer the datasheet schematic for connection details
//==============================================================================
// define the Arduino pin locations as per the table above
#define A0 0
#define CS1 1
#define CS2 2
#define RW 3
#define RESET 4

// define some convenient shortcuts
#define SET_A0 PORTB |= (1<<A0)
#define CLR_A0 PORTB &= ~(1<<A0)
#define SET_CS1 PORTB |= (1<<CS1)
#define CLR_CS1 PORTB &= ~(1<<CS1)
#define SET_CS2 PORTB |= (1<<CS2)
#define CLR_CS2 PORTB &= ~(1<<CS2)
#define SET_RW PORTB |= (1<<RW)
#define CLR_RW PORTB &= ~(1<<RW)
#define SET_RESET PORTB |= (1<<RESET)
#define CLR_RESET PORTB &= ~(1<<RESET)

// define columns/chip enables
#define chip1 (0x01)
#define chip2 (0x02)
#define bchip (0x03)

// page addresses
// page 0 - 0xB8
// page 1 - 0XB9
// page 2 - 0xBA
// page 3 - 0xBB

// initialization commands
#define SoftwareReset (0xE2) // software reset of the controller
#define DisplayStatus (0xAF) // turn display on
#define StartLine (0xC0)     // start line set to zero
#define StartPage (0xB8)     // start page set to zero
#define StartCol (0x00)      // start column set to zero
#define StaticDrive (0xA4)   // turn static drive off
#define DutySelect (0xA9)    // set duty to 1/32
#define Mapping (0xA0)       // set normal mapping


#include "images.h"
//function definitions
//==============================================================================

// Shows a cross dot test
void Cross_Dot();
// Delay a number of milliseconds
void Delay_Nms(int n);
// All pixels on
void Full_on();
// All pixels off
void Full_off();
// Shows a horizontal lines test
void Horizontal_line();
// Initialize the 2 controllers
void Initialize_SBN1661();
// Send a command to 1 or both of the controllers
void sendCommand(uint8_t chip, uint8_t cmd);
// Send a byte of data to 1 or both of the controllers
void sendData(uint8_t chip, uint8_t data);
// Displays the stored picture on the screen. Inverted allows for showing all the pixels inverted
void show_picture(bool inverted);
// (optional) Used to control the flow of the program with a button or other IO
void Step();
// Shows a vertical line test
void Vertical_line();

//==============================================================================
// function implementations
//==============================================================================
// Base functions
void sendCommand(uint8_t chip, uint8_t command)
{
  CLR_A0;
  CLR_RW;
  delay(1);
  PORTD = command;

  // specify where the command should be written
  if (chip == chip1)
  {
      SET_CS1;
      CLR_CS2;
  }
  else if (chip == chip2)
  {
      SET_CS2;
      CLR_CS1;
  }
  else if (chip == bchip)
  {
      SET_CS1;
      SET_CS2;
  }
  
  delay(1);
  CLR_CS1;
  CLR_CS2;
}
/******************************/
void sendData(uint8_t chip, uint8_t data)
{
  SET_A0;
  CLR_RW;
  PORTD = data;
  delayMicroseconds(20);
  if(chip == chip1)
  {
    SET_CS1;
    CLR_CS2;
  }
  else if(chip == chip2)
  {
    CLR_CS1;
    SET_CS2;
  }
  else if(chip == bchip)
  {
    CLR_CS1;
    CLR_CS2;
  }
  delayMicroseconds(20);

  CLR_CS1;
  CLR_CS2;
}
/******************************/
void Cross_Dot()
{
  uint8_t i;
  for(uint8_t page = 0;page < 4; page++)
  {
    sendCommand(bchip, 0xB8+page);
    sendCommand(bchip, 0x00); 

    for(i = 0; i < 30; i++)
    {
      sendData(chip1, 0xAA);
      sendData(chip1, 0x55);
    }
    sendData(chip1, 0xAA);

    for(i = 30; i < 60; i++)
    {
      sendData(chip2, 0x55);	
      sendData(chip2, 0xAA); 	
    }
    sendData(chip2, 0x55);
  }
}
/******************************/
void Delay_Nms(int n)
{
  int i;
  for(i = n; i > 0 ; i--)
  {
    delay(1);
  }
}
/******************************/
void Full_on()
{
  uint8_t i;
  for(uint8_t page = 0; page < 4; page++)
  {
    sendCommand(bchip, 0xB8 + page);
    sendCommand(bchip, 0x00); 

    for(i = 0; i < 61; i++)
    {
      sendData(chip1, 0xFF);
    }
    for(i = 61; i < 122; i++)
    {
      sendData(chip2, 0xFF);
    }
  }
}
/******************************/
void Full_off()
{
  uint8_t i;
  for(uint8_t page = 0; page < 4; page++)
  {
    sendCommand(bchip, 0xB8 + page);
    sendCommand(bchip, 0x00); 

    for(i = 0; i < 61; i++)
    {
      sendData(chip1, 0x00);
    }
    for(i = 61; i < 141; i++)
    {
      sendData(chip2, 0x00);	      	
    }
  }
}
/******************************/
void Horizontal_line()
{
  uint8_t i,z;
  for(z=0;z<4;z++)
  {
    sendCommand(bchip, 0xB8+z);
    sendCommand(bchip, 0x00); 

    for(i=0;i<61;i++)
    {
      sendData(chip1, 0xAA);
    }
    for(i=0;i<122;i++)
    {
      sendData(chip2, 0xAA);
    }
  }
}
/******************************/
void Initialize_SBN1661()
{ 
	SET_RESET;
	delay(1);
  CLR_RESET;
	delay(1);
	SET_RESET;
	delay(1);
	
  sendCommand(bchip, SoftwareReset);  //Software RESET
	delay(1);

	sendCommand(bchip, Mapping);        //Select ADC=0
	delay(1);

  sendCommand(bchip, StaticDrive);    //Satic Drive OFF
	delay(1);

  sendCommand(bchip, DutySelect);     //Select Duty=1/32
	delay(1);

	sendCommand(bchip, StartLine);      //Set Start Line 0(C0H)~31(DFH)
	delay(1);

	sendCommand(bchip, StartPage);      //Set Page Address 0(B8H)~3(BBH)
	delay(1);
	
  sendCommand(bchip, StartCol);       //Set Column Address 0(00H)~(4FH)
  delay(1);

  sendCommand(bchip, DisplayStatus);  //Set Display ON
  delay(1);
}
/******************************/
void Step()
{
  //Adds a check if a button is present to advance to the next image
  // while(AOUTOB==1 & STEP==0) {}
  // if(STEP==1)
  // {
  //   Delay_Nms(10);
  //   while(AOUTOB==0) {}
  // }
  // else if(AOUTOB==1)
  // {
  //   Delay_Nms(10);
  // }
}
/******************************/
void Vertical_line()
{
  uint8_t i;
  for(uint8_t page = 0; page < 4; page++)
  {
    sendCommand(bchip, 0xB8 + page);
    sendCommand(bchip, 0x00); 

    for(i = 0; i < 30; i++)
    {
      sendData(chip1, 0xFF);
      sendData(chip1, 0x00);
    }
    sendData(chip1, 0xFF);

    for(i = 30; i < 61; i++)
    {
      sendData(chip2, 0x00);	
      sendData(chip2, 0xFF); 	
    }
    sendData(chip2, 0x00);
  }
}
/******************************/
void show_picture(bool inverted = false)
{
  uint8_t i;
  for(uint8_t page = 0; page < 4; page++)
  {
    sendCommand(bchip, 0xB8 + page);
    sendCommand(bchip, 0x00);
    if(inverted)
    {
      for(i = 0; i < 61; i++)
      {  
        sendData(chip1, ~pgm_read_byte(&cfag12232a1[page][i]));
      }
      for(i = 61; i < 122; i++)
      {  
        sendData(chip2, ~pgm_read_byte(&cfag12232a1[page][i]));
      }		
    }
    else
    {
      for(i = 0; i < 61; i++)
      {  
        sendData(chip1, pgm_read_byte(&cfag12232a1[page][i]));
      }
      for(i = 61; i < 122; i++)
      {  
        sendData(chip2, pgm_read_byte(&cfag12232a1[page][i]));
      }
    }		
  }
}
/******************************/
void setup() {
  //PORTB is pins D8-D15
  DDRB |= B00011111; //Set PINs 8-12 as outputs
  PORTB = 0;         //Set to reasonable starting value
  //PORTD = Pins D0-D7
  DDRD |= B11111111; //Set all PORTD pins as outputs
  PORTD = 0;         //Set to reasonable starting value

  Initialize_SBN1661();
}

void loop()
{
  Full_off();
  Delay_Nms(1000);
  Step();
  Full_on();
  Delay_Nms(1000);
  Step();
  Vertical_line();
  Delay_Nms(1000);
  Step();
  Horizontal_line();
  Delay_Nms(1000);
  Step();
  Cross_Dot();
  Delay_Nms(1000);
  Step();
  show_picture(true);
  Delay_Nms(1000);
  Step();
  show_picture();
  Delay_Nms(10000);
  Step();
  while(1);
}
