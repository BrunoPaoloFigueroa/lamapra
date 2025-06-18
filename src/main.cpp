#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

void config_intext(void){
  EIMSK |= (1<<INT0);
  EICRA |= (1<<ISC01);
  DDRD &= ~(1<<PD2);
}

void config_ADC(void){
  ADCSRA |= (1<<ADEN)|(1<<ADIE)|(1<<ADPS2);
  ADMUX |= (1<<REFS0);
}

unsigned char porcentaje = 0;
ISR (ADC_vect){
  porcentaje = ADC*(100.0/1023.0);
  OCR0A= porcentaje*(1.2)+5;
  ADCSRA |= (1<<ADSC);
}

void config_timer(void){
  TCCR0A |= (1<<WGM01)|(1<<WGM00);
  OCR0A=60;
}

ISR(INT0_vect){
  PORTB |= 0x01;
  TCNT0 = 0;
  TCCR0B |= (1<<CS02)|(1<<CS00);
  while(!(TIFR0&(1<<OCF0A)));
  TIFR0 |= (1<<OCF0A);
  TCCR0B &=~((1<<CS02)|(1<<CS00));
  PORTB &=~0X01;
}

int main(void){
  config_intext();
  config_ADC();
  config_timer();
  sei();
  ADCSRA |= (1<<ADSC);
  DDRB|=0x01;
  while(1);
}