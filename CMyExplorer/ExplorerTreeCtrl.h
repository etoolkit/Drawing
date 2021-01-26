#if !defined(AFX_EXPLORERTREECTRL_H__F4046C0C_A807_4C34_BC96_6D86C7B8C877__INCLUDED_)
#define AFX_EXPLORERTREECTRL_H__F4046C0C_A807_4C34_BC96_6D86C7B8C877__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ExplorerTreeCtrl.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CExplorerTreeCtrl window

class CExplorerTreeCtrl : public CTreeCtrl
{
// Construction
public:
	CExplorerTreeCtrl();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CExplorerTreeCtrl)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CExplorerTreeCtrl();

	// Generated message map functions
protected:
	//{{AFX_MSG(CExplorerTreeCtrl)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EXPLORERTREECTRL_H__F4046C0C_A807_4C34_BC96_6D86C7B8C877__INCLUDED_)
