// ExplorerListTarget.cpp: implementation of the CExplorerListTarget class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "cmyexplorer.h"
#include "ExplorerListTarget.h"
#include "ExplorerListView.h"
#include "ExplorerTreeView.h"
#include "ShellFileOp.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

extern CExplorerListView * g_ListView;
extern CExplorerTreeView * g_TreeView;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CExplorerListTarget::CExplorerListTarget()
{
	m_nLastDrop = -1;
}

CExplorerListTarget::~CExplorerListTarget()
{

}

DROPEFFECT CExplorerListTarget::OnDragEnter( CWnd* pWnd, COleDataObject* pDataObject, DWORD dwKeyState, CPoint point )
{
	DROPEFFECT dropeffectRet = DROPEFFECT_NONE;

	return dropeffectRet;
}

DROPEFFECT CExplorerListTarget::OnDragOver(CWnd* pWnd, COleDataObject* pDataObject, DWORD dwKeyState, CPoint point )
{
	DROPEFFECT dropeffectRet = DROPEFFECT_NONE;

	if ( pDataObject->IsDataAvailable(CF_HDROP) )
	{
		if ( (dwKeyState & MK_SHIFT) == MK_SHIFT)
			dropeffectRet = DROPEFFECT_MOVE;
		else
			dropeffectRet = DROPEFFECT_COPY;

		int nHit = g_ListView->m_pListCtrl->HitTest(point);
		if ( nHit!=-1 )
		{
			LPLISTPARAM pliParam = (LPLISTPARAM)g_ListView->m_pListCtrl->GetItemData(nHit);
			if ( pliParam->uType==0 )
			{
				g_ListView->m_pListCtrl->SetItemState(nHit,LVIS_DROPHILITED,LVIS_DROPHILITED);
				
				if ( m_nLastDrop!=-1 && m_nLastDrop!=nHit )
					g_ListView->m_pListCtrl->SetItemState(m_nLastDrop,~LVIS_DROPHILITED,LVIS_DROPHILITED);
				m_nLastDrop = nHit;
			}
			else
			{
				g_ListView->m_pListCtrl->SetItemState(m_nLastDrop,~LVIS_DROPHILITED,LVIS_DROPHILITED);
				m_nLastDrop = -1;
			}
		}
		else
		{
			g_ListView->m_pListCtrl->SetItemState(m_nLastDrop,~LVIS_DROPHILITED,LVIS_DROPHILITED);
			m_nLastDrop = -1;
		}
	}
	
	return dropeffectRet;
}

void CExplorerListTarget::OnDragLeave( CWnd* pWnd )
{
	if ( g_ListView && m_nLastDrop!=-1 )
	{
		g_ListView->m_pListCtrl->SetItemState(m_nLastDrop,~LVIS_DROPHILITED,LVIS_DROPHILITED);
		m_nLastDrop = -1;
	}
}

BOOL CExplorerListTarget::OnDrop(CWnd* pWnd, COleDataObject* pDataObject,DROPEFFECT dropEffect, CPoint point)
{	
	if ( g_ListView )
	{
		if ( m_nLastDrop!=-1 )
		{
			g_ListView->m_pListCtrl->SetItemState(m_nLastDrop,~LVIS_DROPHILITED,LVIS_DROPHILITED);
			m_nLastDrop = -1;
		}
				
		//获取拖放的信息
		if ( pDataObject->IsDataAvailable(CF_HDROP) )
		{
			HGLOBAL hglobal = pDataObject->GetGlobalData(CF_HDROP, NULL);
			if( hglobal != NULL )
			{
				CShellFileOp op;
				if ( dropEffect & DROPEFFECT_MOVE )
					op.SetOperationFlags(FO_MOVE, g_ListView, FOF_ALLOWUNDO|FOF_NOCONFIRMMKDIR);
				else
					op.SetOperationFlags(FO_COPY, g_ListView, FOF_ALLOWUNDO|FOF_NOCONFIRMMKDIR);

		//		op.SetOperationFlags(FO_COPY, g_ListView,
		//			FOF_ALLOWUNDO|FOF_FILESONLY|FOF_NOCONFIRMMKDIR);
				
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
				ZeroMemory(szFileName,MAX_PATH);

				LPTREEPARAM ptrItem = (LPTREEPARAM)g_TreeView->GetSelTreeData();
				SHGetPathFromIDList(ptrItem->lpidl,szDest);
				int  nHit = g_ListView->m_pListCtrl->HitTest(point);
				if ( nHit!=-1 )
				{
					LPLISTPARAM pliItem = (LPLISTPARAM)g_ListView->m_pListCtrl->GetItemData(nHit);
					if ( pliItem->uType==0 )
						SHGetPathFromIDList(pliItem->lpidl,szDest);
				}
				op.AddDestFile (szDest);
				BOOL bSuccess, bAPICalled = FALSE, bAborted = FALSE;
				int  nAPIReturn = 0;
				
				bSuccess = op.Go (&bAPICalled, &nAPIReturn, &bAborted);
				
				GlobalFree(hglobal);
				delete szDest;
				
				//refresh
				if ( bSuccess && !bAborted )
				{
					g_TreeView->SendMessage(WM_REFRESHFILES,0,0);
					g_ListView->SendMessage(WM_REFRESHFILES,0,0);
					g_TreeView->SendMessage(WM_REFRESHFILES,(WPARAM)g_TreeView->m_hMoveSrc,0);
				}
			}
		}
	}

	return true;
}
