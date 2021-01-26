// dllmain.cpp : DllMain 的实现。

#include "stdafx.h"
#include "MyATL_i.h"
//#include "MyATL_i.c"
#include "dllmain.h" 
#include <initguid.h>   
using namespace ATL;

CMyATLModule _AtlModule;
//CComModule _Module;
//BEGIN_OBJECT_MAP(ObjectMap)
//	OBJECT_ENTRY(CLSID_LATLClass, CLATLClass)
//END_OBJECT_MAP()
// DLL 入口点
extern "C" BOOL WINAPI DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
{ 
	if (dwReason == DLL_PROCESS_ATTACH)
	{
		//_Module.Init(ObjectMap, hInstance, &LIBID_MyATLLib);		
		DisableThreadLibraryCalls(hInstance);
	}
	else if (dwReason == DLL_PROCESS_DETACH)
		_AtlModule.Term();
	return TRUE;    // ok
 
}