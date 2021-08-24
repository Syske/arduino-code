/*
esp8266+oled显示屏
显示汉字项目
论坛地址：bbskali.cn
博客：blog.bbskali.cn
*/

#include <Arduino.h>
#include <U8g2lib.h>

#ifdef U8X8_HAVE_HW_SPI
#include <SPI.h>
#endif
#ifdef U8X8_HAVE_HW_I2C
#include <Wire.h>
#endif



//U8G2_SSD1306_128X64_NONAME_F_SW_I2C u8g2(U8G2_R0, clock=SDA,  data=SCL, reset=U8X8_PIN_NONE);   // All Boards without Reset of the Display
U8G2_SSD1306_128X64_NONAME_F_SW_I2C u8g2(U8G2_R0, SDA,  SDA, U8X8_PIN_NONE);   // All Boards without Reset of the Display


void setup(void) {
  u8g2.begin();
}

void loop(void) {
  u8g2.clearBuffer();                    // clear the internal memory
  u8g2.setFont(u8g2_font_ncenB08_tr);    // choose a suitable font
  u8g2.drawStr(0,10,"Hello World!");    // write something to the internal memory
  u8g2.sendBuffer();                    // transfer internal memory to the display
  delay(1000);  
}
