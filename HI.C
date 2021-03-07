#include <reg51.h>  //THIS LIBRARY CONTAIN ALL FILES RELATED TO 8051
#include <math.h>
#include <string.h>
#include <stdio.h>
#define lcd P0     // TO DEFINE DATA PIN    
#define input P1
double newtemp,pass1,pass2,T;
// AUTOMATION SIPO
sbit SH = P2^0;
sbit DS = P2^1;
sbit ST = P2^2;
sbit LSBACC = ACC^0;

//LCD
sbit rs = P2^5;  				               // register select pin
sbit rw = P2^6;  				               // read write pin
sbit e  = P2^7;  				               // enable pin
//ADC
sbit rd   = P3^7;		                //defines rd pin of ADC use for reading purposes
sbit wr   = P3^6;		                // define wr pin of ADC use for writing purposes
sbit intr = P3^4;		                //defines intr pin use for sending interrupts from microcontroller
void automation();                     // CALLED ALL FUCNTION 
void delay(unsigned int t);
void cmd(unsigned char ch);
void dat(unsigned char ch);
void l1on();
void l1off();
void l2on();
void l2off();
void l3on();
void l3off();
double adc();
void read();
void disp_temp(double num);
void maine();
void SerialIn(unsigned char);
void LatchData(unsigned char LatchOutput);
void MSDelay(unsigned int value); 
void updown();
void up();
void down();
void specific_data();
void lcd_init();
void first_line(unsigned char *x);
void second_line(unsigned char *x);
void main()
{   
		TMOD=0x20;  // TIMER 1 MODE 2
		SCON=0x50;  // 8 bit uart
		TH1=-3;     //SELECTING BAUD RATE OF 9600 BPPS or th1=0xfd
		TR1=1;      // TO START THE TIMER
	  SH = 0;
	  ST = 0;
	  DS = 0; 
    LatchData(~0x00);
	  first_line("Blue Automation");
	  second_line("fuckers");
    	
  while(1)   // INFINITE LOOP 
  { 
		automation();
  }
}	
void automation()                           // automation
{	while(1)
 {
	char pass0,pass1,pass2,pass3,pass4,pass5,x;
	char array[6];
	char str[6];
	char Charin = 0;
    while(!RI);
    Charin = SBUF ;
    x = Charin ;	
    RI = 0;
   
      pass0 = SBUF;
      array[0]=	SBUF; 
      delay(50);			                    
      pass1 = SBUF;
      array[1]=	SBUF;	 
      delay(50);
      pass2 = SBUF;
	    array[2]=	SBUF;
      delay(50);	 
	    pass3 = SBUF;
      array[3]=	SBUF;	 
	    delay(50);
	    pass4 = SBUF;
	    array[4]=	SBUF;
	    delay(50);
	    pass5 = SBUF;
			array[5]=	SBUF;
	    delay(50);
   if ((pass0 == pass1)&&(pass0 == pass2&&(pass0 == pass3)&&(pass0 == pass4)&&(pass0 == pass5)))
		 { int i;
			 for (i=0;i<6;i++)
			  {
        str[0] = str[0] + array[i];
        }                                			                
       if(x=='1')
      {
			 first_line("You enter: 1 ");
			 LatchData(~0x80);
			 second_line("only 1 ON");
			 break;
			 
      }
       else if(x=='2')
      {
			 first_line("You enter: 2 ");
			 LatchData(~0x40);
			 second_line("only 2 ON");
       break;
			 
      }
			 else if(x=='3')
      {
			first_line("You enter: 3 ");
			LatchData(~0x20);
			second_line("only 3 ON");
      break;
			 
      }
       else if(x=='4')
      {
			first_line("You enter: 4 ");
			LatchData(~0x10);
			second_line("only 4 ON");
      break;
			 
      }
       else if(x=='5')
      {
			first_line("You enter: 5 ");
			LatchData(~0x08);
			second_line("only 5 ON");
      break;
		   
      }
       else if(x=='6')
      {
			first_line("You enter: 6 ");
			LatchData(~0x04);
			second_line("only 6 ON");
      break;
			 
      }
			 else if(x=='7')
      {
			first_line("You enter: 7 ");
			LatchData(~0x02);
			second_line("only 7 ON");
      break;
			 
      }
			 else if(x=='8')
      {
			first_line("You enter: 8 ");
			LatchData(~0x01);
			second_line("only 8 ON");
      break;
			}
		}
		else
    {
			continue;
		}			
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
	cmd(0xc3);
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
void dat(unsigned char ch)
{
	
	rs=1;
	rw=0;
	P0=ch;
	e=1;
	delay(20);
	e=0;
}
double adc()                        // Function to read the values from ADC and send to controller.
{
double temp;
rd=1;     
wr=0;                               // LOW TO HIGH PULSE
delay(1);    
wr=1;
while(intr==1){}                    //Even if there is no body to execute, doesn't mean that the loop terminates. or while(intr==1);
	                                  //Instead it will simply loop repeatedly over the conditional (which may have or rely upon side-effects) until it is satisfied.
	
rd=0;
temp=input;
delay(1);

return temp;
}
void disp_temp(double num)          //displays number on LCD
{
unsigned char UnitDigit  = 0;       //It will contain unit digit of number
unsigned char TenthDigit = 0;       //It will contain 10th position digit of number
unsigned char HundDigit  = 0;       //It will contain 100th position digit of number
unsigned char decimal    = 0;	      //It will contain the decimal position of number
int point;
point     = num*10;
HundDigit = (num/100);
if( HundDigit != 0)	                // If it is zero, then don't display
{dat(HundDigit+0x30);}	        // Make Character of HundDigit and then display it on LCD
                                    //to convert a decimal digit to ASCII you have to add decimal 48 or hexadecimal 0x30 or just the char '0'.
TenthDigit = num - HundDigit*100;   // Findout Tenth Digit
TenthDigit = TenthDigit/10;
if (HundDigit==0 && TenthDigit==0){}// If it is zero, then don't display
else
dat(TenthDigit+0x30);	          // Make Char of TenthDigit and then display it on LCD
UnitDigit = num - HundDigit*100;
UnitDigit = UnitDigit - TenthDigit*10;
dat(UnitDigit+0x30);	          // Make Char of UnitDigit and then display it on LCD
dat('.');
decimal=(point%10);
dat (decimal+0x30);		          // Make Char of Decimal Digit and then display it on LCD
dat(' '); dat('C');
}
void read()
{	                                  // Displays "READING" while controller reads from ADC
cmd(0x0E);                       //turn display ON for cursor blinking
cmd(0x01);                       //clear screen
cmd(0x06); 	                    //increment cursor
dat('R');dat('E');dat('A');dat('D');dat('I');dat('N');dat('G');dat(':');dat('-');
}
void maine()
{
read();					                    // show reading on LCD while controller reads from ADC
while(1)
{			                              // use for checking errors while reading the value from ADC
newtemp = adc();			              //reads first value  from ADC
delay(160);			                    //waits 60 msec
pass1 = adc();		                  // reads the Second value from ADC
delay(160);			                    // waits 60 msec
   if (newtemp == pass1)
		 {	                            //compare first and second value
        break;			                // if first and second value is same breaks the loop
	   }	   
}
while(1)
{			                              //enters in the permanent loop
T=150;                              //set reference voltage acting multiplier factor for temperature accuration
newtemp =(((newtemp*T)/255));        //converts the temperature value according to reference adjusted in decimal
cmd(0x0E);                       //turn display ON for cursor blinking
cmd(0x01);                       //clear screen
cmd(0x06);                       //increment
disp_temp(newtemp);	                //show temperature
delay(300);			                    //waits 3sec before re-measure the value of temperature											
	while(1)
	{			                            // re-measure the value from ADC but this time double check
	newtemp = adc();					    
	delay(60);
	pass1 = adc();
	delay(60);
	pass2 = adc();
   if (newtemp == pass1)
		 {
      if(pass1 == pass2)
				{
	    break;		
				}
		 }	   
  }
                                    // end ADC while loo[
}			                              // end while permanent loop
}
void SerialIn(unsigned char x)
{
	 DS = x;
	 SH = 1;
	 DS = 0;
 	 SH = 0;
}

void LatchData(unsigned char LatchOutput)
{
	 unsigned char x;
	 for(x=0; x<8; x++)
	 { delay(10);
	 	 ACC = LatchOutput;
	 	 ACC = ACC >> x;
	 	 SerialIn(LSBACC);
	 }
	 ST = 1;
	 ST = 0;
}
void lcd_init()
	{
	cmd(0x38);   // 8 BIT MODE 
	cmd(0x0E);   //DISPLAY ON CURSOR BLINKING 
	cmd(0x81);   //STARTING LOCATION LCD 
	}
