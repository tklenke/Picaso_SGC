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

#define RXPIN 12
#define TXPIN 13
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

    Serial.println(pic.DrawCharAtRowCol(12,10,PIC_FONT_MON0_12X16,255,255,255,'A'));

    Serial.println(pic.DrawRectangle(50,350,75,75,true,255,255,255));

    Serial.println(pic.DrawRectangle(150,350,75,75,false,255,255,255));

    Serial.println(pic.DrawRectangle(250,350,75,75,true,255,255,255));  

    Serial.println(pic.DrawRectangle(350,350,75,75,true,255,255,255)); 

    Serial.println("done setup");


  
}


void loop()
{
    char sz[24], szDebug[24];
    int r, g, b;

    for (r = 0; r < 255; r += 10)
        for (g = 0; g < 255; g += 10)
            for (b = 0; b < 255; b += 10)
            {
                pic.DrawStringAtRowCol(1,40,PIC_FONT_MON0_8X8,25,25,112,sz);
                sprintf(sz,"%3.3d,%3.3d,%3.3d", r, g, b);
                pic.DrawStringAtRowCol(1,40,PIC_FONT_MON0_8X8,255,255,255,sz);
                pic.DrawRectangle(300, 1,10,10,false,r,g,b);
                delay(100);
            }


}



