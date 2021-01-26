// ExplorerTreeTarget.cpp: implementation of the CExplorerTreeTarget class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "cmyexplorer.h"
#include "ExplorerTreeTarget.h"
#include "ExplorerTreeView.h"
#include "ExplorerListView.h"
#include "ShellFileOp.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


extern CExplorerTreeView * g_TreeView;
extern CExplorerListView * g_ListView;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CExplorerTreeTarget::CExplorerTreeTarget()
{
	
}

CExplorerTreeTarget::~CExplorerTreeTarget()
{

}

DROPEFFECT CExplorerTreeTarget::OnDragEnter( CWnd* pWnd, COleDataObject* pDataObject, DWORD dwKeyState, CPoint point )
{
	DROPEFFECT dropeffectRet = DROPEFFECT_NONE;

	return dropeffectRet;
}

DROPEFFECT CExplorerTreeTarget::OnDragOver(CWnd* pWnd, COleDataObject* pDataObject, DWORD dwKeyState, CPoint point )
{
	DROPEFFECT dropeffectRet = DROPEFFECT_NONE;
	if ( pDataObject->IsDataAvailable(CF_HDROP) )
	{
		if ( g_TreeView )
		{
			HTREEITEM hHit = g_TreeView->m_pTreeCtrl->HitTest(point);
			if ( hHit )
			{
				g_TreeView->m_pTreeCtrl->SelectDropTarget(hHit);

				//	LPTREEPARAM pshItem = (LPTREEPARAM)g_TreeView->m_pTreeCtrl->GetItemData(hHit);
				//	if ( pshItem->lParam==0 )
				{
					if ( (dwKeyState & MK_SHIFT) == MK_SHIFT)
						dropeffectRet = DROPEFFECT_MOVE;
					else
						dropeffectRet = DROPEFFECT_COPY;
				}
			}
			else
			{
				g_TreeView->m_pTreeCtrl->SelectDropTarget(NULL);
			}
		}
	}
	
	return dropeffectRet;
}

void CExplorerTreeTarget::OnDragLeave( CWnd* pWnd )
{
	g_TreeView->m_pTreeCtrl->SelectDropTarget(NULL);
}

BOOL CExplorerTreeTarget::OnDrop(CWnd* pWnd, COleDataObject* pDataObject,DROPEFFECT dropEffect, CPoint point)
{	
	if ( g_TreeView )
	{
		g_TreeView->m_pTreeCtrl->SelectDropTarget(NULL);
		HTREEITEM hHit = g_TreeView->m_pTreeCtrl->HitTest(point);
		if ( hHit )
		{
			//获取拖放的信息
			if ( pDataObject->IsDataAvailable(CF_HDROP) )
			{
				HGLOBAL hglobal = pDataObject->GetGlobalData(CF_HDROP, NULL);
				if( hglobal != NULL )
				{
					CShellFileOp op;
					if ( dropEffect & DROPEFFECT_MOVE )
						op.SetOperationFlags(FO_MOVE, g_TreeView, FOF_ALLOWUNDO|FOF_NOCONFIRMMKDIR);
					else
						op.SetOperationFlags(FO_COPY, g_TreeView, FOF_ALLOWUNDO|FOF_NOCONFIRMMKDIR);
					
					char * szFileName = new char[MAX_PATH];
					HDROP hDrop = (HDROP)GlobalLock(hglobal);
					int FileNum = DragQueryFile(hDrop,-1,NULL,0);
					for ( int i =0 ; i < FileNum ; i++ )
					{
						ZeroMemory(szFileName,MAX_PATH);
						DragQueryFile(hDrop,i,szFileName,255);
						op.AddSourceFile(szFileName);
					}
					delete szFileName;

					char * szDest = new char[MAX_PATH];
					ZeroMemory(szDest,MAX_PATH);

					LPTREEPARAM ptrItem = (LPTREEPARAM)g_TreeView->m_pTreeCtrl->GetItemData(hHit);
					SHGetPathFromIDList(ptrItem->lpidl,szDest);
					op.AddDestFile (szDest);
					BOOL bSuccess, bAPICalled = FALSE, bAborted = FALSE;
					int  nAPIReturn = 0;
					
					bSuccess = op.Go ( &bAPICalled, &nAPIReturn, &bAborted );
					
					GlobalFree(hglobal);
					delete szDest;
					
					//refresh
					if ( bSuccess && !bAborted )
					{
						g_TreeView->SendMessage(WM_REFRESHFILES,(WPARAM)hHit,0);
						g_ListView->SendMessage(WM_REFRESHFILES,0,0);
						g_TreeView->SendMessage(WM_REFRESHFILES,(WPARAM)g_TreeView->m_hMoveSrc,0);
					}
				}
			}
		}
	}

	return true;
}
