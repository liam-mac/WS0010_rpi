/*
 * File:   WS0010 driver.h
 * Author: Nicolas Lopez Fernandez
 * Modified By: Liam M
 *
 * Created on 3/6/2013, 15:11
 * Modified on 24/7/2017
 */

#include "WS0010_rpi.h"
//Pin 22 is DB0 and Pin 7 is DB7
const int DATA_PORT[] = {RPI_V2_GPIO_P1_22,
			RPI_V2_GPIO_P1_18,
			RPI_V2_GPIO_P1_16,
			RPI_V2_GPIO_P1_15,
			RPI_V2_GPIO_P1_13,
			RPI_V2_GPIO_P1_12,
			RPI_V2_GPIO_P1_11,
			RPI_V2_GPIO_P1_07};

/*
 * Read Busy Flag
 * if return = 1, WS0010 is busy
 */
uint8_t WS0010_ReadBusyFlag(void)
{
    uint8_t retval = 0;
    CLRBIT(DATA_PORT[7]);
    MAKEIN(DATA_PORT[7]);
    CLRBIT(RS_BIT);
    SETBIT(RWB_BIT);
    WS0010_delay_ms(1);
    SETBIT(EN_BIT);
    WS0010_delay_ms(1);
    retval = GETBIT(DATA_PORT[7]);
    MAKEOUT(DATA_PORT[7]);
    CLRBIT(EN_BIT);
    CLRBIT(RWB_BIT);
    return retval;
}

/*
 * Send byte to LCD
 * RS = 0 & RWB = 0 : instruction
 * RS = 1 & RWB = 0 : write to CGRAM or DDRAM
 */
uint8_t WS0010_SendByte(uint8_t mode, uint8_t rs, uint8_t rwb, uint8_t data)
{
    uint8_t retval = 0;
    int i;

    if (rwb == 0)
    {
        CLRBIT(RWB_BIT);

        if(rs == 0)
            CLRBIT(RS_BIT);
        else
            SETBIT(RS_BIT);

    for(i=0; i<8; i++){
        if(0x01 & (data >> i)){
		SETBIT(DATA_PORT[i]);
	}
	else if(~(0x01 & (data >> i))){
		CLRBIT(DATA_PORT[i]);
	}
	else{
		//printf("Error setting data\n");
	}
    }

        SETBIT(EN_BIT);
        WS0010_delay_ms(2);
        CLRBIT(EN_BIT);
        CLRBIT(RS_BIT);
    }
    else
        retval = 1;

    return retval;
}

/*
 * Sets the param to a 8 bit mode 2x16 character LCD
 * West European I font, cursor ON, blink ON
 * cursor increment & display shift OFF
 */
void WS0010_InitParam(WS0010_Param *pParam)
{
    pParam->mode = MODE_8_BIT;
    pParam->line = _2L_5x8C;
    pParam->font = WEST_EU_I;

    pParam->display = ON;
    pParam->cursor = ON;
    pParam->cursor_blink = ON;

    pParam->cursor_dir = INCR;
    pParam->display_shift = RIGHT;
}

