/**
 * \file			stm32l4xx_ssd1351.c
 * \author    		Paul THELU
 * \version			1.0
 * \date			21 January 2020
 * \brief			SSD1351 display driver
 */

#include "../inc/global.h"
#include "../../Monitaure_Drivers/Inc/stm32l4xx_ssd1351.h"

extern SPI_HandleTypeDef hspi1;

/**
 * \brief			Initialize the SSD1351 OLED Display
 * \return			STATUS_OK on success, STATUS_KO otherwise
 */
status_t SSD1351_Init(void)
{
	SSD1351_Stop();

    HAL_GPIO_WritePin(OLED_RST_GPIO_Port, OLED_RST_Pin, GPIO_PIN_SET);
    HAL_Delay(1);	// 10ms
    HAL_GPIO_WritePin(OLED_RST_GPIO_Port, OLED_RST_Pin, GPIO_PIN_RESET);
    HAL_Delay(1);
    HAL_GPIO_WritePin(OLED_RST_GPIO_Port, OLED_RST_Pin, GPIO_PIN_SET);
    HAL_Delay(1);

    SSD1351_Start();

    SSD1351_WriteCommand(SSD1351_COMMANDLOCK);
    SSD1351_WriteData(0x12);						// Unlocked to enter commands
    SSD1351_WriteCommand(SSD1351_COMMANDLOCK);
    SSD1351_WriteData(0xB1);						// Make all commands accessible
    SSD1351_WriteCommand(SSD1351_DISPLAYOFF);
    SSD1351_WriteCommand(SSD1351_CLOCKDIV);
    SSD1351_WriteData(0xF1);
    SSD1351_WriteCommand(SSD1351_MUXRATIO);
    SSD1351_WriteData(0x7F);
    SSD1351_WriteCommand(SSD1351_SETREMAP);
    SSD1351_WriteData(0x74);						// Bit 7:6 = 65,536 Colors, Bit 3 = BGR or RGB
    SSD1351_WriteCommand(SSD1351_SETCOLUMN);
    SSD1351_WriteData(0x00);
    SSD1351_WriteData(0x7F);
    SSD1351_WriteCommand(SSD1351_SETROW);
    SSD1351_WriteData(0x00);
    SSD1351_WriteData(0x7F);
    SSD1351_WriteCommand(SSD1351_DISPLAYENHANCE);	// Enhance display performance
    SSD1351_WriteData(0xA4);
    SSD1351_WriteData(0x00);
    SSD1351_WriteData(0x00);
    SSD1351_WriteCommand(SSD1351_STARTLINE);
    SSD1351_WriteData(0x00);
    SSD1351_WriteCommand(SSD1351_DISPLAYOFFSET);
    SSD1351_WriteData(0x00);
    SSD1351_WriteCommand(SSD1351_SETGPIO);
    SSD1351_WriteData(0x00);						// Disable GPIO pins
    SSD1351_WriteCommand(SSD1351_FUNCTIONSELECT);
    SSD1351_WriteData(0x01);						// External VDD (0 = External, 1 = Internal)
    SSD1351_WriteCommand(SSD1351_PRECHARGE);
    SSD1351_WriteData(0x32);
    SSD1351_WriteCommand(SSD1351_SETVSL);
    SSD1351_WriteData(0xA0);						// Enable External VSL (A0 si diodes qui se suivent))
    SSD1351_WriteData(0xB5);
    SSD1351_WriteData(0x55);
    SSD1351_WriteCommand(SSD1351_PRECHARGELEVEL);
    SSD1351_WriteData(0x17);
    SSD1351_WriteCommand(SSD1351_VCOMH);
    SSD1351_WriteData(0x05);
    SSD1351_WriteCommand(SSD1351_CONTRASTABC);
    SSD1351_WriteData(0xC8);
    SSD1351_WriteData(0x80);
    SSD1351_WriteData(0xC8);
    SSD1351_WriteCommand(SSD1351_CONTRASTMASTER);
    SSD1351_WriteData(0x03);						// 0x0F = Maximum contrast
    SSD1351_WriteCommand(SSD1351_PRECHARGE2);
    SSD1351_WriteData(0x01);
    SSD1351_WriteCommand(SSD1351_NORMALDISPLAY);
    OLED_FillScreen(OLED_Color565(0xFF,0x00,0xFF));
    SSD1351_WriteCommand(SSD1351_DISPLAYON);

    return STATUS_OK;
}

