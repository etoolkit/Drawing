// LATLClass.cpp : CLATLClass µÄÊµÏÖ

#include "stdafx.h"
#include "LATLClass.h"


// CLATLClass


HRESULT CLATLClass::Initialize(LPCITEMIDLIST pidlFolder,	
	LPDATAOBJECT lpdobj, 
	HKEY hkeyProgID)
{		
	if(NULL==lpdobj){
		return E_INVALIDARG;
	}
	FORMATETC feFmtEtc = { CF_HDROP, NULL, DVASPECT_CONTENT, -1, TYMED_HGLOBAL };
	STGMEDIUM mdmSTG = { TYMED_HGLOBAL };

	if(FAILED(lpdobj->GetData(&feFmtEtc, &mdmSTG)))
	{
		return E_INVALIDARG;
	}

	//get user droped file counts
	HDROP hDrop = (HDROP)GlobalLock(mdmSTG.hGlobal);
	// Make sure it worked.
	if(NULL==hDrop)
	{
		return E_INVALIDARG;
	}

	int nDropCount = ::DragQueryFile(hDrop, 0xFFFFFFFF, NULL, 0);
	if(1==nDropCount){		
		::DragQueryFile(hDrop, 0, m_pzDropFile, MAX_PATH);		
	}	
	GlobalUnlock(hDrop);
	//Release the medium storage
	::ReleaseStgMedium(&mdmSTG);
	//return SUCCEED flag
	return S_OK;
}

HRESULT CLATLClass::QueryContextMenu(HMENU hmenu, 
	UINT indexMenu, UINT idCmdFirst, 
	UINT idCmdLast, UINT uFlags)
{
	if(!(uFlags & CMF_DEFAULTONLY)){
		::InsertMenu(hmenu, indexMenu, MF_BYPOSITION, 
			idCmdFirst, _T("&Register Component"));
		::InsertMenu(hmenu, indexMenu, MF_BYPOSITION, 
			idCmdFirst+1, _T("&UnRegister Component"));
		return MAKE_HRESULT(SEVERITY_SUCCESS, 0, USHORT(2));
	}
	return MAKE_HRESULT(SEVERITY_SUCCESS, 0, USHORT(0));
}

HRESULT CLATLClass::GetCommandString(
	UINT        idCmd,
	UINT        uType,
	UINT        *pwReserved,
	LPSTR       pszName,
	UINT        cchMax)
{
	USES_CONVERSION;

	HRESULT hr = E_INVALIDARG;

	LPCTSTR IDM_REG_MNU_HLPTXT = _T("Register Anyone(DLL/OCX...)Com Component");
	LPCTSTR IDM_UNREG_MNU_HLPTXT = _T("UnRegister Anyone(DLL/OCX...)Com Component");

	if(uType & GCS_HELPTEXT)
	{
		switch(idCmd){

		case 0:
			{
				if(uType & GCS_UNICODE)
				{
					lstrcpynW((LPWSTR)pszName, T2CW(IDM_REG_MNU_HLPTXT), cchMax);
				}
				else
				{
					lstrcpynA (pszName, T2CA(IDM_REG_MNU_HLPTXT), cchMax );
				}
				hr = S_OK;
			}
			break;

		case 1:
			{
				if(uType & GCS_UNICODE)
				{
					lstrcpynW((LPWSTR)pszName, T2CW(IDM_UNREG_MNU_HLPTXT), cchMax);
				}
				else
				{
					lstrcpynA (pszName, T2CA(IDM_UNREG_MNU_HLPTXT), cchMax );
				}
				hr = S_OK;
			}
			break;
		}	
	}
	return hr;
}

HRESULT CLATLClass::InvokeCommand(LPCMINVOKECOMMANDINFO lpici)
{
	USES_CONVERSION;

	int nFlag = HIWORD(lpici->lpVerb);
	//user clicked menu's id 
	int nMnuId = LOWORD(lpici->lpVerb);

	if(0 != nFlag){
		return E_INVALIDARG;
	}

	//response the menu item command 
	switch(nMnuId){

	case 0:
		{
			TCHAR szCmdLine[256];
			memset(szCmdLine, 0x0, 256*sizeof(TCHAR));
			wsprintf(szCmdLine, _T("regsvr32 \"%s\""), m_pzDropFile);
			::WinExec(W2A(szCmdLine), SW_HIDE);
			return S_OK;
		}		
		break;

	case 1:
		{	
			TCHAR szCmdLine[256];
			memset(szCmdLine, 0x0, 256*sizeof(TCHAR));
			wsprintf(szCmdLine, _T("regsvr32 /u \"%s\""), m_pzDropFile);			
			::WinExec(W2A(szCmdLine), SW_HIDE);
			return S_OK;
		}
		break;
	}
	return E_INVALIDARG;
}