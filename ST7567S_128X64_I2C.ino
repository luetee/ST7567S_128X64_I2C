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
  Lcd.DisplayPixel(a,a);              //display one pixel. X=0-127, Y=0-63
  delay(5);
  }
  for(int a=0; a<64; a++){
  Lcd.ClearPixel(a,a);                //Does not display a pixel. X=0-127, Y=0-63
  delay(5);
  }

  Lcd.Clear(true);
  for(int a=0; a<64; a++){
  Lcd.ClearPixel(a,a);                //Does not display a pixel. X=0-127, Y=0-63
  delay(5);
  }
  delay(5000);

  Lcd.Clear(false);
  for(int a=0; a<128; a++){
  Lcd.DisplayPixel(a,0);              //display one pixel. X=0-127, Y=0-63
  Lcd.DisplayPixel(a,63);              //display one pixel. X=0-127, Y=0-63
  }
  for(int a=0; a<64; a++){
  Lcd.DisplayPixel(0, a);              //display one pixel. X=0-127, Y=0-63
  Lcd.DisplayPixel(127,a);              //display one pixel. X=0-127, Y=0-63
  Lcd.DisplayPixel(4,a);
  }
  Lcd.DrawLine(5, 5, 125, 60, false);
  Lcd.DrawLine(125, 5, 5, 60, false);
  Lcd.DrawLine(53, 31, 73, 31, false);
  Lcd.DrawLine(63, 20, 63, 40, false);

  Lcd.draw_circle(20, 30, 10, DRAW_ALL, false, false);
  Lcd.draw_circle(20, 30, 15, DRAW_ALL, false, false);

  Lcd.draw_circle(105, 30, 15, DRAW_ALL, false, true);

  delay(5000);

  Lcd.Clear(false);
  Lcd.DrawLine(53, 31, 73, 31, false);
  Lcd.DrawLine(63, 20, 63, 40, false);
  Lcd.draw_circle(90, 30, 10, DRAW_ALL, false, false);
  Lcd.draw_circle(30, 30, 10, DRAW_ALL, false, true);
  delay(5000);

  Lcd.Clear(true);
  Lcd.DrawLine(53, 31, 73, 31, true);
  Lcd.DrawLine(63, 20, 63, 40, true);
  Lcd.draw_circle(90, 30, 10, DRAW_ALL, true, false);
  Lcd.draw_circle(30, 30, 10, DRAW_ALL, true, true);
  delay(5000);

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
  delay(5000);
  Lcd.Clear(false);                        //All pixels turn off.
  
  Lcd.DisplayPicture();               //Displays the image data for the picture.c file
  delay(5000);

}
