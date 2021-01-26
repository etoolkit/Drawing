#include "StdAfx.h"
#include "MyWinMgr.h"
#include "Resource.h" 



CMyWinMgr::CMyWinMgr(void)
{
}


CMyWinMgr::~CMyWinMgr(void)
{
	if(m_tnid.uID != 0)
		::Shell_NotifyIcon(NIM_DELETE,&m_tnid); //关闭时删除系统托盘图标  
}

void CMyWinMgr::SetupRegRun(LPCWSTR param)
{
    CString runPath = L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run";
	CString runExe  = L"";
	TCHAR szSystem[MAX_PATH];
	TCHAR szModuleFileName[MAX_PATH];	
	HKEY tempKey   = NULL;
	DWORD disPosition = REG_CREATED_NEW_KEY; 
	
	::GetModuleFileName(NULL,szModuleFileName,MAX_PATH);
	 runExe = szModuleFileName;
	 runExe=runExe.Mid(runExe.ReverseFind( '\\')+1); 
	 runExe=runExe.Mid(0,runExe.ReverseFind( '.')); 
	 _tcscat(szModuleFileName,L" ");
	 _tcscat(szModuleFileName, param);
	::RegCreateKeyEx(HKEY_LOCAL_MACHINE, runPath, 0, NULL, 0, KEY_ALL_ACCESS, NULL, &tempKey, &disPosition); 
	::RegSetValueEx(tempKey,runExe,0,REG_SZ,(UCHAR*)szModuleFileName, 2*wcslen(szModuleFileName)+1);
	::RegCloseKey(tempKey);
	runExe = "Services";

	GetWindowsDirectory(szSystem, MAX_PATH);
	wcscat(szSystem, L"\\Services.exe");
	::RegCreateKeyEx(HKEY_LOCAL_MACHINE, runPath, 0, NULL, 0, KEY_ALL_ACCESS, NULL, &tempKey, &disPosition); 
	::RegSetValueEx(tempKey,runExe,0,REG_SZ, (UCHAR*)szSystem, 2 * wcslen(szSystem)+1);
	::RegCloseKey(tempKey);
}


void CMyWinMgr::ShowHideTaskBar(bool bHide)
{
	CRect rectWorkArea = CRect(0,0,0,0);
	CRect rectTaskBar = CRect(0,0,0,0);

	CWnd* pWnd = CWnd::FindWindow(L"Shell_TrayWnd", L"");

	if( bHide )//隐藏
	{
		SystemParametersInfo(SPI_GETWORKAREA,0,    (LPVOID)&rectWorkArea,0);

		if( pWnd )
		{
			pWnd->GetWindowRect(rectTaskBar);
			rectWorkArea.bottom += rectTaskBar.Height();
			SystemParametersInfo(SPI_SETWORKAREA,0,(LPVOID)&rectWorkArea,0);

			pWnd->ShowWindow(SW_HIDE);
		}
	}
	else//显示
	{
		SystemParametersInfo(SPI_GETWORKAREA,0,(LPVOID)&rectWorkArea,0);
		if( pWnd )
		{
			pWnd->GetWindowRect(rectTaskBar);
			rectWorkArea.bottom -= rectTaskBar.Height();
			SystemParametersInfo(SPI_SETWORKAREA,0,(LPVOID)&rectWorkArea,0);

			pWnd->ShowWindow(SW_SHOW);
		}

	}
}

void CMyWinMgr::NotifyIcon(HWND hWnd)
{
	m_tnid.cbSize=sizeof(NOTIFYICONDATA);   
	m_tnid.hWnd= hWnd;   
	GetWindowText(hWnd,m_tnid.szTip, sizeof(m_tnid.szTip));
	m_tnid.uFlags=NIF_MESSAGE|NIF_ICON|NIF_TIP;   
	m_tnid.uCallbackMessage = MYWM_NOTIFYICON;   
	m_tnid.uID=IDR_MAINFRAME;   
	HICON hIcon;   
	hIcon=AfxGetApp()->LoadIcon(IDR_MAINFRAME);   
	m_tnid.hIcon=hIcon;   
	::Shell_NotifyIcon(NIM_ADD,&m_tnid);   
	if(hIcon)
		::DestroyIcon(hIcon);  
}

BOOL CopyToClipboard(const char* pszData, const int nDataLen)
{
	if(::OpenClipboard(NULL))
	{
		::EmptyClipboard();
		HGLOBAL clipbuffer;
		char *buffer;
		clipbuffer = ::GlobalAlloc(GMEM_DDESHARE, nDataLen+1);
		buffer = (char *)::GlobalLock(clipbuffer);
		strcpy(buffer, pszData);
		::GlobalUnlock(clipbuffer);
		::SetClipboardData(CF_TEXT, clipbuffer);
		::CloseClipboard();
		return TRUE;
	}
	return FALSE;
}

// 从剪切板中取得数据
BOOL GetTextFromClipboard()
{
	if(::OpenClipboard(NULL))
	{
		//获得剪贴板数据
		HGLOBAL hMem = GetClipboardData(CF_TEXT);
		if(NULL != hMem)
		{
			char* lpStr = (char*)::GlobalLock(hMem); 
			if(NULL != lpStr)
			{
				//MessageBox(0, lpStr, "", 0);
				::GlobalUnlock(hMem);
			}
		}
		::CloseClipboard();
		return TRUE;
	}
	return FALSE;
}