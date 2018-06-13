#include "ansicontrol.hpp"
#include "uart.hpp"

void SetColor(Color color)
{
	char str[] = "\033[31;40m";
	str[3] = color+'0';
	__stdout << str;
}

void ClearSetting()
{
	char str[] = "\033[0m";
	__stdout << str;
}

void ClearScrean()
{
	char str[]="\033[2J";
	__stdout << str;
}

void ClearToEnd()
{
	char str[]="\033[K";
	__stdout << str;
}

void HideCursor()
{
	char str[]="\033[?25l";
	__stdout << str;
}

void ShowCursor()
{
	char str[]="\033[?25h";
	__stdout << str;
}


void MoveCursor(Direction dir, int n)
{
	if(n<10)
	{
		char str[]="\033[nA";
		str[2]=n+'0';
		switch (dir)
		{
			case UP:
				str[3] = 'A';
				break;
			case DOWN:
				str[3] = 'B';
				break;
			case LEFT:
				str[3] = 'D';
				break;
			case RIGHT:
				str[3] = 'C';
				break;
			default:
				break;
		}
		__stdout << str;
	}
	else if(n<100)
	{
		char str[]="\033[nnA";
		str[2]=n/10+'0';
		str[3]=n%10+'0';
		switch (dir)
		{
			case UP:
				str[4] = 'A';
				break;
			case DOWN:
				str[4] = 'B';
				break;
			case LEFT:
				str[4] = 'D';
				break;
			case RIGHT:
				str[4] = 'C';
				break;
			default:
				break;
		}
		__stdout << str;
	
	}
	else
	{
	}
}

void SetCursor1(int y, int x)
{
	if(x>99) x=99;
	if(y>99) y=99;
	if(x<0) x=0;
	if(y<0) y=0;

	char str[20]="\033[";
	int strlen = 0;
	if(y>=10)
	{
		str[2] = y/10 + '0';
		str[3] = y%10 + '0';
		str[4] = ';';
		if(x>=10)
		{
			str[5] = x/10 + '0';
			str[6] = x%10 + '0';
			str[7] = 'H';
			strlen = 8;
		}
		else
		{
			str[5] = x + '0';
			str[6] = 'H';
			strlen = 7;
		}
	}
	else
	{
		str[2] = y + '0';
		str[3] = ';';
		if(x>=10)
		{
			str[4] = x/10 + '0';
			str[5] = x%10 + '0';
			str[6] = 'H';
			strlen = 7;
		}
		else
		{
			str[4] = x + '0';
			str[5] = 'H';
			strlen = 6;
		}
	}

	int i = 0;
	while(i<strlen)
	{
		__stdout << str[i];
		i++;
	}
}
