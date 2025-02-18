# CFAG12232A1 Example Code

## Information
Example firmware for Crystalfontz CFAG12232A1 Family 
- [CFAG12232A1-TMI-TJ](https://www.crystalfontz.com/product/cfag12232A1tmitj)
- [CFAG12232A1-YYH-TJ](https://www.crystalfontz.com/product/cfag12232A1yyhtj)

Code written for Seeeduino, an open-source (important!) 3.3v capable Arduino clone.

https://www.seeedstudio.com/Seeeduino-V4.2-p-2517.html

https://github.com/SeeedDocument/SeeeduinoV4/raw/master/resources/Seeeduino_v4.2_sch.pdf

## Controller
The controller is a Wuxi i-core AiP31520

https://www.crystalfontz.com/controllers/Wuxi/AiP31520/

Please refer to the display datasheet before connecting to a power source.

Further, a variable resistor connection is required for contrast adjustment and its connection scheme has been outlined in the datasheet.

Additionally, refer to the datasheet for proper backlight connection

## Connection Guide
|  Function - 6800 Parallel        | Seeeduino | Port  | Display pin |
| -------------------------------: |:----------|:-----:|:-----------:|
|                                  |**Power Lines**                  |
|  Power 3.3V                      | 3.3V      |       |  2          |
|  Ground                          | GND       |       |  1          |
|                                  |**Control Lines**                |
|  Data/Instruction (A0)           | D8        | PORTB |  4          |
|  Read/Write       (R/W)          | D9        | PORTB |  11         |
|  Chip enable IC1  (CS1)          | D10       | PORTB |  5          |
|  Chip enable IC2  (CS2)          | D11       | PORTB |  6          |
|  Reset            (RES)          | D12       | PORTB |  18         |
|                                  |**Data Lines**                   |
|  DATA BUS LINE (DB0)             | D0        | PORTD |  10         |
|  DATA BUS LINE (DB1)             | D1        | PORTD |  11         |
|  DATA BUS LINE (DB2)             | D2        | PORTD |  12         |
|  DATA BUS LINE (DB3)             | D3        | PORTD |  13         |
|  DATA BUS LINE (DB4)             | D4        | PORTD |  14         |
|  DATA BUS LINE (DB5)             | D5        | PORTD |  15         |
|  DATA BUS LINE (DB6)             | D6        | PORTD |  16         |
|  DATA BUS LINE (DB7)             | D7        | PORTD |  17         |

## Example explanation
This example highlights the ability to display several fullscreen demos as well as the splash screen for this product family

This splash screen was converted to code using our utility [Image2Code](https://github.com/crystalfontz/Image2Code)
