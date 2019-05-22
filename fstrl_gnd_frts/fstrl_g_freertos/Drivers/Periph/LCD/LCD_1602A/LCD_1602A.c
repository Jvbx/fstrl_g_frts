//
//  LCD1602A.cpp
//  lcd-1602a-arduino-lib
//
//  Created by ??? on 3/4/16.
//  Copyright � 2016 Marcus. All rights reserved.
//

#include "LCD_1602A.h"
//#include "timerdelay.h"
#include "dwt_stm32_delay.h"

//#define VALID_PORT(port) (port >= 0 && port <= 13)

/*
__STATIC_INLINE void LCD1602A_Delay_ms(uint16_t delay_ms)
{
    osDelay(delay_ms);//LL_mDelay(delay_ms);
}
*/
//---Функция задержки в микросекундах---//
__STATIC_INLINE void LCD1602A_Delay_us(uint32_t delay_us)
{
DWT_Delay_us(delay_us); //TimerDelay_us(delay_us);
}


__STATIC_INLINE void LCD1602A_E_Strobe(void)
{
	 LL_GPIO_SetOutputPin(LCD_PORT_E, LCD_PIN_E);
	 LCD1602A_Delay_us(2);
	 LL_GPIO_ResetOutputPin(LCD_PORT_E, LCD_PIN_E);
	 LCD1602A_Delay_us(1);
}



__STATIC_INLINE void LCD1602A_SetPinDataIn(GPIO_TypeDef *GPIOx, uint32_t Pin)
{
	//if (LL_GPIO_GetPinMode(GPIOx, Pin) == LL_GPIO_MODE_INPUT) {return;}
	LL_GPIO_ResetOutputPin(GPIOx, Pin);
	LL_GPIO_SetPinMode(GPIOx, Pin, LL_GPIO_MODE_INPUT);
	LL_GPIO_SetPinPull(GPIOx, Pin, LL_GPIO_PULL_UP);
}


__STATIC_INLINE void LCD1602A_SetPinDataOut(GPIO_TypeDef *GPIOx, uint32_t Pin)
{
	//if (LL_GPIO_GetPinMode(GPIOx, Pin) == LL_GPIO_MODE_OUTPUT) {return;}
	LL_GPIO_SetPinMode(GPIOx, Pin, LL_GPIO_MODE_OUTPUT);
	LL_GPIO_ResetOutputPin(GPIOx, Pin);
    LL_GPIO_SetPinPull(GPIOx, Pin, LL_GPIO_PULL_DOWN);
    LL_GPIO_SetPinOutputType(GPIOx, Pin, LL_GPIO_OUTPUT_PUSHPULL);

}

__STATIC_INLINE void LCD1602A_SetPins(GPIO_TypeDef *GPIOx, uint32_t Pin)
{
	//if (LL_GPIO_GetPinMode(GPIOx, Pin) != LL_GPIO_MODE_OUTPUT) {return;}
	LL_GPIO_SetOutputPin(GPIOx, Pin);
}

__STATIC_INLINE void LCD1602A_ResetPins(GPIO_TypeDef *GPIOx, uint32_t Pin)
{
	//if (LL_GPIO_GetPinMode(GPIOx, Pin) != LL_GPIO_MODE_OUTPUT) {return;}
    LL_GPIO_ResetOutputPin(GPIOx, Pin);
}

/*
__STATIC_INLINE uint8_t LCD1602A_ReadPin(GPIO_TypeDef *GPIOx, uint32_t Pin)
{
   // if (LL_GPIO_GetPinMode(GPIOx, Pin) != LL_GPIO_MODE_INPUT) {return 0;}
    if (LL_GPIO_IsInputPinSet(GPIOx, Pin)) return 1;
    return 0;
}
*/

__STATIC_INLINE void LCD1602A_ClearDataBus(void)
{
	LL_GPIO_ResetOutputPin(LCD_DATA_PORT, LCD_BUS_MASK);
}

