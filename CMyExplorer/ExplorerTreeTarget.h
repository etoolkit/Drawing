// ExplorerTreeTarget.h: interface for the CExplorerTreeTarget class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_EXPLORERTREETARGET_H__3D94BEBD_D2C1_4863_BD59_19BEF62105BB__INCLUDED_)
#define AFX_EXPLORERTREETARGET_H__3D94BEBD_D2C1_4863_BD59_19BEF62105BB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CExplorerTreeTarget : public COleDropTarget
{

public:
	CExplorerTreeTarget();
	virtual ~CExplorerTreeTarget();

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

#endif // !defined(AFX_EXPLORERTREETARGET_H__3D94BEBD_D2C1_4863_BD59_19BEF62105BB__INCLUDED_)
