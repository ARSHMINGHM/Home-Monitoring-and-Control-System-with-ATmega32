#include<stdio.h>
#include <mega32.h>

#include <delay.h>

// Alphanumeric LCD functions
#include <alcd.h>

// Declare your global variables here
char txt[32];

// Timer 0 overflow interrupt service routine


// Voltage Reference: AREF pin
#define ADC_VREF_TYPE ((0<<REFS1) | (0<<REFS0) | (0<<ADLAR))

// Read the AD conversion result
unsigned int read_adc(unsigned char adc_input)
{
ADMUX=adc_input | ADC_VREF_TYPE;
// Delay needed for the stabilization of the ADC input voltage
delay_us(10);
// Start the AD conversion
ADCSRA|=(1<<ADSC);
// Wait for the AD conversion to complete
while ((ADCSRA & (1<<ADIF))==0);
ADCSRA|=(1<<ADIF);
return ADCW;
}
void PwmLed_init(){
      TCCR2=(1<<WGM20) | (1<<COM21) | (1<<WGM21) | (1<<CS20);
      DDRD |= (1<<DDD7);
}
void PwmLed_init_rev(){
      TCCR2=(0<<WGM20) | (0<<COM21) | (0<<WGM21) | (0<<CS20);
}
void PwmFAN_init(){
      TCCR0=(1<<WGM00) | (1<<COM01) | (1<<WGM01) | (1<<CS00);
      DDRB |= (1<<DDB3);
}
void PwmFAN_init_rev(){
      TCCR0=(0<<WGM00) | (0<<COM01) | (0<<WGM01) | (0<<CS00);
}
int LCD_LED_LEVEL(int LedOn,int LAYER_OF_LCD,int PotentiometerInput){
      if(PINA.4){LAYER_OF_LCD = 0;}
      if(LedOn){
                  sprintf(txt,"LED light  %2d                 ",PotentiometerInput);
            }
      else{
                  sprintf(txt,"LED light  %2d                 ",PotentiometerInput);
            }
      lcd_gotoxy(0,0);
      lcd_puts(txt);
      return LAYER_OF_LCD;
}
int COUNT_Pressed (int pressedUP){
      if(PINA.1){
            pressedUP++;
      }
      else if (PINA.2){
            pressedUP--;
      }
      return pressedUP;
}
int COUNT_Pressed_TEMP (int TEMP_LEVEL){
      if(PINA.1){
            TEMP_LEVEL++;
      }
      else if (PINA.2){
            TEMP_LEVEL--;
      }
      sprintf(txt,"OFF     %2d \n        ___       ",TEMP_LEVEL);
      lcd_gotoxy(0,0);
      lcd_puts(txt);
      return TEMP_LEVEL;
}
int LCD_MENU(int pressedUP,int LAYER_OF_LCD){
      if(pressedUP % 3 == 0){
            sprintf(txt,"LED  FAN  TEMP \n___            ");
            lcd_gotoxy(0,0);
            lcd_puts(txt);
            if(PINA.3){LAYER_OF_LCD = 1;}
            }
      else if(pressedUP % 3 == 2){
            sprintf(txt,"LED  FAN  TEMP \n     ___       ");
            lcd_gotoxy(0,0);
            lcd_puts(txt);
            if(PINA.3){LAYER_OF_LCD = 10;}
      }
      else if(pressedUP % 3 == 1){
            sprintf(txt,"LED  FAN  TEMP \n          ___  ");
            lcd_gotoxy(0,0);
            lcd_puts(txt);
            if(PINA.3){LAYER_OF_LCD = 20;}
      }
      return LAYER_OF_LCD;
}
int LCD_CHANGE_TEMP(int LAYER_OF_LCD,int pressedUP,int TEMP_LEVEL,int tempreture){
      if(pressedUP % 2 == 0){
            sprintf(txt,"OFF     %2d  %2d \n___             ",TEMP_LEVEL,tempreture);
            lcd_gotoxy(0,0);
            lcd_puts(txt);
            if(PINA.3){TEMP_LEVEL = 0;}
            }
      else if(pressedUP % 2 == 1){
            sprintf(txt,"OFF     %2d  %2d \n        ___    ",TEMP_LEVEL,tempreture);
            lcd_gotoxy(0,0);
            lcd_puts(txt);
            if(PINA.3){
                  while(PINA.4 == 0){
                        TEMP_LEVEL = COUNT_Pressed_TEMP(TEMP_LEVEL);
                  }
            }
            if(PINA.4){
            LAYER_OF_LCD = 0;
      }
      return LAYER_OF_LCD;
      }
}
int LCD_FAN_SPEED(int LAYER_OF_LCD,int pressedUP , int FAN_LEVEL){
      if(pressedUP % 3 == 0){
            sprintf(txt,"SLOW  MED  FAST \n___           ");
            lcd_gotoxy(0,0);
            lcd_puts(txt);
            if(PINA.3){FAN_LEVEL = 100;}
            }
      else if(pressedUP % 3 == 2){
            sprintf(txt,"SLOW  MED  FAST \n     ___      ");
            lcd_gotoxy(0,0);
            lcd_puts(txt);
            if(PINA.3){FAN_LEVEL = 177;}
      }
      else if(pressedUP % 3 == 1){
            sprintf(txt,"SLOW  MED  FAST \n          ___ ");
            lcd_gotoxy(0,0);
            lcd_puts(txt);
            if(PINA.3){FAN_LEVEL = 255;}
      }
      if(PINA.4){
            LAYER_OF_LCD = 0;
      }
      return LAYER_OF_LCD;
}
void main(void)
{
// Declare your local variables here
int PotentiometerInput ;
int LedOn;
int pressedUP =0;
int LAYER_OF_LCD = 0;
int tempreture;
int TEMP_LEVEL = 30;
int FAN_LEVEL = 255;
// Input/Output Ports initialization
// Port A initialization
// Function: Bit7=In Bit6=In Bit5=In Bit4=In Bit3=In Bit2=In Bit1=In Bit0=In 
DDRA=(0<<DDA7) | (0<<DDA6) | (0<<DDA5) | (0<<DDA4) | (0<<DDA3) | (0<<DDA2) | (0<<DDA1) | (0<<DDA0);
// State: Bit7=T Bit6=T Bit5=T Bit4=T Bit3=T Bit2=T Bit1=T Bit0=T 
PORTA=(0<<PORTA7) | (0<<PORTA6) | (0<<PORTA5) | (0<<PORTA4) | (0<<PORTA3) | (0<<PORTA2) | (0<<PORTA1) | (0<<PORTA0);

// Port B initialization
// Function: Bit7=In Bit6=In Bit5=In Bit4=In Bit3=Out Bit2=In Bit1=In Bit0=In 
DDRB=(0<<DDB7) | (0<<DDB6) | (0<<DDB5) | (0<<DDB4) | (1<<DDB3) | (0<<DDB2) | (0<<DDB1) | (0<<DDB0);
// State: Bit7=T Bit6=T Bit5=T Bit4=T Bit3=0 Bit2=T Bit1=T Bit0=T 
PORTB=(0<<PORTB7) | (0<<PORTB6) | (0<<PORTB5) | (0<<PORTB4) | (0<<PORTB3) | (0<<PORTB2) | (0<<PORTB1) | (0<<PORTB0);

// Port C initialization
// Function: Bit7=In Bit6=In Bit5=In Bit4=In Bit3=Out Bit2=Out Bit1=Out Bit0=Out 
DDRC=(0<<DDC7) | (0<<DDC6) | (0<<DDC5) | (0<<DDC4) | (1<<DDC3) | (1<<DDC2) | (1<<DDC1) | (0<<DDC0);
// State: Bit7=T Bit6=T Bit5=T Bit4=T Bit3=0 Bit2=0 Bit1=0 Bit0=0 
PORTC=(0<<PORTC7) | (0<<PORTC6) | (0<<PORTC5) | (0<<PORTC4) | (0<<PORTC3) | (0<<PORTC2) | (0<<PORTC1) | (0<<PORTC0);

// Port D initialization
// Function: Bit7=In Bit6=In Bit5=In Bit4=In Bit3=In Bit2=In Bit1=In Bit0=In 
DDRD=(0<<DDD7) | (0<<DDD6) | (0<<DDD5) | (0<<DDD4) | (0<<DDD3) | (0<<DDD2) | (0<<DDD1) | (0<<DDD0);
// State: Bit7=T Bit6=T Bit5=T Bit4=T Bit3=T Bit2=T Bit1=T Bit0=T 
PORTD=(0<<PORTD7) | (0<<PORTD6) | (0<<PORTD5) | (0<<PORTD4) | (0<<PORTD3) | (0<<PORTD2) | (0<<PORTD1) | (0<<PORTD0);

// Timer/Counter 0 initialization
// Clock source: System Clock
// Clock value: 31.250 kHz
// Mode: Normal top=0xFF
// OC0 output: Disconnected
// Timer Period: 8 ms
TCCR0=(0<<WGM00) | (0<<COM01) | (0<<COM00) | (0<<WGM01) | (1<<CS02) | (0<<CS01) | (0<<CS00);
TCNT0=0x06;
OCR0=0x00;

// Timer/Counter 1 initialization
// Clock source: System Clock
// Clock value: Timer1 Stopped
// Mode: Normal top=0xFFFF
// OC1A output: Disconnected
// OC1B output: Disconnected
// Noise Canceler: Off
// Input Capture on Falling Edge
// Timer1 Overflow Interrupt: Off
// Input Capture Interrupt: Off
// Compare A Match Interrupt: Off
// Compare B Match Interrupt: Off
TCCR1A=(0<<COM1A1) | (0<<COM1A0) | (0<<COM1B1) | (0<<COM1B0) | (0<<WGM11) | (0<<WGM10);
TCCR1B=(0<<ICNC1) | (0<<ICES1) | (0<<WGM13) | (0<<WGM12) | (0<<CS12) | (0<<CS11) | (0<<CS10);
TCNT1H=0x00;
TCNT1L=0x00;
ICR1H=0x00;
ICR1L=0x00;
OCR1AH=0x00;
OCR1AL=0x00;
OCR1BH=0x00;
OCR1BL=0x00;

// Timer/Counter 2 initialization
// Clock source: System Clock
// Clock value: Timer2 Stopped
// Mode: Normal top=0xFF
// OC2 output: Disconnected
ASSR=0<<AS2;
TCCR2=(1<<WGM20) | (0<<COM21) | (0<<COM20) | (1<<WGM21) | (1<<CS22) | (0<<CS21) | (0<<CS20);
TCNT2=0x00;
OCR2=0x00;

// Timer(s)/Counter(s) Interrupt(s) initialization
TIMSK=(0<<OCIE2) | (0<<TOIE2) | (0<<TICIE1) | (0<<OCIE1A) | (0<<OCIE1B) | (0<<TOIE1) | (0<<OCIE0) | (1<<TOIE0);

// External Interrupt(s) initialization
// INT0: Off
// INT1: Off
// INT2: Off
MCUCR=(0<<ISC11) | (0<<ISC10) | (0<<ISC01) | (0<<ISC00);
MCUCSR=(0<<ISC2);

// USART initialization
// USART disabled
UCSRB=(0<<RXCIE) | (0<<TXCIE) | (0<<UDRIE) | (0<<RXEN) | (0<<TXEN) | (0<<UCSZ2) | (0<<RXB8) | (0<<TXB8);

// Analog Comparator initialization
// Analog Comparator: Off
// The Analog Comparator's positive input is
// connected to the AIN0 pin
// The Analog Comparator's negative input is
// connected to the AIN1 pin
ACSR=(1<<ACD) | (0<<ACBG) | (0<<ACO) | (0<<ACI) | (0<<ACIE) | (0<<ACIC) | (0<<ACIS1) | (0<<ACIS0);

// ADC initialization
// ADC Clock frequency: 1000.000 kHz
// ADC Voltage Reference: AREF pin
// ADC Auto Trigger Source: ADC Stopped
ADMUX=ADC_VREF_TYPE;
ADCSRA=(1<<ADEN) | (0<<ADSC) | (0<<ADATE) | (0<<ADIF) | (0<<ADIE) | (0<<ADPS2) | (1<<ADPS1) | (1<<ADPS0);
SFIOR=(0<<ADTS2) | (0<<ADTS1) | (0<<ADTS0);

// SPI initialization
// SPI disabled
SPCR=(0<<SPIE) | (0<<SPE) | (0<<DORD) | (0<<MSTR) | (0<<CPOL) | (0<<CPHA) | (0<<SPR1) | (0<<SPR0);

// TWI initialization
// TWI disabled
TWCR=(0<<TWEA) | (0<<TWSTA) | (0<<TWSTO) | (0<<TWEN) | (0<<TWIE);

// Alphanumeric LCD initialization
// Connections are specified in the
// Project|Configure|C Compiler|Libraries|Alphanumeric LCD menu:
// RS - PORTB Bit 0
// RD - PORTB Bit 1
// EN - PORTB Bit 2
// D4 - PORTB Bit 4
// D5 - PORTB Bit 5
// D6 - PORTB Bit 6
// D7 - PORTB Bit 7
// Characters/line: 20
lcd_init(20);



// Global enable interrupts
#asm("sei")


while (1)
      {
            LedOn = PINC.0;
            if(LedOn){
                  PotentiometerInput = read_adc(5);
                  OCR2 = PotentiometerInput;
                  PwmLed_init();
            }
            else{
                  PORTD.7 = LedOn;
                  PwmLed_init_rev();
            }
            tempreture = read_adc(0);
            tempreture = (int)((float)tempreture/1023*5*100);
            if(tempreture > TEMP_LEVEL){
                  OCR0 = FAN_LEVEL;
                  PwmFAN_init();
            }
            else{
                  PORTB.3 = 0;
                  PwmFAN_init_rev();
            }
            pressedUP = COUNT_Pressed(pressedUP);
            if (LAYER_OF_LCD == 0){
                  LAYER_OF_LCD = LCD_MENU(pressedUP,LAYER_OF_LCD);
            }
            else if (LAYER_OF_LCD == 1){
                  LAYER_OF_LCD = LCD_LED_LEVEL(LedOn,LAYER_OF_LCD,PotentiometerInput);
            }
            else if (LAYER_OF_LCD == 20){
                  LAYER_OF_LCD = LCD_CHANGE_TEMP(LAYER_OF_LCD,pressedUP,TEMP_LEVEL,tempreture);
            }
            else if (LAYER_OF_LCD == 10){
                  LAYER_OF_LCD = LCD_FAN_SPEED(LAYER_OF_LCD,pressedUP,FAN_LEVEL);
            }
            
      }
}
