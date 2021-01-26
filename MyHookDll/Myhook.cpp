#include "StdAfx.h"
#include "Myhook.h"
#include <Psapi.h>
#pragma data_seg("mydata")              //编译器识别的指令用以在虚拟内存中开辟一个数据段存放该指令下面的数据
#include <winuser.h>
#include <SetupAPI.h>
#include "..\Laozi\ProcessMgr.h"
HINSTANCE glhInstance=NULL;             //DLL实例（或者说模块）的句柄。
HHOOK glhHook=NULL;                     //鼠标钩子的句柄。
HHOOK glhKeyHook=NULL;                     //鼠标钩子的句柄。
//HWND GlobalWndHandle[100]={NULL,.....};//用来存放被隐藏的窗口的句柄，以数组的形式保存。
//该数组必须初始化，原因见下文。我以“......”省略。
UINT Global_i=0;                       //用以在循环中序列化窗口数组的变量。
BOOL Condition1=0;                     //用以记录左键按下或释放的标志变量。
BOOL Condition2=0;                     //用以记录右键按下或释放的标志变量。
BOOL HideOrVisitableFlag=0;            //用以标识当再次有左、右键同时按下的情况发生时是隐藏还是显示窗口。
BOOL Check1=0;                         //用来表示控件Check1状态的标志变量。
BOOL Check2=0;                         //用来表示控件Check2状态的标志变量。
BOOL Check3=0;       
HWND glhPrevTarWnd=NULL; 
//上次鼠标所指的窗口句柄 
HWND glhDisplayWnd=NULL; 
#pragma data_seg()

#pragma comment(lib, "psapi.lib")

static AFX_EXTENSION_MODULE MyHookDllDLL = { NULL, NULL };

extern "C" int APIENTRY
	DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
{
	// 如果使用 lpReserved，请将此移除
	UNREFERENCED_PARAMETER(lpReserved);

	if (dwReason == DLL_PROCESS_ATTACH)
	{
		TRACE0("MyHookDll.DLL 正在初始化!\n");

		// 扩展 DLL 一次性初始化
		if (!AfxInitExtensionModule(MyHookDllDLL, hInstance))
			return 0;

		// 将此 DLL 插入到资源链中
		// 注意: 如果此扩展 DLL 由
		//  MFC 规则 DLL (如 ActiveX 控件)隐式链接到，
		//  而不是由 MFC 应用程序链接到，则需要
		//  将此行从 DllMain 中移除并将其放置在一个
		//  从此扩展 DLL 导出的单独的函数中。使用此扩展 DLL 的
		//  规则 DLL 然后应显式
		//  调用该函数以初始化此扩展 DLL。否则，
		//  CDynLinkLibrary 对象不会附加到
		//  规则 DLL 的资源链，并将导致严重的
		//  问题。

		new CDynLinkLibrary(MyHookDllDLL);
		glhInstance = hInstance;
	}
	else if (dwReason == DLL_PROCESS_DETACH)
	{
		TRACE0("MyHookDll.DLL 正在终止!\n");

		// 在调用析构函数之前终止该库
		AfxTermExtensionModule(MyHookDllDLL);
	}
	return 1;   // 确定
}

