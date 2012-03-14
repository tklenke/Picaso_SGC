/*
	picaso_sgc.cpp - Arduino Library for 4Dsystems.com.au SGC derived products including uVGA-II(SGC)
	Copyright(c) February 2012  Tom Klenke
	http://github.com/tklenke
	
	Derived from:
	displayshield4d.h - Arduino Library for 4Display-Shield by 4d Systems
	Copyright(c) December 2010 Oscar Gonzalez - www.BricoGeek.com

	http://code.google.com/p/displayshield4d/

	Licensed under GNU General Public License v3 
	http://creativecommons.org/licenses/by/3.0/
	
	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
	IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
	FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
	AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
	LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
	OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
	THE SOFTWARE.

*/

#include "picaso_sgc.h"
#include <SoftwareSerial.h>
#include <stdio.h>
#include <string.h>
#include <inttypes.h>
#include "Arduino.h"

	#define PIC_INITBAUDRATE	9600
	#define PIC_INITDELAYMS		3000

	#define	PIC_AUTOBAUD					0x55
	#define PIC_SET_BAUD_RATE				0x51
	#define PIC_GET_DEVICE_INFO				0x56	
	
	#define PIC_REPLACE_BACKGROUND_COLOR	0x42
	
	#define	PIC_CLEAR_SCREEN				0x45

	#define	PIC_DISPLAY_CONTROL				0x59
//		#define	PIC_DISPLAY_MODE_BACKLIGHT		0x00
//		#define PIC_DISPLAY_MODE_ON_OFF			0x01
//		#define PIC_DISPLAY_MODE_CONTRAST		0x02
//		#define PIC_DISPLAY_MODE_POWERUP_SHUTDOWN	0x03
		#define PIC_DISPLAY_MODE_ORIENTATION		0x04
		#define PIC_DISPLAY_MODE_IMAGE_FORMAT		0x06
//		#define PIC_DISPLAY_MODE_PROTECT_FAT		0x08
//		#define PIC_DISPLAY_MODE_PAGE_DISPLAY		0x09
//		#define PIC_DISPLAY_MODE_PAGE_READ		0x0A
//		#define PIC_DISPLAY_MODE_PAGE_WRITE		0x0B
		#define PIC_DISPLAY_MODE_RESOLUTION		0x0C
		#define PIC_DISPLAY_MODE_SCREEN_SAVER_DELAY	0x0D
		#define PIC_DISPLAY_MODE_SCREEN_SAVER_NEXT	0x0E

	#define PIC_SET_VOLUME					0x76
	#define PIC_SLEEP					0x5A
		//options needed
		
	#define PIC_READ_GPIO_PIN				0x69
	
	#define PIC_WRITE_GPIO_PIN				0x79
	
	#define PIC_READ_GPIO_BUS				0x61
	
	#define PIC_WRITE_GPIO_BUS				0x57
	

	
	#define PIC_SCREENCOPY		        0x63

	#define PIC_ACK			0x06
	#define PIC_NAK 		0x15
	#define PIC_STRINGTERMINATOR	(uint8_t)0x00
	#define PIC_NULL		(uint8_t)0x00
	
	// Graphics
	#define PIC_ADD_BITMAP_CHARACTER		0x41
	#define PIC_DRAW_BITMAP_CHARACTER		0x44
	#define PIC_DRAW_CIRCLE				0x43
	#define PIC_DRAW_TRIANGLE			0x47
	#define PIC_DRAW_ICON				0x49
	#define PIC_SET_BACKGROUND_COLOR		0x4B
	#define PIC_DRAW_LINE				0x4C
	#define PIC_DRAW_POLYGON			0x67
	#define PIC_DRAW_RECTANGLE			0x72
	#define PIC_DRAW_ELLIPSE			0x65
	#define PIC_DRAW_PIXEL				0x50
	#define PIC_READ_PIXEL				0x52
	#define PIC_SCREEN_COPY_PASTE			0x63
	#define PIC_REPLACE_COLOR			0x6B
	#define PIC_SET_PEN_SIZE			0x70

	// Text
	#define	PIC_SET_FONT				0x46	
	#define	PIC_SET_TRANSPARENT_OPAQUE_TEXT		0x4F
	#define PIC_DRAW_CHAR_TEXT_FORMAT		0x54	
	#define PIC_DRAW_CHAR_GRAPHICS_FORMAT	        0x74
	#define PIC_DRAW_STRING_TEXT_FORMAT		0x73
	#define PIC_DRAW_STRING_GRAPHICS_FORMAT	        0x53
	#define PIC_DRAW_TEXT_BUTTON			0x62	
		
