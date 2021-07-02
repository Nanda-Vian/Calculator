/*
 * GccApplication2.c
 *
 * Created: 6/27/2021 4:06:57 PM
 * Author : Nanda Vian
 */ 
#define F_CPU 8000000UL

#define Satu		0x31
#define Dua			0x32
#define Tiga		0x33
#define Empat		0x34
#define Lima		0x35
#define Enam		0x36
#define Tujuh		0x37
#define Delapan		0x38
#define Sembilan	0x39
#define Nol			0x30
#define Tambah		0x2B
#define Kurang		0x2D
#define Kali		0x2A
#define Bagi		0xFD
#define Sama_Dengan	0x3D



#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <avr/pgmspace.h>
#include <string.h>
#include <stdint.h>
#include <stdio.h>
#include "lcd.h"


unsigned char simbol_angka[10]	= {Satu,Dua,Tiga,Empat,Lima,Enam,Tujuh,Delapan,Sembilan,Nol};
unsigned char simbol[6]			= {Tambah,Kurang,Bagi,Kali,Sama_Dengan}; 
char print_buffer[20], Operator[8];
volatile unsigned char Angka_Huruf;
volatile uint8_t Angka_Sementara, Busy_Flag;
uint8_t Key_Pressed, i;
uint8_t Counter_Operator =0, Counter_Operand = 0, Counter_angka = 0, Counter_LCD = 0; //Counter
uint8_t Operator_flag = 0, Sign_flag = 0, Output_flag = 0; 
double operand[8];

void RESET_CALC(void);
void Perhitungan(uint8_t Posisi_operand, uint8_t Posisi_Operator);
	
const unsigned char Kelebihan[] PROGMEM = "Kelebihan CUK!";
const unsigned char Masukin[] PROGMEM = "Masukin Angka!";

int main(void)
{
	//inisialisasi display
	lcd_init(LCD_DISP_ON);
	
	//inisialisasi  port
	DDRD  = ((1<<DDD4)|(1<<DDD5)|(1<<DDD6)|(1<<DDD7));
	PORTD = 0xFF;
	PORTC = (1<<PORTC5);
	
	//Insialisasi intterupt
	EIMSK = 0x03;
	PCICR = ((1<<PCIE2)|(1<<PCIE1));
	PCMSK2 = (1<<PCINT17);
	PCMSK1 = (1<<PCINT13);
	sei();
	
	
	
	
 	while(1){
		  while (Busy_Flag == 0)
		  {
			  Key_Pressed = 1;
			  PORTD ^= ((1<<PORTD4));
			  _delay_loop_1(50);
			  PORTD ^= ((1<<PORTD4));
			  Key_Pressed = 2;
			  PORTD ^= ((1<<PORTD5));
			  _delay_loop_1(50);
			  PORTD ^= ((1<<PORTD5));
			  Key_Pressed = 3;
			  PORTD ^= ((1<<PORTD6));
			  _delay_loop_1(50);
			  PORTD ^= ((1<<PORTD6));
			  Key_Pressed = 4;
			  PORTD ^= ((1<<PORTD7));
			  _delay_loop_1(50);
			  PORTD ^= ((1<<PORTD7));
			  _delay_loop_1(50);
		  }
		  
		  lcd_putc(Angka_Huruf);
		  /*
		  if(Counter_LCD<=16){
			  if(Counter_angka==0){
				  operand[Counter_Operand]= Angka_Sementara;
			  }
			  
			  operand[Counter_Operand] *= 10.0;
			  operand[Counter_Operand] += Angka_Sementara;
			  
			  if(Angka_Huruf==Tambah||Angka_Huruf==Kurang||Angka_Huruf==Kali||Angka_Huruf==Bagi){
				  Counter_Operator++;
				  Operator[Counter_Operator] = Angka_Huruf;
				  Counter_Operand++;
			  }
			  
			  if(Counter_Operand >8 || Counter_Operator >8){
				  lcd_puts_p((char*) Kelebihan);
				  RESET_CALC();
			  }
	
			  if(Angka_Huruf==Sama_Dengan){
				  if(Counter_Operator==0){
					lcd_puts_p((char*) Masukin);
				  }
				  					  
			  }
			  Counter_angka++;
			  			  				  
		  } else {
			  lcd_puts_p((char*) Kelebihan);
			  RESET_CALC();
		  }*/
		  sei();
		  Busy_Flag = 0;
		    
		  _delay_loop_2(0xFF00);
		  

	}
}

ISR (PCINT2_vect){
	cli();
	if(Key_Pressed==1){
		Angka_Huruf = Empat;
		Angka_Sementara = 4;
	} else if(Key_Pressed==2){
		Angka_Huruf = Lima;
		Angka_Sementara = 5;
	} else if(Key_Pressed==3){
		Angka_Huruf = Enam;
		Angka_Sementara = 6;
	} else	Angka_Huruf = Tambah;
	Busy_Flag = 1;
	
}

ISR (PCINT1_vect){
	cli();
	if(Key_Pressed==1){
		Busy_Flag = 1;
		Angka_Huruf = Satu;
		Angka_Sementara = 1;
	} else if(Key_Pressed==2){
		Busy_Flag = 1;
		Angka_Huruf = Dua;
		Angka_Sementara = 2;
	} else if(Key_Pressed==3){
		Busy_Flag = 1;
		Angka_Huruf = Tiga;
		Angka_Sementara = 3;
	} else lcd_clrscr();
}

ISR (INT0_vect){
	cli();
	if(Key_Pressed==1){
		Angka_Huruf = Tujuh;
		Angka_Sementara = 7;
	} else if(Key_Pressed==2){
		Angka_Huruf = Delapan;
		Angka_Sementara = 8;
	} else if(Key_Pressed==3){
		Angka_Huruf = Sembilan;
		Angka_Sementara = 9;
	} else	Angka_Huruf = Kurang;
	Busy_Flag = 1;
}

ISR (INT1_vect){
	cli();
	if(Key_Pressed==1){
		Angka_Huruf = Kali;
	} else if(Key_Pressed==2){
		Angka_Huruf = Nol;
		Angka_Sementara = 0;
	} else if(Key_Pressed==3){
		Angka_Huruf = Bagi;
	} else Angka_Huruf = Sama_Dengan;
	Busy_Flag = 1;
}

/*
void RESET_CALC (void){
	for(i = 0; i <= 8; ++i){
		Operator[i] = 0;
		operand[i] = 0.0;
	}
	Counter_Operand = Counter_Operator = Counter_angka = Counter_LCD = 0;
}

void Perhitungan(uint8_t Posisi_operand, uint8_t Posisi_Operator){
	
}*/


