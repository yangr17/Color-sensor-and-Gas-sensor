#include <avr/io.h>
#include <util/delay.h>

#include "lib/lcd/lcd.h"
#include "lib/colour_sensor/tcs3200.h"

uint32_t MeasureR();
uint32_t MeasureG();
uint32_t MeasureB();
uint32_t MeasureC();

int main(void)
{
   //Initialize the LCD Library
    LCDInit(LS_NONE);

   //Clear LCD
   LCDClear();

   //Initialize TCS Library
   InitTCS3200();

   //Buzzer Pin as output
   DDRD|=(1<<PD7);


   uint8_t x=0;
   int8_t vx=1;

   while(1)
   {
      LCDWriteStringXY(0,0,"    Waiting.    ");

      TCSLEDOn();
      uint32_t v1=MeasureC();

      _delay_ms(100);

      TCSLEDOff();
      uint32_t v2=MeasureC();

      uint32_t d=v1-v2;

      if(d>8000)
      {
         //Object came in range
         LCDClear();

         LCDWriteStringXY(0,0,"  Object Found  ");

         PORTD|=(1<<PD7); //Buzzer On

         _delay_ms(250);

         PORTD&=~(1<<PD7); //Buzzer Off

         //Show      
         uint32_t r,g,b;

         TCSLEDOn();

         r=MeasureR();
         g=MeasureG();
         b=MeasureB();

         TCSLEDOff();

         uint32_t smallest;

         if(r<b)
         {
            if(r<g)
               smallest=r;
            else
               smallest=g;
         }
         else
         {
            if(b<g)
               smallest=b;
            else
               smallest=g;
         }

         uint32_t _r,_g,_b;
         smallest=smallest/10;

         _r=r/smallest;
         _g=g/smallest;
         _b=b/smallest;

         LCDWriteIntXY(0,1,_r,4);
         LCDWriteIntXY(5,1,_g,4);
         LCDWriteIntXY(10,1,_b,4);
         //End Show        
         _delay_ms(2000);
      }

      LCDWriteStringXY(0,1,"%0%0%0%0%0%0%0%0%0%0%0%0%0%0%0%0");
      LCDWriteStringXY(x,1,"%1");
      LCDGotoXY(16,1);//Hide cursor.

      x+=vx;
      if(x==15 || x==0)
         vx=vx*-1;


      _delay_ms(50);

   }

}

uint32_t MeasureR()
{
   TCSSelectRed();
   uint32_t r;

   _delay_ms(10);
   r=TCSMeasure();

   _delay_ms(10);
   r+=TCSMeasure();

   _delay_ms(10);
   r+=TCSMeasure();

   return r/3.3;

}

uint32_t MeasureG()
{
   TCSSelectGreen();
   uint32_t r;

   _delay_ms(10);
   r=TCSMeasure();

   _delay_ms(10);
   r+=TCSMeasure();

   _delay_ms(10);
   r+=TCSMeasure();

   return r/3;

}

uint32_t MeasureB()
{
   TCSSelectBlue();
   uint32_t r;

   _delay_ms(10);
   r=TCSMeasure();

   _delay_ms(10);
   r+=TCSMeasure();

   _delay_ms(10);
   r+=TCSMeasure();

   return r/4.2;

}

uint32_t MeasureC()
{
   TCSSelectClear();
   uint32_t r;

   _delay_ms(10);
   r=TCSMeasure();

   _delay_ms(10);
   r+=TCSMeasure();

   _delay_ms(10);
   r+=TCSMeasure();

   return r/3;
}