void LCD1602A_WaitReady(void)
{
	uint8_t Busy_Flag = 0xFF;
	uint16_t retrycount = 0xFFFF;
	LCD1602A_ResetPins(LCD_DATA_PORT, LCD_PIN_MASK);
	LCD1602A_SetPinDataIn(LCD_PORT_D7, LCD_PIN_D7);
	LCD1602A_SetPins(LCD_PORT_RW, LCD_PIN_RW);
	do
		{
			LCD1602A_SetPins(LCD_PORT_E, LCD_PIN_E);
			LCD1602A_Delay_us(2);
			Busy_Flag = (LL_GPIO_IsInputPinSet(LCD_PORT_D7, LCD_PIN_D7) > 0);
			LCD1602A_ResetPins(LCD_PORT_E, LCD_PIN_E);
			LCD1602A_Delay_us(1);
			LCD1602A_E_Strobe();
			retrycount --;
		}
	while (Busy_Flag && retrycount);
	LCD1602A_SetPinDataOut(LCD_PORT_D7, LCD_PIN_D7);
	LCD1602A_ResetPins(LCD_DATA_PORT, LCD_PIN_MASK);
	LCD1602A_ResetPins(LCD_PORT_RW, LCD_PIN_RW);
}

uint8_t LCD1602A_GetBusyStatus(void)
{
	uint8_t Busy_Flag = 0xFF;
	LCD1602A_ResetPins(LCD_DATA_PORT, LCD_PIN_MASK);
	LCD1602A_SetPinDataIn(LCD_PORT_D7, LCD_PIN_D7);
	LCD1602A_SetPins(LCD_PORT_RW, LCD_PIN_RW);
			LCD1602A_SetPins(LCD_PORT_E, LCD_PIN_E);
			LCD1602A_Delay_us(2);
			Busy_Flag = (LL_GPIO_IsInputPinSet(LCD_PORT_D7, LCD_PIN_D7) > 0);
			LCD1602A_ResetPins(LCD_PORT_E, LCD_PIN_E);
			LCD1602A_Delay_us(1);
			LCD1602A_E_Strobe();
	LCD1602A_SetPinDataOut(LCD_PORT_D7, LCD_PIN_D7);
	LCD1602A_ResetPins(LCD_DATA_PORT, LCD_PIN_MASK);
	LCD1602A_ResetPins(LCD_PORT_RW, LCD_PIN_RW);
	return Busy_Flag;
}


//---Отсылка байта в дисплей---//

void LCD1602A_SendByte(char databyte, uint8_t isData)
{

	LCD1602A_ResetPins(LCD_PORT_RW, LCD_PIN_RW);
	LCD1602A_ResetPins(LCD_OUT_PORT, LCD_PIN_MASK);
	if (isData) LCD1602A_SetPins(LCD_PORT_RS, LCD_PIN_RS);

	//first halfbyte
	LCD1602A_SetPins(LCD_DATA_PORT, ((databyte & 0xF0) << 8U));
	LCD1602A_E_Strobe();
	LCD1602A_ClearDataBus();
    //second halfbyte
	LCD1602A_SetPins(LCD_DATA_PORT, (((databyte & 0x0F) << 4U) << 8U));
	LCD1602A_E_Strobe();
	LCD1602A_ResetPins(LCD_OUT_PORT, LCD_PIN_MASK);
}

void LCD1602A_SendCmdHalfByte(char cmdbyte)
{
	LCD1602A_ResetPins(LCD_PORT_RW, LCD_PIN_RW);
    LCD1602A_ResetPins(LCD_OUT_PORT, LCD_PIN_MASK);

    //second halfbyte, lower half only 0b----XXXX
    LCD1602A_SetPins(LCD_DATA_PORT, (((cmdbyte & 0x0F) << 4U) << 8U));
    LCD1602A_E_Strobe();
    LCD1602A_ResetPins(LCD_OUT_PORT, LCD_PIN_MASK);
}

__STATIC_INLINE void LCD1602A_SendCmd(char cmdbyte)
{
	LCD1602A_SendByte(cmdbyte, CMD);
}

__STATIC_INLINE void LCD1602A_SendData(char databyte)
{
	LCD1602A_SendByte(databyte, DATA);
}

void LCD1602A_SetCursorPos(char Row, char Col)
{
	   char address;
	   if (Row == 0)
	   address = 0;
	   else
	   address = 0x40;
	   address |= Col;
	   LCD1602A_WaitReady();
	   LCD1602A_SendCmd((LCD_SET_DDRAM_ADDR | address));
}


