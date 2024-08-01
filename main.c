/*
 * lab3 digital2 slave.c
 *
 * Created: 26/07/2024 10:43:23
 * Author : diego
 */ 

#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <stdint.h>
#include <avr/interrupt.h>
#include "Librerias/ADC.h"
#include "Librerias/SPI.h"
volatile uint8_t adc_activo = 0;
volatile uint8_t pot1 = 0;
volatile uint8_t pot2 = 0;

int main(void)
{
	cli();
	ADC_(128);
	ADC1();
	spi_init(slave_ss, MSB, idle_high, first_edge);
	ADCSRA |= (1<<ADSC);//HABILITAR CONVERSION
	SPCR |= (1<<SPIE); //activar int spi
	sei();
    /* Replace with your application code */
    while (1) 
    {
		_delay_ms(100);
		ADCSRA |= (1<<ADSC);//HABILITAR CONVERSION
    }
}

ISR(ADC_vect){
	adc_activo = ADMUX & ((1<<MUX3) | (1<<MUX2) | (1<<MUX1) | (1<<MUX0));
	ADCSRA &= ~(1<<ADEN);
	if (adc_activo == 1){
		pot1 = ADCH;
		ADC2();
		} else if (adc_activo == 2){
		pot2 = ADCH;
		ADC1();
	}
	ADCSRA |= (1<<ADIF); //apagar bandera
	ADCSRA |= (1<<ADEN);
}
ISR(SPI_STC_vect){//Interrupcion del SPI
	uint8_t valor = SPDR;
	if (valor == 'q'){
		spi_write(pot1);
	}else if(valor == 'w'){
		spi_write(pot2);
	}
}
