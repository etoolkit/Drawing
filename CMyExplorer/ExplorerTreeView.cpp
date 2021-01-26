// ExplorerTreeView.cpp : implementation file
//

#include "stdafx.h"
#include "CMyExplorer.h"
#include "ExplorerTreeView.h"
#include "ExplorerListView.h"
#include "ShellContextMenu.h"
#include "ShellFileOp.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CExplorerTreeView * g_TreeView = NULL;
extern CExplorerListView * g_ListView;
CEvent g_event;

/////////////////////////////////////////////////////////////////////////////
// CExplorerTreeView

IMPLEMENT_DYNCREATE(CExplorerTreeView, CTreeView)

CExplorerTreeView::CExplorerTreeView()
{
	g_TreeView       = this;
	m_pTreeCtrl      = &GetTreeCtrl();
	m_psfDesk        = NULL;
	m_pidlDesk       = NULL;
	m_pidlMyComputer = NULL;

	m_hDesktop       = NULL;
	m_hMyComputer    = NULL;
	m_hSelItem       = NULL;
	m_hMoveSrc       = NULL;

	m_pSelLiParam    = NULL;
}

CExplorerTreeView::~CExplorerTreeView()
{
//	m_pTreeCtrl->SetImageList(NULL,TVSIL_NORMAL);
}


BEGIN_MESSAGE_MAP(CExplorerTreeView, CTreeView)
	//{{AFX_MSG_MAP(CExplorerTreeView)
	ON_WM_CREATE()
	ON_NOTIFY_REFLECT(TVN_ITEMEXPANDING, OnItemexpanding)
	ON_NOTIFY_REFLECT(TVN_SELCHANGING, OnSelchanging)
	ON_NOTIFY_REFLECT(TVN_BEGINDRAG, OnBegindrag)
	ON_NOTIFY_REFLECT(NM_RCLICK, OnRclick)
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_REFRESHFILES,OnRefreshTree)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CExplorerTreeView drawing

void CExplorerTreeView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: add draw code here
}

/////////////////////////////////////////////////////////////////////////////
// CExplorerTreeView diagnostics

#ifdef _DEBUG
void CExplorerTreeView::AssertValid() const
{
	CTreeView::AssertValid();
}

void CExplorerTreeView::Dump(CDumpContext& dc) const
{
	CTreeView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CExplorerTreeView message handlers

int CExplorerTreeView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CTreeView::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here
	m_pTreeCtrl->ModifyStyle(0,TVS_HASBUTTONS|TVS_SHOWSELALWAYS);

	
	InitExplorerTree();
	InitImageList();
	
	int i = m_TreeTarget.Register(this);

	return 0;
}

void CExplorerTreeView::OnRclick(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	CPoint pt;
	GetCursorPos(&pt);
	m_pTreeCtrl->ScreenToClient(&pt);
	HTREEITEM hItem = m_pTreeCtrl->HitTest(pt,NULL);
	if ( hItem==NULL )
		return;
	LPTREEPARAM ptrParam  = (LPTREEPARAM)m_pTreeCtrl->GetItemData(hItem);

	CShellContextMenu scm;        
	scm.SetObjects(ptrParam->lpidl);    
	m_pTreeCtrl->ClientToScreen(&pt);
	UINT idCommand = scm.ShowContextMenu(this, pt);

	switch( idCommand )
	{
	case 18: //chose delete
		{
			HTREEITEM hParentItem = m_pTreeCtrl->GetParentItem(hItem);
			SendMessage(WM_REFRESHFILES,(WPARAM)hParentItem,0);
			g_ListView->SendMessage(WM_REFRESHFILES,0,0);
			break;
		}
	case 27:  //chose paste
		{
			SendMessage(WM_REFRESHFILES,(WPARAM)hItem,0);
			g_ListView->SendMessage(WM_REFRESHFILES,0,0);
			break;
		}
	}

	*pResult = 0;
}

