#include "lcd_st7567s.h"
#include <Arduino.h>
#include <Wire.h>
#include "font.c"
#include "picture.c"

/******************************************
*******************************************/
lcd_st7567s::lcd_st7567s(){
  
}

/******************************************
 * Write a command to LCD.
 * Define instructions on page 23 of the ST7567A data sheet.
*******************************************/
void lcd_st7567s::WriteByte_command(int dat){
  Wire.beginTransmission(addr);      // transmit to device 0x3f
  Wire.write(0x00);                  // Co=0,A0=0. data= Co-A0-0-0-0-0-0-0. 
  Wire.write(dat);                   // sends restart command. 
  Wire.endTransmission();            // stop transmitting
}

/******************************************
 * Init the LCD.
 * This initialization function is called when using LCD.
*******************************************/
void lcd_st7567s::Init(){
  Wire.begin();
  delay(10);
/*
  // 	ST7567A
  WriteByte_command(restart);     //resets Start Line (S[5:0]), Column Address (X[7:0]), 
  delay(10);                      //Page Address (Y[3:0]) and COM Direction (MY) to their default setting.
  WriteByte_command(BS);          //select bias
  WriteByte_command(seg);         //seg output scan direction setting
  WriteByte_command(com);         //com output scan direction setting
  
  //********Adjust display brightness********
  WriteByte_command(regRatio);    //Controls the regulation ratio of the built-in regulator.
  WriteByte_command(EV1);
  WriteByte_command(EV2);

  WriteByte_command(powerCon1);   //Controls the built-in power circuits.
  WriteByte_command(powerCon2);
  WriteByte_command(powerCon3);
  Clear();                        //initalize DDRAM
  
  //**********display setting mode**********
  WriteByte_command(enter_EC);    //enter extension command set.
  WriteByte_command(DSM_ON);      //display settin mode on.
  WriteByte_command(exit_EC);     //exit extension command set.
  WriteByte_command(DT);          //set the display duty, DT=33.
  WriteByte_command(BA);          //This command has priority over the Bias Select (BS).
  WriteByte_command(FR);          //specifies the frame rate for duty. duty=250.
  //*

  WriteByte_command(displayON);   //display on.
  WriteByte_command(startLine);   //Set display start line.
*/

    WriteByte_command(0xE2);    //Internal reset
    delay(10);
    WriteByte_command(0xA2);    //0xa2 LCD bias ratio is 1/9; 0xa3 is bias ratio 1/7; 128X64=1/9
    
    //// normal screen
    WriteByte_command(0xA0);    //(0xa0-0xa1 is ADC direction selection) 0xa0 is segment forward, 0xa1 is segment reverse
    WriteByte_command(0xC8);    //com output mode drive direction is reverse
		//// flip screen
    //WriteByte_command(0xA1);    // may be bug, loss first 4 bits in each row
    //WriteByte_command(0xC0);
    
    //WriteByte_command(0xA7);    // 0xA6 Normal, 0xA7 Inverse


    //********Adjust display brightness********
    WriteByte_command(0x25);    //0x20-0x27 is the internal Rb/Ra resistance adjustment setting of V5 voltage RR=4.5V
    //WriteByte_command(0x24);    // only 0x25 and 0x24 is ok 
    WriteByte_command(0x81);    //Electricity Mode Settings
    WriteByte_command(0x20);
    
    WriteByte_command(0x2C);    //Internal Power Supply Control Mode
    WriteByte_command(0x2E);
    WriteByte_command(0x2F);
    Clear(false);                        //initalize DDRAM

    WriteByte_command(0xAF);    
    WriteByte_command(0x40);

}

/******************************************
 * Used to test screen pixels.
*******************************************/
void lcd_st7567s::testPixel(int t){
  for(int x=0; x<8; x++){
    WriteByte_command(0xb0 + x);  //y, page address y=1-0-1-1-y3-y2-y1-y0, 1-page with 8-rows

    /*******automatically increased by one******/
    WriteByte_command(0x10);      //x, column address x=0-0-0-0-1-x7-x6-x5-x4
    WriteByte_command(0x00);      //x, column address x=0-0-0-0-0-x3-x2-x1-x0
    
    for(int i=0; i<128; i++){
      WriteByte_dat(0xff);        //row=bit0--bit7, display on
      delay(t);
      }  
  }
  for(int x=0; x<8; x++){
    WriteByte_command(0xb0 + x);
    WriteByte_command(0x00);
    WriteByte_command(0x10);
    for(int i=0; i<128; i++){
      WriteByte_dat(0x00);       //row=bit0--bit7, display off
      delay(t);
      }  
  }
}

