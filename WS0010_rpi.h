/* 
 * File:   WS0010_rpi.h
 * Original Author: Nicolas
 * Modified By: Liam M
 *
 * Created on 3/6/2013, 13:19
 * Modified on 24/7/2017
 */

#ifndef WS0010_DRIVER_H
#define	WS0010_DRIVER_H

#ifdef	__cplusplus
extern "C" {
#endif

/* Includes */
//#include <xc.h>
#include <stdint.h>
#include <bcm2835.h>
#include <stdbool.h>

/* Defines */
#ifdef __PICC__
    /* Set XTAL FREQ for PIC delay functions */
    #define _XTAL_FREQ 4000000L
#endif

#define WS0010_delay_ms(var) bcm2835_delay(var) 
#define WS0010_delay_us(var) bcm2835_delayMicroseconds(var)

//#define DATA_PORT PORTB
extern const int DATA_PORT[];
#define RS_BIT RPI_V2_GPIO_P1_40 
#define RWB_BIT RPI_V2_GPIO_P1_36
#define EN_BIT RPI_V2_GPIO_P1_38

#define OUT BCM2835_GPIO_FSEL_OUTP
#define IN BCM2835_GPIO_FSEL_INPT

#define LINE_SIZE 8
#define MAX_LINES 2

/* Macros */
#define SETBIT(b) bcm2835_gpio_write(b, HIGH) 
#define CLRBIT(b) bcm2835_gpio_write(b, LOW) 
#define GETBIT(b) bcm2835_gpio_lev(b) 
#define MAKEIN(b) bcm2835_gpio_fsel(b, IN)
#define MAKEOUT(b) bcm2835_gpio_fsel(b, OUT)

/* Enumeration */
/*
 * Parallel Mode:
 * --------------
 * 4 bit
 * 8 bit
 */
enum PMODE {MODE_4_BIT, MODE_8_BIT};

/*
 * Cursor increment/decrement:
 * ---------------------------
 */
enum CURSOR {DECR, INCR};

/*
 * Line & Char size:
 * -----------
 * 1/8  : 1 line 5x8
 * 1/11 : 1 line 5x10
 * 1/16 : 2 lines 5x8
 */
enum LINE {_1L_5x8C, _1L_5x10C, _2L_5x8C};

/*
 * Character Generator ROM:
 * ------------------------
 * ENG_JAP    : English & Japanese character
 * WEST_EU_I  : Western European character-I
 * ENG_RUS    : English Russian
 * WEST_EU_II : Western European character-II
 */
enum CGROM {ENG_JAP, WEST_EU_I, ENG_RUS, WEST_EU_II};

/*
 * ON/OFF State:
 * -------------
 */
enum STATE {OFF, ON};

/*
 * Shift direction:
 * ----------------
 */
enum DIR {RIGHT, LEFT};

/*
 * WS0010 instructions:
 * --------------------
 * Clear display
 * Return Home
 * Entry Mode Set : Sets cursor move direction(bit 2) and
 *                  specifies display shift (bit 0)
 * Display Control : Sets entire Display (bit 2) ON/OFF.
 *                   Sets Cursor (bit 1) ON/OFF.
 *                   Sets Blinking (bit 0) of Cursor.
 * Cursor/Display Shift/Mode/Pwr : Moves cursor & shifts display.
 *                                 without changing DDRAM contents.
 *                                 Sets Graphic/Character Mode.
 *                                 Sets internal power on/off.
 * Function Set : Sets interface data length (DL).
 *                Sets number of display lines (N).
 *                Sets Character Font (F).
 *                Sets Font Table (FT).
 * Set CGRAM : Sets CGRAM Address. CGRAM data is sent and received
 *             after this setting.
 * Set DDRAM : Sets DDRAM Address. The DDRAM data Is sent and
 *             received after this setting.
 */
enum INSTR {CLEAR_DISPLAY = 0x01, RETURN_HOME = 0x02, ENTRY_MODE_SET = 0x04,
            DISPLAY_CONTROL = 0x08, CDMP_PARAM = 0x10, FUNCTION_SET = 0x20,
            SET_CGRAM = 0x40, SET_DDRAM = 0x80};

/* Structures */
typedef struct{
    enum PMODE mode;
    enum LINE line;
    enum CGROM font;

    enum STATE display;
    enum STATE cursor;
    enum STATE cursor_blink;

    enum CURSOR cursor_dir;
    enum DIR display_shift;
}WS0010_Param;

/* Functions */
uint8_t WS0010_ReadBusyFlag(void);
uint8_t WS0010_SendByte(uint8_t mode, uint8_t rs, uint8_t rwb, uint8_t data);
void WS0010_InitLCD(WS0010_Param *pParam);
void WS0010_InitParam(WS0010_Param *pParam);
void WS0010_ClearScreen(WS0010_Param *pParam);
void WS0010_WriteCharToLCD(WS0010_Param *pParam, const char character);
void WS0010_WriteToLCD(WS0010_Param *pParam, const char *pbuffer, uint8_t size);
uint8_t WS0010_SetXY(WS0010_Param *pParam, uint8_t x, uint8_t y);
uint8_t WS0010_LineOne(WS0010_Param *pParam);
uint8_t WS0010_LineTwo(WS0010_Param *pParam);

#ifdef	__cplusplus
}
#endif

#endif	/* WS0010_DRIVER_H */