POINT g_ptPressed; 
BOOL g_bFirstRecord=TRUE, g_bRPressed=FALSE,b_Ignore=FALSE;
WPARAM lastMouseMessage=0;
byte counter = 0;
bool IsIgnore()
{ 
	HWND hFGWnd=::GetForegroundWindow();
	CString proname=CProcessMgr::GetProcessFileName(hFGWnd);
	CString s1 = L"360chrome.exe";	
	CDWordArray pIdTempArr;
	s1=s1.Mid(s1.ReverseFind( L'\\ ')+1); 
	proname=proname.Mid(proname.ReverseFind( L'\\ ')+1).MakeLower(); 
	if(s1.MakeLower()==proname)
		return true;
	if("chrome.exe"==proname)
		return true;
// 	CProcessMgr::GetProcessIdByText(pIdTempArr, s1,true);
// 	vector<HWND> wnds;
// 	bool b=false;
// 	for (int j = 0; j < pIdTempArr.GetCount(); j++)
// 	{
// 		wnds= CProcessMgr::EnumWnds(pIdTempArr.GetAt(j));
// 		vector<HWND>::iterator w;  
// 		for (w = wnds.begin(); w != wnds.end();w++)  
// 		{
// 			if(*w==hFGWnd)
// 			{
// 				return true;
// 			}
// 		}
// 	}
	return false;
}
LRESULT WINAPI MouseProc(int nCode,WPARAM wParam,LPARAM lParam) 
{ 	

	if (counter>0)//忽略counter次鼠标消息
	{
		counter--;
		return CallNextHookEx(glhHook, nCode, wParam, lParam);
	}

	LPMOUSEHOOKSTRUCT pMouseHook=(MOUSEHOOKSTRUCT FAR *) lParam; 
	if(g_bRPressed == FALSE && WM_MOUSEMOVE == wParam)
		return CallNextHookEx(glhHook, nCode, wParam, lParam);
	if(WM_RBUTTONDOWN==wParam)//右键按下
	{
		b_Ignore =IsIgnore();
		g_ptPressed.x=((MSLLHOOKSTRUCT*)lParam)->pt.x;
		g_ptPressed.y=((MSLLHOOKSTRUCT*)lParam)->pt.y;
		g_bFirstRecord=TRUE;
		lastMouseMessage=WM_RBUTTONDOWN;
		g_bRPressed=TRUE;		
		// 		return CallNextHookEx(g_hMouseMove, nCode, wParam, lParam);
		if(nCode<0 || b_Ignore) return CallNextHookEx(glhHook, nCode, wParam, lParam);
		return -1;
	}
	else if(g_bRPressed == FALSE && WM_MOUSEMOVE == wParam)//普通鼠标移动
		return CallNextHookEx(glhHook, nCode, wParam, lParam);
	else if(WM_MBUTTONUP==wParam)//鼠标中键按下
	{
		SendMessage(glhDisplayWnd, 0x405, 0, 0);
		
		return CallNextHookEx(glhHook, nCode, wParam, lParam);
	}
	else if(g_bRPressed==TRUE && WM_MOUSEMOVE == wParam)//GetAsyncKeyState(VK_RBUTTON)	//按下右键移动
	{
		if(b_Ignore)
			return  CallNextHookEx(glhHook, nCode, wParam, lParam);
		if(g_bFirstRecord)//如果是刚刚按下右键
		{
			POINT b;
			b.x=((MSLLHOOKSTRUCT*)lParam)->pt.x;
			b.y=((MSLLHOOKSTRUCT*)lParam)->pt.y;
			if((b.x-g_ptPressed.x)*(b.x-g_ptPressed.x)+(b.y-g_ptPressed.y)*(b.y-g_ptPressed.y) > 50)//如果偏移量足够
			{
				g_bFirstRecord=FALSE;
				lastMouseMessage=WM_MOUSEMOVE;
				SendMessage(glhDisplayWnd, WM_MYHOOK_RPRESS,g_ptPressed.x, g_ptPressed.y);//开始记录鼠标手势
			}
			//else return CallNextHookEx(g_hMouseMove, nCode, wParam, lParam);
		}
		else
		{
			lastMouseMessage=WM_MOUSEMOVE;
			SendMessage(glhDisplayWnd, WM_MYHOOK_RMOVE, ((MSLLHOOKSTRUCT*)lParam)->pt.x, ((MSLLHOOKSTRUCT*)lParam)->pt.y);
		}
	}
	else if(WM_RBUTTONUP==wParam)//松开右键
	{
		g_bFirstRecord=FALSE; 
		if (g_bRPressed ==TRUE && WM_RBUTTONDOWN == lastMouseMessage)
		{		
			g_bRPressed=FALSE;
			lastMouseMessage=WM_RBUTTONUP;
			if(b_Ignore)
				return CallNextHookEx(glhHook, nCode, wParam, lParam);
			counter=2;
			mouse_event(MOUSEEVENTF_RIGHTDOWN|MOUSEEVENTF_RIGHTUP, 0, 0, 0, 0);
			if(nCode<0 ) return CallNextHookEx(glhHook, nCode, wParam, lParam);
			return 1;
			//return CallNextHookEx(g_hMouseMove, nCode, wParam, lParam);
		}
		else if(g_bRPressed==TRUE && (WM_MOUSEMOVE==lastMouseMessage /*|| WM_MOUSEWHEEL== lastMouseMessage*/))
		{
		//	if(!(WM_MOUSEMOVE== lastMouseMessage && b_Ignore ))
				SendMessage(glhDisplayWnd, WM_MYHOOK_RPRESS, 0, 0);
			lastMouseMessage=WM_RBUTTONUP;
			g_bRPressed=FALSE;
			if(nCode<0) return CallNextHookEx(glhHook, nCode, wParam, lParam);
			if(b_Ignore) return CallNextHookEx(glhHook, nCode, wParam, lParam);
			return -1;
		}
		else if(g_bRPressed==TRUE &&WM_MOUSEWHEEL== lastMouseMessage)
		{
			//SendMessage(glhDisplayWnd, WM_MYHOOK_RPRESS, 0, 0);
			lastMouseMessage=WM_RBUTTONUP;
			g_bRPressed=FALSE;
			if(b_Ignore)
			CallNextHookEx(glhHook, nCode, wParam, lParam);
			else return -1;
		}
		//return 1;
	}
	else if(WM_MOUSEWHEEL== wParam)
	{
		if(g_bRPressed)
		{
			int a= HIWORD(((MSLLHOOKSTRUCT*)lParam)->mouseData);
			if(a==WHEEL_DELTA)
					SendMessage(glhDisplayWnd, WM_MYHOOK_XWHEEL,0,1);
			else 	SendMessage(glhDisplayWnd, WM_MYHOOK_XWHEEL,0,2);
			lastMouseMessage = WM_MOUSEWHEEL;
		}		 
	}
	return CallNextHookEx(glhHook,nCode,wParam,lParam); 
	//继续传递消息 
} 

