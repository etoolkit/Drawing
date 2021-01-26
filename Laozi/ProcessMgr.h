#pragma once 
#include <vector>
#include <afxcoll.h>
using namespace std;

class CProcessMgr
{
public:
	
	static 	int ProcessList();
	static	CString GetProcessFileName(HWND hwnd );
	static  int  GetProcessIdByText(CDWordArray &pidArr, CString sText, bool bWholeMatch = false);
	static vector<HWND> EnumWnds(DWORD pId,CStringArray *text=NULL,bool child=true);
	static int FindSoft(CStringArray &sofeArr);
};