/******************************************
 * Write a data to LCD.
 * Define instructions on page 23 of the ST7567A data sheet.
*******************************************/
void lcd_st7567s::WriteByte_dat(int dat){
  Wire.beginTransmission(addr);      // transmit to device 0x7e
  Wire.write(0x40);                  // Co=0,A0=1. data= Co-A0-0-0-0-0-0-0. 
  Wire.write(dat);                   // sends data. 
  Wire.endTransmission();            // stop transmitting
}

/******************************************
 * Continuously write data to LCD.
 * Define instructions on page 23 of the ST7567A data sheet.
*******************************************/
void lcd_st7567s::WriteCont_dat(int str[]){
  int len = 0;
  len = sizeof(str);
  Wire.beginTransmission(addr);      // transmit to device 0x7e
  for(int i=0; i<len-1; i++){
    Wire.write(0xc0);                // Co=1,A0=1. data= Co-A0-0-0-0-0-0-0. 
    Wire.write(str[i]);              // sends command.
  }
  Wire.write(0x40);                  // Co=0,A0=1. data= Co-A0-0-0-0-0-0-0. 
  Wire.write(str[len-1]);            // sends command.
  Wire.endTransmission();            // stop transmitting
}

/******************************************
 * read one byte RAM data to MCU.
 * Define instructions on page 23 of the ST7567A data sheet.
*******************************************/
int lcd_st7567s::ReadByte_dat(int col, int page){
  int dat;
  WriteByte_command(0xb0 + page);     //y, page address y=1-0-1-1-y3-y2-y1-y0, 1-page with 8-rows 
  WriteByte_command(0x10 + col/16);   //x, column address x=0-0-0-0-1-x7-x6-x5-x4
  WriteByte_command(col%16);          //x, column address x=0-0-0-0-0-x3-x2-x1-x0 

  for(int i=0; i<2; i++){
    Wire.requestFrom(addr, 2);       //request 2 bytes from slave device
    while(Wire.available()){         //slave may send less than requested
      dat = Wire.read();             //receive a byte as character
    }
  }
  return dat;
}

/******************************************
 * display one pixel. Read-modify-Write command on page 42 of ST7567A datasheet.
 * x=0-128, y=0-31
*******************************************/
void lcd_st7567s::DisplayPixel(int x,int y){
  int dat;
  WriteByte_command(0xb0 + y/8);     //y, page address y=1-0-1-1-y3-y2-y1-y0, 1-page with 8-rows 
  WriteByte_command(0x10 + x/16);    //x, column address x=0-0-0-0-1-x7-x6-x5-x4
  WriteByte_command(x%16);           //x, column address x=0-0-0-0-0-x3-x2-x1-x0 
  
  WriteByte_command(RMW);            //start Read-modify-Write
  for(int i=0; i<2; i++){
    Wire.requestFrom(addr, 2);       //request 2 bytes from slave device
    while(Wire.available()){         //slave may send less than requested
      dat = Wire.read();             //receive a byte as character
    }
  }
  dat = (dat | (1<<(y%8)));
  WriteByte_dat(dat);                //row=bit0--bit7
  WriteByte_command(END);            //end Read-modify-Write
}

