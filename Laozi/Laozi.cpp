
// Laozi.cpp : 定义应用程序的类行为。
//

#include "stdafx.h"
#include "Laozi.h"
#include "LaoziDlg.h"
#include "GetCurrentModule.h"
#include "ProcessMgr.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CLaoziApp

BEGIN_MESSAGE_MAP(CLaoziApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CLaoziApp 构造

CLaoziApp::CLaoziApp()
{
	// 支持重新启动管理器
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_RESTART;

	// TODO: 在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中
}


// 唯一的一个 CLaoziApp 对象

CLaoziApp theApp;


// CLaoziApp 初始化

BOOL CLaoziApp::InitInstance()
{
	// 如果一个运行在 Windows XP 上的应用程序清单指定要
	// 使用 ComCtl32.dll 版本 6 或更高版本来启用可视化方式，
	//则需要 InitCommonControlsEx()。否则，将无法创建窗口。
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// 将它设置为包括所有要在应用程序中使用的
	// 公共控件类。
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();

	
	AfxEnableControlContainer();

	// 创建 shell 管理器，以防对话框包含
	// 任何 shell 树视图控件或 shell 列表视图控件。
	CShellManager *pShellManager = new CShellManager;

	// 标准初始化
	// 如果未使用这些功能并希望减小
	// 最终可执行文件的大小，则应移除下列
	// 不需要的特定初始化例程
	// 更改用于存储设置的注册表项
	// TODO: 应适当修改该字符串，
	// 例如修改为公司或组织名
	 m_hMutex = ::CreateMutex(NULL,TRUE,L"Laozi");
	  int er=::GetLastError();
	 //int argc(0);
	 //LPWSTR *argv=::CommandLineToArgvW(::GetCommandLineW(),&argc);   
	 //bool bDebug = false;
	 //for (int i = 0; i < argc; i++)
	 //{
		// if(StrCmp(argv[i], L"debug")==0)		 
		//	 bDebug = true;		
	 //} 
	//if(!bDebug)	
	
		if  (er == ERROR_ALREADY_EXISTS)
	{
		//AfxMessageBox(“您已经运行了本软件！谢谢使用 ”);  //已经运行了程序
		TCHAR tCurPath[MAX_PATH];	
		::GetModuleFileName(::GetCurrentModule(), tCurPath,MAX_PATH);
		CString s1 = tCurPath;
		s1=s1.Mid(s1.ReverseFind( L'\\ ')+1); 
		 CDWordArray pIdTempArr;
		CProcessMgr::GetProcessIdByText(pIdTempArr,s1, true);
		for (int j = 0; j < pIdTempArr.GetCount(); j++)
		{
			vector<HWND> wnds= CProcessMgr::EnumWnds(pIdTempArr.GetAt(j),NULL,false);
			vector<HWND>::iterator w;  
			for (w = wnds.begin(); w != wnds.end();w++)  
			{  
				//if(!::IsWindowVisible(*w))
				if(IsWindow(*w))
				{
					TCHAR s[200];
					::GetWindowText(*w,s, 200);
					if(CString(s).Find(L"超级助手")>=0)
					{
						::ShowWindow(*w,SW_SHOW); 		
						SetForegroundWindow(*w); 
						break;
					}
					
				}							
			}

		}

		return FALSE;
	}

	SetRegistryKey(_T("上班卫士"));
	MyInit(); 
	CLaoziDlg dlg;
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal(); 


	// 删除上面创建的 shell 管理器。
	if (pShellManager != NULL)
	{
		delete pShellManager;
	}

	// 由于对话框已关闭，所以将返回 FALSE 以便退出应用程序，
	//  而不是启动应用程序的消息泵。

	
	return FALSE;
}



int CLaoziApp::MyInit(void)
{
	m_hotKeyIdHideWnd = GlobalAddAtom(L"hotKeyHideWnd") - 0xC000;
	TCHAR tCurPath[MAX_PATH];	
	::GetModuleFileName(::GetCurrentModule(), tCurPath,MAX_PATH);
	PathRemoveFileSpec(tCurPath);
	m_modulePath = tCurPath;
	TCHAR xmlPath[MAX_PATH];
	_tcscpy(xmlPath, tCurPath);
	TCHAR pPath[MAX_PATH]={0};
	if(SHGetSpecialFolderPath(NULL,pPath,CSIDL_PERSONAL,0))
	{
	 _tcscpy(xmlPath,pPath);
	 PathAppend(xmlPath,L"超级助手");
	}
	m_config.ReadConfig(xmlPath);

	return 0;
}


int CLaoziApp::ExitInstance()
{
	// TODO: 在此添加专用代码和/或调用基类
	CloseHandle(m_hMutex);
	return CWinApp::ExitInstance();
}