//SoftwareSerial sSerial(PIC_RXPIN,PIC_TXPIN);

/*******************************************************
	Constructor
 ********************************************************/
PicasoSGC::PicasoSGC() {
	// Constructor
}

/*******************************************************
	Function: 	
		Init
	Description:	
		Initialize pins, serial port and display
	Params:	None	
	Return:	1 if succesful or 0 if not
 ********************************************************/
uint8_t PicasoSGC::Init(uint8_t rxPin, uint8_t txPin, uint8_t resetPin)
{
        PicasoSGC::resetPin = resetPin;
	//set the pins
	pinMode(rxPin, INPUT);
	pinMode(txPin, OUTPUT);
	pinMode(resetPin, OUTPUT);
	
	//set up the port and reset the card
        sSerial = new SoftwareSerial(rxPin, txPin);
	sSerial->begin(PIC_INITBAUDRATE);
	Reset();  
	delay(PIC_INITDELAYMS);
	
	//check for valid card 
	sSerial->write(PIC_AUTOBAUD); 
	delay(10);
	if (!ValidAck())
          return 0;

       //set Default Pen to Filled
        if (!SetPenSize(PIC_PEN_SOLID))
          return 0;

       //set Default Font to PIC_FONT_MON0_5X7
       if (!SetFont(PIC_FONT_MON0_5X7))
         return 0;
  
       //set Default Text to Transparent
       if (!SetTransparentOpaqueText(PIC_TEXT_TRANSPARENT))
         return 0;
         
       return 1;
}


/*******************************************************
	Function:	
		Reset 
	Description:
		Phisicaly reset the display. D7 jumper must be conected
		Also is a good thing to have pinMode(7, OUTPUT) set in Setup function
	Params:	None
	Return: No return
 ********************************************************/
uint8_t PicasoSGC::Reset() {
	digitalWrite(resetPin, LOW);
	delay(10);                  
	digitalWrite(resetPin, HIGH);
	delay(10); 
}

/*******************************************************
	Function: 	
		SetBaud
	Description:	
		Sets baud rate between arduino and card
	Params:	Use define to set baud rates	
	Return:	1 if succesful or 0 if not
 ********************************************************/
uint8_t PicasoSGC::SetBaud(uint8_t rate) {
       long baud;
  
	switch (rate)
	{
		case PIC_300_BAUD : { baud = 300; }; break;
		case PIC_1200_BAUD: { baud = 1200; }; break;
		case PIC_2400_BAUD: { baud = 2400; }; break;
		case PIC_4800_BAUD: { baud = 4800; }; break;
		case PIC_9600_BAUD: { baud = 9600; }; break;
		case PIC_14400_BAUD: { baud = 14400; }; break;
		case PIC_19200_BAUD: { baud = 19200; }; break;
		case PIC_31250_BAUD: { baud = 31250; }; break;
		case PIC_38400_BAUD: { baud = 38400; }; break;
		case PIC_57600_BAUD: { baud = 57600; }; break;
//                case PIC_115200_BAUD: { baud = 115200; }; break;
		baud = 9600;
	}  
     
	sSerial->write(PIC_SET_BAUD_RATE);
	sSerial->write(rate);

        sSerial->begin(baud);
	return ValidAck();
}

void PicasoSGC::GetDeviceInfo()
{
	sSerial->write(PIC_GET_DEVICE_INFO);
	sSerial->write(PIC_NULL); //output to serial only

	byte incomingByte;
	char counter=0;
	while (!sSerial->available()) { delayMicroseconds(150); }
	
	do 
	{
		incomingByte = sSerial->read();

		switch (counter)
		{
		  case 0: { device_type 		= incomingByte; }; break;
  		  case 1: { device_hardware_rev 	= incomingByte; }; break;
		  case 2: { device_firmware_rev 	= incomingByte; }; break;
		  case 3: { device_width 		= incomingByte; }; break;
		  case 4: { device_height 	= incomingByte; }; break;
		}

		counter++;
	} while (sSerial->available());		
	
}

