#pragma once
#define MYWM_NOTIFYICON WM_USER+1  
BOOL CopyToClipboard(const char* pszData, const int nDataLen);
BOOL GetTextFromClipboard();	
class CMyWinMgr
{
public:
	CMyWinMgr(void);
	~CMyWinMgr(void);

	void ShowHideTaskBar(bool bHide);
	void NotifyIcon(HWND hwnd);
	 void SetupRegRun(LPCWSTR param = NULL);
	 void GetWindowsByTitle(CString sTitle,CWnd);
	NOTIFYICONDATA m_tnid;  
};

