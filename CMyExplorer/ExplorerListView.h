#if !defined(AFX_EXPLORERLISTVIEW_H__3C9517AF_311D_486D_98D6_0C48C329C7D2__INCLUDED_)
#define AFX_EXPLORERLISTVIEW_H__3C9517AF_311D_486D_98D6_0C48C329C7D2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ExplorerListView.h : header file
//
#include <afxcview.h>
#include "ShellPidl.h"
#include "ExplorerListTarget.h"

/////////////////////////////////////////////////////////////////////////////
// CExplorerListView view
extern int g_nThreadNum;
extern HANDLE g_hThread[2000];
class CExplorerListView : public CListView, public CShellPidl
{
protected:
	CExplorerListView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CExplorerListView)

// Attributes
public:
	CListCtrl  * m_pListCtrl;
	CExplorerListTarget     m_ListTarget;

protected:
	DWORD        m_dViewType; 

// Operations
public:
	bool m_BStop;
	
	int SetListViewType(DWORD viewType);
	static int CALLBACK SortListView(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort);
	void SetListContext(IShellFolder *psfParent, LPITEMIDLIST pidlParent);
	void SetItems(SHFILEINFO &sfi, LPLISTPARAM &pliParam);
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CExplorerListView)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CExplorerListView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
protected:
	void DeleteAllItems();
	void InitImageList();
	
	//{{AFX_MSG(CExplorerListView)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnRclick(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDblclk(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnBegindrag(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnEndlabeledit(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	afx_msg long OnRefreshList(WPARAM wParam, LPARAM lParam);
	afx_msg void OnMenuSelect(UINT nItemID, UINT nFlags, HMENU hSysMenu);
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EXPLORERLISTVIEW_H__3C9517AF_311D_486D_98D6_0C48C329C7D2__INCLUDED_)
