/*
	picaso_sgc.h - Arduino Library for 4Dsystems.com.au SGC derived products including uVGA-II(SGC)
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

#ifndef _PICASO_SGC_H_
	#define _PICASO_SGC_H_

	#include <inttypes.h>
        #include <SoftwareSerial.h>
	//Use with SetBaud(rate)
	// Supported baud rates are 300, 1200, 2400, 4800, 9600, 14400, 19200, 28800, 31250, 38400, 57600, and 115200. (softwareserial.h)
		#define PIC_300_BAUD 		0x01
		#define PIC_1200_BAUD 		0x03
		#define PIC_2400_BAUD 		0x04
		#define PIC_4800_BAUD 		0x05
		#define PIC_9600_BAUD 		0x06
		#define PIC_14400_BAUD 		0x07
		#define PIC_19200_BAUD 		0x08
		#define PIC_31250_BAUD 		0x09		
		#define PIC_38400_BAUD 		0x0A
		#define PIC_57600_BAUD 		0x0C
//		#define PIC_115200_BAUD		0x0D
	
	//use with SetOrientation(orientation) 
		#define PIC_DISPLAY_MODE_ORIENTATION_90DEG 	0x01 
		#define PIC_DISPLAY_MODE_ORIENTATION_270DEG     0x02
		#define PIC_DISPLAY_MODE_ORIENTATION_NATIVE     0x03
		#define PIC_DISPLAY_MODE_ORIENTATION_180DEG     0x04

	//Use with SetResolution(resolution)	
		#define PIC_DISPLAY_MODE_RESOLUTION_320_240	(uint8_t)0x00
		#define PIC_DISPLAY_MODE_RESOLUTION_640_480	0x01
		#define PIC_DISPLAY_MODE_RESOLUTION_800_480	0x02

	//Use with SetVolume(volume)	
		#define PIC_SET_VOLUME_MIN		0x08
		#define PIC_SET_VOLUME_MAX		0x7F
		#define PIC_SET_VOLUME_MUTE_ON		(uint8_t)0x00
		#define PIC_SET_VOLUME_MUTE_OFF		0xFF
		#define PIC_SET_VOLUME_UP_8		0xFE
		#define PIC_SET_VOLUME_DOWN_8		0x01
		
	//Use with Sleep(sleep_option, sleep_time)
		#define PIC_SLEEP_P1_WAKE		0x08
		#define PIC_SLEEP_P0_WAKE		0x04
		#define PIC_SLEEP_SERIAL_WAKE	        0x01
		
		#define	PIC_FONT_MON0_5X7		(uint8_t)0x00
		#define	PIC_FONT_MON0_8X8		0x01
		#define	PIC_FONT_MON0_8X12		0x02
                #define	PIC_FONT_MON0_12X16		0x03
		#define	PIC_FONT_PROPORTIONAL_5X7	0x10
		#define	PIC_FONT_PROPORTIONAL_8X8	0x11
		#define	PIC_FONT_PROPORTIONAL_8X12	0x12
                #define	PIC_FONT_PROPORTIONAL_12X16	0x13
                
        //Use with SetPen                
	        #define PIC_PEN_SOLID			(uint8_t)0x00
	        #define PIC_PEN_WIRE_FRAME		0x01	

        //Use with SetTeransparentOpaqueText
		#define	PIC_TEXT_TRANSPARENT	        (uint8_t)0x00
		#define	PIC_TEXT_OPAQUE			0x01

	// Class definition
	class PicasoSGC 
	{
		public:
			PicasoSGC();

			uint8_t Init(uint8_t rxPin, uint8_t txPin, uint8_t resetPin);
			uint8_t Reset();
			uint8_t SetBaud(uint8_t rate);

			//char *GetDeviceType();
			//uint8_t GetDeviceWidth();
			//uint8_t GetDeviceHeight();
			
			uint8_t ReplaceBackgroundColor(uint8_t red, uint8_t green, uint8_t blue);
			uint8_t ClearScreen();
                        uint8_t SetOrientation(uint8_t orientation);
                        uint8_t SetResolution(uint8_t resolution);
			uint8_t SetVolume(uint8_t volume);
			uint8_t Sleep(uint8_t sleep_option, uint8_t sleep_time);
			uint8_t ReadGPIOPin(uint8_t pin);
			uint8_t WriteGPIOPin(uint8_t pin, bool val);
			//ReadGPIOBus()
			//WriteGPIOBus()
			
			//Graphics Commands
			//AddUserBitmapCharacter()
			//DrawUserBitmapCharacer()
			//DrawCircle(uint8_t x, uint8_t y, uint8_t radius, uint8_t filled, unsigned int color)
			//DrawTriangle(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, uint8_t x3, uint8_t y3, uint8_t filled, unsigned int color)
			//DrawImageIcon()
			//SetBackgroundColor()
			uint8_t DrawLine(unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2, uint8_t red, uint8_t green, uint8_t blue);
			//DrawPolygon()
			//DrawRectangle(uint8_t x, uint8_t y, uint8_t width, uint8_t height, char filled, unsigned int color)
			//DrawElipse()
			//DrawPixel(uint8_t x, uint8_t y, unsigned int color)
			//ReadPixel(uint8_t x, uint8_t y)
			//ScreenCopyPaste(uint8_t source_x, uint8_t source_y, uint8_t dest_x, uint8_t dest_y, uint8_t width, uint8_t height)
			uint8_t SetPenSize(uint8_t val);

			// Text functions
			uint8_t SetFont(uint8_t font_type);
                        uint8_t SetTransparentOpaqueText(uint8_t text_mode);
			uint8_t DrawStringAtRowCol(uint8_t row, uint8_t column, uint8_t font, uint8_t red, uint8_t green, uint8_t blue, char *text);


		private:
			void WriteColor(uint8_t red, uint8_t green, uint8_t blue);
			unsigned int RGB(uint8_t red, uint8_t green, uint8_t blue);
                        
			void GetDeviceInfo();
			uint8_t GetReply();
			uint8_t ValidAck();				
		
			uint8_t	device_type;
			uint8_t	device_hardware_rev;
			uint8_t	device_firmware_rev;
			uint8_t	device_width;
			uint8_t	device_height;

                        SoftwareSerial *sSerial;
                        uint8_t resetPin;

	};

#endif
