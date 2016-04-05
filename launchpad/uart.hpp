#ifndef UART_HPP_
#define UART_HPP_
#include  <msp430.h>
typedef struct QUEUE
{
	unsigned char head;
	unsigned char tail;
	unsigned char buffer[64];
}QUEUE;

extern QUEUE rx_buf;
void QUEUEInit(QUEUE *p);
unsigned char QUEUERead(QUEUE *p, unsigned char *data);
unsigned char QUEUEWrite(QUEUE *p, unsigned char data);
void uartinit();
void uart_send(unsigned char data);
void uart_send_string(char *str);

class OStream
{
public:
	OStream(){}
	virtual void PutChar(char c) = 0;
	virtual void PutString(char* s) = 0;
};

class StdOStream: public OStream
{
public:
	StdOStream(){}
	void PutChar(char c)
	{
		uart_send(c);
	}
	void PutString(char *s)
	{
		uart_send_string(s);
	}
};

class ErrOStream: public OStream
{
public:
	ErrOStream(){}
	void PutChar(char c)
	{
	}
	void PutString(char *s)
	{
	}
};

class DebugOStream: public OStream
{
public:
	DebugOStream(){}
	void PutChar(char c)
	{
	}
	void PutString(char *s)
	{
	}
};
OStream &  operator<<(OStream& stream,char c);
OStream &  operator<<(OStream& stream,char*c);
extern StdOStream   __stdout;
extern ErrOStream   __errout;
extern DebugOStream __debugout;

#endif /* UART_HPP_ */