//---Очистка дисплея---//
void LCD1602A_ClearScreen(void)
{
	LCD1602A_WaitReady();
	LCD1602A_SendCmd(LCD_CLEAR_DISPLAY);
	LCD1602A_WaitReady();
	LCD1602A_SendCmd(LCD_RETURN_HOME);
	LCD1602A_WaitReady();
}


void LCD1602A_EntryModeSet(uint8_t left_to_right, uint8_t shift_all_display)
{
	uint8_t lcd_cmd = (LCD_ENTRY_MODE_SET|(left_to_right << LCD_SHIFT_RIGHT)|(shift_all_display));
	LCD1602A_WaitReady();
    LCD1602A_SendCmd(lcd_cmd);
}

void LCD1602A_DisplayControl(uint8_t display_state, uint8_t cursor_show, uint8_t cursor_blink)
{
	uint8_t lcd_cmd = (LCD_DISPLAY_CONTROL|(display_state << LCD_DISPLAY_ON)|(cursor_show << LCD_CURSOR_SHOW)|(cursor_blink));
	LCD1602A_WaitReady();
	LCD1602A_SendCmd(lcd_cmd);
}




void LCD1602A_CursorSeekMode(uint8_t entire_display_seek, uint8_t seek_right)
{
	uint8_t lcd_cmd = (LCD_DISPLAY_CURSOR_SEEK|(entire_display_seek << LCD_DISPLAY_SEEK)|(seek_right << LCD_SEEK_DIRECTION));
    LCD1602A_SendCmd(lcd_cmd);
}




void LCD1602A_FunctionSet(uint8_t display_8bit, uint8_t two_lines, uint8_t bigfont)
{
	uint8_t lcd_cmd = (LCD_FUNCTION_SET|(display_8bit << LCD_BIT_MODE)|(two_lines << LCD_2_LINES)|(bigfont << LCD_FONT_SIZE_BIG));
    LCD1602A_SendCmd(lcd_cmd);
}

void LCD1602A_CGRAM_Set(uint8_t cgram_address)
{
	uint8_t lcd_cmd = (LCD_SET_SGRAM_ADDR|(cgram_address & LCD_SGRAM_MASK));
    LCD1602A_SendCmd(lcd_cmd);
}

void LCD1602A_DDRAM_Set(uint8_t ddram_address)
{
	uint8_t lcd_cmd = (LCD_SET_DDRAM_ADDR|(ddram_address & LCD_DDRAM_MASK));
    LCD1602A_SendCmd(lcd_cmd);
}







void LCD1602A_4BIT_Preinit(void)
{
	LCD1602A_ResetPins(LCD_PORT_RW, LCD_PIN_RW);
    LCD1602A_ResetPins(LCD_OUT_PORT, LCD_PIN_MASK);

	LCD1602A_SendCmdHalfByte(LCD_INIT_STAGE_0);
	LCD1602A_Delay_us(4100);
	LCD1602A_SendCmdHalfByte(LCD_INIT_STAGE_0);
	LCD1602A_Delay_us(100);
	LCD1602A_SendCmdHalfByte(LCD_INIT_STAGE_0);
	LCD1602A_Delay_us(100);
	LCD1602A_SendCmdHalfByte(LCD_INIT_4_BIT);
	LCD1602A_Delay_us(100);
	LCD1602A_FunctionSet(OFF, ON, OFF);
	LCD1602A_Delay_us(80);
	LCD1602A_DisplayControl(ON, OFF, OFF);
	LCD1602A_Delay_us(80);
	LCD1602A_SendCmd(0x01);
	LCD1602A_Delay_us(80);
	LCD1602A_DisplayControl(ON, ON, ON);
	LCD1602A_Delay_us(80);
	LCD1602A_EntryModeSet(ON, OFF);
	LCD1602A_DDRAM_Set(0x00);
	LCD1602A_WaitReady();
}


void LCD1602A_Init(void)
{
	LCD1602A_Delay_us(60000);
	LCD1602A_4BIT_Preinit();
}


//---Печать строки---//

void LCD1602A_PrintStr(char *Text)
{
	 char *c;
	    c = Text;
	    while ((c != 0) && (*c != 0))
	    {
	    	LCD1602A_WaitReady();
	    	LCD1602A_SendData(*c);
	        c++;
	    }
}

