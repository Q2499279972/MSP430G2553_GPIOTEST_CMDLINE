#include "cli.hpp"
#include "uart.hpp"
#include <string.h>

int setdir(int pin,int port,int dir)
{
	if(pin!=1 && pin!=2) return -1;
	if(port<0 || port>8) return -2;
	if(dir!=0 && dir!=1) return -3;
	if(pin==1)
	{
		if(dir==1)
			P1DIR|=(1<<port);
		else
			P1DIR&=~(1<<port);
	}
	else
	{
		if(dir==1)
			P2DIR|=(1<<port);
		else
			P2DIR&=~(1<<port);
	}
	return 0;
}
int setvalue(int pin,int port,int value)
{
	if(pin!=1 && pin!=2) return -1;
	if(port<0 || port>8) return -2;
	if(value!=0 && value!=1) return -3;
	if(pin==1)
	{
		if(value==1)
			P1OUT|=(1<<port);
		else
			P1OUT&=~(1<<port);
	}
	else
	{
		if(value==1)
			P2OUT|=(1<<port);
		else
			P2OUT&=~(1<<port);
	}
	return 0;
}

int simple_str2int(char* str,int* value)
{
	int n=0;
	while(*str!=0)
	{
		if(*str<='9'&&*str>='0')
		{
			n*=10;
			n+=(*str)-'0';
			str++;
		}
		else
		{
			return 0;
		}
	}
	*value = n;
	return 1;
}

Line::Line()
{
	m_CurrentCursorPosition = 0;
	m_CurrentCharacterNumber = 0;
}

void Line::AcceptKeyCR(){}
void Line::AcceptKeyTAB(){}
void Line::AcceptKeyDEL()
{
	if(m_CurrentCursorPosition == m_CurrentCharacterNumber)
		return ;
	//m_CurrentCursorPosition;
	m_CurrentCharacterNumber--;
	int i;
	for(i=m_CurrentCursorPosition;i<m_CurrentCharacterNumber;i++)
	{
		Contents[i]=Contents[i+1];
		//__stdout<<Contents[i];
	}
	//__stdout<<' ';
	//MoveCursor(LEFT,m_CurrentCharacterNumber-m_CurrentCursorPosition+1);
}
void Line::AcceptKeyBS()
{
	if(m_CurrentCursorPosition ==0)
		return ;
	int i;
	m_CurrentCursorPosition --;
	m_CurrentCharacterNumber --;
	//MoveCursor(LEFT);
	for(i=m_CurrentCursorPosition;i<m_CurrentCharacterNumber;i++)
	{
		Contents[i]=Contents[i+1];
		//__stdout<<Contents[i];
	}
	//__stdout<<' ';
	//MoveCursor(LEFT,m_CurrentCharacterNumber-m_CurrentCursorPosition+1);
}
void Line::AcceptKeyETX()
{}
void Line::AcceptKeyNormal(unsigned char key)
{
	if(m_CurrentCharacterNumber >= MAX_CHARACTERS)//–¥≤ªœ¬¡À
	{
		return;
	}
	else
	{
		if(m_CurrentCursorPosition == m_CurrentCharacterNumber)
		{
			Contents[m_CurrentCursorPosition] = key;
			m_CurrentCursorPosition++;
			m_CurrentCharacterNumber++;
			//__stdout << key;
		}
		else
		{
			int i;
			for(i=m_CurrentCharacterNumber;i>m_CurrentCursorPosition;i--)
			{
				Contents[i] = Contents[i-1];
			}
			Contents[m_CurrentCursorPosition] = key;
			m_CurrentCursorPosition++;
			m_CurrentCharacterNumber++;

			//__stdout << key;
			for(i=0;i<m_CurrentCharacterNumber-m_CurrentCursorPosition;i++)
			{
				//__stdout << Contents[m_CurrentCursorPosition+i];
			}
			//MoveCursor(LEFT,m_CurrentCharacterNumber-m_CurrentCursorPosition);
		}
	}
}
void Line::AcceptKeyUP(){}
void Line::AcceptKeyDown(){}
void Line::AcceptKeyLeft()
{
	if(m_CurrentCursorPosition>0)
	{
		m_CurrentCursorPosition--;
		//MoveCursor(LEFT);
	}
}
void Line::AcceptKeyRight()
{
	if(m_CurrentCursorPosition<m_CurrentCharacterNumber)
	{
		m_CurrentCursorPosition++;
		//MoveCursor(RIGHT);
	}
}
void Line::AcceptKeyHome()
{
	while(m_CurrentCursorPosition>0)
	{
		m_CurrentCursorPosition--;
		//MoveCursor(LEFT);
	}
}
void Line::AcceptKeyEnd()
{
	while(m_CurrentCursorPosition<m_CurrentCharacterNumber)
	{
		m_CurrentCursorPosition++;
		//MoveCursor(RIGHT);
	}
}

