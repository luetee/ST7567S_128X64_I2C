/******************************
 * original code from
 * URL: https://github.com/mworkfun/ST7567A_128X32DOT_LCD
 * 
 *  
 * Modify for LCD ST7567S 128x64
 * screen:
 * x-----------------------> +
 * y                    |
 * |                    |
 * |---------------------
 * | 
 * v +
 *  
 ******************************/

#include"lcd_st7567s.h"

//create an lcd object.
lcd_st7567s Lcd;

/////////////////////////////////////////////////////////
void setup() {
  //Serial.begin(115200);
  Lcd.Init();
  
}

/////////////////////////////////////////////////////////
void loop() {

  Lcd.testPixel(2);                   //use to test LCD screen. Parameter is velocity.
  
  for(int a=0; a<64; a++){
  Lcd.DisplayPixel(a,a);              //display one pixel. X=0-31, Y=0-127
  delay(5);
  }
  for(int a=0; a<64; a++){
  Lcd.ClearPixel(a,a);                //Does not display a pixel. X=0-31, Y=0-127
  delay(5);
  }

  for(int a=0; a<128; a++){
  Lcd.DisplayPixel(a,0);              //display one pixel. X=0-31, Y=0-127
  Lcd.DisplayPixel(a,63);              //display one pixel. X=0-31, Y=0-127
  }
  for(int a=0; a<64; a++){
  Lcd.DisplayPixel(0, a);              //display one pixel. X=0-31, Y=0-127
  Lcd.DisplayPixel(127,a);              //display one pixel. X=0-31, Y=0-127
  Lcd.DisplayPixel(4,a);
  }
  delay(1500);

/**/  
  Lcd.Cursor(7, 0);                    //Character display position. y=0-3, x=0-17
  Lcd.Display("KEYES");               //Maximun 18 characters.
  Lcd.Cursor(0, 1);
  Lcd.Display("ABCDEFGHIJKLMNOPQR");
  Lcd.Cursor(0, 2);
  Lcd.Display("123456789+-*/<>=$@");
  Lcd.Cursor(0, 3);
  Lcd.Display("%^&(){}:;'|?,.~\\[]");
  Lcd.Cursor(0, 4);
  Lcd.Display("ABCDEFGHIJKLMNOPQR");
  Lcd.Cursor(0, 5);
  Lcd.Display("123456789+-*/<>=$@");
  Lcd.Cursor(0, 6);
  Lcd.Display("%^&(){}:;'|?,.~\\[]");
  Lcd.Cursor(0, 7);
  Lcd.Display("ABCDEFGHIJKLMNOPQR");
  delay(1500);
  Lcd.Clear();                        //All pixels turn off.
  
  Lcd.DisplayPicture();               //Displays the image data for the picture.c file
  delay(1500);

}
