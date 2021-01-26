// ShellPidl.cpp: implementation of the CShellPidl class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CMyExplorer.h"
#include "ShellPidl.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CShellPidl::CShellPidl()
{
	::SHGetMalloc(&m_pMalloc);
}

CShellPidl::~CShellPidl()
{
	if ( m_pMalloc )
		m_pMalloc->Release();
}

LPITEMIDLIST CShellPidl::ConcatPidl(LPITEMIDLIST pidlDest, LPITEMIDLIST pidlSrc)
{
	UINT cbDest=0;
    UINT cbSrc=0;
    if (pidlDest)  //May be NULL
       cbDest = GetPidlSize(pidlDest) - sizeof(pidlDest->mkid.cb);
    cbSrc = GetPidlSize(pidlSrc);

	// Allocate a new item identifier list. 
	LPITEMIDLIST pidlNew = (LPITEMIDLIST)m_pMalloc->Alloc(cbSrc+cbDest); 
	if (pidlNew == NULL) 
		return NULL; 
	ZeroMemory(pidlNew,cbSrc+cbDest);
	// Copy the specified item identifier. 
	if (pidlDest)
		CopyMemory(pidlNew, pidlDest, cbDest); 
	CopyMemory(((USHORT*)(((LPBYTE)pidlNew)+cbDest)), pidlSrc, cbSrc); 

	return pidlNew; 
}

LPITEMIDLIST CShellPidl::CopyItemIDList(LPITEMIDLIST pidl)
{
	// Allocate a new item identifier list. 
	int nSize = GetPidlSize(pidl);
	LPITEMIDLIST pidlNew = (LPITEMIDLIST)m_pMalloc->Alloc(nSize); 
	ZeroMemory(pidlNew,nSize);
	if (pidlNew == NULL) 
		return NULL; 
	// Copy the specified item identifier. 
	CopyMemory(pidlNew, pidl, nSize); 

	return pidlNew; 
}

UINT CShellPidl::GetPidlSize(LPCITEMIDLIST pidl)
{
	UINT cbTotal = 0;
    if (pidl)
    {
        cbTotal += sizeof(pidl->mkid.cb);       // Null terminator
        while (pidl->mkid.cb)
        {
            cbTotal += pidl->mkid.cb;
            pidl = Next(pidl);
        }
    }
    return cbTotal;
}

LPCITEMIDLIST CShellPidl::Next(LPCITEMIDLIST pidl)
{
	LPSTR lpMem=(LPSTR)pidl;
	
	lpMem+=pidl->mkid.cb;
	
	return (LPITEMIDLIST)lpMem;
}


BOOL CShellPidl::ComparePidls(LPSHELLFOLDER pFolder, LPCITEMIDLIST pidl1, LPCITEMIDLIST pidl2)
{
	if (pFolder == NULL)
		SHGetDesktopFolder(&pFolder);
	if (pidl1 == NULL || pidl2 == NULL)
		return false;
	return (short)pFolder->CompareIDs(0,pidl1,pidl2) == 0;
}

LPITEMIDLIST CShellPidl::CopyAbsItemID(LPITEMIDLIST pidl, int n)
{
	ASSERT(pidl);
    if (pidl == NULL)
		return NULL;
	LPITEMIDLIST first_pidl=NULL;
	LPITEMIDLIST abs_pidl=NULL;
	LPITEMIDLIST new_abs_pidl=NULL;
    for(int i=0;i < n && pidl && pidl->mkid.cb;i++)
    {
		first_pidl = CopyItemID(pidl);
		new_abs_pidl = ConcatPidl(abs_pidl,first_pidl);
		if (abs_pidl)
		{
			m_pMalloc->Free(abs_pidl);
		}
		abs_pidl = new_abs_pidl;
		if (first_pidl)
		{
			m_pMalloc->Free(first_pidl);
		}
        pidl = (const LPITEMIDLIST)Next(pidl);
    }
	return new_abs_pidl;
}

LPITEMIDLIST CShellPidl::CopyItemID(LPITEMIDLIST pidl, int n)
{
	ASSERT(pidl);
	if (n == 0)
	{
		int cb = pidl->mkid.cb;
		int nSize = cb + sizeof(pidl->mkid.cb);
		// Allocate a new item identifier list. 
		LPITEMIDLIST pidlNew = (LPITEMIDLIST)m_pMalloc->Alloc(nSize); 
		ZeroMemory(pidlNew,nSize);
		if (pidlNew == NULL) 
			return NULL; 
		// Copy the specified item identifier. 
		CopyMemory(pidlNew, pidl, nSize-sizeof(pidl->mkid.cb)); 
		return pidlNew; 
	}
	else
	{
		LPITEMIDLIST pidl_index=NULL;
  		for(int i=0;i < n && pidl->mkid.cb;i++)
		{
			pidl_index = pidl;
			pidl = (const LPITEMIDLIST)Next(pidl);
		}
		return pidl_index ? CopyItemID(pidl_index,0) : NULL;
	}
	return NULL;
}