void Line::Flush()
{
	int i;
	for(i=0;i<m_CurrentCharacterNumber;i++)
		__stdout<<Contents[i];
}




void Cli::AcceptKey(unsigned char key)
{
	switch (FSMKey)
	{
		case 0:
		{
			if(key == KEY_ESC)
			{
				FSMKey = 1;
			}
			else
			{
				KeyNormal(key);
			}
			return;
		}
		case 1:
		{
			if(key == 0x5b)
			{
				FSMKey = 2;
			}
			else
			{
				FSMKey = 0;
			}
			return;
		}
		case 2:
		{
			switch (key)
			{
				case KEY_UP:
					KeyUp();
					break;
				case KEY_DOWN:
					KeyDown();
					break;
				case KEY_LEFT:
					KeyLeft();
					break;
				case KEY_RIGHT:
					KeyRight();
					break;
				case KEY_HOME:
					KeyHome();
					break;
				case KEY_END:
					KeyEnd();
					break;
				default:
					KeyOthers(key);
					break;
			}
			FSMKey = 0;
			return;
		}
	}
}

void Cli::KeyNormal(unsigned char key)
{
	if(key >= 0x20 && key <= 0x7e)
	{
		this->Cmdline.AcceptKeyNormal(key);
	}
	else
	{
		if(key == KEY_CR)
		{
			char pos[21]={0};
			char cmd[41];// BAD IDEA HERE
			char cmdlem=this->Cmdline.m_CurrentCharacterNumber;
			this->Cmdline.CopyToArray(cmd);
			int k=cmdpaser(cmd,cmdlem,pos);
			int i;
			for(i=0;i<cmdlem;i++)
			{
				if(cmd[i]==' ') cmd[i]=0;
			}
			if(k>4)
			{
				Statueline.SetLine("err! too many args");
				goto CR_RET;
			}
			else if(k!=0)
			{
				if(strcmp("setdir",cmd+pos[0])==0)
				{
					if(k!=4)
					{
						Statueline.SetLine("setdir:need 3 args");
						goto CR_RET;
					}
					int a1,a2,a3;
					if(simple_str2int(cmd+pos[1],&a1) && simple_str2int(cmd+pos[2],&a2) && simple_str2int(cmd+pos[3],&a3))
					{
						if(setdir(a1,a2,a3)==0)
						{
							Statueline.SetLine("setdir:OK");
							RunOKFlag = 1;
							goto CR_RET;
						}
						else
						{
							Statueline.SetLine("setdir:args out of range");
							goto CR_RET;
						}
					}
					else
					{
						Statueline.SetLine("setdir:args should be int");
						goto CR_RET;
					}
				}
				else if(strcmp("setvalue",cmd+pos[0])==0)
				{
					if(k!=4)
					{
						Statueline.SetLine("setvalue:need 3 args");
						goto CR_RET;
					}
					int a1,a2,a3;
					if(simple_str2int(cmd+pos[1],&a1) && simple_str2int(cmd+pos[2],&a2) && simple_str2int(cmd+pos[3],&a3))
					{
						if(setvalue(a1,a2,a3)==0)
						{
							Statueline.SetLine("setvalue:OK");
							RunOKFlag = 1;
							goto CR_RET;
						}
						else
						{
							Statueline.SetLine("setvalue:args out of range");
							goto CR_RET;
						}
					}
					else
					{
						Statueline.SetLine("setvalue:args should be int");
						goto CR_RET;
					}
				}

			}
			CR_RET:
			LastCmdline=Cmdline;
			Cmdline.Reset();
			return;
			//Prompt();
		}
		if(key == KEY_TAB)
		{

		}
		if(key == KEY_BS)
		{
			Cmdline.AcceptKeyBS();
		}
		if(key == KEY_ETX)
		{
			//__stdout<< "\n\r" <<"INFO:"<<"CTRL C Do Nothing!";
			Cmdline.Reset();
			//Prompt();
		}
		if(key == KEY_DEL)
		{
			Cmdline.AcceptKeyDEL();
		}
	}
}
void Cli::KeyUp()
{
	Cmdline=LastCmdline;
	return;
}

void Cli::KeyDown()
{
	return;
}

void Cli::KeyLeft()
{
	this->Cmdline.AcceptKeyLeft();
}

void Cli::KeyRight()
{
	this->Cmdline.AcceptKeyRight();
}

void Cli::KeyHome()
{
	this->Cmdline.AcceptKeyHome();
}

void Cli::KeyEnd()
{
	this->Cmdline.AcceptKeyEnd();
}

void Cli::KeyOthers(unsigned char key)
{
	return;
}