void CExplorerTreeView::OnItemexpanding(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	TV_ITEM tvItem = pNMTreeView->itemNew;
	if ( pNMTreeView->action==2 )      //展开
	{
		SendMessage(WM_REFRESHFILES,(WPARAM)tvItem.hItem,0);
		SetSubItemFolder(tvItem);
	}

	*pResult = 0;
}
struct tagFileList
{
	IShellFolder *psfParent; LPITEMIDLIST pidlParent;
};


DWORD WINAPI ThreadShowFiles(  LPVOID lpParameter)
{
	
	LPTREEPARAM pliParam=(LPTREEPARAM)lpParameter;
	while(1)  
	{
		int dwRet =  WaitForMultipleObjects(64,g_hThread+1300,true,INFINITE);
		
		if(dwRet==WAIT_FAILED)
			break;
		if(dwRet == WAIT_TIMEOUT)
			break;
		if(dwRet==WAIT_OBJECT_0)
			break;
		switch(dwRet)  
		{  
		case WAIT_OBJECT_0:
				  break;
		case WAIT_TIMEOUT:  
			break;  
		case WAIT_FAILED:  
			break;  
		}
	}
		g_ListView->m_BStop = false;
		g_nThreadNum = 0;
	g_ListView->SetListContext(pliParam->lpsf,pliParam->lpidl);
	 
	return 0;
}
void CExplorerTreeView::OnSelchanging(NMHDR* pNMHDR, LRESULT* pResult) 
{
	
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	TV_ITEM tvItem = pNMTreeView->itemNew;
	m_hSelItem     = tvItem.hItem;

	LPTREEPARAM pliParam  = (LPTREEPARAM)tvItem.lParam;
	if ( g_ListView )
	{
		HANDLE hThread=NULL;
		DWORD dwThreadID=0;//保存线程ID
		g_ListView->m_BStop = true;
		  
		hThread=CreateThread(0,0,ThreadShowFiles,pliParam,0,&dwThreadID);//创建线程
		 
		CloseHandle(hThread);	
		//g_ListView->SetListContext(pliParam->lpsf,pliParam->lpidl);
	}
	*pResult = 0;
}

void CExplorerTreeView::OnBegindrag(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	TV_ITEM tvItem = pNMTreeView->itemNew;

	HTREEITEM hItem = tvItem.hItem;
	m_hMoveSrc = m_pTreeCtrl->GetParentItem(hItem);

	HTREEITEM hSel = m_pTreeCtrl->GetSelectedItem();
	
	LPTREEPARAM ptrItem = (LPTREEPARAM)tvItem.lParam;
	
	char * szSrc = new char[MAX_PATH];
	ZeroMemory(szSrc,MAX_PATH);
	SHGetPathFromIDList(ptrItem->lpidl,szSrc);

	//创建拖拽数据源
	int hgblSize = sizeof(DROPFILES) + ( strlen(szSrc)+2 )* sizeof(_TCHAR);
	HGLOBAL hgblData = ::GlobalAlloc(GMEM_ZEROINIT|GMEM_MOVEABLE|GMEM_DDESHARE,hgblSize);
	if (hgblData == NULL)
		return;
	
	LPDROPFILES pDropFiles = (LPDROPFILES)GlobalLock(hgblData);

	pDropFiles->pFiles = sizeof(DROPFILES);
	pDropFiles->fWide = FALSE;
	LPSTR pGlobalString = (LPSTR)(pDropFiles) + sizeof(DROPFILES);


	strcpy((char*)(pGlobalString), szSrc);
	pGlobalString += ( strlen(szSrc)+1 ) * sizeof(_TCHAR);
	pGlobalString[0] = _T('\0');
	
	GlobalUnlock(hgblData);
	delete szSrc;

	COleDataSource *poleSourceObj = new COleDataSource;
	poleSourceObj->CacheGlobalData(CF_HDROP,hgblData);
	DROPEFFECT dropeffect = poleSourceObj->DoDragDrop();

	*pResult = 0;
}