uint8_t PicasoSGC::ReplaceBackgroundColor(uint8_t red, uint8_t green, uint8_t blue) {
	sSerial->write(PIC_REPLACE_BACKGROUND_COLOR);
	WriteColor(red, green, blue);

	return ValidAck();
}

/*******************************************************
	Function: 
		Clear
	Description:
		Clear entire display
	Params:	None
	Return: 0 if succesful or 1 if not
 ********************************************************/
uint8_t PicasoSGC::ClearScreen() {
	sSerial->write(PIC_CLEAR_SCREEN);
	return ValidAck();
}

uint8_t PicasoSGC::SetOrientation(uint8_t orientation){
  sSerial->write(PIC_DISPLAY_CONTROL);
  sSerial->write(PIC_DISPLAY_MODE_ORIENTATION);
  sSerial->write(orientation);
  
  return ValidAck();
}

uint8_t PicasoSGC::SetResolution(uint8_t resolution){
  sSerial->write(PIC_DISPLAY_CONTROL);
  sSerial->write(PIC_DISPLAY_MODE_RESOLUTION);
  sSerial->write(resolution);
  
  return ValidAck();
}

uint8_t PicasoSGC::SetVolume(uint8_t volume) {
	sSerial->write(PIC_SET_VOLUME);
	sSerial->write(volume);
	
	return ValidAck();
}

uint8_t PicasoSGC::Sleep(uint8_t sleep_option, uint8_t sleep_time) {
	sSerial->write(PIC_SLEEP);
	sSerial->write(sleep_option);
	sSerial->write(sleep_time);
	
	return ValidAck();
}

uint8_t PicasoSGC::ReadGPIOPin(uint8_t pin) {
	sSerial->write(PIC_READ_GPIO_PIN);
	sSerial->write(pin);
	
	return GetReply();
}

uint8_t PicasoSGC::WriteGPIOPin(uint8_t pin, bool val) {
	sSerial->write(PIC_WRITE_GPIO_PIN);
	sSerial->write(pin);
	sSerial->write(val);
	
	return ValidAck();
}

///////////////////////////////////////////////////////////
//  GRAPHICS COMMANDS
///////////////////////////////////////////////////////////

/*******************************************************
	Function: 
		SetPenSize
	Description:
		This  command  determines  if  certain graphics  objects  are drawn in solid or  wire frame fashion.
	Params:	0 for solid or 1 for wireframe
	Return: 0 if succesful or 1 if not
 ********************************************************/
uint8_t PicasoSGC::SetPenSize(uint8_t val)
{
	sSerial->write(PIC_SET_PEN_SIZE);
	sSerial->write(val);

	return ValidAck();
}

/*******************************************************
	Function: 
		DrawLine
	Description:
	Params:
		x1, y1, x2, y2 and RGB color
		As allways, take care of your screen size!
	Return: 0 if succesful or 1 if not
 ********************************************************/
uint8_t PicasoSGC::DrawLine(unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2, uint8_t red, uint8_t green, uint8_t blue)
{
	sSerial->write(PIC_DRAW_LINE);

	sSerial->write(x1 >> 8);			// MSB
	sSerial->write(x1 & 0xFF);		// LSB	

	sSerial->write(y1 >> 8);			// MSB
	sSerial->write(y1 & 0xFF);		// LSB	

	sSerial->write(x2 >> 8);			// MSB
	sSerial->write(x2 & 0xFF);		// LSB	

	sSerial->write(y2 >> 8);			// MSB
	sSerial->write(y2 & 0xFF);		// LSB	

	WriteColor(red, green, blue);

	return ValidAck();
}

///////////////////////////////////////////////////////////
//  TEXT COMMANDS
///////////////////////////////////////////////////////////

/*******************************************************
	Function: 
		SetFont
	Description:
		This  command selects  one of  the available internal  fonts.
	Params:
		font_type: PIC_FONT5x7, PIC_FONT8x8 or PIC_FONT8x12
	Return:	
		Return 0 if successful or 1 if not
 ********************************************************/
