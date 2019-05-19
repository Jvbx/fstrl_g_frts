//
//  LCD1602A.hpp
//  lcd-1602a-arduino-lib
//
//  Created by ??? on 3/4/16.
//  Copyright � 2016 Marcus. All rights reserved.
//

#ifndef LCD1602A_h
#define LCD1602A_h

#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include "gpio.h"
#include "main.h"

#define     LCD_BUS_WIDTH_4          4
#define     LCD_BUS_WIDTH_8          0

//Hardware pins assignment
#define     LCD_OUT_PORT             GPIOB
#define     LCD_DATA_PORT            LCD_OUT_PORT

#define     LCD_PORT_RW              GPIOA
#define     LCD_PIN_RW               LCD_RW_Pin          // PA15

#define     LCD_PORT_RS              GPIOB
#define     LCD_PIN_RS               LCD_RS_Pin          // PB9

#define     LCD_PORT_E               GPIOB
#define     LCD_PIN_E                LCD_E_Pin           // PB8

#define     LCD_PORT_D7              GPIOB
#define     LCD_PIN_D7               LCD_D7_Pin          // PB7

#define     LCD_PORT_D6              GPIOB
#define     LCD_PIN_D6               LCD_D6_Pin          // PB6

#define     LCD_PORT_D5              GPIOB
#define     LCD_PIN_D5               LCD_D5_Pin          // PB5

#define     LCD_PORT_D4              GPIOB
#define     LCD_PIN_D4               LCD_D4_Pin          // PB4

#define     LCD_PIN_MASK       ((LCD_PIN_RS | LCD_PIN_E | LCD_PIN_D7 | LCD_PIN_D6 | LCD_PIN_D5 | LCD_PIN_D4))
#define     LCD_BUS_MASK       ((LCD_PIN_D7 | LCD_PIN_D6 | LCD_PIN_D5 | LCD_PIN_D4))

#define     LCD_PIN_OUTPUT     LL_GPIO_MODE_OUTPUT
#define     LCD_PIN_INPUT      LL_GPIO_MODE_INPUT

#define     LOW    GPIO_PIN_RESET
#define     HIGH   GPIO_PIN_SET



#define     DATA 1
#define     CMD  0

//LCD160A Instructions:

//Single instructions
#define LCD_CLEAR_DISPLAY                     0x01    //0000 0001
#define LCD_RETURN_HOME                       0x02    //0000 001x

//Multiple option instructions:

//Entry mode set bitmask:
#define LCD_ENTRY_MODE_SET                    0x04    //0000 0100, base command mask
#define LCD_SHIFT_RIGHT                       1        //0000 0010, H - move right, otherwise (L) - left
#define LCD_SHIFT_DISPLAY                     0       //0000 0001, H - shift display, otherwise (L) - shift cursor

//Display and Cursor ON/OFF
#define LCD_DISPLAY_CONTROL                   0x08    //0000 1000, base command mask
#define LCD_DISPLAY_ON                        2        //0000 0100, H - entire display on, L - display off
#define LCD_CURSOR_SHOW                       1        //0000 0010, H - cursor shown, L - hidden
#define LCD_CURSOR_BLINK                      0         //0000 0001, H - blinking on, L - blinking off

//Cursor or Display shift
#define LCD_DISPLAY_CURSOR_SEEK               0x10     //0001 0000, shift entire display/cursor without writing/reading data
#define LCD_DISPLAY_SEEK                      3        //0000 1000, H - shift display, address counter not changed,
                                                       // L - move cursor, AC incremented or decremented
#define LCD_SEEK_DIRECTION                    2        //0000 01xx, H - move right

//Function Set
#define LCD_FUNCTION_SET                      0x20    // 0010 0000, 4 or 8 bit mode, 1/2 lines, font size control
#define LCD_8_BIT_MODE                        4        // 0001 0000, H - 8 bit mode, L - 4 bit mode
#define LCD_2_LINES                           3        // 0000 1000, H - 2 line mode, L - 1 line
#define LCD_FONT_SIZE_BIG                     2        // 0000 0100, H - 5x11 character size, L - 5x8 size

//Set CGRAM adress
#define LCD_SET_SGRAM_ADDR                    0x40    // 0100 0000
#define LCD_SGRAM_MASK                        0x3F    // 0011 1111 - AND mask for cgram address

//Set DDRAM adress
#define LCD_SET_DDRAM_ADDR                    0x80    // 1000 0000
#define LCD_DDRAM_MASK                        0x7F    // 0111 1111 - AND mask for ddram address

//busy flag mask (read operation)
#define LCD_BUSY_FLAG                         0x80    // 1000 0000 - busy flag, H - busy.  other bits contains current AC

//init sequence
#define LCD_INIT_STAGE_0                      0x03    //0000 0011
#define LCD_INIT_4_BIT                        0x02    //0000 0010
#define LCD_INIT_4_STAGE_2                    0xC0    //0011 0010
#define LCD_INIT_STAGE_3                      (LCD_FUNCTION_SET|LCD_2_LINES|LCD_FONT_SIZE_BIG)    //0010 1100

void LCD1602A_Init();
void LCD1602A_SetCursorPos(char Row, char Col);
void LCD1602A_ClearScreen();
void LCD1602A_PrintStr(char *Text);
void LCD1602A_DisplayControl(uint8_t display_state, uint8_t cursor_show, uint8_t cursor_blink);

/*   void LCD1602A_initialize(LCD1602A* dev);
 uint16_t LCD1602A_getMode(LCD1602A* dev);
 void LCD1602A_sendCommand(LCD1602A* dev, uint16_t flags);
 void LCD1602A_sendCharacter(LCD1602A* dev, char character);
 void LCD1602A_sendString(LCD1602A* dev, const char* string);
 void LCD1602A_sendData(LCD1602A* dev, char value);
 void LCD1602A_setBackgroundLight(LCD1602A* dev, uint16_t status);
 void LCD1602A_setContrast(LCD1602A* dev, double contrast);
 void LCD1602A_clearDisplay(LCD1602A* dev);
 void LCD1602A_setCursor(LCD1602A* dev, uint16_t enable);
 void LCD1602A_setBlinkCursor(LCD1602A* dev, uint16_t enable);
 void LCD1602A_sendDisplayFlags(LCD1602A* dev);
 void LCD1602A_sendWorkingMode(LCD1602A* dev);
 void LCD1602A_sendInputMode(LCD1602A* dev, uint16_t moveRight, uint16_t staticCursor);
 void LCD1602A_sendDataMode(LCD1602A* dev, uint16_t fourBitsMode, uint16_t doubleLine, uint16_t hd);
 //void sendDataMode(uint16_t fourBitsMode){sendDataMode(fourBitsMode, 1, 1);}
 void LCD1602A_setVisible(LCD1602A* dev, uint16_t value);
 void LCD1602A_resetCursor(LCD1602A* dev);
 void LCD1602A_seekCursor(LCD1602A* dev, uint16_t index, uint16_t line);

 */
#endif /* LCD1602A_h */
