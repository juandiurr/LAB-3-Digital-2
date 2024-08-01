/*
 * lab3 digital2.c
 *
 * Created: 25/07/2024 20:02:54
 * Author : diego
 */ 

#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <stdint.h>
#include <avr/interrupt.h>
#include "Librerias/UART.h"
#include "Librerias/SPI.h"
volatile uint8_t uart = 0;
volatile uint8_t num = 0;
uint8_t pot1 = 0;
uint8_t pot1u = 0;
uint8_t pot1d = 0;
uint8_t pot1c = 0;
uint8_t pot2 = 0;
uint8_t pot2u = 0;
uint8_t pot2d = 0;
uint8_t pot2c = 0;
uint8_t lol = 0;



int main(void)
{
	cli();
	DDRD |= (1<<DDD2) | (1<<DDD3) | (1<<DDD4) | (1<<DDD5) | (1<<DDD6) | (1<<DDD7);//leds
	PORTD = 0;
	DDRC |= (1<<DDC5) | (1<<DDC6);//leds
	PORTC = 0;
	spi_init(div16, MSB, idle_high, first_edge);
	init_uart();
	/*writetextUART("Seleccione valor de potenciometro 1 (q)  o potenciometro 2 (w)");
	writeUART(10);
	writetextUART("Presione e para mandar un numero a los leds");
	writeUART(10);*/
	sei();
    /* Replace with your application code */
    while (1) 
    {
		
		if (uart == 'q'){//potenciometro 1
			PORTB &= ~(1<<PORTB2); //SLAVE SELECT
			spi_write('q');
			pot1 = spi_Read();
			//writetextUART("Valor potenciometro 1: ");
			pot1u = pot1%10;// + 48;
			pot1d = (pot1%100)/10;// + 48;
			pot1c = pot1/100;// + 48;
			writeUART(pot1c);
			_delay_ms(10);
			//writeUART('.');
			writeUART(pot1d);
			_delay_ms(10);
			writeUART(pot1u);
			_delay_ms(10);
			writeUART('p');
			
			/*if (pot1 > 0 && pot1 <= 10){
				writeUART('1');
			}else if (pot1 > 10 && pot1 <= 20){
				writeUART('2');
			}else if (pot1 > 20 && pot1 <= 30){
				writeUART('3');
			}else if (pot1 > 30 && pot1 <= 40){
				writeUART('4');
			}else if (pot1 > 40 && pot1 <= 50){
				writeUART('5');
			}else if (pot1 > 50 && pot1 <= 60){
				writeUART('6');
			}else if (pot1 > 60 && pot1 <= 70){
				writeUART('7');
			}else if (pot1 > 70 && pot1 <= 80){
				writeUART('8');
			}else if (pot1 > 80 && pot1 <= 90){
				writeUART('9');
			}*/
			
				
			PORTB |= (1<<PORTB2);
			uart = 1;
		}else if(uart == 'w'){
			PORTB &= ~(1<<PORTB2); //SLAVE SELECT
			spi_write('w');
			pot2 = spi_Read();
			//writetextUART("Valor potenciometro 2: ");
			pot2u = pot2%10;// + 48;
			pot2d = (pot2%100)/10;// + 48;
			pot2c = pot2/100;// + 48;
			writeUART(pot2c);
			_delay_ms(10);
			//writeUART('.');
			writeUART(pot2d);
			_delay_ms(10);
			writeUART(pot2u);
			_delay_ms(10);
			writeUART('p');
			PORTB |= (1<<PORTB2);
			uart = 1;
		}
		if (uart == 1){
			/*writetextUART("Seleccione valor de potenciometro 1 o potenciometro 2");
			writeUART(10);
			writetextUART("Presione e para mandar un numero a los leds");
			writeUART(10);*/
			uart = 0;
		}
		if (num < 256){
			if(uart == 'e'){
				PORTD = num;
				PORTC = (num<<4);
				num = 0;
				lol = 0;
				uart = 1;
			}
		}else{
			//writetextUART("numero mayor que 255");
			uart = 1;
		}
    }
}


ISR(USART_TX_vect){
	pot1 = pot1;
	pot2 = pot2;
}
ISR(USART_RX_vect){
	uart = UDR0;
	if (uart != 'e' && uart != 'q' && uart != 'w'){
		if (lol == 0){
			num = (uart - 48)*100;
			lol = 1;
		}else if (lol == 1){
			num = num + (uart - 48)*10;
			lol = 2;
		}else if (lol == 2){
			num = num + (uart - 48);
			lol = 0;
		}
		
	}
	
}


