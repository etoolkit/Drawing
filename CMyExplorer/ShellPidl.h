// ShellPidl.h: interface for the CShellPidl class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SHELLPIDL_H__2407E43F_1F47_47CE_86FA_E4562590DB0A__INCLUDED_)
#define AFX_SHELLPIDL_H__2407E43F_1F47_47CE_86FA_E4562590DB0A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

typedef struct tagTREEPARAM
{
   LPSHELLFOLDER lpsf;
   LPITEMIDLIST  lpidl;
   UINT		     uParam;		
} TREEPARAM, *LPTREEPARAM;

typedef struct tagLISTPARAM
{
	LPSHELLFOLDER lpsf;
	LPITEMIDLIST  lpidl;
	UINT     uType;
	TCHAR    szName[128];
}LISTPARAM, *LPLISTPARAM;

class CShellPidl  
{
public:
	LPITEMIDLIST CopyItemID(LPITEMIDLIST pidl,int n=0);
	LPITEMIDLIST CopyAbsItemID(LPITEMIDLIST pidl,int n);
	BOOL ComparePidls(LPSHELLFOLDER pFolder,LPCITEMIDLIST pidl1,LPCITEMIDLIST pidl2);
	LPCITEMIDLIST Next(LPCITEMIDLIST pidl);
	CShellPidl();
	virtual ~CShellPidl();

 IMalloc  * m_pMalloc;

protected:
	UINT GetPidlSize(LPCITEMIDLIST pidl);
	LPITEMIDLIST CopyItemIDList(LPITEMIDLIST pidl);
	LPITEMIDLIST ConcatPidl(LPITEMIDLIST pidlDest,LPITEMIDLIST pidlSrc);
};

#endif // !defined(AFX_SHELLPIDL_H__2407E43F_1F47_47CE_86FA_E4562590DB0A__INCLUDED_)
