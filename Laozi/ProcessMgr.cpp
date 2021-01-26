#include "StdAfx.h"
#include "ProcessMgr.h"
#include <Tlhelp32.h>  
#include <Psapi.h>
#include <winuser.h> 
#include <SetupAPI.h>
#pragma comment(lib, "psapi.lib")


 int CProcessMgr::ProcessList()
{
	  
		HANDLE SnapShot, ProcessHandle;  
		SHFILEINFO shSmall;  
		PROCESSENTRY32 ProcessInfo;   
		WCHAR ProcessName[100] = L"Explorer.exe";  // ��Ҫ�رյĽ�������  

		SnapShot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);  
		if(SnapShot != NULL)   
		{  
			ProcessInfo.dwSize = sizeof(ProcessInfo);  // ����ProcessInfo�Ĵ�С  
			BOOL Status = Process32First(SnapShot, &ProcessInfo);  
			while(Status)  
			{  
				// ��ȡ�����ļ���Ϣ  
				SHGetFileInfo(ProcessInfo.szExeFile, 0, &shSmall, sizeof(shSmall), SHGFI_ICON|SHGFI_SMALLICON);  

				// �������Ƿ���Ҫ�ر�  
				 wcslwr(ProcessInfo.szExeFile);  

				if(!wcscmp(wcslwr(ProcessName), ProcessInfo.szExeFile))   
				{  
					// ��ȡ���̾����ǿ�йر�  
					ProcessHandle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, ProcessInfo.th32ProcessID);  
					TerminateProcess(ProcessHandle, 1);  
					break;  
				}  

				// ��ȡ��һ�����̵���Ϣ  
				Status = Process32Next(SnapShot, &ProcessInfo);  
			}  
		}  

		// ��explorer.exe  
		ShellExecute(NULL, L"open", wcslwr(ProcessName), NULL, NULL, SW_SHOWNORMAL);  

		return 0;  
	 
	return 0;
}

 int CProcessMgr::GetProcessIdByText(CDWordArray &pidArr,CString sText,bool bWholeMatch)
 {
	 PROCESSENTRY32 pe;  // ������Ϣ  
	 pidArr.RemoveAll();
	 pe.dwSize = sizeof(PROCESSENTRY32);  
	 HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0); // ���̿���  
	 if(!Process32First(hSnapshot, &pe)) // �õ���һ�����̵Ŀ���  
		 return 0;  
	 do  
	 {  
		 if( sText!=L"")
		 { 
			 if((bWholeMatch?  CString(pe.szExeFile).CompareNoCase(sText) == 0 : 
				 CString(pe.szExeFile).MakeLower().Find(sText.MakeLower()) >= 0))
				 pidArr.Add(pe.th32ProcessID);
		 }
		 else
			 pidArr.Add(pe.th32ProcessID);
		 if(!Process32Next(hSnapshot, &pe))  
			 break;
	 }while(1);
	 // while (bWholeMatch?  CString(pe.szExeFile).CompareNoCase(sText+".exe") != 0 : 
		// CString(pe.szExeFile).MakeLower().Find(sText) < 0);  // ��������ֱ���Ҵ�Ŀ�����  
	
	return pidArr.GetCount();   
 }
 
 CString CProcessMgr::GetProcessFileName(HWND hwnd)
{
	CString s;
	TCHAR title[MAX_SUBTITLE_LEN];
	GetWindowText(hwnd,title,sizeof(title));
	DWORD proId=0;
	HANDLE hProcess=NULL;
	GetWindowThreadProcessId(hwnd, &proId);
	hProcess = OpenProcess(PROCESS_ALL_ACCESS,FALSE, proId);
	HMODULE hModule=0;
	GetModuleFileNameEx(hProcess, hModule,(LPWSTR)title, sizeof(title));
	CloseHandle(hProcess);
	return title;
}

 vector<HWND> m_hwndArray;


 bool m_bChildWnd  ;
 CStringArray m_sFindWndText;
 BOOL CALLBACK EnumChildWindowCallBack(HWND hWnd, LPARAM lParam)  
 {  
	 DWORD dwPid = 0;  
	 GetWindowThreadProcessId(hWnd, &dwPid); // ����ҵ����������Ľ���  
	 if(dwPid == lParam) // �ж��Ƿ���Ŀ����̵Ĵ���  
	 {  
		 TRACE("0x%08X   \n ", hWnd); // ���������Ϣ  
		 if(m_sFindWndText.GetCount()==0)
			 m_hwndArray.push_back(hWnd);
		 else
		 {
			 TCHAR tmp[1024]; 
			 ::GetWindowText(hWnd,tmp,sizeof(tmp));	
			  if(!CString(tmp).IsEmpty())
			 for (int i=0;i<m_sFindWndText.GetCount();i++)
			 {									  
				 if(CString(tmp).MakeLower().Find(m_sFindWndText.ElementAt(i))>=0)	
					 m_hwndArray.push_back(hWnd);
			 }
		 }
		 
		 TCHAR buf[MAX_SUBTITLE_LEN];  
		 SendMessage(hWnd, WM_GETTEXT, MAX_SUBTITLE_LEN, (LPARAM)buf);  
		//TRACE(L"%s\n", buf);  
		 EnumChildWindows(hWnd, EnumChildWindowCallBack, lParam);    // �ݹ�����Ӵ���  
	 }  
	 return TRUE;   
 }  

 BOOL CALLBACK EnumWindowCallBack(HWND hWnd, LPARAM lParam)  
 {  
	 DWORD dwPid = 0;  
	 GetWindowThreadProcessId(hWnd, &dwPid); // ����ҵ����������Ľ���  
	 if(dwPid == lParam) // �ж��Ƿ���Ŀ����̵Ĵ���  
	 {  
		// CString s = CProcessMgr:: GetProcessFileName(hWnd);
		 printf("0x%08X    ", hWnd); // ���������Ϣ  
		 TCHAR buf[256];  
		 if(m_sFindWndText.GetCount()==0)
			 m_hwndArray.push_back(hWnd);
		 else
		 {
			 TCHAR tmp[1024];
			   ::GetWindowText(hWnd,tmp,sizeof(tmp));	
			   TRACE(L"%s\n", tmp);  
			   if(!CString(tmp).IsEmpty())
			 for (int i=0;i<m_sFindWndText.GetCount();i++)
			 {
								  
			 if(CString(tmp).MakeLower().Find(m_sFindWndText.ElementAt(i))>=0)	
				 m_hwndArray.push_back(hWnd);
			 }
		 }
		 SendMessage(hWnd, WM_GETTEXT, MAX_SUBTITLE_LEN, (LPARAM)buf);  
		 if(m_bChildWnd)
			 EnumChildWindows(hWnd, EnumChildWindowCallBack, lParam);    // ���������Ӵ���  
	 }  
	 return TRUE;  

	 
 }  

 vector<HWND> CProcessMgr::EnumWnds(DWORD pid,CStringArray *text,bool child)
 {
	 m_sFindWndText.RemoveAll();
	 if(text !=NULL)
	 for (int i=0;i<text->GetCount();i++)
	 {
		 m_sFindWndText.Add(text->ElementAt(i).MakeLower());
	 }
	 m_bChildWnd = child;
	 m_hwndArray.clear();
	 EnumWindows(EnumWindowCallBack, pid);  
	 return m_hwndArray;
 }

 LRESULT GetValue(HKEY hKey, LPCTSTR name, LPTSTR value, LPLONG size)
 {
	 return ::RegQueryValueEx(hKey, name, NULL, NULL, (LPBYTE)value, (LPDWORD)size);
 }
 int  DumpSoftware(LPCTSTR szKey , HKEY hParent, CString &path)
 {
	 LRESULT lr;
	 HKEY hKey;
	 LONG size;
	 TCHAR buffer[MAX_PATH] ;
	 CString name;
	 lr = RegOpenKey(hParent, szKey, &hKey);

	 //���ܴ�ע���
	 if(lr != ERROR_SUCCESS)
	 {
		// cout << _T("���ܴ�ע���") << szKey << _T("(") << lr << _T(")") << endl;
		 return -1;
	 } 

	 size = sizeof(buffer);
	 lr = GetValue(hKey, _T("DisplayName"), &buffer[0], &size);
	 name = buffer;
	 //ע���򿪳ɹ�
	 if(lr == ERROR_SUCCESS)
	 {
		 if(size > 0)
		 {
			// cout << _T("��ʾ���֣�" ) << buffer << endl;
		 }
	 }
	 else
	 {  
		 size = sizeof(buffer);
		 lr = GetValue(hKey, _T("QuietDisplayName"), &buffer[0], &size);
		 if(ERROR_SUCCESS== lr && size > 0 )
		 {
			// cout << _T("��ʾ���֣�" ) << buffer << endl;
		 }
	 }


	 size = sizeof(buffer);
	 lr = GetValue(hKey, _T("DisplayVersion"), &buffer[0], &size);

	 //��ȡ����汾��
	 if(ERROR_SUCCESS == lr && size > 0)
	 {
		// cout << _T("����汾�ţ�") << buffer << endl;
	 }

	 size = sizeof(buffer);
	 lr = GetValue(hKey, _T("Publisher"), &buffer[0], &size);

	 //���������
	 if(ERROR_SUCCESS == lr && size > 0)
	 {
		// cout << _T("�����̣�") << buffer << endl;
	 }

	 size = sizeof(buffer);
	 lr = GetValue(hKey, _T("InstallLocation"), &buffer[0], &size);

	 //��ȡ��װ·��
	 if(ERROR_SUCCESS == lr && size > 0)
	 {
		// cout << _T("��װ·����") << buffer << endl;
	 }

	 size = sizeof(buffer);
	 lr = GetValue(hKey, _T("InstallSource"), &buffer[0], &size);
	 path = buffer;
	 //��ȡ��װԴ
	 if(ERROR_SUCCESS == lr && size > 0)
	 {
		// cout << _T("��װԴ��") << buffer << endl;
	 }

	 size = sizeof(buffer);
	 lr = GetValue(hKey, _T("UninstallPath"), &buffer[0], &size);

	 //��ȡж��·��
	 if(ERROR_SUCCESS == lr && size > 0)
	 {
		 //cout << _T("ж��·����") << buffer << endl;
	 }

	 size = sizeof(buffer);
	 lr = GetValue(hKey, _T("UninstallString"), &buffer[0], &size);

	 //���ж������
	 if(ERROR_SUCCESS == lr && size > 0)
	 {
		 //cout << _T("���ж�����") << buffer << endl;
	 }
	 RegCloseKey(hKey);
 }

 int CProcessMgr::FindSoft(CStringArray &softArr)
 {
	 unsigned long index;
	 TCHAR buffer[MAX_PATH];
	 HKEY hKey;
	 LRESULT lr;
	 CString path = NULL;
	 lr = RegOpenKey(HKEY_LOCAL_MACHINE, _T("SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Uninstall"), & hKey);
	 if(lr != ERROR_SUCCESS)
	 {
		// cout << _T("���ܴ�ע���") << lr << endl;
		 return -1;
	 }
	 int total=0;
	 for(index =0; ;index++)
	 {
		 lr = RegEnumKey(hKey, index, &buffer[0], sizeof(buffer));
		 switch(lr)
		 {
		 case ERROR_SUCCESS:
			
			 if(DumpSoftware(buffer,hKey, path) > 0)
			 {
				total++;
				softArr.Add(path);
			 }
			 break;
		 case ERROR_NO_MORE_ITEMS:
			// cout<<_T("������������") << endl;
			 RegCloseKey(hKey);
			 return 0;
		 default:
			// cout<<_T("δ��������������") << lr << endl;
			 RegCloseKey(hKey);
			 return -2;
		 }
	 }
	 //cout<<_T("������������") << endl;
	 RegCloseKey(hKey);
 }

