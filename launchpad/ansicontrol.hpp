#ifndef _ANSI_CONTROL_HPP_
#define _ANSI_CONTROL_HPP_
enum Direction
{
	UP,
	DOWN,
	LEFT,
	RIGHT
};
enum Color
{
	BLACK = 0,
	RED= 1,
	GREEN = 2,
	YELLOW = 3,
	BLUE = 4,
	PURPLE = 5,
	DARKGREEN=6,
	WRITE = 7
};

enum KeyName
{
	KEY_DEL   = 0x7f,
	KEY_ETX   = 0x03,
	KEY_SPACE = 0x20,
	KEY_BEL   = 0x07,
	KEY_BS    = 0x08,
	KEY_TAB   = 0x09,
	KEY_LF    = 0x0a,
	KEY_CR    = 0x0d,
	KEY_ESC   = 0x1b,
	KEY_ANS   = 0x3f,
	KEY_UP    = 0x41,
	KEY_DOWN  = 0x42,
	KEY_RIGHT = 0x43,
	KEY_LEFT  = 0x44,
	KEY_HOME  = 72,
	KEY_END   = 70
	//...
};
void SetColor(Color color);
void ClearSetting();
void ClearScrean();
void ClearToEnd();
void HideCursor();
void ShowCursor();
void GetCursor1();
void MoveCursor(Direction dir, int n = 1);
void SetCursor1(int y, int x);
#endif
