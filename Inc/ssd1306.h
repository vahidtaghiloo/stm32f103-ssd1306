/*
  *****************************************************************************
  * @file    ssd1306.h
  * @author  
  * @brief   Header file for SSD1306 I2C OLED Display.
  *****************************************************************************
*/

#ifndef SSD1306_H_
#define SSD1306_H_

#include "stm32f1xx_hal.h"

#define SSD1306_I2C_PORT		hi2c1
// I2c address
#define SSD1306_I2C_ADDR        0x78
// SSD1306 width in pixels
#define SSD1306_WIDTH           128
// SSD1306 height in pixels
#define SSD1306_HEIGHT          32

void SSD1306_Init();
void SSD1306_Fill();
void SSD1306_Clear();
void SSD1306_Update_Screen();
void SSD1306_Write_Command(uint8_t command);
void SSD1306_Draw_Pixel(uint8_t x, uint8_t y);
char SSD1306_Write_Char(char ch);
char SSD1306_Write_String(char *str);
void goto_xy(uint8_t x, uint8_t y);

#endif /* SSD1306_H_ */