HTREEITEM CExplorerTreeView::FindSubItemByPath(HTREEITEM hParent, LPSTR szPathName)
{
	HTREEITEM hNextItem,hChildItem = m_pTreeCtrl->GetChildItem(hParent);
	
	char * szPath = new char[MAX_PATH];
	while( hChildItem )
	{
		//ZeroMemory(szPath,MAX_PATH);
		LPTREEPARAM ptrParam = (LPTREEPARAM)m_pTreeCtrl->GetItemData(hChildItem);
		SHGetPathFromIDList(ptrParam->lpidl,szPath);
		if ( strcmp(szPathName,szPath)==0 )
		{
			delete szPath;
			return hChildItem;
		}
		hNextItem = m_pTreeCtrl->GetNextItem(hChildItem, TVGN_NEXT);
		hChildItem = hNextItem;	
	}
	delete szPath;
	return NULL;
}

long CExplorerTreeView::OnRefreshTree(WPARAM wParam, LPARAM lParam)
{
	HTREEITEM hRefresh;
	if  ( wParam==0 )
		hRefresh = m_hSelItem;
	else
		hRefresh = (HTREEITEM)wParam;
	if ( hRefresh==m_hDesktop )
		return 0;
	return 0;
	

	//删除多余项
	char szPathName[MAX_PATH] ;
	ZeroMemory(szPathName,MAX_PATH);
	HTREEITEM hNextItem,hChildItem = m_pTreeCtrl->GetChildItem(hRefresh);
	while( hChildItem )
	{
		LPTREEPARAM ptrParam = (LPTREEPARAM)m_pTreeCtrl->GetItemData(hChildItem);
		SHGetPathFromIDList(ptrParam->lpidl,szPathName);
		//路径不为空、且不存在时才能删除
		//系统的文件夹路径为空,不能删
		if ( strlen(szPathName)!=0 && GetFileAttributes(szPathName)==INVALID_FILE_ATTRIBUTES )
		{
			hNextItem = m_pTreeCtrl->GetNextItem(hChildItem, TVGN_NEXT);
			if ( hChildItem==m_hSelItem )
				m_pTreeCtrl->SelectItem(hRefresh);
			LPTREEPARAM ptrParam = (LPTREEPARAM)m_pTreeCtrl->GetItemData(hChildItem);
			m_pMalloc->Free(ptrParam);
			m_pTreeCtrl->DeleteItem(hChildItem);
		}
		else
		{
			hNextItem = m_pTreeCtrl->GetNextItem(hChildItem, TVGN_NEXT);
		}
		hChildItem = hNextItem;
	}
 


	//添加新项
	IEnumIDList  * peidl;
	LPITEMIDLIST pidl, pidlAbs;
	IShellFolder * psf = NULL;

	LPTREEPARAM    ptrParent  = (LPTREEPARAM)m_pTreeCtrl->GetItemData(hRefresh);
	IShellFolder * psfParent  = ptrParent->lpsf;
	LPITEMIDLIST   pidlParent = ptrParent->lpidl;
	HRESULT hr = psfParent->EnumObjects(NULL,SHCONTF_FOLDERS,&peidl);
	if ( NOERROR!=hr )
		return 0;


	while (NOERROR == hr)
	{
		hr = peidl->Next(1, &pidl, NULL);
		if (hr == S_FALSE || pidl == NULL)
			break;
		
		pidlAbs = ConcatPidl(pidlParent,pidl);
		
		hr = psfParent->BindToObject(pidl, NULL, IID_IShellFolder, (void**)&psf);
		if (hr == S_FALSE || psf == NULL)
			break;	 
		SHGetPathFromIDList(pidlAbs,szPathName);
	
		if ( FindSubItemByPath(hRefresh,szPathName)==NULL )
		{
			SHFILEINFO sfi;
			SHGetFileInfo((LPCTSTR)pidlAbs, 
				0,
				&sfi, 
				sizeof(SHFILEINFO), 
				SHGFI_PIDL|SHGFI_SYSICONINDEX|SHGFI_SMALLICON|SHGFI_DISPLAYNAME);
		
			TV_INSERTSTRUCT TCItem;
			TCItem.hParent             = hRefresh;
			if ( hRefresh==m_hMyComputer )
				TCItem.hInsertAfter = TVI_LAST;
			else
				TCItem.hInsertAfter = TVI_SORT;
			TCItem.item.mask           = TVIF_TEXT|TVIF_PARAM|TVIF_IMAGE|TVIF_SELECTEDIMAGE;
			TCItem.item.pszText        = sfi.szDisplayName;
			TCItem.item.iImage         = sfi.iIcon;
			TCItem.item.iSelectedImage = sfi.iIcon;

			LPTREEPARAM ptrItem = (LPTREEPARAM)m_pMalloc->Alloc(sizeof(TREEPARAM));
			ptrItem->lpsf    = psf;
			ptrItem->lpidl   = pidlAbs;
			ptrItem->uParam  = 1;
			TCItem.item.lParam   = (LPARAM)ptrItem;
			
			HTREEITEM hCur = m_pTreeCtrl->InsertItem(&TCItem);
			AddChildren(psf,pidlAbs, hCur, false);
		} 
	}

	peidl->Release();

	return 0;
}

