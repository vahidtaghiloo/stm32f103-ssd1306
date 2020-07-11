/*
  *****************************************************************************
  * @file    ssd1306.c
  * @author  
  * @brief   Source file for SSD1306 I2C OLED Display.
  *****************************************************************************
*/

#include <stdint.h>
#include "ssd1306.h"
#include "font.h"

extern I2C_HandleTypeDef hi2c1;
static uint8_t SSD1306_Buffer[SSD1306_WIDTH * SSD1306_HEIGHT / 4];
uint8_t current_x = 0;
uint8_t current_y = 0;
const font_width = 6;

/**
  * @brief  Initiates SSD1306 OLED screen
  */

void SSD1306_Init(){
	uint8_t init_data[] = {0x00, 	    //0x00 means all the upcoming bytes are commands
				  	       			 0xae,		//set display off
				           			 0x81,		//set contrast 
				           			 0x7f,
				           			 0xa4,		//entire display on - resume to RAM content
				           			 0xa6,		//normal display
				           			 0x20,		//set memory addressing mode
				           			 0x00,		//horizontal addressing mode
					         			 0x21,		//set column address
					         			 0x00,		//column start address
					         			 0x7f,		//column end address
					         			 0x22,		//set page address
					         			 0x00,		//page start address
					         			 0x03,		//page end address
					       			 //0x02,		//page addressing mode
					       			 //0x00,		//set  lower column address
				         			 //0x10, 	  //set higher column address
				         		 	 //0xb0,		//page0 as page start address
				           			 0x40,		//line0 set display start  
				           			 0xa0,		//set seg0 as column address 0  
				           			 0xa8, 	    //set MUX ratio
				           			 0x1f,		//MUX ratio set as 31
				           			 0xc8,		//scan from com0 to com[n-1]
				           			 0xd3,		//set display offset
                   			 0x00,		//no offset
				           			 0xda,		//set COM pins configuration 
				           			 0x02,
				           			 0xd5,		//set display clock divide ratio 
				           			 0xf0, 
				           			 0xd9,		//set pre-charge ratio 
				           			 0x22, 
				           			 0xdb,		//set vcom deselect level
				           			 0x20,		//0.77 * vcc  
				           			 0x8d,		//charge pump setting 
				           			 0x14,		//enable dc-dc 
				           			 0xaf		    //set display on
				          		   };
	//according to datasheet 100 ms delay for screen to boot 
	HAL_Delay(100);
	HAL_I2C_Master_Transmit(&hi2c1, SSD1306_I2C_ADDR, init_data, sizeof(init_data), HAL_MAX_DELAY);
}

/**
  * @brief  Writes one byte of command to SSD1306 OLED screen.
  * @param  command from command table on page 28 of SSD1306 datasheet.
  */

void SSD1306_Write_Command(uint8_t command){
	HAL_I2C_Mem_Write(&hi2c1, SSD1306_I2C_ADDR, 0x00, 1, &command, 1, HAL_MAX_DELAY);
}

/**
  * @brief  Fills the whole screen.
  */

void SSD1306_Fill(){
	uint32_t i;
	for(i = 0; i < sizeof(SSD1306_Buffer); i++){
		SSD1306_Buffer[i] = 0xff;
	}
}

/**
  * @brief  Clears the whole screen.
  */

void SSD1306_Clear(){
	uint32_t i;
	for(i = 0; i < sizeof(SSD1306_Buffer); i++){
		SSD1306_Buffer[i] = 0x00;
	}
}

/**
  * @brief  Updates the screen (Writes the data to SSD1306 internal RAM).
  */

void SSD1306_Update_Screen(){
	uint8_t i;
	for(i = 0; i < 4; i++){
		//SSD1306_Write_Command(0x00);
		//SSD1306_Write_Command(0x00);
		//SSD1306_Write_Command(0x02);
		//SSD1306_Write_Command(0xB3 - i);
		HAL_I2C_Mem_Write(&hi2c1, SSD1306_I2C_ADDR, 0x40, 1, &SSD1306_Buffer[SSD1306_WIDTH * i], SSD1306_WIDTH, HAL_MAX_DELAY);
	}
}

/**
  * @brief  Draws a pixel on screen.
  * @param  x is x coordinate.
  * @param  y is y cordinate.
  * (0,0) is the lower left corner and Can be changed by changing 
  * seg0 as column address 0 & scan from com0 to com[n-1] from init_data[]
  */

void SSD1306_Draw_Pixel(uint8_t x, uint8_t y){
	SSD1306_Buffer[x + (y / 8) * SSD1306_WIDTH] |= 1 << (y % 8);
}

/**
  * @brief  Writes a character on screen.
  * @param  ch is the desired character.
  */

char SSD1306_Write_Char(char ch){
	uint8_t i;
	for (i = 0; i < font_width; i++){
		SSD1306_Buffer[current_x + i + (current_y * SSD1306_WIDTH)] = Font[((ch - 32) * font_width) + i];
	}
	current_x += font_width;
}

/**
  * @brief  Writes a string on screen.
  * @param  str is the desired string.
  */

char SSD1306_Write_String(char* str){
	while(*str){
		SSD1306_Write_Char(*str);
		str++;
	}
}

/**
  * @brief  Sets cursor on (x,y).
  * @param  x is x coordinate.
  * @param  y line indicator
	* y is 0, 1, 2 or 3.
  * (0,0) is the lower left corner.
  */

void goto_xy(uint8_t x, uint8_t y){
	current_x = x;
	current_y = y;
}