/**
 * \brief			SSD1351 OLED Display Stop command
 * \return			STATUS_OK on success, STATUS_KO otherwise
 */
status_t SSD1351_Stop(void)
{
	/* Turn OFF the power regulator */
    HAL_GPIO_WritePin(_13V_EN_GPIO_Port, _13V_EN_Pin, GPIO_PIN_RESET);

    return STATUS_OK;
}

/**
 * \brief			SSD1351 OLED Display Start command
 * \return			STATUS_OK on success, STATUS_KO otherwise
 */
status_t SSD1351_Start(void)
{
	/* Turn ON the power regulator */
	HAL_GPIO_WritePin(_13V_EN_GPIO_Port, _13V_EN_Pin, GPIO_PIN_SET);
	HAL_Delay(2);

	return STATUS_OK;
}

/**
 * \brief			SSD1351 OLED Display Sleep mode
 * \return			STATUS_OK on success, STATUS_KO otherwise
 */
status_t SSD1351_Sleep(void)
{
	/* Turn OFF the OLED Display */
	if (SSD1351_WriteCommand(SSD1351_DISPLAYOFF) != STATUS_OK)		return STATUS_KO;

    return STATUS_OK;
}

/**
 * \brief			SSD1351 OLED Display Deep Sleep mode
 * \return			STATUS_OK on success, STATUS_KO otherwise
 */
status_t SSD1351_DeepSleep(void)
{
	/* Turn OFF the OLED Display */
	if (SSD1351_WriteCommand(SSD1351_DISPLAYOFF) != STATUS_OK)		return STATUS_KO;

	/* Turn OFF the OLED voltage regulator */
	if (SSD1351_WriteCommand(SSD1351_FUNCTIONSELECT) != STATUS_OK)	return STATUS_KO;
	if (SSD1351_WriteData(0x00) != STATUS_OK)						return STATUS_KO;

    return STATUS_OK;
}

/**
 * \brief			SSD1351 OLED Display Wakes Up from Sleep or Deep Sleep modes
 * \return			STATUS_OK on success, STATUS_KO otherwise
 */
status_t SSD1351_WakeUp(void)
{
	/* Turn ON the OLED voltage regulator */
	if (SSD1351_WriteCommand(SSD1351_FUNCTIONSELECT) != STATUS_OK)	return STATUS_KO;
	if (SSD1351_WriteData(0x01) != STATUS_OK)						return STATUS_KO;
	HAL_Delay(1);

    /* Turn ON the OLED Display */
	if (SSD1351_WriteCommand(SSD1351_DISPLAYON) != STATUS_OK)		return STATUS_KO;

    return STATUS_OK;
}

/**
 * \brief			Writes commmands to the display
 * \param[in]		a: Data to transmit
 * \return			STATUS_OK on success, STATUS_KO otherwise
 */
status_t SSD1351_WriteCommand(uint8_t data)
{
	HAL_GPIO_WritePin(GPIOB, OLED_D_C_Pin | OLED_CS_Pin, GPIO_PIN_RESET);	// DC & CS = 0

	if (HAL_SPI_Transmit(&hspi1, &data, 1, SPI_MAX_DELAY) != HAL_OK)	return(STATUS_KO);

	HAL_GPIO_WritePin(OLED_CS_GPIO_Port, OLED_CS_Pin, GPIO_PIN_SET);		// CS = 1

    return(STATUS_OK);
}

/**
 * \brief			Writes data to the display
 * \param[in]		a: Data to transmit
 * \return			STATUS_OK on success, STATUS_KO otherwise
 */
status_t SSD1351_WriteData(uint8_t data)
{
	HAL_GPIO_WritePin(OLED_D_C_GPIO_Port, OLED_D_C_Pin, GPIO_PIN_SET);	// DC = 1
    HAL_GPIO_WritePin(OLED_CS_GPIO_Port, OLED_CS_Pin, GPIO_PIN_RESET);	// CS = 0

    if (HAL_SPI_Transmit(&hspi1, &data, 1, SPI_MAX_DELAY) != HAL_OK)	return STATUS_KO;

    HAL_GPIO_WritePin(OLED_CS_GPIO_Port, OLED_CS_Pin, GPIO_PIN_SET);	// CS = 1

    return(STATUS_OK);
}