BOOL CExplorerTreeView::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if ( pMsg->message==WM_KEYDOWN )
	{
		switch ( pMsg->wParam )
		{
		case 0x2e:       //DEL
			{
				CShellFileOp op;
				short lShift = GetKeyState(VK_LSHIFT);
				short rShift = GetKeyState(VK_RSHIFT);
				if ( HIBYTE(lShift)==0xFF || HIBYTE(rShift)==0xFF )
					op.SetOperationFlags(FO_DELETE, this,NULL);
				else
					op.SetOperationFlags(FO_DELETE, this,FOF_ALLOWUNDO);
				
				LPTREEPARAM ptrItem = GetSelTreeData();

				char * szSrc = new char[MAX_PATH];
				ZeroMemory(szSrc,MAX_PATH);
				SHGetPathFromIDList(ptrItem->lpidl,szSrc);
				
				op.AddSourceFile(szSrc);
				BOOL bSuccess, bAPICalled = FALSE, bAborted = FALSE;
				int  nAPIReturn = 0;
				bSuccess = op.Go(&bAPICalled, &nAPIReturn, &bAborted);
				
				delete szSrc;

				//refresh
				if ( bSuccess && !bAborted )
				{
					HTREEITEM hSelect = g_TreeView->m_hSelItem;
					HTREEITEM hParent = g_TreeView->m_pTreeCtrl->GetParentItem(hSelect);
					g_TreeView->m_pTreeCtrl->SelectItem(hParent);
			//		g_TreeView->m_pTreeCtrl->DeleteItem(hSelect);
					g_TreeView->SendMessage(WM_REFRESHFILES,0,0);
					g_ListView->SendMessage(WM_REFRESHFILES,0,0);
				}
				return true;
			}
		case 0x43:     //Ctrl+C
			{
				short ctrl = GetKeyState(VK_CONTROL);
				if ( HIBYTE(ctrl)!=0xff )
					break;

				UINT uDropEffect = RegisterClipboardFormat("Preferred DropEffect");
				HGLOBAL  hGblEffect  = GlobalAlloc(GHND,sizeof(DWORD));
				DWORD * dwDropEffect = (DWORD*)GlobalLock(hGblEffect);
				*dwDropEffect        = DROPEFFECT_COPY;
				GlobalUnlock(hGblEffect);

				LPTREEPARAM ptrItem = (LPTREEPARAM)GetSelTreeData();
				char * szSrc = new char[MAX_PATH];
				ZeroMemory(szSrc,MAX_PATH);
				SHGetPathFromIDList(ptrItem->lpidl,szSrc);
				
				//创建拖拽数据源
				int hgblSize = sizeof(DROPFILES) + ( strlen(szSrc)+2 )* sizeof(_TCHAR);
				HGLOBAL hgblData = ::GlobalAlloc(GHND,hgblSize);
				if (hgblData == NULL)
					return true;
				
				LPDROPFILES pDropFiles = (LPDROPFILES)GlobalLock(hgblData);
				
				pDropFiles->pFiles = sizeof(DROPFILES);
				pDropFiles->fWide = FALSE;
				LPSTR pGlobalString = (LPSTR)(pDropFiles) + sizeof(DROPFILES);
				
				
				strcpy((char*)(pGlobalString), szSrc);
				pGlobalString += ( strlen(szSrc)+1 ) * sizeof(_TCHAR);
				pGlobalString[0] = _T('\0');
				
				GlobalUnlock(hgblData);
				delete szSrc;

				if ( ::OpenClipboard(NULL) ) 
				{
					::EmptyClipboard();
					::SetClipboardData(uDropEffect,hGblEffect);
					::SetClipboardData(CF_HDROP, hgblData);
					::CloseClipboard();
				}
				return true;
			}
		case 0x58:     //Ctrl+X
			{ 
				short ctrl = GetKeyState(VK_CONTROL);
				if ( HIBYTE(ctrl)!=0xff )
					break;

				UINT uDropEffect     = RegisterClipboardFormat("Preferred DropEffect");
				HGLOBAL  hGblEffect = GlobalAlloc(GMEM_ZEROINIT|GMEM_MOVEABLE|GMEM_DDESHARE,sizeof(DWORD));
				DWORD * dwDropEffect =(DWORD*)GlobalLock(hGblEffect);
				*dwDropEffect        = DROPEFFECT_MOVE;
				GlobalUnlock(hGblEffect);
		
				LPTREEPARAM ptrItem = (LPTREEPARAM)GetSelTreeData();
				char * szSrc = new char[MAX_PATH];
				ZeroMemory(szSrc,MAX_PATH);
				SHGetPathFromIDList(ptrItem->lpidl,szSrc);
				
				//创建拖拽数据源
				int hgblSize = sizeof(DROPFILES)+(strlen(szSrc)+2)*sizeof(_TCHAR);
				
				HGLOBAL hgblData = ::GlobalAlloc(GMEM_ZEROINIT|GMEM_MOVEABLE|GMEM_DDESHARE,hgblSize);
				if (hgblData == NULL)
					return true;	
				
				LPDROPFILES pDropFiles = (LPDROPFILES)GlobalLock(hgblData);
				
				pDropFiles->pFiles = sizeof(DROPFILES);
				pDropFiles->fWide = FALSE;
				LPSTR pGlobalString = (LPSTR)(pDropFiles) + sizeof(DROPFILES);
				
				strcpy((char*)(pGlobalString), szSrc);
				pGlobalString += ( strlen(szSrc)+1 ) * sizeof(_TCHAR);
				pGlobalString[0] = _T('\0');
				
				GlobalUnlock(hgblData);
				delete szSrc;

				if ( ::OpenClipboard(NULL) ) 
				{
					::EmptyClipboard();
					::SetClipboardData(uDropEffect,hGblEffect);
					::SetClipboardData(CF_HDROP, hgblData);
					::CloseClipboard();

					m_hMoveSrc = m_pTreeCtrl->GetParentItem(m_hSelItem);
				}
				return true;
			}
		case 0x56:     //Ctrl+V
			{
				short ctrl = GetKeyState(VK_CONTROL);
				if ( HIBYTE(ctrl)!=0xFF )
					break;

				GetParent()->PostMessage(WM_COMMAND,MAKEWPARAM(0,ID_EDIT_PASTE),0);
				break;
			}
		}
	}


	return CTreeView::PreTranslateMessage(pMsg);
}