/******************************************
 * Does not display a pixel. Read-modify-Write command on page 42 of ST7567A datasheet.
 * x=0-128, y=0-31
*******************************************/
void lcd_st7567s::ClearPixel(int x,int y){
  int dat;
  WriteByte_command(0xb0 + y/8);     //y, page address y=1-0-1-1-y3-y2-y1-y0, 1-page with 8-rows 
  WriteByte_command(0x10 + x/16);    //x, column address x=0-0-0-0-1-x7-x6-x5-x4
  WriteByte_command(x%16);           //x, column address x=0-0-0-0-0-x3-x2-x1-x0 
  
  WriteByte_command(RMW);            //start Read-modify-Write
  for(int i=0; i<2; i++){
    Wire.requestFrom(addr, 2);       //request 2 bytes from slave device
    while(Wire.available()){         //slave may send less than requested
      dat = Wire.read();             //receive a byte as character
    }
  }
  //Serial.println(dat);             // print the data
  //dat = (dat & (0xfe<<(y%8)));
  dat &= ~(1<<(y%8));
  WriteByte_dat(dat);                //row=bit0--bit7
  WriteByte_command(END);            //end Read-modify-Write 
}

/******************************************
 * clear screen, all pixel off.
 * screen size: 128*32 dot
*******************************************/
void lcd_st7567s::Clear(bool invcolor){
  for(int x=0; x<8; x++){
    WriteByte_command(0xb0 + x);   //y, page address y=1-0-1-1-y3-y2-y1-y0, 1-page with 8-rows
    
    /*******automatically increased by one******/
    WriteByte_command(0x10);       //x, column address x=0-0-0-0-1-x7-x6-x5-x4
    WriteByte_command(0x00);       //x, column address x=0-0-0-0-0-x3-x2-x1-x0
    
    for(int i=0; i<128; i++){
      if (invcolor) {
        WriteByte_dat(0xff);         //row=bit0--bit7
      }
      else {
        WriteByte_dat(0x00);         //row=bit0--bit7
      }
    }  
  }
}
///////////////////reserve///////////////////
void lcd_st7567s::FontSize(int num){
  
}

/******************************************
 * Character display position. x=0-7, y=0-17
*******************************************/
void lcd_st7567s::Cursor(int x, int y){
  if(x>17){x=17;}
  if(y>7){x=7;}
  cursor[0]=y;
  cursor[1]=x;
}

/******************************************
 * display picture.
*******************************************/
void lcd_st7567s::DisplayPicture(){
  for(int x=0; x<9; x++){
    WriteByte_command(0xb0 + x);   //y, page address y=1-0-1-1-y3-y2-y1-y0, 1-page with 8-rows
    
    /*******automatically increased by one******/
    WriteByte_command(0x10);       //x, column address x=0-0-0-0-1-x7-x6-x5-x4
    WriteByte_command(0x00);       //x, column address x=0-0-0-0-0-x3-x2-x1-x0
    
    for(int i=0; i<128; i++){
      WriteByte_dat(pgm_read_word_near(picture+i+x*128)); 
      }  
  }
}

/******************************************
 * Writes the data from the font.c file to RAM.
*******************************************/
void lcd_st7567s::WriteFont(int num){
  for(int i=0; i<7; i++){
    //reference: https://www.arduino.cc/reference/en/language/variables/utilities/progmem/
    WriteByte_dat(pgm_read_word_near(font_7x8[num]+i));   
    //Serial.println(pgm_read_word_near(font_7x8[num]+i),HEX); 
    }
}

