// LATLClass.h : CLATLClass ������

#pragma once
#include "resource.h"       // ������



#include "MyATL_i.h"
#include <shobjidl.h>



#if defined(_WIN32_WCE) && !defined(_CE_DCOM) && !defined(_CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA)
#error "Windows CE ƽ̨(�粻�ṩ��ȫ DCOM ֧�ֵ� Windows Mobile ƽ̨)���޷���ȷ֧�ֵ��߳� COM ���󡣶��� _CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA ��ǿ�� ATL ֧�ִ������߳� COM ����ʵ�ֲ�����ʹ���䵥�߳� COM ����ʵ�֡�rgs �ļ��е��߳�ģ���ѱ�����Ϊ��Free����ԭ���Ǹ�ģ���Ƿ� DCOM Windows CE ƽ̨֧�ֵ�Ψһ�߳�ģ�͡�"
#endif

using namespace ATL;


// CLATLClass

class ATL_NO_VTABLE CLATLClass :
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CLATLClass, &CLSID_LATLClass>,
	public IDispatchImpl<ILATLClass, &IID_ILATLClass, &LIBID_MyATLLib, /*wMajor =*/ 1, /*wMinor =*/ 0>,
	public IShellExtInit,
	public IContextMenu
{
public:
	CLATLClass()
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_LATLCLASS)


BEGIN_COM_MAP(CLATLClass)
	COM_INTERFACE_ENTRY(ILATLClass)
	COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY(IShellExtInit)
	COM_INTERFACE_ENTRY(IContextMenu)
END_COM_MAP()



	DECLARE_PROTECT_FINAL_CONSTRUCT()

	HRESULT FinalConstruct()
	{
		return S_OK;
	}

	void FinalRelease()
	{
	}

public:
	// IShellExtInit
	STDMETHOD(Initialize)(LPCITEMIDLIST, LPDATAOBJECT, HKEY);

	// IContextMenu
	STDMETHOD(GetCommandString)(UINT, UINT, UINT*, LPSTR, UINT);
	STDMETHOD(InvokeCommand)(LPCMINVOKECOMMANDINFO);
	STDMETHOD(QueryContextMenu)(HMENU, UINT, UINT, UINT, UINT);

protected:
	TCHAR m_pzDropFile[MAX_PATH];	//drop file name		


};

OBJECT_ENTRY_AUTO(__uuidof(LATLClass), CLATLClass)
