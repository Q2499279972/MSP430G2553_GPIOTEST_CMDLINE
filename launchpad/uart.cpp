#include "uart.hpp"

QUEUE rx_buf;
void QUEUEInit(QUEUE *p)
{
	p->head = 0;
	p->tail = 0;
}

unsigned char QUEUERead(QUEUE *p, unsigned char *data)
{
	if(p->head == p->tail)
	{
		return 0;
	}
	else
	{
		data[0] = p->buffer[p->head];
		p->head++;
		p->head&=63;
		return 1;
	}
}

unsigned char QUEUEWrite(QUEUE *p, unsigned char data)
{
	if( ((p->tail + 1)& 63 ) == p->head )
	{
		return 0;
	}
	else
	{
		p->buffer[p->tail] = data;
		p->tail++;
		p->tail&=63;
		return 1;
	}
}

void uartinit()
{
	QUEUEInit(&rx_buf);
	UCA0CTL1 |= UCSSEL_2;                     // SMCLK
	UCA0BR0 = 131;                            // 16MHz 9600
	UCA0BR1 = 6;                              // 16MHz 9600
	UCA0MCTL = UCBRS0;                        // Modulation UCBRSx = 1
	UCA0CTL1 &= ~UCSWRST;                     // **Initialize USCI state machine**
	IE2 |= UCA0RXIE;                          // Enable USCI_A0 RX interrupt
	_EINT();                                  // Enable interrupt
	P1SEL = BIT1 + BIT2 ; // P1.1 = RXD, P1.2=TXD
	P1SEL2 = BIT1 + BIT2 ; // P1.1 = RXD, P1.2=TXD
}

#pragma vector=USCIAB0RX_VECTOR
__interrupt void USCI0RX_ISR(void)
{
    while (!(IFG2&UCA0TXIFG));                // USCI_A0 TX buffer ready?
    char data = UCA0RXBUF;
    QUEUEWrite(&rx_buf,data);
}

void uart_send(unsigned char data)
{

    while(!(IFG2& UCA0TXIFG));
    UCA0TXBUF = data;
    IFG2&=~UCA0TXIFG;
}

void uart_send_string(char *str)
{
	while((*str)!=0)
	{
		uart_send(*str);
		str++;
	}
}



StdOStream   __stdout;
ErrOStream   __errout;
DebugOStream __debugout;

OStream & operator<<(OStream& stream,char c)
{
	stream.PutChar(c);
	return stream;
}

OStream & operator<<(OStream& stream,char* s)
{
	stream.PutString(s);
	return stream;
}