/******************************************
 * display font.
*******************************************/
void lcd_st7567s::Display(char *str){
  int len = 0;
  len = strlen(str);
  
  WriteByte_command(0xb0 + cursor[0]);           //y, page address y=1-0-1-1-y3-y2-y1-y0, 1-page with 8-rows
  /*******automatically increased by one******/
  WriteByte_command(0x10 + cursor[1]*7/16);      //x, column address x=0-0-0-0-1-x7-x6-x5-x4
  WriteByte_command(0x00 + cursor[1]*7%16);      //x, column address x=0-0-0-0-0-x3-x2-x1-x0

  for(int num=0; num<len; num++){ 
    switch(str[num]){
      case '0': WriteFont(0); break;
      case '1': WriteFont(1); break;
      case '2': WriteFont(2); break;
      case '3': WriteFont(3); break;
      case '4': WriteFont(4); break;
      case '5': WriteFont(5); break;
      case '6': WriteFont(6); break;
      case '7': WriteFont(7); break;
      case '8': WriteFont(8); break;
      case '9': WriteFont(9); break;
      case 'a': WriteFont(10); break;
      case 'b': WriteFont(11); break;
      case 'c': WriteFont(12); break;
      case 'd': WriteFont(13); break;
      case 'e': WriteFont(14); break;
      case 'f': WriteFont(15); break;
      case 'g': WriteFont(16); break;
      case 'h': WriteFont(17); break;
      case 'i': WriteFont(18); break;
      case 'j': WriteFont(19); break;
      case 'k': WriteFont(20); break;
      case 'l': WriteFont(21); break;
      case 'm': WriteFont(22); break;
      case 'n': WriteFont(23); break;
      case 'o': WriteFont(24); break;
      case 'p': WriteFont(25); break;
      case 'q': WriteFont(26); break;
      case 'r': WriteFont(27); break;
      case 's': WriteFont(28); break;
      case 't': WriteFont(29); break;
      case 'u': WriteFont(30); break;
      case 'v': WriteFont(31); break;
      case 'w': WriteFont(32); break;
      case 'x': WriteFont(33); break;
      case 'y': WriteFont(34); break;
      case 'z': WriteFont(35); break;
      case 'A': WriteFont(36); break;
      case 'B': WriteFont(37); break;
      case 'C': WriteFont(38); break;
      case 'D': WriteFont(39); break;
      case 'E': WriteFont(40); break;
      case 'F': WriteFont(41); break;
      case 'G': WriteFont(42); break;
      case 'H': WriteFont(43); break;
      case 'I': WriteFont(44); break;
      case 'J': WriteFont(45); break;
      case 'K': WriteFont(46); break;
      case 'L': WriteFont(47); break;
      case 'M': WriteFont(48); break;
      case 'N': WriteFont(49); break;
      case 'O': WriteFont(50); break;
      case 'P': WriteFont(51); break;
      case 'Q': WriteFont(52); break;
      case 'R': WriteFont(53); break;
      case 'S': WriteFont(54); break;
      case 'T': WriteFont(55); break;
      case 'U': WriteFont(56); break;
      case 'V': WriteFont(57); break;
      case 'W': WriteFont(58); break;
      case 'X': WriteFont(59); break;
      case 'Y': WriteFont(60); break;
      case 'Z': WriteFont(61); break;
      case '!': WriteFont(62); break;
      case '"': WriteFont(63); break;
      case '#': WriteFont(64); break;
      case '$': WriteFont(65); break;
      case '%': WriteFont(66); break;
      case '&': WriteFont(67); break;
      case '\'': WriteFont(68); break;
      case '(': WriteFont(69); break;
      case ')': WriteFont(70); break;
      case '*': WriteFont(71); break;
      case '+': WriteFont(72); break;
      case ',': WriteFont(73); break;
      case '-': WriteFont(74); break;
      case '/': WriteFont(75); break;
      case ':': WriteFont(76); break;
      case ';': WriteFont(77); break;
      case '<': WriteFont(78); break;
      case '=': WriteFont(79); break;
      case '>': WriteFont(80); break;
      case '?': WriteFont(81); break;
      case '@': WriteFont(82); break;
      case '{': WriteFont(83); break;
      case '|': WriteFont(84); break;
      case '}': WriteFont(85); break;
      case '~': WriteFont(86); break;
      case ' ': WriteFont(87); break;
      case '.': WriteFont(88); break;
      case '^': WriteFont(89); break;
      case '_': WriteFont(90); break;
      case '`': WriteFont(91); break;
      case '[': WriteFont(92); break;
      case '\\': WriteFont(93); break;
      case ']': WriteFont(94); break;
      default: break;
    }  
  }
}