/////////////////////////////////////////////////////////
//  method
/////////////////////////////////////////////////////////

void CExplorerTreeView::InitImageList()
{
    SHFILEINFO sfi;
    HIMAGELIST himl = (HIMAGELIST)SHGetFileInfo((LPCTSTR)_T("C:\\"), 
		0,
		&sfi, 
		sizeof(SHFILEINFO), 
		SHGFI_SYSICONINDEX | SHGFI_SMALLICON);
	if ( himl )
    {
        ::SendMessage(GetSafeHwnd(), TVM_SETIMAGELIST, (WPARAM)TVSIL_NORMAL,
            (LPARAM)himl);
	}
}

void CExplorerTreeView::InitExplorerTree()
{
//	IShellFolder *psfDesk;
//	ITEMIDLIST   *pidlDesk;
	::SHGetDesktopFolder(&m_psfDesk);
	::SHGetSpecialFolderLocation(NULL,CSIDL_DESKTOP,&m_pidlDesk);
	::SHGetSpecialFolderLocation(NULL,CSIDL_DRIVES,&m_pidlMyComputer);

	SHFILEINFO    sfi;
	SHGetFileInfo((LPCTSTR)m_pidlDesk,
		            0,
					&sfi, 
					sizeof(SHFILEINFO), 
					SHGFI_PIDL|SHGFI_SYSICONINDEX|SHGFI_SMALLICON|SHGFI_DISPLAYNAME);	

	TV_INSERTSTRUCT TCItem;
	ZeroMemory(&TCItem,sizeof(TV_INSERTSTRUCT));
	TCItem.hParent             = TVI_ROOT;
	TCItem.hInsertAfter        = TVI_LAST;
	TCItem.item.mask           = TVIF_TEXT|TVIF_PARAM|TVIF_IMAGE|TVIF_STATE|TVIF_SELECTEDIMAGE;

	TCItem.item.pszText        = sfi.szDisplayName;
	TCItem.item.iImage         = sfi.iIcon;
	TCItem.item.iSelectedImage = sfi.iIcon;
//	TCItem.item.state         |= TVIS_DROPHILITED;

	LPTREEPARAM ptrItem = (LPTREEPARAM)m_pMalloc->Alloc(sizeof(TREEPARAM));
	ptrItem->lpsf    = m_psfDesk;
	ptrItem->lpidl   = m_pidlDesk;
	ptrItem->uParam  = 1;
	TCItem.item.lParam  = (LPARAM)ptrItem;

	m_hDesktop = m_pTreeCtrl->InsertItem(&TCItem);

	AddChildren(m_psfDesk,m_pidlDesk,m_hDesktop,true);
	
	m_pTreeCtrl->Expand(m_hMyComputer,TVE_EXPAND);
	m_pTreeCtrl->SelectItem(m_hMyComputer);
}

