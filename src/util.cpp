
#include <SPI.h>

//-----------------------------------------------------------------------------

void msgPrint(String msg){
	Serial.print(msg);
}

//-----------------------------------------------------------------------------

void msgPrintln(String msg){
	Serial.println(msg);
}

//-----------------------------------------------------------------------------------------------

void exit(String reason){
	msgPrint("Exit Application - ");
	msgPrintln(reason);
	while(true);
}