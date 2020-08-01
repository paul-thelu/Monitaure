/*
 * File:   stm32l4xx_ssd1351.h
 * Author: Paul
 */

#ifndef MONITAURE_DRIVERS_INC_STM32L4XX_SSD1351_H_
#define	MONITAURE_DRIVERS_INC_STM32L4XX_SSD1351_H_

/* Private defines -----------------------------------------------------------*/
#define SSD1351_WIDTH           128
#define SSD1351_HEIGHT          128
#define SSD1351_SETCOLUMN 		0x15
#define SSD1351_SETROW    		0x75
#define SSD1351_WRITERAM   		0x5C
#define SSD1351_READRAM   		0x5D
#define SSD1351_SETREMAP 		0xA0
#define SSD1351_STARTLINE 		0xA1
#define SSD1351_DISPLAYOFFSET 	0xA2
#define SSD1351_DISPLAYALLOFF 	0xA4
#define SSD1351_DISPLAYALLON  	0xA5
#define SSD1351_NORMALDISPLAY 	0xA6
#define SSD1351_INVERTDISPLAY 	0xA7
#define SSD1351_FUNCTIONSELECT 	0xAB
#define SSD1351_DISPLAYOFF 		0xAE
#define SSD1351_DISPLAYON     	0xAF
#define SSD1351_PRECHARGE 		0xB1
#define SSD1351_DISPLAYENHANCE	0xB2
#define SSD1351_CLOCKDIV 		0xB3
#define SSD1351_SETVSL          0xB4
#define SSD1351_SETGPIO 		0xB5
#define SSD1351_PRECHARGE2 		0xB6
#define SSD1351_SETGRAY 		0xB8
#define SSD1351_USELUT          0xB9
#define SSD1351_PRECHARGELEVEL 	0xBB
#define SSD1351_VCOMH           0xBE
#define SSD1351_CONTRASTABC		0xC1
#define SSD1351_CONTRASTMASTER	0xC7
#define SSD1351_MUXRATIO        0xCA
#define SSD1351_COMMANDLOCK     0xFD
#define SSD1351_HORIZSCROLL     0x96
#define SSD1351_STOPSCROLL      0x9E
#define SSD1351_STARTSCROLL     0x9F

// Ecran OLED //
status_t SSD1351_WriteCommand(uint8_t data);
status_t SSD1351_WriteData(uint8_t data);
status_t OLED_SetCursor(unsigned char x, unsigned char y);
//void OLED_DrawPixel(unsigned char x, unsigned char y, unsigned int color);
unsigned int OLED_Color565(unsigned char r,unsigned char g,unsigned char b);
//void OLED_DrawLine(int x0,int y0,int x1,int y1,unsigned int color);
void OLED_RawFillRectangle(unsigned char x,unsigned char y,unsigned char w,unsigned char h,unsigned int color);
void OLED_FillRectangle(unsigned char x,unsigned char y,unsigned char w,unsigned char h,unsigned int color);
void OLED_FillScreen(unsigned int color);
/*void OLED_RawFastHLine(unsigned char x, unsigned char y, unsigned char w,unsigned int color);
void OLED_RawFastVLine(unsigned char x, unsigned char y, unsigned char h,unsigned int color);
void OLED_DrawFastVLine(unsigned char x, unsigned char y, unsigned char h,unsigned int color);
void OLED_DrawFastHLine(unsigned char x, unsigned char y, unsigned char w,unsigned int color);
void OLED_Circle(unsigned char x0,unsigned char y0,unsigned char rayon,unsigned int color);
void OLED_CircleHelper(unsigned char x0,unsigned char y0,unsigned char rayon,int cornername,unsigned int color);
void OLED_FillCircleHelper(unsigned char x0,unsigned char y0,unsigned char rayon,int cornername,int delta,unsigned int color);
void OLED_FillCircle(unsigned char x0,unsigned char y0,unsigned char rayon,unsigned int color);
void OLED_RoundRect(unsigned char x,unsigned char y,unsigned char w,unsigned char h,unsigned char r,unsigned int color);
void OLED_FillRoundRect(unsigned char x,unsigned char y,unsigned char w,unsigned char h,unsigned char r,unsigned int color);
void OLED_DrawBitmap(const unsigned int *bitmap);
void OLED_DrawIcon(unsigned char x, unsigned char y, const unsigned int *iconTAB);
void OLED_DrawChar(unsigned char x, unsigned char y, unsigned char character, unsigned int color);
void OLED_DrawString(unsigned char x, unsigned char y, unsigned char *string, unsigned int color);*/

status_t SSD1351_Stop(void);
status_t SSD1351_Start(void);
status_t SSD1351_Sleep(void);
status_t SSD1351_DeepSleep(void);
status_t SSD1351_WakeUp(void);
status_t SSD1351_Init(void);

#endif	/* MONITAURE_DRIVERS_INC_STM32L4XX_SSD1351_H_ */

