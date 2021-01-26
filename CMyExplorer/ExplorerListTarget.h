// ExplorerListTarget.h: interface for the CExplorerListTarget class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_EXPLORERLISTTARGET_H__DAC8D4AA_18A7_415D_B7E1_0CE152B74599__INCLUDED_)
#define AFX_EXPLORERLISTTARGET_H__DAC8D4AA_18A7_415D_B7E1_0CE152B74599__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CExplorerListTarget : public COleDropTarget
{
public:
	CExplorerListTarget();
	virtual ~CExplorerListTarget();

protected:
	int  m_nLastDrop;

// Overrides
public:
		virtual DROPEFFECT OnDragEnter( CWnd* pWnd, COleDataObject* pDataObject, 
		DWORD dwKeyState, CPoint point );
		virtual DROPEFFECT OnDragOver( CWnd* pWnd, COleDataObject* pDataObject, 
		DWORD dwKeyState, CPoint point );
		virtual void OnDragLeave( CWnd* pWnd );
		virtual BOOL OnDrop(CWnd* pWnd, COleDataObject* pDataObject,
		DROPEFFECT dropEffect, CPoint point);
};

#endif // !defined(AFX_EXPLORERLISTTARGET_H__DAC8D4AA_18A7_415D_B7E1_0CE152B74599__INCLUDED_)