//void Inject()
//{
// HWND hWnd=::FindWindow(NULL,_T("Windows ���������"));
// DWORD dwId;
// ::GetWindowThreadProcessId(hWnd,&dwId);
// HANDLE hProcess=::OpenProcess(PROCESS_VM_OPERATION | PROCESS_CREATE_THREAD |PROCESS_VM_READ|PROCESS_VM_WRITE,FALSE,dwId);
// DWORD dwErr=::GetLastError();
// char szDll[MAX_PATH]="D:\\MyDllInject\\Debug\\GetModule.dll";
// PVOID pvoid=::VirtualAllocEx(hProcess,NULL,sizeof(szDll),MEM_COMMIT,PAGE_EXECUTE_READWRITE);
// dwErr=::GetLastError();
// DWORD dwWrite;
// BOOL result=::WriteProcessMemory(hProcess,pvoid,szDll,sizeof(szDll),&dwWrite);
// 
// PROC proc=::GetProcAddress(::GetModuleHandle(_T("kernel32.dll")),"LoadLibraryA");
// HANDLE hRemote=::CreateRemoteThread(hProcess,NULL,0,(LPTHREAD_START_ROUTINE)proc,pvoid,0,NULL);
// ::WaitForSingleObject(hRemote,INFINITE);
// ::VirtualFreeEx(hProcess,pvoid,sizeof(szDll),0);
// ::CloseHandle(hProcess);
// ::CloseHandle(hRemote);
//}
// 
//----------------DLL����----------------------------
//#include "windows.h"
//#include "tlHelp32.h"
//--�������--
//BOOL WINAPI MyTerminateProcess(HANDLE hProcess,UINT uExitCode)
//{
// MessageBox(NULL,_T("��ֹ����"),_T("��ʾ"),MB_OK);
// return TRUE;
//}
//--��Ҫ���ú�������DLLMAIN�����DLL_PROCESS_ATTACH:����
//void ReplaceApi()
//{
// PROC g_proc=(PROC)::TerminateProcess;
// HMODULE hModule=::GetModuleHandle(NULL);
// IMAGE_DOS_HEADER *pImgDosHeader=(IMAGE_DOS_HEADER*)hModule;
// IMAGE_NT_HEADERS *pNtHeader = (IMAGE_NT_HEADERS*)((BYTE*)hModule+pImgDosHeader->e_lfanew);
// IMAGE_IMPORT_DESCRIPTOR *pImgImport=(IMAGE_IMPORT_DESCRIPTOR*)((BYTE*)hModule+pNtHeader->OptionalHeader.DataDirectory[1].VirtualAddress);
// while(pImgImport->FirstThunk)
// {
//  char *szDllName=(char*)((BYTE*)hModule+pImgImport->Name);
//  if(lstrcmpiA(szDllName,"kernel32.dll") == 0)
//  {
//   //*����û�����Ȱ󶨵�PE�ļ���˵��OriginalFirstThunk��FirstThunk�ǲ��е�������ͬ�����顣���Ƕ����Ѿ������󶨵�
//   //PE�ļ���˵��FirstThunk�����е�Ԫ�ػᱻ���ó������ĺ�����ַ(VA);
//   IMAGE_THUNK_DATA32 *pThunk=(IMAGE_THUNK_DATA32*)((BYTE*)hModule+pImgImport->FirstThunk);
//   while(pThunk->u1.Function)
//   {
//    if(pThunk->u1.AddressOfData & IMAGE_ORDINAL_FLAG32)
//    {
//     
//    }
//    else
//    {
//     IMAGE_IMPORT_BY_NAME *pName=(PIMAGE_IMPORT_BY_NAME)((DWORD)hModule+pThunk->u1.AddressOfData);
//     DWORD *lpAddr=(DWORD*)&(pThunk->u1.Function);
//     if(*lpAddr == (DWORD)g_proc)
//     {
//      DWORD *lpNewProc = (DWORD*)MyTerminateProcess;
//      //�޸�ҳ��������
//      DWORD dwOldProtect;
//      MEMORY_BASIC_INFORMATION mbi;
//      ::VirtualQuery(lpAddr,&mbi,sizeof(mbi));
//      ::VirtualProtect(lpAddr,sizeof(DWORD),PAGE_READWRITE,&dwOldProtect);
//      ::WriteProcessMemory(GetCurrentProcess(),lpAddr,&lpNewProc,sizeof(DWORD),NULL);
//      ::VirtualProtect(lpAddr,sizeof(DWORD),dwOldProtect,0);
//      break;
//     }
//     char *szName=(char*)(ULONG)(pName->Name);
//    }
//    pThunk++;
//   }
//  }
//  pImgImport++;
// }
// ::MessageBoxA(NULL,"XXX","YYY",MB_OK);
//}