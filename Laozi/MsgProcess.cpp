#include "StdAfx.h"
#include "MsgProcess.h"
#include "../MyHookDll/Myhook.h"


CMsgProcess::CMsgProcess(void)
{
}


CMsgProcess::~CMsgProcess(void)
{
}

int CMsgProcess::ProcessMsg(UINT msg,WPARAM wparam,LPARAM lparam)
{
	int nRtn = 0;
	switch(msg)
	{
	case WM_MYHOOK_MOUSE:
		nRtn = MouseMsg(wparam,lparam);
		break;
	case  WM_MYHOOK_KEY:
		nRtn  = KeyboardMsg(wparam,lparam);
		break;
	
	}
	return nRtn;
}

int CMsgProcess::MouseMsg(WPARAM wparam,LPARAM lparam)
{
	LPMOUSEHOOKSTRUCT pMouseHook=(MOUSEHOOKSTRUCT FAR *) lparam; 
	HWND glhTargetWnd=pMouseHook->hwnd; 
	HWND MainWnd=glhTargetWnd; 
	static HWND glhLastTargetWnd =NULL;
	// Left button pressed somewhere
	if(wparam == WM_LBUTTONDOWN)
	{
		glhTargetWnd = WindowFromPoint(pMouseHook->pt);
		if(glhTargetWnd == glhLastTargetWnd)
			return 0;
		glhLastTargetWnd = glhTargetWnd;
		if(IsWindow(glhTargetWnd))
		{	
			WCHAR szCaption[256],szParentCaption[256];
			CString sTitle=L"";
			CString sParentTitle=L"";

			HWND ParentWnd = GetParent(glhTargetWnd);
			while (ParentWnd !=NULL) 
			{ 
				MainWnd=ParentWnd; 
				ParentWnd=GetParent(MainWnd); 
				//取应用程序主窗口句柄 
			} 				
			GetWindowText(glhTargetWnd,szCaption,sizeof(szCaption));
			GetWindowText(MainWnd,szParentCaption,sizeof(szParentCaption)); 
			//GetClassName(glhTargetWnd,szCaption,sizeof(szCaption));
			sTitle = szCaption;
			sParentTitle = szParentCaption;
			if (sParentTitle.Find(L"VPN")>-1)
			{
				if(sTitle == L"password")
				{
					//setw(glhTargetWnd,L"dfdf");
				
					CString s = L"Vpncount1";
					for (int i=0;i<s.GetLength();i++)
					{
						::SendMessage(glhTargetWnd, WM_IME_CHAR, (WCHAR)s.GetAt(i), 0); 
					}
					//PostMessage(glhTargetWnd, WM_KEYDOWN, VK_RETURN, 0); //发送回车键 
				}
			}
		}
	}

	return 0;
}

int CMsgProcess::KeyboardMsg(WPARAM wparam,LPARAM lparam)
{
	return 0;
}

int CMsgProcess::HotkeyMsg(WPARAM wparam,LPARAM lparam)
{

	return 0;
}