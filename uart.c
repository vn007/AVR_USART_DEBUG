/*
 * uart.c
 *
 * Created: 02.08.2016 4:04:36
 *  Author: LV
 */

#include "uart.h"
#include "math.h"

void init_UART(unsigned long baudrate)
{
	uint8_t ubrr = round(F_CPU/(16*baudrate))-1;
	//	��������� �������� UART
	UBRR0H=(uint8_t)(ubrr>>8);	//	UBRR=f/(16*baud)-1 f=8000000�� baud=9600,
//	UBRR0L=25;	//	���������� ����������� ��������������� ����� ������, f=16000000�� baud=38400, ��������
//	UBRR0L=51;	//	���������� ����������� ��������������� ����� ������, f=16000000�� baud=19200
//	UBRR0L=103;	//	���������� ����������� ��������������� ����� ������, f=16000000�� baud=9600
	UBRR0L=(uint8_t)ubrr;	//	UBRR=f/(16*baud)-1 f=8000000�� baud=9600,


	//			RXC			-	���������� �����
	//			|TXC		-	���������� ��������
	//			||UDRE 		-	���������� ������ ��� ��������
	//			|||FE		-	������ �����
	//			||||DOR		-	������ ������������ ������
	//			|||||PE		-	������ ��������
	//			||||||U2X	-	������� ��������
	//			|||||||MPCM	-	����������������� �����
	//			76543210
	UCSR0A =  0b00000000;

	//			UMSEL01		-	1 ��� USART
	//			|UMSEL00	-	�����:1-���������� 0-�����������
	//			||UPM1		-	UPM0:1 ��������
	//			|||UPM0		-	UPM0:1 ��������
	//			||||USBS	-	���� ����: 0-1, 1-2
	//			|||||UCSZ1	-	UCSZ0:2 ������ ����� ������
	//			||||||UCSZ0	-	UCSZ0:2 ������ ����� ������
	//			|||||||UCPOL-	� ���������� ������ - ������������
	//			76543210
	UCSR0C =  0b00000110;	//	8-������� �������

	//			RXCIE		-	���������� ��� ����� ������
	//			|TXCIE		-	���������� ��� ���������� ��������
	//			||UDRIE		-	���������� ���������� ������ ��� ��������
	//			|||RXEN		-	���������� �����
	//			||||TXEN	-	���������� ��������
	//			|||||UCSZ2	-	UCSZ0:2 ������ ����� ������
	//			||||||RXB8	-	9 ��� �������� ������
	//			|||||||TXB8	-	9 ��� ���������� ������
	//			76543210
	UCSR0B =  0b10011000;	//	��������� ���� � �������� �� UART, ���������� ��� ����� ������
}
//---------------------------------------------------------------------------------

void send_Uart(unsigned char c)		//	�������� �����
{
	while(!(UCSR0A&(1<<UDRE0)))	{}	//	���������������, ����� ������� ��������

	UDR0 = c;
}
//---------------------------------------------------------------------------------

void send_Uart_str(unsigned char *s)	//	�������� ������
{
	while (*s != 0) send_Uart(*s++);
}
//---------------------------------------------------------------------------------

void send_int_Uart(unsigned int c)		//	�������� ����� �� 0000 �� 9999
{
	unsigned char temp;
	c=c%10000;
	temp=c/100;
	send_Uart(temp/10+'0');
	send_Uart(temp%10+'0');
	temp=c%100;;
	send_Uart(temp/10+'0');
	send_Uart(temp%10+'0');
}
//---------------------------------------------------------------------------------

unsigned char getch_Uart(void)		//	��������� �����
{
	while(!(UCSR0A&(1<<RXC0))) {}	//	���������������, ����� ������� ��������

	return UDR0;
}
//---------------------------------------------------------------------------------