uint8_t PicasoSGC::SetFont(uint8_t font_type)
{
        sSerial->write(PIC_SET_FONT); 
	sSerial->write(font_type);

        return ValidAck();
}

/*******************************************************
	Function: 
		SetTransparentOpaqueText
	Description:
		This command will  change the attribute of the text so that an object behind the text can either be blocked or transparent.
	Params:
		font_mode: PIC_TEXT_TRANSPARENT or PIC_TEXT_OPAQUE
	Return:	
		0 if succesful or 1 if not
 ********************************************************/
uint8_t PicasoSGC::SetTransparentOpaqueText(uint8_t text_mode)
{
        sSerial->write(PIC_SET_TRANSPARENT_OPAQUE_TEXT); 
	sSerial->write(text_mode);

        return ValidAck();
}

/*******************************************************
	Function: 
		DrawStringAtRowCol
	Description:
		This command will  change the attribute of the text so that an object behind the text can either be blocked or transparent.
	Params:
		Row Column
	Return:	
		0 if succesful or 1 if not
 ********************************************************/
uint8_t PicasoSGC::DrawStringAtRowCol(uint8_t row, uint8_t column, uint8_t font, uint8_t red, uint8_t green, uint8_t blue, char *text)
{
        int i = 0;
        int len;
	
	sSerial->write(PIC_DRAW_STRING_TEXT_FORMAT);

	sSerial->write(column);
	sSerial->write(row);
	sSerial->write(font);
        WriteColor(red, green, blue);
        
        len = strlen(text);
        if (len > 256)
          len = 256;
          
        for (i = 0; i < len; i++)
          sSerial->write(text[i]);

	sSerial->write(PIC_STRINGTERMINATOR); // String terminator

	return ValidAck();
}

/*******************************************************
	Function: 
		DrawCharAtRowCol
	Description:
		This command will  change the attribute of the text so that an object behind the text can either be blocked or transparent.
	Params:
		Row Column
	Return:	
		0 if succesful or 1 if not
 ********************************************************/
uint8_t PicasoSGC::DrawCharAtRowCol(uint8_t row, uint8_t column, uint8_t font, uint8_t red, uint8_t green, uint8_t blue, char cChar)
{
	SetFont(font);

	sSerial->write(PIC_DRAW_CHAR_TEXT_FORMAT);

	sSerial->write(cChar);
	sSerial->write(column);
	sSerial->write(row);
	sSerial->write(font);
        WriteColor(red, green, blue);
        

	return ValidAck();
}

///////////////////////////////////////////////////////////
//  INTERNAL UTILITY FUNCTIONS
///////////////////////////////////////////////////////////

/*******************************************************
	Function: 
		GetReply
	Description:
		Used to get command response
	Params:	None
	Return: Return PIC_ACK is done or PIC_NAK if not
 ********************************************************/
uint8_t PicasoSGC::GetReply() {

	byte incomingByte = PIC_ACK;
	while (!sSerial->available()) { delayMicroseconds(150); }
	incomingByte = sSerial->read();
	return incomingByte;
}

/*******************************************************
	Function: 
		ValidAck
	Description:
		Used to get command response and evaluate if ack or no ack
	Params:	None
	Return: 1 = acknowledged  0 = not acknowledged
 ********************************************************/
uint8_t PicasoSGC::ValidAck() {

	byte incomingByte = PIC_ACK;
	while (!sSerial->available()) { delayMicroseconds(150); }
	incomingByte = sSerial->read();
	if (PIC_ACK == incomingByte)
		return true;  //no error
	else
		return false;   //error
}

/*******************************************************
	Function: 
		WriteColor
	Description:
		Takes RGB value and writes to serial in 2byte format
	Params:	red, green, blue - From 0 to 254
	Return:	none
 ********************************************************/
void PicasoSGC::WriteColor(uint8_t red, uint8_t green, uint8_t blue) 
{
	unsigned int color;

	int outR = ((red * 31) / 255);
	int outG = ((green * 63) / 255);
	int outB = ((blue * 31) / 255);

	color = (outR << 11) | (outG << 5) | outB;
	
	sSerial->write(color >> 8);			// MSB
	sSerial->write(color & 0xFF);			// LSB	
}

