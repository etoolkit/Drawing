
// Laozi.cpp : ����Ӧ�ó��������Ϊ��
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


// CLaoziApp ����

CLaoziApp::CLaoziApp()
{
	// ֧����������������
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_RESTART;

	// TODO: �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}


// Ψһ��һ�� CLaoziApp ����

CLaoziApp theApp;


// CLaoziApp ��ʼ��

BOOL CLaoziApp::InitInstance()
{
	// ���һ�������� Windows XP �ϵ�Ӧ�ó����嵥ָ��Ҫ
	// ʹ�� ComCtl32.dll �汾 6 ����߰汾�����ÿ��ӻ���ʽ��
	//����Ҫ InitCommonControlsEx()�����򣬽��޷��������ڡ�
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// ��������Ϊ��������Ҫ��Ӧ�ó�����ʹ�õ�
	// �����ؼ��ࡣ
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();

	
	AfxEnableControlContainer();

	// ���� shell ���������Է��Ի������
	// �κ� shell ����ͼ�ؼ��� shell �б���ͼ�ؼ���
	CShellManager *pShellManager = new CShellManager;

	// ��׼��ʼ��
	// ���δʹ����Щ���ܲ�ϣ����С
	// ���տ�ִ���ļ��Ĵ�С����Ӧ�Ƴ�����
	// ����Ҫ���ض���ʼ������
	// �������ڴ洢���õ�ע�����
	// TODO: Ӧ�ʵ��޸ĸ��ַ�����
	// �����޸�Ϊ��˾����֯��
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
		//AfxMessageBox(�����Ѿ������˱������ллʹ�� ��);  //�Ѿ������˳���
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
					if(CString(s).Find(L"��������")>=0)
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

	SetRegistryKey(_T("�ϰ���ʿ"));
	MyInit(); 
	CLaoziDlg dlg;
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal(); 


	// ɾ�����洴���� shell ��������
	if (pShellManager != NULL)
	{
		delete pShellManager;
	}

	// ���ڶԻ����ѹرգ����Խ����� FALSE �Ա��˳�Ӧ�ó���
	//  ����������Ӧ�ó������Ϣ�á�

	
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
	 PathAppend(xmlPath,L"��������");
	}
	m_config.ReadConfig(xmlPath);

	return 0;
}


int CLaoziApp::ExitInstance()
{
	// TODO: �ڴ����ר�ô����/����û���
	CloseHandle(m_hMutex);
	return CWinApp::ExitInstance();
}
