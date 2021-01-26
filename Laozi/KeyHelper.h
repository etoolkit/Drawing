#pragma once
class CKeyHelper
{
public:
	CKeyHelper(void);
	~CKeyHelper(void);
	static CString GetKeyStr(UINT key);
	static UINT GetKey(CString key);
	static BOOL RegisetHotkey(HWND hwnd, int id, CString strkey);	
};

