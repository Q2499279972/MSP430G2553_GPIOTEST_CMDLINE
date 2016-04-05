/*
 * main.c
 */
#include  <msp430.h>
#include "cli.hpp"
#include "uart.hpp"

int timerflag = 0;
void sysinit()
{
    // Initialize System Clocks
    WDTCTL = WDTPW + WDTHOLD;             // Stop watchdog timer
    BCSCTL1 = CALBC1_16MHZ;                // Set DCO to 1, 8, 12 or 16MHz
    DCOCTL = CALDCO_16MHZ;
}




void ioinit()
{
	P1DIR&=~BIT0;
	P1DIR&=~BIT3;
	P1DIR&=~BIT4;
	P1DIR&=~BIT5;
	P1DIR&=~BIT6;
	P1DIR&=~BIT7;

	P2DIR&=~BIT0;
	P2DIR&=~BIT1;
	P2DIR&=~BIT2;
	P2DIR&=~BIT3;
	P2DIR&=~BIT4;
	P2DIR&=~BIT5;

	//P1REN|=0xf9;
	//P2REN|=0x3f;
}

void timerinit()
{
  CCTL0 = CCIE;                             // CCR0 interrupt enabled
  CCR0 = 50000;
  TACTL = TASSEL_2 + MC_1 + ID_3;                  // SMCLK, upmode
}

#pragma vector=TIMER0_A0_VECTOR
__interrupt void Timer_A (void)
{
	static int cnt=0;
	cnt++;
	if(cnt==80)
	{
		cnt=0;
		timerflag =1;
	}
}
void printiostate()
{
	unsigned char i;
	unsigned char bitmask=1;
	char str[]="p1.0(I)=1\r\n";
	for(i=0;i<8;i++)
	{
		if(i==1||i==2)
		{
			bitmask<<=1;
			continue;
		}
		if(P1DIR&bitmask)
		{
			str[3]=i+'0';
			str[5]='O';
			str[8]=(P1OUT&bitmask)?'1':'0';
		}
		else
		{
			str[3]=i+'0';
			str[5]='I';
			str[8]=(P1IN&bitmask)?'1':'0';
		}
		bitmask<<=1;
		uart_send_string(str);
	}

	char str2[]="p2.0(I)=1\r\n";
	bitmask=1;
	for(i=0;i<8;i++)
	{
		if(i==6||i==7)
		{
			bitmask<<=1;
			continue;
		}
		if(P2DIR&bitmask)
		{
			str2[3]=i+'0';
			str2[5]='O';
			str2[8]=(P2OUT&bitmask)?'1':'0';
		}
		else
		{
			str2[3]=i+'0';
			str2[5]='I';
			str2[8]=(P2IN&bitmask)?'1':'0';
		}
		bitmask<<=1;
		uart_send_string(str2);
	}
}



Cli cli;


int main(void) {
	sysinit();
	uartinit();
	ioinit();
	timerinit();
	ClearScrean();
	while(1)
	{
		unsigned char buffer;
		if(QUEUERead(&rx_buf,&buffer))
		{
			HideCursor();
			SetCursor1(16,1);
			cli.AcceptKey(buffer);
			cli.Flush();
			SetCursor1(16,6+cli.Cmdline.m_CurrentCursorPosition);
			ShowCursor();
		}
		if(timerflag==1||cli.RunOKFlag==1)
		{
			cli.RunOKFlag=0;
			timerflag = 0;
			HideCursor();
			SetCursor1(1,1);
			printiostate();

			SetCursor1(16,1);
			//cli.AcceptKey(buffer);
			cli.Flush();
			SetCursor1(16,6+cli.Cmdline.m_CurrentCursorPosition);
			ShowCursor();
		}
	}
}