/**
 * \brief			Sets the cursor to the corresponding x and y display coordinates
 * \param[in]		x: Display x coordinates
 * \param[in]		y: Display y coordinates
 */
status_t OLED_SetCursor(unsigned char x, unsigned char y)
{
    if ((x >= SSD1351_WIDTH) || (y >= SSD1351_HEIGHT)) {
    	return STATUS_KO;
    }

    // Column
    SSD1351_WriteCommand(SSD1351_SETCOLUMN);
    SSD1351_WriteData(x);
    SSD1351_WriteData(SSD1351_WIDTH - 1);
    // Row
    SSD1351_WriteCommand(SSD1351_SETROW);
    SSD1351_WriteData(y);
    SSD1351_WriteData(SSD1351_HEIGHT - 1);

    SSD1351_WriteCommand(SSD1351_WRITERAM);

    return STATUS_OK;
}

/*void OLED_DrawPixel(unsigned char x, unsigned char y, unsigned int color)
{
    // Transform x and y based on current rotation.
    switch(0)	// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    {
        // Case 0: No rotation
        case 1:  // Rotated 90 degrees clockwise.
            swap(x, y);
            x = SSD1351_WIDTH - x - 1;
            break;
        case 2:  // Rotated 180 degrees clockwise.
            x = SSD1351_WIDTH - x - 1;
            y = SSD1351_HEIGHT - y - 1;
            break;
        case 3:  // Rotated 270 degrees clockwise.
            swap(x, y);
            y = SSD1351_HEIGHT - y - 1;
            break;
    }

    // Bounds check.
    if ((x >= SSD1351_WIDTH) || (y >= SSD1351_HEIGHT)) return;
    if ((x < 0) || (y < 0)) return;

    OLED_SetCursor(x,y);

    SSD1351_WriteData(color >> 8);
    SSD1351_WriteData(color);
}*/

unsigned int OLED_Color565(unsigned char r,unsigned char g,unsigned char b)
{
    unsigned int c;

    c = r >> 3;
    c <<= 6;
    c |= g >> 2;
    c <<= 5;
    c |= b >> 3;

    return c;
}

/*void OLED_DrawLine(int x0,int y0,int x1,int y1,unsigned int color)
{
    int steep = abs(y1 - y0) > abs(x1 - x0);
    if(steep)
    {
        swap(x0, y0);
        swap(x1, y1);
    }

    if(x0 > x1)
    {
        swap(x0, x1);
        swap(y0, y1);
    }

    int dx, dy;
    dx = x1 - x0;
    dy = abs(y1 - y0);

    int err = dx / 2;
    int ystep;

    if(y0 < y1) {ystep = 1;}
    else        {ystep = -1;}

    for(;x0<=x1;x0++)
    {
        if(steep)   {OLED_DrawPixel(y0, x0, color);}
        else        {OLED_DrawPixel(x0, y0, color);}
        err -= dy;
        if(err < 0)
        {
            y0 += ystep;
            err += dx;
        }
    }
}*/

// Draw a filled rectangle with no rotation.
void OLED_RawFillRectangle(unsigned char x,unsigned char y,unsigned char w,unsigned char h,unsigned int color)
{
    int i;
    // Bounds check
    if ((x >= SSD1351_WIDTH) || (y >= SSD1351_HEIGHT)) {
    	return;
    }

    // Y bounds check
    if (y+h > SSD1351_HEIGHT) {
    	h = SSD1351_HEIGHT - y - 1;
    }

    // X bounds check
    if (x+w > SSD1351_WIDTH) {
    	w = SSD1351_WIDTH - x - 1;
    }

    // set location
    SSD1351_WriteCommand(SSD1351_SETCOLUMN);
    SSD1351_WriteData(x);
    SSD1351_WriteData(x+w-1);
    SSD1351_WriteCommand(SSD1351_SETROW);
    SSD1351_WriteData(y);
    SSD1351_WriteData(y+h-1);
    // fill!
    SSD1351_WriteCommand(SSD1351_WRITERAM);

    for (i = 0; i < w * h; ++i) {
        SSD1351_WriteData(color >> 8);
        SSD1351_WriteData(color);
    }
}

