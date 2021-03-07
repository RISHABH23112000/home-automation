#include<reg51.h>
//led
sbit led1= P2^0;
sbit key= P2^1;

//LCD
sbit rs = P2^5;  				               // register select pin
sbit rw = P2^6;  				               // read write pin
sbit e  = P2^7;                        // enable pin
void delay(unsigned int t);
void cmd(unsigned char ch);
void dat(unsigned char ch);  
void lcd_init();
void first_line(unsigned char *x);
void second_line(unsigned char *x);
void ISR_ex0(void);
void Delay();
void dec_counter(int a);
void lcddata(double b);
void disp_temp(double num);
void read();
void counting();
void engine_off(int a);
void main()
{        
         P0 = 0x00;             // Make all pins zero
         P1 = 0x00;             // Make all pins zero
         P2 = 0x00;             // Make all pins zero
         P3 = 0x04;             // making Port3 pin 2 high
         IE=0x81;               // Enable INT0
         IT0=1;
         read();
	       second_line("Max Horn 999"); 
	       	// Set Falling Edge Trigger for INT0 (whwn zero is applied)   
 while(1)
    {    	
         led1 = 1;
    }
}
int a = 0;
void counting()
{	int c;
	c = a++;
  disp_temp(c);
	engine_off(c);
}
void engine_off(int a)
{ if(a >= 999)
	{key = 1;}
	else
	{key = 0;}
}

void ISR_ex0(void) interrupt 0  // ISR for external interrupt INT0
{     
         counting();
	       led1=~led1;
         Delay();              
}
void read()
{	  // Displays "count" while controller reads from ADC
cmd(0x0E);    //turn display ON for cursor blinking
cmd(0x01);    //clear screen
cmd(0x06); 	 //increment cursor
	lcddata('C');lcddata('o');lcddata('u');lcddata('n');lcddata('t');lcddata(':');
}
void Delay()
{
     int i;
     TMOD = 0x01;               // Timer 0 mode 1
     for(i=0;i<200;i++)
     {
       TH0=0xF8;                 //initial values for 1sec delay
       TL0=0xCC;
       TR0 = 1;                  // timer0 start
       while (TF0 == 0);         // check overflow condition
       TR0 = 0;                  // Stop Timer
       TF0 = 0;                  // Clear flag
     }
}
void first_line(unsigned char *x)
{ 
	cmd(0x01);   //CLEAR SCREEN
  lcd_init();
	cmd(0x18);
  while(*x != '\0') // searching the null terminator in the sentence
    {   dat(*x);
        x++;
    }
}
void second_line(unsigned char *x)
{ 
  lcd_init();
	cmd(0xc1);
  while(*x != '\0') // searching the null terminator in the sentence
    {   dat(*x);
        x++;
    }
}
void delay(unsigned int t)
{
	unsigned int a,b;
	for(a=0;a<t;a++)
	for(b=0;b<1275;b++);
	return;
}

void cmd(unsigned char ch)
{
    
	rs=0;
	rw=0;
	P0=ch;
	e=1;
	delay(20);
	e=0;
	return;
}
void lcddata(double b)
{	
	rs=1;
	rw=0;
	P0=b;
	e=1;
	delay(20);
	e=0;
}
void dat(unsigned char ch)
{
	
	rs=1;
	rw=0;
	P0=ch;
	e=1;
	delay(20);
	e=0;
}
void lcd_init()
	{
	cmd(0x38);   // 8 BIT MODE 
	cmd(0x0E);   //DISPLAY ON CURSOR BLINKING 
	cmd(0x81);   //STARTING LOCATION LCD 
	}
void disp_temp(double num)            //displays number on LCD
{

unsigned char UnitDigit  = 0;  //It will contain unit digit of number
unsigned char TenthDigit = 0;  //It will contain 10th position digit of number
unsigned char HundDigit = 0;  //It will contain 100th position digit of number
unsigned char decimal=0;	  //It will contain the decimal position of number
cmd(0x86);
HundDigit=(num/100);
if( HundDigit != 0)	          // If it is zero, then don't display
lcddata(HundDigit+0x30);	  // Make Character of HundDigit and then display it on LCD
TenthDigit = num - HundDigit*100;  // Findout Tenth Digit
TenthDigit = TenthDigit/10;
if (HundDigit==0 && TenthDigit==0){} 	          // If it is zero, then don't display
else
lcddata(TenthDigit+0x30);	  // Make Char of TenthDigit and then display it on LCD
UnitDigit = num - HundDigit*100;
UnitDigit = UnitDigit - TenthDigit*10;
lcddata(UnitDigit+0x30);	  // Make Char of UnitDigit and then display it on LCD

}