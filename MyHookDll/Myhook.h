#pragma once
#define  WM_MYHOOK_KEY		WM_USER + 101
#define WM_MYHOOK_MOUSE     WM_USER + 102
#define WM_MYHOOK_RMOVE		WM_USER + 103
#define WM_MYHOOK_RPRESS	WM_USER + 104
#define WM_MYHOOK_XWHEEL	WM_USER + 105
	BOOL startMousehook(HWND hWnd = NULL); 
	//安装钩子函数 
	BOOL stopMousehook(); 
	BOOL startKeyboardhook(); 
	BOOL stopKeyboardhook(); 
