
// Laozi.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������ 
#include "MyConfig.h"
//#include "MouseSignalInfo.h"


// CLaoziApp:
// �йش����ʵ�֣������ Laozi.cpp
//

class CLaoziApp : public CWinApp
{
public:
	CLaoziApp();
	UINT m_hotKeyIdHideWnd;
	CMyConfig m_config;
	CString m_modulePath;
	//CMouseSignalInfo m_sigalInfo;
// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
	int MyInit(void);
	virtual int ExitInstance();

private:
	HANDLE m_hMutex ;
};

extern CLaoziApp theApp;