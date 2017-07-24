#include <stdio.h>
#include <stdint.h>
#include "WS0010.h"

#pragma config FOSC=HS, WDTE=OFF,BOREN=OFF, LVP=OFF, CDP=OFF, WRT=OFF, DEBUG=OFF, CP=OFF

void main(void)
{
	WS0010_Param param;
	WS0010_InitParam(&param);
	param.cursor=ON;
	param.cursor_blink=ON;
	
	WS0010_InitLCD(&param);
	WS0010_WriteToLCD(&param, "Hello", 5); 
	WS0010_LineTwo(&param);
	WS0010_WriteToLCD(&param, "World ", 5);
	while(1);
}