void CExplorerTreeView::AddChildren(IShellFolder * psfParent, LPITEMIDLIST pidlParent, HTREEITEM hParent, BOOL bExpand)
{
	IEnumIDList  * peidl;
	LPITEMIDLIST pidl, pidlAbs;
	IShellFolder * psf = NULL;

	HRESULT hr = psfParent->EnumObjects(NULL,SHCONTF_FOLDERS,&peidl);
	if ( NOERROR!=hr )
		return;

	while (NOERROR == hr)
	{
		hr = peidl->Next(1, &pidl, NULL);
		if (hr == S_FALSE || pidl == NULL)
			break;
		pidlAbs = ConcatPidl(pidlParent,pidl);

		hr = psfParent->BindToObject(pidl, NULL, IID_IShellFolder, (void**)&psf);
		if (hr == S_FALSE || psf == NULL)
			break;

		SHFILEINFO    sfi;
		SHGetFileInfo((LPCTSTR)pidlAbs,
			0,
			&sfi, 
			sizeof(SHFILEINFO), 
			SHGFI_PIDL|SHGFI_SYSICONINDEX|SHGFI_SMALLICON|SHGFI_DISPLAYNAME);	
		
		TV_INSERTSTRUCT TCItem;
		TCItem.hParent             = hParent;
		TCItem.item.mask           = TVIF_TEXT|TVIF_PARAM|TVIF_IMAGE|TVIF_SELECTEDIMAGE;
		TCItem.item.pszText        = sfi.szDisplayName;
		TCItem.item.iImage         = sfi.iIcon;
		TCItem.item.iSelectedImage = sfi.iIcon;
		
		LPTREEPARAM ptrItem = (LPTREEPARAM)m_pMalloc->Alloc(sizeof(TREEPARAM));
		ptrItem->lpsf    = psf;
		ptrItem->lpidl   = pidlAbs;
		if ( bExpand )
			ptrItem->uParam  = 1;
		else
			ptrItem->uParam  = 0;
		TCItem.item.lParam   = (LPARAM)ptrItem;
		
		HTREEITEM hCur = m_pTreeCtrl->InsertItem(&TCItem);
		if ( hParent==m_hDesktop )
		{
			if ( ComparePidls(m_psfDesk, m_pidlMyComputer, pidlAbs) )
				m_hMyComputer = hCur;
		}
		   
		if ( bExpand )
		{
			char szBuffer[_MAX_PATH];
			::SHGetPathFromIDList(pidlAbs, szBuffer);
			//if((CString(sfi.szDisplayName).Compare("网络")==0 )&& CString(szBuffer).Empty())
			if(CString(sfi.szDisplayName).Compare("网络")!=0 )
			AddChildren( psf,pidlAbs, hCur, false);
		}
	}

	peidl->Release();


	if ( m_psfDesk->CompareIDs(0,m_pidlDesk,pidlParent)==0 )
	{
	}
	else if ( m_psfDesk->CompareIDs(0,m_pidlMyComputer,pidlParent)==0 )
	{
	}
	else
	{
		m_pTreeCtrl->SortChildren(hParent);
	}
	if ( bExpand )
		m_pTreeCtrl->Expand(hParent,TVE_EXPAND);

}