void lcd_st7567s::DrawLine(int x1, int y1, int x2, int y2, bool invcolor) {

  int tmp;
  int x,y;
  int dx, dy;
  int err;
  int ystep;

  int swapxy = 0;

  if ( x1 > x2 ) dx = x1-x2; else dx = x2-x1;
  if ( y1 > y2 ) dy = y1-y2; else dy = y2-y1;

  if ( dy > dx ) 
  {
    swapxy = 1;
    tmp = dx; dx =dy; dy = tmp;
    tmp = x1; x1 =y1; y1 = tmp;
    tmp = x2; x2 =y2; y2 = tmp;
  }
  if ( x1 > x2 ) 
  {
    tmp = x1; x1 =x2; x2 = tmp;
    tmp = y1; y1 =y2; y2 = tmp;
  }
  err = dx >> 1;
  if ( y2 > y1 ) ystep = 1; else ystep = -1;
  y = y1;

  for( x = x1; x <= x2; x++ )
  {
    if ( swapxy == 0 )
      if (invcolor) {
        ClearPixel(x, y);
      }
      else {
        DisplayPixel(x, y);
      }
    else 
      if (invcolor) {
        ClearPixel(y, x);
      }
      else {
        DisplayPixel(y, x); 
      }
    err -= dy;
    if ( err < 0 ) 
    {
      y += ystep;
      err += dx;
    }
  }  
}
/*==============================================*/
/* Circle */
void lcd_st7567s::draw_circle_section(int x, int y, int x0, int y0, int option, bool invcolor, bool solid)
{
    /* upper right */
    if ( option & DRAW_UPPER_RIGHT )
    {
      if (solid) {
        DrawLine(x0 + x, y0 - y, x0, y0 - y, invcolor);
        DrawLine(x0 + y, y0 - x, x0, y0 - x, invcolor);
      }
      else {
        if (invcolor) {
          ClearPixel(x0 + x, y0 - y);
          ClearPixel(x0 + y, y0 - x);        
        }
        else {
          DisplayPixel(x0 + x, y0 - y);
          DisplayPixel(x0 + y, y0 - x);
        }        
      }
    }
    
    /* upper left */
    if ( option & DRAW_UPPER_LEFT )
    {
      if (solid) {
        DrawLine(x0 - x, y0 - y, x0, y0 - y, invcolor);
        DrawLine(x0 - y, y0 - x, x0, y0 - x, invcolor);
      }
      else {
        if (invcolor) {
          ClearPixel(x0 - x, y0 - y);
          ClearPixel(x0 - y, y0 - x);
        }
        else {
          DisplayPixel(x0 - x, y0 - y);
          DisplayPixel(x0 - y, y0 - x);
        }
      }
    }
    
    /* lower right */
    if ( option & DRAW_LOWER_RIGHT )
    {
      if (solid) {
        DrawLine(x0 + x, y0 + y, x0, y0 + y, invcolor);
        DrawLine(x0 + y, y0 + x, x0, y0 + x, invcolor);
      }
      else {
        if (invcolor) {
          ClearPixel(x0 + x, y0 + y);
          ClearPixel(x0 + y, y0 + x);
        }
        else {
          DisplayPixel(x0 + x, y0 + y);
          DisplayPixel(x0 + y, y0 + x);
        }
      }
    }
    
    /* lower left */
    if ( option & DRAW_LOWER_LEFT )
    {
      if (solid) {
        DrawLine(x0 - x, y0 + y, x0, y0 + y, invcolor);
        DrawLine(x0 - y, y0 + x, x0, y0 + x, invcolor);
      }
      else {
        if (invcolor) {
          ClearPixel(x0 - x, y0 + y);
          ClearPixel(x0 - y, y0 + x);
        }
        else {
          DisplayPixel(x0 - x, y0 + y);
          DisplayPixel(x0 - y, y0 + x);
        }
      }
    }
}

void lcd_st7567s::draw_circle(int x0, int y0, int rad, int option, bool invcolor, bool solid)
{
    int f;
    int ddF_x;
    int ddF_y;
    int x;
    int y;

    f = 1;
    f -= rad;
    ddF_x = 1;
    ddF_y = 0;
    ddF_y -= rad;
    ddF_y *= 2;
    x = 0;
    y = rad;

    draw_circle_section(x, y, x0, y0, option, invcolor, solid);
    
    while ( x < y )
    {
      if (f >= 0) 
      {
        y--;
        ddF_y += 2;
        f += ddF_y;
      }
      x++;
      ddF_x += 2;
      f += ddF_x;

      draw_circle_section(x, y, x0, y0, option, invcolor, solid);    
    }
}
/*==============================================*/
