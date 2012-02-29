/*************************************************

        4Display Shield Library Example
        Code: Oscar Gonzalez December 2010
        www.BricoGeek.com
  
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

 *************************************************/
#include <SoftwareSerial.h>
#include <picaso_sgc.h>

#define RXPIN 2
#define TXPIN 3
#define RESETPIN 8

PicasoSGC  pic;

void setup()
{
  delay(1000); 
  Serial.begin(9600);
  Serial.println("Starting");
  Serial.println(pic.Init(RXPIN, TXPIN, RESETPIN));
  Serial.println(pic.SetBaud(PIC_57600_BAUD));
  
  //Need to set resolution and orientation before starting any drawing functions
  Serial.println(pic.SetResolution(PIC_DISPLAY_MODE_RESOLUTION_800_480));
  Serial.println(pic.SetOrientation(PIC_DISPLAY_MODE_ORIENTATION_270DEG));
  
  Serial.println(pic.ReplaceBackgroundColor(25,25,112));
  Serial.println(pic.ClearScreen());

  Serial.println(pic.DrawLine(0,0,480,800,255,255,255));

  Serial.println(pic.DrawStringAtRowCol(1,10,PIC_FONT_MON0_5X7,255,255,255,"Mono 5x7"));
  Serial.println(pic.DrawStringAtRowCol(2,10,PIC_FONT_PROPORTIONAL_5X7,255,255,255,"Pro 5x7"));
  
  Serial.println(pic.DrawStringAtRowCol(3,10,PIC_FONT_MON0_8X8,255,255,255,"Mono 8x8"));
  Serial.println(pic.DrawStringAtRowCol(4,10,PIC_FONT_PROPORTIONAL_8X8,255,255,255,"Pro 8x8"));
  
  Serial.println(pic.DrawStringAtRowCol(5,10,PIC_FONT_MON0_8X12,255,255,255,"Mono 8x12"));
  Serial.println(pic.DrawStringAtRowCol(6,10,PIC_FONT_PROPORTIONAL_8X12,255,255,255,"Pro 8x12"));
  
  Serial.println(pic.DrawStringAtRowCol(10,10,PIC_FONT_PROPORTIONAL_12X16,255,255,255,"Pro 12x16"));
  Serial.println(pic.DrawStringAtRowCol(11,10,PIC_FONT_MON0_12X16,255,255,255,"Mono 12x16"));
  
  Serial.println("done setup");
}


void loop()
{
  
  int i;
  

/*  oled.Clear();
  
  oled.setfont(OLED_FONT5x7);
  oled.setfontmode(OLED_FONT_TRANSPARENT);
  
  oled.drawstringblock(5, 5, 0, oled.RGB(255, 255, 255), 1, 1, "DisplayShield4d");
  oled.drawstringblock(5, 18, 0, oled.RGB(0, 255, 0), 1, 1, "Oscar Gonzalez");
  oled.drawstringblock(5, 31, 0, oled.RGB(255, 0, 255), 1, 1, "www.BricoGeek.com");
  delay(2000); oled.Clear();  
  
  oled.drawstringblock(5, 5, 0, oled.RGB(255, 0, 255), 1, 1, "Color");
  delay(1000); oled.Clear();
  oled.drawstringblock(5, 5, 0, oled.RGB(0, 255, 255), 2, 2, "Display");
  delay(1000); oled.Clear();
  oled.drawstringblock(5, 5, 0, oled.RGB(255, 255, 0), 4, 4, "Demo!");
  delay(1000); oled.Clear();
  
  for (i=0 ; i<128 ; i++) { oled.line(0, i, 127, i, oled.RGB(255, 0, 0)); }
  for (i=0 ; i<128 ; i++) { oled.line(0, i, 127, i, oled.RGB(0, 0, 0)); }
  
  oled.Clear();
  
  for (i=0 ; i<128 ; i++) { oled.line(i, 0, i, 127, oled.RGB(0, 255, 0)); }
  for (i=0 ; i<128 ; i++) { oled.line(i, 0, i, 127, oled.RGB(0, 0, 0)); }
  
  oled.Clear();

  for (i=0 ; i<128 ; i++) { oled.line(127-i, 0, 127-i, 127, oled.RGB(0, 0, 255)); }  
  for (i=0 ; i<128 ; i++) { oled.line(127-i, 0, 127-i, 127, oled.RGB(0, 0, 0)); }  
  
  oled.Clear();
  
  for (i=0 ; i<128 ; i+=3) { oled.line(0, i, 128, i, oled.RGB(255, i*2, i*2)); }
  
  delay(1000);
  oled.Clear();
  
  // Rectangle
  oled.rectangle(10, 10, 75, 30, OLED_WIREFRAME, oled.RGB(255, 0, 255));
  delay(500);
  oled.rectangle(20, 20, 75, 30, OLED_SOLID, oled.RGB(255, 0, 255));
  delay(1000);
  
  oled.Clear();
  
  // Circle
  oled.circle(40, 40, 15, OLED_WIREFRAME, oled.RGB(255, 255, 0));
  delay(500);  
  oled.circle(50, 50, 15, OLED_SOLID, oled.RGB(255, 255, 0));
  delay(1000);  

  oled.Clear();  
  
  // Triangle
  oled.triangle(64, 120, 100, 64, 20, 20, OLED_WIREFRAME, oled.RGB(0, 255, 255));
  delay(1000);  
  
  oled.triangle(64, 120, 100, 64, 20, 20, OLED_SOLID, oled.RGB(0, 255, 255));
  delay(1000);   
 
  oled.Clear();   
*/

}



