// ExplorerTreeCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "cmyexplorer.h"
#include "ExplorerTreeCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CExplorerTreeCtrl

CExplorerTreeCtrl::CExplorerTreeCtrl()
{
}

CExplorerTreeCtrl::~CExplorerTreeCtrl()
{
}


BEGIN_MESSAGE_MAP(CExplorerTreeCtrl, CTreeCtrl)
	//{{AFX_MSG_MAP(CExplorerTreeCtrl)
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CExplorerTreeCtrl message handlers

int CExplorerTreeCtrl::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CTreeCtrl::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here
	
	return 0;
}
