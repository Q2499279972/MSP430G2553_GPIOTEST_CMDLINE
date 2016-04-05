#ifndef CLI_HPP_
#define CLI_HPP_
#include "ansicontrol.hpp"
#include "uart.hpp"


class Line
{

public:
	static const int MAX_CHARACTERS = 40;

	char Contents[MAX_CHARACTERS];
	int m_CurrentCursorPosition;
	int m_CurrentCharacterNumber;

	Line();
	void CopyToArray(char *s)
	{
		int i=0;
		while(i<m_CurrentCharacterNumber)
		{
			s[i] = Contents[i];
			i++;
		}
		s[i] = 0;
	}

	void SetLine(char *s)
	{
		int i=0;
		m_CurrentCharacterNumber=0;
		m_CurrentCursorPosition=0;
		while(*s!=0)
		{
			Contents[i]=*s;
			m_CurrentCharacterNumber++;
			m_CurrentCursorPosition++;
			i++;
			s++;
			if(i==MAX_CHARACTERS) break;
		}
	}
	virtual void Reset()
	{
		m_CurrentCursorPosition = 0;
		m_CurrentCharacterNumber = 0;
	}
	virtual void AcceptKeyCR();
	virtual void AcceptKeyTAB();
	virtual void AcceptKeyBS();
	virtual void AcceptKeyETX();
	virtual void AcceptKeyNormal(unsigned char);
	virtual void AcceptKeyUP();
	virtual void AcceptKeyDown();
	virtual void AcceptKeyLeft();
	virtual void AcceptKeyRight();
	virtual void AcceptKeyHome();
	virtual void AcceptKeyEnd();
	virtual void AcceptKeyDEL();
	virtual void Flush();
};

class Cli
{
	//enum
	//{
	//	KEY_DEL   = 0x7f,
	//	KEY_ETX   = 0x03,
	//	KEY_SPACE = 0x20,
	//	KEY_BEL   = 0x07,
	//	KEY_BS    = 0x08,
	//	KEY_TAB   = 0x09,
	//	KEY_LF    = 0x0a,
	//	KEY_CR    = 0x0d,
	//	KEY_ESC   = 0x1b,
	//	KEY_ANS   = 0x3f,
	//	KEY_UP    = 0x41,
	//	KEY_DOWN  = 0x42,
	//	KEY_RIGHT = 0x43,
	//	KEY_LEFT  = 0x44,
	//	KEY_HOME  = 72,//0x48,
	//	KEY_END   = 70,//0x4b,
	////...
	//}KeyName;

public:
	//char PromptString[20];
	Line Cmdline;
	Line LastCmdline;
	Line Statueline;
	int RunOKFlag;
	int FSMKey;
public:
	Cli(){RunOKFlag=0;FSMKey=0;}
	void Prompt()
	{
		//__stdout << "\n\r";
		//__stdout << PromptString;
		__stdout<<"CMD>>";
	}
	void Reset()
	{
		Cmdline.Reset();
		FSMKey = 0;
		//PromptString[0]=0;
		::ClearSetting();
		::ClearScrean();
		::ShowCursor();
		::SetCursor1(0,0);
		//__stdout << PromptString;
	}

	void AcceptKey(unsigned char key);

	void KeyNormal(unsigned char key);
	void KeyUp();
	void KeyDown();
	void KeyLeft();
	void KeyRight();
	void KeyHome();
	void KeyEnd();
	void KeyOthers(unsigned char key);
	void Flush()
	{
		Prompt();
		Cmdline.Flush();
		ClearToEnd();
		__stdout<<"\r\n";
		Statueline.Flush();
		ClearToEnd();
	}


	int cmdpaser(char *cmd,int len,char *pos)
	{
		int n=0;
		if(cmd[0]!=0 && cmd[0]!=' ' &&len!=0)
		{
			pos[n++]=0;
		}
		int i;
		for(i=0;i<len-1;i++)
		{
			if(cmd[i]==' '&&cmd[i+1]!=' '&&cmd[i+1]!=0)
			{
				pos[n++]=i+1;
			}
		}
		return n;
	}
};


#endif /* CLI_HPP_ */