/*******************************************************
	Function: 
		RGB
	Description:
		Returns correct int format for color parm
	Params:	red, green, blue - From 0 to 254
	Return:	2byte color
 ********************************************************/
unsigned int PicasoSGC::RGB(uint8_t red, uint8_t green, uint8_t blue) 
{

	char text[255];

	int outR = ((red * 31) / 255);
	int outG = ((green * 63) / 255);
	int outB = ((blue * 31) / 255);

	return (outR << 11) | (outG << 5) | outB;

}








/*******************************************************
	Function: 
		SetBackground
	Description:
		changes the current background colour. Once this command is sent, only the background colour will change.

	Params:	color
	Return:	Return PIC_ACK is done or PIC_NAK if not
 ********************************************************
uint8_t PicasoSGC::SetBackground(unsigned int color)
{
	sSerial->write(PIC_SETBACKGROUND);

	// Color
	sSerial->write(color >> 8);				// MSB
	sSerial->write(color & 0xFF);			// LSB
        GetReply();

        sSerial->write(0x43);
        sSerial->write((uint8_t)0);
        sSerial->write(0x3F);
        sSerial->write((uint8_t)0);
        sSerial->write(0x3F);
        sSerial->write((uint8_t)0);
        sSerial->write(0x22);
        sSerial->write((uint8_t)0);
        sSerial->write(0x1F);

 
	return GetReply();


}

/*******************************************************
	Function: 
		SetContrast
	Description:
		Set current contrast

	Params:	cval - 0 to 15
	Return:	Return PIC_ACK is done or PIC_NAK if not
 ********************************************************
uint8_t PicasoSGC::SetContrast(char val)
{
	sSerial->write(PIC_COMMAND_CONTROL);

	// Set contrast
	sSerial->write(val); // 0-15

	return GetReply();
}

/*******************************************************
	Function: 
		SetState
	Description:
		This command changes some of the display settings

	Params:	state - Can be COMMAND_DISPLAY_ON, COMMAND_DISPLAY_OFF, PIC_COMMAND_SHUTDOWN, PIC_COMMAND_POWEROFF (for low power save state)
	Return:	Return PIC_ACK is done or PIC_NAK if not
 ********************************************************
uint8_t PicasoSGC::SetState(char state)
{
	sSerial->write(PIC_COMMAND_CONTROL);

        sSerial->write(0x0C);
        sSerial->write(0x02);
	// Set contrast
	//sSerial->write(state);
        Serial.println(GetReply());
	sSerial->write(PIC_COMMAND_CONTROL);

        sSerial->write(0x04);
        sSerial->write(0x03);
        Serial.println(GetReply());
	return 0;
}

/*******************************************************
	Function: 
		SetState
	Description:
		Puts GOLDELOX-SGC chip in to low power mode and optionally waits for certain conditions to wake it up

	Params:	wake_cond - Can be PIC_COMMAND_STOP_SD, PIC_COMMAND_WAKEONKOYSTICK or PIC_COMMAND_WAKEONsSerial
	Return:	Return PIC_ACK is done or PIC_NAK if not
 ********************************************************
uint8_t PicasoSGC::Sleep(char wake_cond)
{
	sSerial->write(PIC_COMMAND_SLEEP);

	// Set wakeup condition
	sSerial->write(wake_cond);

	return GetReply();
}

/*******************************************************
	Function: 
		ScreenCopy
	Description:
		This command copies a specified area of the screen as a bitmap block to another screen position

	Params:	source_x, source_y, dest_x, dest_y, width and height
	Return:	Return PIC_ACK is done or PIC_NAK if not
 ********************************************************
uint8_t PicasoSGC::ScreenCopy(uint8_t source_x, uint8_t source_y, uint8_t dest_x, uint8_t dest_y, uint8_t width, uint8_t height)
{

	sSerial->write(PIC_SCREENCOPY);

	sSerial->write(source_x);
	sSerial->write(source_y);
	sSerial->write(dest_x);
	sSerial->write(dest_y);
	sSerial->write(width);
	sSerial->write(height);

	return GetReply();

}

/*******************************************************
	Function: 
		putpixel
	Description:
		Basic function to draw one pixel
	Params:
		x, y and color. Please check your screen size in order to use this function!
	Return: Return PIC_ACK is done or PIC_NAK if not
 ********************************************************
uint8_t PicasoSGC::putpixel(uint8_t x, uint8_t y, unsigned int color)
{
	sSerial->write(PIC_PUTPIXEL);
	sSerial->write(x);
	sSerial->write(y);

	// Color
	sSerial->write(color >> 8);				// MSB			
	sSerial->write(color & 0xFF);				// LSB

    	return GetReply();
}

/*******************************************************
	Function: 
		readpixel
	Description:
		Basic function to get the color of one pixel
	Params:
	Return: Returns 16bits color info (MSB/LSB)
 ********************************************************
uint16_t PicasoSGC::readpixel(uint8_t x, uint8_t y)
{
	sSerial->write(PIC_READPIXEL);
	sSerial->write(x);
	sSerial->write(y);

	while (!sSerial->available()) { delayMicroseconds(150); } // Let's display take the time...

    	return sSerial->read() & sSerial->read();
}



/*******************************************************
	Function: 
		rectangle
	Description:
	Params:
		x, y, width, height, filled and color
		NOTE: filled must be 0 for solid or 1 for wireframe
		As allways, take care of your screen size!
	Return:	Return PIC_ACK is done or PIC_NAK if not
 ********************************************************
uint8_t PicasoSGC::rectangle(uint8_t x, uint8_t y, uint8_t width, uint8_t height, char filled, unsigned int color)
{

	SetPenSize(filled);

	sSerial->write(PIC_RECTANGLE);

	sSerial->write(x);						// X1
	sSerial->write(y);						// Y1
	sSerial->write(x+width);						// X2
	sSerial->write(y+height);						// Y2

	// Color
	sSerial->write(color >> 8);					// MSB			
	sSerial->write(color & 0xFF);					// LSB

	return GetReply();

}

/*******************************************************
	Function: 
		circle
	Description:
	Params:
		x, y, radius, filled and color
		NOTE: filled must be 0 for solid or 1 for wireframe
		As allways, take care of your screen size!
	Return:	Return PIC_ACK is done or PIC_NAK if not
 ********************************************************
uint8_t PicasoSGC::circle(uint8_t x, uint8_t y, uint8_t radius, uint8_t filled, unsigned int color)
{
	SetPenSize(filled);

        sSerial->write(PIC_CIRCLE); 

	sSerial->write(x);	// x
	sSerial->write(y);	// y
	sSerial->write(radius);	// radius

	// Color
	sSerial->write(color >> 8);		// MSB			
	sSerial->write(color & 0xFF);		// LSB

        return GetReply();

}

/*******************************************************
	Function: 
		triangle
	Description:
	Params:
		x and y of each vertex, filled and color
		NOTE: filled must be 0 for solid or 1 for wireframe
		The vertices must be specified in an anticlock wise
	Return:	Return PIC_ACK is done or PIC_NAK if not
 ********************************************************
uint8_t PicasoSGC::triangle(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, uint8_t x3, uint8_t y3, uint8_t filled, unsigned int color)
{

	SetPenSize(filled);

        sSerial->write(PIC_TRIANGLE); 

	sSerial->write(x1);	// x
	sSerial->write(y1);	// y
	sSerial->write(x2);	// x
	sSerial->write(y2);	// y
	sSerial->write(x3);	// x
	sSerial->write(y3);	// y

	// Color
	sSerial->write(color >> 8);		// MSB			
	sSerial->write(color & 0xFF);		// LSB

        return GetReply();

}






/*


char *PicasoSGC::GetDeviceType()
{
	switch (device_type)
	{
		case 0x00: { return "micro-PIC"; }; break;
		case 0x01: { return "micro-LCD"; }; break;
		case 0x02: { return "micro-VGA"; }; break;
	}
}

uint8_t PicasoSGC::GetDeviceWidth()
{
	return device_width;
}

uint8_t PicasoSGC::GetDeviceHeight()
{
	return device_height;
}
*/
