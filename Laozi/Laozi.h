
// Laozi.h : PROJECT_NAME 应用程序的主头文件
//

#pragma once

#ifndef __AFXWIN_H__
	#error "在包含此文件之前包含“stdafx.h”以生成 PCH 文件"
#endif

#include "resource.h"		// 主符号 
#include "MyConfig.h"
//#include "MouseSignalInfo.h"


// CLaoziApp:
// 有关此类的实现，请参阅 Laozi.cpp
//

class CLaoziApp : public CWinApp
{
public:
	CLaoziApp();
	UINT m_hotKeyIdHideWnd;
	CMyConfig m_config;
	CString m_modulePath;
	//CMouseSignalInfo m_sigalInfo;
// 重写
public:
	virtual BOOL InitInstance();

// 实现

	DECLARE_MESSAGE_MAP()
	int MyInit(void);
	virtual int ExitInstance();

private:
	HANDLE m_hMutex ;
};

extern CLaoziApp theApp;