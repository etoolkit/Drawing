#pragma once
class CMsgProcess
{
public:
	CMsgProcess(void);
	~CMsgProcess(void);
	int ProcessMsg(UINT msg,WPARAM wparam,LPARAM lparam);
private:
	int MouseMsg(WPARAM wparam,LPARAM lparam);
	int KeyboardMsg(WPARAM wparam,LPARAM lparam);
	int HotkeyMsg(WPARAM wparam,LPARAM lparam);
};

