#include "StdAfx.h"
#include "Myhook.h"
#include <Psapi.h>
#pragma data_seg("mydata")              //������ʶ���ָ�������������ڴ��п���һ�����ݶδ�Ÿ�ָ�����������
#include <winuser.h>
#include <SetupAPI.h>
#include "..\Laozi\ProcessMgr.h"
HINSTANCE glhInstance=NULL;             //DLLʵ��������˵ģ�飩�ľ����
HHOOK glhHook=NULL;                     //��깳�ӵľ����
HHOOK glhKeyHook=NULL;                     //��깳�ӵľ����
//HWND GlobalWndHandle[100]={NULL,.....};//������ű����صĴ��ڵľ�������������ʽ���档
//����������ʼ����ԭ������ġ����ԡ�......��ʡ�ԡ�
UINT Global_i=0;                       //������ѭ�������л���������ı�����
BOOL Condition1=0;                     //���Լ�¼������»��ͷŵı�־������
BOOL Condition2=0;                     //���Լ�¼�Ҽ����»��ͷŵı�־������
BOOL HideOrVisitableFlag=0;            //���Ա�ʶ���ٴ������Ҽ�ͬʱ���µ��������ʱ�����ػ�����ʾ���ڡ�
BOOL Check1=0;                         //������ʾ�ؼ�Check1״̬�ı�־������
BOOL Check2=0;                         //������ʾ�ؼ�Check2״̬�ı�־������
BOOL Check3=0;       
HWND glhPrevTarWnd=NULL; 
//�ϴ������ָ�Ĵ��ھ�� 
HWND glhDisplayWnd=NULL; 
#pragma data_seg()

#pragma comment(lib, "psapi.lib")

static AFX_EXTENSION_MODULE MyHookDllDLL = { NULL, NULL };

extern "C" int APIENTRY
	DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
{
	// ���ʹ�� lpReserved���뽫���Ƴ�
	UNREFERENCED_PARAMETER(lpReserved);

	if (dwReason == DLL_PROCESS_ATTACH)
	{
		TRACE0("MyHookDll.DLL ���ڳ�ʼ��!\n");

		// ��չ DLL һ���Գ�ʼ��
		if (!AfxInitExtensionModule(MyHookDllDLL, hInstance))
			return 0;

		// ���� DLL ���뵽��Դ����
		// ע��: �������չ DLL ��
		//  MFC ���� DLL (�� ActiveX �ؼ�)��ʽ���ӵ���
		//  �������� MFC Ӧ�ó������ӵ�������Ҫ
		//  �����д� DllMain ���Ƴ������������һ��
		//  �Ӵ���չ DLL �����ĵ����ĺ����С�ʹ�ô���չ DLL ��
		//  ���� DLL Ȼ��Ӧ��ʽ
		//  ���øú����Գ�ʼ������չ DLL������
		//  CDynLinkLibrary ���󲻻ḽ�ӵ�
		//  ���� DLL ����Դ���������������ص�
		//  ���⡣

		new CDynLinkLibrary(MyHookDllDLL);
		glhInstance = hInstance;
	}
	else if (dwReason == DLL_PROCESS_DETACH)
	{
		TRACE0("MyHookDll.DLL ������ֹ!\n");

		// �ڵ�����������֮ǰ��ֹ�ÿ�
		AfxTermExtensionModule(MyHookDllDLL);
	}
	return 1;   // ȷ��
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

	if (counter>0)//����counter�������Ϣ
	{
		counter--;
		return CallNextHookEx(glhHook, nCode, wParam, lParam);
	}

	LPMOUSEHOOKSTRUCT pMouseHook=(MOUSEHOOKSTRUCT FAR *) lParam; 
	if(g_bRPressed == FALSE && WM_MOUSEMOVE == wParam)
		return CallNextHookEx(glhHook, nCode, wParam, lParam);
	if(WM_RBUTTONDOWN==wParam)//�Ҽ�����
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
	else if(g_bRPressed == FALSE && WM_MOUSEMOVE == wParam)//��ͨ����ƶ�
		return CallNextHookEx(glhHook, nCode, wParam, lParam);
	else if(WM_MBUTTONUP==wParam)//����м�����
	{
		SendMessage(glhDisplayWnd, 0x405, 0, 0);
		
		return CallNextHookEx(glhHook, nCode, wParam, lParam);
	}
	else if(g_bRPressed==TRUE && WM_MOUSEMOVE == wParam)//GetAsyncKeyState(VK_RBUTTON)	//�����Ҽ��ƶ�
	{
		if(b_Ignore)
			return  CallNextHookEx(glhHook, nCode, wParam, lParam);
		if(g_bFirstRecord)//����Ǹոհ����Ҽ�
		{
			POINT b;
			b.x=((MSLLHOOKSTRUCT*)lParam)->pt.x;
			b.y=((MSLLHOOKSTRUCT*)lParam)->pt.y;
			if((b.x-g_ptPressed.x)*(b.x-g_ptPressed.x)+(b.y-g_ptPressed.y)*(b.y-g_ptPressed.y) > 50)//���ƫ�����㹻
			{
				g_bFirstRecord=FALSE;
				lastMouseMessage=WM_MOUSEMOVE;
				SendMessage(glhDisplayWnd, WM_MYHOOK_RPRESS,g_ptPressed.x, g_ptPressed.y);//��ʼ��¼�������
			}
			//else return CallNextHookEx(g_hMouseMove, nCode, wParam, lParam);
		}
		else
		{
			lastMouseMessage=WM_MOUSEMOVE;
			SendMessage(glhDisplayWnd, WM_MYHOOK_RMOVE, ((MSLLHOOKSTRUCT*)lParam)->pt.x, ((MSLLHOOKSTRUCT*)lParam)->pt.y);
		}
	}
	else if(WM_RBUTTONUP==wParam)//�ɿ��Ҽ�
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
	//����������Ϣ 
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
		switch(wParam) //���̰�����ʶ
		{
		case '1':break; //�����ּ�1������
		case '2':break;
		case '3':break;
		case '4':break;

		case 'A':break; //����ĸ��A������
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
	//������ʾĿ�괰�ڱ���༭��ľ�� 
	return bResult; 
} 
BOOL startKeyboardhook()
{ 
	BOOL bResult=FALSE; 
	glhKeyHook=SetWindowsHookEx(WH_KEYBOARD_LL,(HOOKPROC)KeyboardProc,glhInstance,0); 
	if(glhKeyHook!=NULL) 
		bResult=TRUE; 
	//������ʾĿ�괰�ڱ���༭��ľ�� 
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
			glhDisplayWnd=NULL;//����� 
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
			glhDisplayWnd=NULL;//����� 
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

