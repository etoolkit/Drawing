// MyHookDll.cpp : 定义 DLL 的初始化例程。
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