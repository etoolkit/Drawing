// MyHookDll.cpp : ���� DLL �ĳ�ʼ�����̡�
//

#include "stdafx.h"
#define DllExport __declspec(dllexport)
#include "Myhook.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

DllExport  void WINAPI InstallHook(HWND hWnd)
{
	startKeyboardhook();
	startMousehook(hWnd);
}
DllExport void WINAPI UnInstallHook()
{
	stopKeyboardhook();
	stopMousehook();
}