void OLED_FillRectangle(unsigned char x,unsigned char y,unsigned char w,unsigned char h,unsigned int color)
{
    // Transform x and y based on current rotation.
    switch (0)	// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    {
		case 0:  // No rotation
			OLED_RawFillRectangle(x, y, w, h, color);
		  break;
		case 1:  // Rotated 90 degrees clockwise.
			//swap(x, y);
			x = SSD1351_WIDTH - x - h;
			OLED_RawFillRectangle(x, y, h, w, color);
		  break;
		case 2:  // Rotated 180 degrees clockwise.
			x = SSD1351_WIDTH - x - w;
			y = SSD1351_HEIGHT - y - h;
			OLED_RawFillRectangle(x, y, w, h, color);
			break;
		case 3:  // Rotated 270 degrees clockwise.
			//swap(x, y);
			y = SSD1351_HEIGHT - y - w;
			OLED_RawFillRectangle(x, y, h, w, color);
			break;
    }
}

void OLED_FillScreen(unsigned int color)
{
    OLED_FillRectangle(0, 0, SSD1351_WIDTH, SSD1351_HEIGHT, color);
}

/*void OLED_RawFastHLine(unsigned char x, unsigned char y, unsigned char w,unsigned int color)
{
    int i = 0;
    // Bounds check
    if ((x >= SSD1351_WIDTH) || (y >= SSD1351_HEIGHT))
      return;

    // X bounds check
    if (x+w > SSD1351_WIDTH)    {w = SSD1351_WIDTH - x - 1;}

    if (w < 0) return;

    // set location
    SSD1351_WriteCommand(SSD1351_SETCOLUMN);
    SSD1351_WriteData(x);
    SSD1351_WriteData(x+w-1);
    SSD1351_WriteCommand(SSD1351_SETROW);
    SSD1351_WriteData(y);
    SSD1351_WriteData(y);
    // fill!
    SSD1351_WriteCommand(SSD1351_WRITERAM);

    for (i=0;i<w;i++)
    {
        SSD1351_WriteData(color >> 8);
        SSD1351_WriteData(color);
    }
}

// Draw a vertical line ignoring any screen rotation.
void OLED_RawFastVLine(unsigned char x, unsigned char y, unsigned char h,unsigned int color)
{
    int i=0;
    // Bounds check
    if ((x >= SSD1351_WIDTH) || (y >= SSD1351_HEIGHT))
    return;

    // X bounds check
    if (y+h > SSD1351_HEIGHT)   {h = SSD1351_HEIGHT - y - 1;}

    if (h < 0) return;

    // set location
    SSD1351_WriteCommand(SSD1351_SETCOLUMN);
    SSD1351_WriteData(x);
    SSD1351_WriteData(x);
    SSD1351_WriteCommand(SSD1351_SETROW);
    SSD1351_WriteData(y);
    SSD1351_WriteData(y+h-1);
    // fill!
    SSD1351_WriteCommand(SSD1351_WRITERAM);

    for (i=0; i < h; i++)
    {
        SSD1351_WriteData(color >> 8);
        SSD1351_WriteData(color);
    }
}

void OLED_DrawFastVLine(unsigned char x, unsigned char y, unsigned char h,unsigned int color)
{
    // Transform x and y based on current rotation.
    switch(0)	// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    {
        case 0:  // No rotation
            OLED_RawFastVLine(x, y, h, color);
            break;
        case 1:  // Rotated 90 degrees clockwise.
            swap(x, y);
            x = SSD1351_WIDTH - x - h;
            OLED_RawFastHLine(x, y, h, color);
            break;
        case 2:  // Rotated 180 degrees clockwise.
            x = SSD1351_WIDTH - x - 1;
            y = SSD1351_HEIGHT - y - h;
            OLED_RawFastVLine(x, y, h, color);
            break;
        case 3:  // Rotated 270 degrees clockwise.
            swap(x, y);
            y = SSD1351_HEIGHT - y - 1;
            OLED_RawFastHLine(x, y, h, color);
            break;
    }
}

void OLED_DrawFastHLine(unsigned char x, unsigned char y, unsigned char w,unsigned int color)
{
    // Transform x and y based on current rotation.
    switch(0)	// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    {
        case 0:  // No rotation.
            OLED_RawFastHLine(x, y, w, color);
            break;
        case 1:  // Rotated 90 degrees clockwise.
            swap(x, y);
            x = SSD1351_WIDTH - x - 1;
            OLED_RawFastVLine(x, y, w, color);
            break;
        case 2:  // Rotated 180 degrees clockwise.
            x = SSD1351_WIDTH - x - w;
            y = SSD1351_HEIGHT - y - 1;
            OLED_RawFastHLine(x, y, w, color);
            break;
        case 3:  // Rotated 270 degrees clockwise.
            swap(x, y);
            y = SSD1351_HEIGHT - y - w;
            OLED_RawFastVLine(x, y, w, color);
            break;
    }
}

void OLED_Circle(unsigned char x0,unsigned char y0,unsigned char rayon,unsigned int color)
{
    int f = 1 - rayon;
    int ddF_x = 1;
    int ddF_y = -2 * rayon;
    int x = 0;
    int y = rayon;

    OLED_DrawPixel(x0,y0+rayon,color);
    OLED_DrawPixel(x0,y0-rayon,color);
    OLED_DrawPixel(x0+rayon,y0,color);
    OLED_DrawPixel(x0-rayon,y0,color);

    while (x < y)
    {
        if (f >= 0)
        {
            y--;
            ddF_y += 2;
            f += ddF_y;
        }
        x++;
        ddF_x += 2;
        f += ddF_x;

        OLED_DrawPixel(x0 + x, y0 + y,color);
        OLED_DrawPixel(x0 - x, y0 + y,color);
        OLED_DrawPixel(x0 + x, y0 - y,color);
        OLED_DrawPixel(x0 - x, y0 - y,color);
        OLED_DrawPixel(x0 + y, y0 + x,color);
        OLED_DrawPixel(x0 - y, y0 + x,color);
        OLED_DrawPixel(x0 + y, y0 - x,color);
        OLED_DrawPixel(x0 - y, y0 - x,color);
    }
}

void OLED_CircleHelper(unsigned char x0,unsigned char y0,unsigned char rayon,int cornername,unsigned int color)
{
    int f     = 1 - rayon;
    int ddF_x = 1;
    int ddF_y = -2 * rayon;
    int x     = 0;
    int y     = rayon;

    while(x<y)
    {
        if(f >= 0)
        {
            y--;
            ddF_y += 2;
            f     += ddF_y;
        }
        x++;
        ddF_x += 2;
        f     += ddF_x;
        if(cornername & 0x4)
        {
            OLED_DrawPixel(x0 + x, y0 + y,color);
            OLED_DrawPixel(x0 + y, y0 + x,color);
        }
        if(cornername & 0x2)
        {
            OLED_DrawPixel(x0 + x, y0 - y,color);
            OLED_DrawPixel(x0 + y, y0 - x,color);
        }
        if(cornername & 0x8)
        {
            OLED_DrawPixel(x0 - y, y0 + x,color);
            OLED_DrawPixel(x0 - x, y0 + y,color);
        }
        if(cornername & 0x1)
        {
            OLED_DrawPixel(x0 - y, y0 - x,color);
            OLED_DrawPixel(x0 - x, y0 - y,color);
        }
    }
}

void OLED_FillCircleHelper(unsigned char x0,unsigned char y0,unsigned char rayon,int cornername,int delta,unsigned int color)
{
    int f     = 1 - rayon;
    int ddF_x = 1;
    int ddF_y = -2 * rayon;
    int x     = 0;
    int y     = rayon;

    while(x<y)
    {
        if(f >= 0)
        {
            y--;
            ddF_y += 2;
            f     += ddF_y;
        }
        x++;
        ddF_x += 2;
        f     += ddF_x;

        if (cornername & 0x1)
        {
            OLED_DrawFastVLine(x0+x, y0-y, 2*y+1+delta,color);
            OLED_DrawFastVLine(x0+y, y0-x, 2*x+1+delta,color);
        }
        if (cornername & 0x2)
        {
            OLED_DrawFastVLine(x0-x, y0-y, 2*y+1+delta,color);
            OLED_DrawFastVLine(x0-y, y0-x, 2*x+1+delta,color);
        }
    }
}

void OLED_FillCircle(unsigned char x0,unsigned char y0,unsigned char rayon,unsigned int color)
{
    OLED_DrawFastVLine(x0, y0-rayon, 2*rayon+1, color);
    OLED_FillCircleHelper(x0, y0, rayon, 3, 0, color);
}

void OLED_RoundRect(unsigned char x,unsigned char y,unsigned char w,unsigned char h,unsigned char r,unsigned int color)
{
    OLED_DrawFastHLine(x + r, y, w - 2 * r, color); // Top
    OLED_DrawFastHLine(x + r, y + h - 1, w - 2 * r, color); // Bottom
    OLED_DrawFastVLine(x, y + r , h - 2 * r, color); // Left
    OLED_DrawFastVLine(x + w - 1, y + r, h - 2 * r, color); // Right
    // draw four corners
    OLED_CircleHelper(x + r, y + r, r, 1, color);
    OLED_CircleHelper(x + w - r - 1, y + r, r, 2, color);
    OLED_CircleHelper(x + w - r - 1, y + h - r - 1, r, 4, color);
    OLED_CircleHelper(x + r, y + h - r - 1, r, 8, color);
}

void OLED_FillRoundRect(unsigned char x,unsigned char y,unsigned char w,unsigned char h,unsigned char r,unsigned int color)
{
    OLED_RawFillRectangle(x + r, y, w - 2 * r, h, color);

    // draw four corners
    OLED_FillCircleHelper(x + w - r - 1, y + r, r, 1, h - 2 * r - 1, color);
    OLED_FillCircleHelper(x + r, y + r, r, 2, h - 2 * r - 	1, color);
}

void OLED_DrawBitmap(const unsigned int *bitmap)
{
    int i,j;
    int c = 0;

    for(i=0 ; i<SSD1351_HEIGHT ; i++)
    {
        for(j=0 ; j<SSD1351_WIDTH ; j++)
        {
            OLED_DrawPixel(i,j,bitmap[c]);
            c++;
        }
    }
}*/