int CALLBACK CExplorerTreeView::SortMyComputer(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort)
{
	CTreeCtrl * pTreeCtrl = (CTreeCtrl*)lParamSort;

	CString    str1 = pTreeCtrl->GetItemText((HTREEITEM)lParam1);
	if ( str1.Right(2)==":)" )
	{
		str1 = str1.Right(3);
	}
	else
	{
		str1 = "Z" + str1;
	}
	
	CString    str2 = pTreeCtrl->GetItemText((HTREEITEM) lParam2);
	if ( str2.Right(2)==":)" )
	{
		str2 = str2.Right(3);
	}
	else
	{
		str2 = "Z" + str2;
	}
	
	return strcmp(str2, str1);
}

/*给子项添加文件夹
*/
void CExplorerTreeView::SetSubItemFolder(TV_ITEM tvItemParent)
{
	HTREEITEM hItemParent = tvItemParent.hItem;
	
	HTREEITEM hItem = m_pTreeCtrl->GetChildItem(hItemParent);
	while (hItem != NULL)
	{
		LPTREEPARAM ptrItem = (LPTREEPARAM)m_pTreeCtrl->GetItemData(hItem);
		CString s = m_pTreeCtrl->GetItemText(hItem);
		if ( ptrItem->uParam==0 )
		{
			TV_ITEM tvItem;
			tvItem.hItem = hItem;
			tvItem.mask = TVIF_HANDLE|TVIF_PARAM|TVIF_IMAGE|TVIF_SELECTEDIMAGE;
			m_pTreeCtrl->GetItem(&tvItem);
			AddChildren(ptrItem->lpsf,ptrItem->lpidl,hItem,false);
			ptrItem->uParam = 1;
		}
		hItem = m_pTreeCtrl->GetNextItem(hItem, TVGN_NEXT);
	}
//	PostMessage(WM_REFRESHFILES,(WPARAM)hItemParent,0);
}

void CExplorerTreeView::SetTreeContext(IShellFolder *psfParent, LPITEMIDLIST pidlParent)
{
	HTREEITEM hSelect = m_pTreeCtrl->GetSelectedItem();
	m_pTreeCtrl->Expand(hSelect,TVE_EXPAND);
	
	HTREEITEM hChildItem = m_pTreeCtrl->GetChildItem(hSelect);
	while (hChildItem != NULL)
	{
		LPTREEPARAM ptrItem = (LPTREEPARAM)m_pTreeCtrl->GetItemData(hChildItem);
		if ( ComparePidls(m_psfDesk,pidlParent,ptrItem->lpidl) )
		{
			m_pTreeCtrl->SelectItem(hChildItem);
		}
		hChildItem = m_pTreeCtrl->GetNextItem(hChildItem, TVGN_NEXT);
	}

}

LPTREEPARAM CExplorerTreeView::GetSelTreeData()
{
	LPTREEPARAM pliParam  = (LPTREEPARAM)m_pTreeCtrl->GetItemData(m_hSelItem);		
	return pliParam;
}