void WS0010_InitLCD(WS0010_Param *pParam)
{
    int i;
    bcm2835_init();
    /* Wait for power stabilization */
    WS0010_delay_ms(25);

     CLRBIT(RS_BIT);
     MAKEOUT(RS_BIT);
//bcm2835_gpio_fsel(21, BCM2835_GPIO_FSEL_OUTP);
    MAKEOUT(RWB_BIT);
    CLRBIT(RWB_BIT);
    MAKEOUT(EN_BIT);
    CLRBIT(EN_BIT);
    for(i=0; i<8; i++){
        MAKEOUT(DATA_PORT[i]);
	CLRBIT(DATA_PORT[i]);
    }



    /* Function Set */
    WS0010_SendByte(pParam->mode, 0, 0, FUNCTION_SET | (pParam->mode << 4) |
                    ((pParam->line == _2L_5x8C) << 3) | (pParam->font & 0x03));
    WS0010_delay_ms(2);
    /* Wait Busy Flag */
    while (WS0010_ReadBusyFlag());
    WS0010_delay_ms(2);
    /* Display ON/OFF control */
    WS0010_SendByte(pParam->mode, 0, 0, DISPLAY_CONTROL | (pParam->display << 2) |
                    (pParam->cursor << 1) | (pParam->cursor_blink));

    WS0010_delay_ms(2);
    /* Wait Busy Flag */
    while (WS0010_ReadBusyFlag());
    WS0010_delay_ms(2);

    /* Display Clear */
    WS0010_SendByte(pParam->mode, 0, 0, CLEAR_DISPLAY);

    WS0010_delay_ms(2);
    /* Wait Busy Flag */
    while (WS0010_ReadBusyFlag());
    WS0010_delay_ms(2);

    /* Return Home */
    WS0010_SendByte(pParam->mode, 0, 0, RETURN_HOME);

    WS0010_delay_ms(2);
    /* Wait Busy Flag */
    while (WS0010_ReadBusyFlag());
    WS0010_delay_ms(2);

    /* Entry Mode Set */
    WS0010_SendByte(pParam->mode, 0, 0, ENTRY_MODE_SET | (pParam->cursor_dir << 1) | pParam->display_shift);

    WS0010_delay_ms(2);
    /* Wait Busy Flag */
    while (WS0010_ReadBusyFlag());
    WS0010_delay_ms(2);

    /* Initialization End */
}

/*
 * Clear Screen & Return Home
 */
void WS0010_ClearScreen(WS0010_Param *pParam)
{
    /* Display Clear */
    WS0010_SendByte(pParam->mode, 0, 0, CLEAR_DISPLAY);

    WS0010_delay_ms(2);
    /* Wait Busy Flag */
    while (WS0010_ReadBusyFlag());
    WS0010_delay_ms(2);

    /* Return Home */
    WS0010_SendByte(pParam->mode, 0, 0, RETURN_HOME);

    WS0010_delay_ms(2);
    /* Wait Busy Flag */
    while (WS0010_ReadBusyFlag());
    WS0010_delay_ms(2);
}

/*
 * Write char to LCD
 */
void WS0010_WriteCharToLCD(WS0010_Param *pParam, const char character)
{
    WS0010_SendByte(pParam->mode, 1, 0, character);
    while (WS0010_ReadBusyFlag());
    WS0010_delay_ms(1);
}
/*
 * Write string to LCD
 */
void WS0010_WriteToLCD(WS0010_Param *pParam, const char *pbuffer, uint8_t size)
{
    uint8_t i;

    for (i = 0; (i < size) && (pbuffer[i] != 0x00); i++)
    {
        WS0010_WriteCharToLCD(pParam, pbuffer[i]);
    }
}

/*
 * Set X-Y coordonates
 */
uint8_t WS0010_SetXY(WS0010_Param *pParam, uint8_t x, uint8_t y)
{
    uint8_t retval = 0;
    uint8_t address = 0;

    if (x < LINE_SIZE && y < MAX_LINES)
        /* 
         * Calculate DDRAM address
         * 0x40 is first char of second line
         */
        address = x + (y * 0x40);
    else
        retval = 1;

    /* Set DDRAM address */
    if (!retval)
    {
        WS0010_SendByte(pParam->mode, 0, 0, SET_DDRAM | (address & 0x7F));
        WS0010_delay_ms(1);
    }
    
    return retval;
}

uint8_t WS0010_LineOne(WS0010_Param *pParam)
{
    /* Set DDRAM address */
        WS0010_SendByte(pParam->mode, 0, 0, SET_DDRAM | 0x00);
        WS0010_delay_ms(1);
}

uint8_t WS0010_LineTwo(WS0010_Param *pParam)
{
    if(MAX_LINES >= 2){
	/* Set DDRAM address */
	WS0010_SendByte(pParam->mode, 0, 0, SET_DDRAM | 0x40);
        WS0010_delay_ms(1);
   } 
}