LRESULT CALLBACK KeyboardProc(int nCode,WPARAM wParam,LPARAM lParam)
{

	//if(glhDisplayWnd==NULL)glhDisplayWnd=AfxGetApp()->GetMainWnd()->GetSafeHwnd();
	LRESULT RetVal = CallNextHookEx( glhKeyHook, nCode, wParam, lParam );
	//if(((DWORD)lParam&0x40000000) && (HC_ACTION==nCode))
	{
		HWND hdc = GetForegroundWindow();
		TCHAR title[256];
		GetWindowText(hdc,title,sizeof(title));
		DWORD proId=0;
		HANDLE hProcess=NULL;
		GetWindowThreadProcessId(hdc, &proId);
		hProcess = OpenProcess(PROCESS_ALL_ACCESS,FALSE, proId);
		HMODULE hModule=0;
		GetModuleFileNameEx(hProcess, hModule,title, sizeof(title));
		CloseHandle(hProcess);
		::SendMessage(glhDisplayWnd,WM_MYHOOK_KEY,/* (WPARAM)(LPTSTR)(LPCTSTR)*/wParam,lParam);
		switch(wParam) //键盘按键标识
		{
		case '1':break; //当数字键1被按下
		case '2':break;
		case '3':break;
		case '4':break;

		case 'A':break; //当字母键A被按下
		case 'B':break;
		case 'C':break;
		case 'D':break;
		}
	}
	SendMessage(glhDisplayWnd,WM_MYHOOK_KEY,wParam,lParam);
	return RetVal;
}
BOOL startMousehook(HWND hWnd /*= NULL*/)
{ 
	BOOL bResult=FALSE; 
	glhHook=SetWindowsHookEx(WH_MOUSE_LL,MouseProc,glhInstance,0); 
	if(glhHook!=NULL) 
		bResult=TRUE; 
	//glhDisplayWnd=hWnd; 
	glhDisplayWnd =hWnd;
	//设置显示目标窗口标题编辑框的句柄 
	return bResult; 
} 
BOOL startKeyboardhook()
{ 
	BOOL bResult=FALSE; 
	glhKeyHook=SetWindowsHookEx(WH_KEYBOARD_LL,(HOOKPROC)KeyboardProc,glhInstance,0); 
	if(glhKeyHook!=NULL) 
		bResult=TRUE; 
	//设置显示目标窗口标题编辑框的句柄 
	return bResult; 
} 
BOOL  stopMousehook()
{ 
	BOOL bResult=FALSE; 
	if(glhHook) 
	{ 
		bResult= UnhookWindowsHookEx(glhHook); 
		if(bResult) 
		{ 
			glhPrevTarWnd=NULL; 
			glhDisplayWnd=NULL;//清变量 
			glhHook=NULL; 
		} 
	} 
	return bResult; 
} 
BOOL  stopKeyboardhook()
{ 
	BOOL bResult=FALSE; 
	if(glhKeyHook) 
	{ 
		bResult= UnhookWindowsHookEx(glhKeyHook); 
		if(bResult) 
		{ 
			glhPrevTarWnd=NULL; 
			glhDisplayWnd=NULL;//清变量 
			glhHook=NULL; 
		} 
	} 
	return bResult; 
} 

BOOL  InitInstance ()
{
	/*hins=AfxGetInstanceHandle();
	GetModuleFileName(AfxGetInstanceHandle( ),szBuf,sizeof(szBuf));
	p = szBuf;
	while(strchr(p,''\\''))
	{
	p = strchr(p,''\\'');
	p++;
	}
	*p = ''\0'';
	msg=szBuf;*/
	return TRUE;
}