/*************************************************************************************************
* Fonction :    Draw Icon
* Entr�e :      La position x et y de l'icone (en haut � gauche) ainsi que le tableau de l'ic�ne
* Sortie :      Rien
* Description :	Permet d'afficher un ic�ne pr�alablement d�fini dans un tableau constant (declarations.h)
**************************************************************************************************/
/*void OLED_DrawIcon(unsigned char x, unsigned char y, const unsigned int *iconTAB)
{
    unsigned char i,x0 = 0;
    x0=x;
    for(i=0 ; iconTAB[i] != '\t'; i++)  // On incr�mente le tableau jusqu'� ce que le caract�re de fin de tableau soit d�tect�
    {
        x++;    // On incr�mente la valeur de x sur l'�crran OLED
        if(iconTAB[i] == '\n')      // Si le caract�re de fin de ligne est d�tect�...
        {
            y++;    // On incr�mente de un l'axe des ordonn�s
            x=x0;   // On reposition le curseur en x, � l'origine
        }
        else
        {
            OLED_DrawPixel(x,y,iconTAB[i]); // Si le caract�re de fin de ligne n'est pas d�tect�, alors on allume le pixel correspondant
        }
    }
}

// Draw a character
void OLED_DrawChar(unsigned char x, unsigned char y, unsigned char character, unsigned int color)
{
    int a,i,j;
    unsigned char x0 = x;
    //unsigned char y0 = y;
    switch(character)
    {
        case '0':   i=0;j=35;
            break;
        case '1':   i=34;j=70;
            break;
        case '2':   i=69;j=105;
            break;
        case '3':   i=104;j=140;
            break;
        case '4':   i=139;j=175;
            break;
        case '5':   i=174;j=210;
            break;
        case '6':   i=209;j=245;
            break;
        case '7':   i=244;j=280;
            break;
        case '8':   i=279;j=315;
            break;
        case '9':   i=314;j=350;
            break;
        case '.':   i=351;j=385;
            break;
        default:
            break;
    }

    for(a=i;a<j;a++)
    {
        x++;
        if(a%5 == 0)
        {
            y--;
            x=x0;
        }
        else if(character[a] == 0)
        {
            OLED_DrawPixel(x,y,color);
        }
    }
}

// Draw a String
void OLED_DrawString(unsigned char x, unsigned char y, unsigned char *string, unsigned int color)
{
    int i = 0;

    for(i=0 ; string[i]!='\0' ; i++)
    {
        OLED_DrawChar(x,y,string[i],color);
        x += 6;
    }
}*/

