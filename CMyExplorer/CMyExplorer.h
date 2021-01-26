// CMyExplorer.h : main header file for the CMYEXPLORER application
//

#if !defined(AFX_CMYEXPLORER_H__EA3C422F_51BE_4B05_A23E_3FD84B54B845__INCLUDED_)
#define AFX_CMYEXPLORER_H__EA3C422F_51BE_4B05_A23E_3FD84B54B845__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

#define   WM_REFRESHFILES  WM_USER+245

/////////////////////////////////////////////////////////////////////////////
// CCMyExplorerApp:
// See CMyExplorer.cpp for the implementation of this class
//

class CCMyExplorerApp : public CWinApp
{
public:
	CCMyExplorerApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCMyExplorerApp)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	//}}AFX_VIRTUAL

// Implementation

public:
	//{{AFX_MSG(CCMyExplorerApp)
	afx_msg void OnAppAbout();
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CMYEXPLORER_H__EA3C422F_51BE_4B05_A23E_3FD84B54B845__INCLUDED_)
