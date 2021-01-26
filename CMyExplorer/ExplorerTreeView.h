#if !defined(AFX_EXPLORERTREEVIEW_H__CEBD0E69_9A5E_4C4D_B5D5_B30D9F1F0DDC__INCLUDED_)
#define AFX_EXPLORERTREEVIEW_H__CEBD0E69_9A5E_4C4D_B5D5_B30D9F1F0DDC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ExplorerTreeView.h : header file
//
#include <afxcview.h>
#include "ShellPidl.h"
#include "ExplorerTreeTarget.h"
#include <afxmt.h>

/////////////////////////////////////////////////////////////////////////////
// CExplorerTreeView view
extern CEvent g_event;

class CExplorerTreeView : public CTreeView, public CShellPidl
{
protected:
	CExplorerTreeView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CExplorerTreeView)


// Attributes
public:
	CTreeCtrl    * m_pTreeCtrl;
	IShellFolder * m_psfDesk;
	ITEMIDLIST   * m_pidlDesk, * m_pidlMyComputer;

	HTREEITEM      m_hDesktop;
	HTREEITEM      m_hMyComputer;

	HTREEITEM      m_hMoveSrc;
	HTREEITEM      m_hSelItem;
	LPTREEPARAM    m_pSelLiParam;

	CExplorerTreeTarget m_TreeTarget;

// Operations

public:
	LPTREEPARAM GetSelTreeData();
	void SetTreeContext(IShellFolder *psfParent, LPITEMIDLIST pidlParent);
	void SetSubItemFolder(TV_ITEM tvItemParent);
	static int CALLBACK SortMyComputer(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort);
	void InitExplorerTree();
	void InitImageList();
	void AddChildren(IShellFolder *psfParent, LPITEMIDLIST pidlParent, HTREEITEM hParent, BOOL bExpand);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CExplorerTreeView)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
	//}}AFX_VIRTUAL
// Implementation
protected:
	virtual ~CExplorerTreeView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
protected:

	HTREEITEM FindSubItemByPath(HTREEITEM hParent, LPSTR szPathName);

	
	//{{AFX_MSG(CExplorerTreeView)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnItemexpanding(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSelchanging(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnBegindrag(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnRclick(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	afx_msg long OnRefreshTree(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EXPLORERTREEVIEW_H__CEBD0E69_9A5E_4C4D_B5D5_B30D9F1F0DDC__INCLUDED_)
