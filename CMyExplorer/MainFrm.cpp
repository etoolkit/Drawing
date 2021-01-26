// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "CMyExplorer.h"
#include "MainFrm.h"
#include "ExplorerListView.h"
#include "ExplorerTreeView.h"
#include "ShellContextMenu.h"
#include "ShellFileOp.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern CExplorerListView * g_ListView;
extern CExplorerTreeView * g_TreeView;

/////////////////////////////////////////////////////////////////////////////
// CMainFrame

IMPLEMENT_DYNAMIC(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	//{{AFX_MSG_MAP(CMainFrame)
	ON_WM_CREATE()
	ON_WM_SETFOCUS()
	ON_COMMAND(ID_VIEW_SMALLICON, OnViewSmallicon)
	ON_COMMAND(ID_VIEW_LIST, OnViewList)
	ON_COMMAND(ID_VIEW_LARGEICON, OnViewLargeicon)
	ON_COMMAND(ID_EDIT_COPY, OnEditCopy)
	ON_COMMAND(ID_EDIT_CUT, OnEditCut)
	ON_COMMAND(ID_EDIT_PASTE, OnEditPaste)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // status line indicator
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

/////////////////////////////////////////////////////////////////////////////
// CMainFrame construction/destruction

CMainFrame::CMainFrame()
{
	// TODO: add member initialization code here
	
}

CMainFrame::~CMainFrame()
{
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	// create a view to occupy the client area of the frame
/*	if (!m_wndView.Create(NULL, NULL, AFX_WS_DEFAULT_VIEW,
		CRect(0, 0, 0, 0), this, AFX_IDW_PANE_FIRST, NULL))
	{
		TRACE0("Failed to create view window\n");
		return -1;
	}*/
	
	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}

	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		  sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}

	// TODO: Delete these three lines if you don't want the toolbar to
	//  be dockable
	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&m_wndToolBar);

	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	cs.dwExStyle &= ~WS_EX_CLIENTEDGE;
	cs.lpszClass = AfxRegisterWndClass(0);
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMainFrame message handlers
void CMainFrame::OnSetFocus(CWnd* pOldWnd)
{
	// forward focus to the view window
//	m_wndView.SetFocus();
}

BOOL CMainFrame::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo)
{
	// let the view have first crack at the command
//	if (m_wndView.OnCmdMsg(nID, nCode, pExtra, pHandlerInfo))
//		return TRUE;

	// otherwise, do default handling
	return CFrameWnd::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}

DWORD WINAPI ThreadInit(  LPVOID lpParameter)
{
	g_TreeView->InitExplorerTree();
	g_TreeView-> InitImageList();
	return 0;
}
BOOL CMainFrame::OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext) 
{
	// TODO: Add your specialized code here and/or call the base class
	CRect rect;
	GetClientRect(&rect);

	if ( !m_wndSplitter.CreateStatic(this,1,2) )
	{
		TRACE0("Failed to CreateStaticSplitter\n");
		return FALSE;
	}

	// add the first splitter pane - the default view in column 0
	if (!m_wndSplitter.CreateView(0, 0, RUNTIME_CLASS(CExplorerTreeView), 
		CSize(240,rect.bottom), pContext))
	{
		TRACE0("Failed to create first pane\n");
		return FALSE;
	}
 
	// add the second splitter pane - an input view in column 1
	if (!m_wndSplitter.CreateView(0, 1, RUNTIME_CLASS(CExplorerListView), 
		CSize(rect.right,rect.bottom-350), pContext))
	{
		TRACE0("Failed to create second pane\n");
		return FALSE;
	}

	m_pTreeView = (CExplorerTreeView*)m_wndSplitter.GetPane(0,0);
	m_pListView = (CExplorerListView*)m_wndSplitter.GetPane(0,1);
	HANDLE hThread=NULL;
	DWORD dwThreadID=0;//保存线程ID
//	hThread=CreateThread(0,0, ThreadInit,NULL,0,&dwThreadID);//创建线程
	CloseHandle(hThread);
	return CFrameWnd::OnCreateClient(lpcs, pContext);
}

///////////////////////////////////////////

void CMainFrame::OnViewSmallicon() 
{
	// TODO: Add your command handler code here
	g_ListView->SetListViewType(LVS_SMALLICON);
}

void CMainFrame::OnViewList() 
{
	// TODO: Add your command handler code here
	g_ListView->SetListViewType(LVS_LIST);
}

void CMainFrame::OnViewLargeicon() 
{
	// TODO: Add your command handler code here
	g_ListView->SetListViewType(LVS_ICON);
}


void CMainFrame::OnEditCopy() 
{
	// TODO: Add your command handler code here
	UINT uDropEffect = RegisterClipboardFormat("Preferred DropEffect");
	HGLOBAL  hGblEffect = GlobalAlloc(GMEM_ZEROINIT|GMEM_MOVEABLE|GMEM_DDESHARE,sizeof(DWORD));
	DWORD * dwDropEffect =(DWORD*)GlobalLock(hGblEffect);
	*dwDropEffect        = DROPEFFECT_COPY;
	GlobalUnlock(hGblEffect);
	
	POSITION pos = m_pListView->m_pListCtrl->GetFirstSelectedItemPosition();
	if ( pos==NULL )
		return;
				
	char * szSrc = new char[MAX_PATH];
	CStringArray sDragArray;
	while(pos)
	{
		int nItem = m_pListView->m_pListCtrl->GetNextSelectedItem(pos);
		LPLISTPARAM pliItem = (LPLISTPARAM)m_pListView->m_pListCtrl->GetItemData(nItem);
		
		ZeroMemory(szSrc,MAX_PATH);
		SHGetPathFromIDList(pliItem->lpidl,szSrc);
		
		sDragArray.Add(szSrc);
	}
	delete szSrc;
	
	//创建拖拽数据源
	int hgblSize = sizeof(DROPFILES);
	for ( int i=0 ; i<sDragArray.GetSize() ; i++ )
	{
		hgblSize += (sDragArray.GetAt(i).GetLength()+1) * sizeof(_TCHAR);
	}
	hgblSize += sizeof(_TCHAR);
	
	HGLOBAL hgblData = ::GlobalAlloc(GMEM_ZEROINIT|GMEM_MOVEABLE|GMEM_DDESHARE,hgblSize);
	if (hgblData == NULL)
		return;	
	
	LPDROPFILES pDropFiles = (LPDROPFILES)GlobalLock(hgblData);
	
	pDropFiles->pFiles = sizeof(DROPFILES);
	pDropFiles->fWide = FALSE;
	
	LPSTR pGlobalString = (LPSTR)(pDropFiles) + sizeof(DROPFILES);
	
	for (int i=0 ; i<sDragArray.GetSize() ; i++ )
	{
		strcpy((char*)(pGlobalString), sDragArray.GetAt(i) );
		pGlobalString += ( sDragArray.GetAt(i).GetLength()+1 ) * sizeof(_TCHAR);
	}
	pGlobalString[0] = _T('\0');
	
	GlobalUnlock(hgblData);
	
	if( ::OpenClipboard(NULL) )
	{
		::EmptyClipboard();
		::SetClipboardData(uDropEffect,hGblEffect);
		::SetClipboardData( CF_HDROP, hgblData );
		::CloseClipboard();
	}
}

void CMainFrame::OnEditCut() 
{
	// TODO: Add your command handler code here
	
	UINT uDropEffect = RegisterClipboardFormat("Preferred DropEffect");
	HGLOBAL  hGblEffect = ::GlobalAlloc(GHND, (DWORD)sizeof(DWORD)); 
	DWORD * dwDropEffect = (DWORD*)GlobalLock(hGblEffect);
	*dwDropEffect = DROPEFFECT_MOVE;
	GlobalUnlock(hGblEffect);

	POSITION pos = m_pListView->m_pListCtrl->GetFirstSelectedItemPosition();
	if ( pos==NULL )
		return;
				
	char * szSrc = new char[MAX_PATH];
	CStringArray sDragArray;
	while(pos)
	{
		int nItem = m_pListView->m_pListCtrl->GetNextSelectedItem(pos);
		LPLISTPARAM pliItem = (LPLISTPARAM)m_pListView->m_pListCtrl->GetItemData(nItem);
		
		ZeroMemory(szSrc,MAX_PATH);
		SHGetPathFromIDList(pliItem->lpidl,szSrc);
		
		sDragArray.Add(szSrc);
	}
	delete szSrc;
	
	//创建拖拽数据源
	int hgblSize = sizeof(DROPFILES);
	for ( int i=0 ; i<sDragArray.GetSize() ; i++ )
	{
		hgblSize += (sDragArray.GetAt(i).GetLength()+1) * sizeof(_TCHAR);
	}
	hgblSize += sizeof(_TCHAR);
	
	HGLOBAL hgblData = ::GlobalAlloc(GHND,hgblSize);
	if (hgblData == NULL)
		return;	
	
	LPDROPFILES pDropFiles = (LPDROPFILES)GlobalLock(hgblData);
	
	pDropFiles->pFiles = sizeof(DROPFILES);
	pDropFiles->fWide = FALSE;
	
	LPSTR pGlobalString = (LPSTR)(pDropFiles) + sizeof(DROPFILES);
	
	for (int i=0 ; i<sDragArray.GetSize() ; i++ )
	{
		strcpy((char*)(pGlobalString), sDragArray.GetAt(i) );
		pGlobalString += ( sDragArray.GetAt(i).GetLength()+1 ) * sizeof(_TCHAR);
	}
	pGlobalString[0] = _T('\0');
	
	GlobalUnlock(hgblData);
	
	if( ::OpenClipboard(NULL) )
	{
		::EmptyClipboard();
		::SetClipboardData(uDropEffect,hGblEffect);
		::SetClipboardData( CF_HDROP, hgblData );
		::CloseClipboard();

		g_TreeView->m_hMoveSrc = g_TreeView->m_hSelItem;
	}
}

void CMainFrame::OnEditPaste() 
{
	// TODO: Add your command handler code here
	if ( !IsClipboardFormatAvailable(CF_HDROP) )
		return;
	UINT uDropEffect = RegisterClipboardFormat("Preferred DropEffect");
	if ( !IsClipboardFormatAvailable(uDropEffect) )
		return;
	
	if ( !::OpenClipboard(NULL) ) 
		return; 
	DWORD dwEffect = *(DWORD*)::GetClipboardData(uDropEffect);
	HGLOBAL hglobal = ::GetClipboardData(CF_HDROP);
	::CloseClipboard();

	if( hglobal != NULL )
	{
		CShellFileOp op;
		if ( dwEffect & DROPEFFECT_MOVE )
			op.SetOperationFlags(FO_MOVE, this, FOF_ALLOWUNDO|FOF_NOCONFIRMMKDIR);
		else
			op.SetOperationFlags(FO_COPY, this, FOF_ALLOWUNDO|FOF_NOCONFIRMMKDIR);

		char * szFileName;
		szFileName = new char[MAX_PATH];
		HDROP hDrop = (HDROP)GlobalLock(hglobal);
		int FileNum = DragQueryFile(hDrop,-1,NULL,0);
		
		for ( int i =0 ; i < FileNum ; i++ )
		{
			ZeroMemory(szFileName,MAX_PATH);
			DragQueryFile(hDrop,i,szFileName,255);
			op.AddSourceFile(szFileName);
		}
		delete szFileName;
		
		//设置粘贴目标
		char * szDest = new char[MAX_PATH];
		ZeroMemory(szDest,MAX_PATH);
		
		LPTREEPARAM ptrItem = (LPTREEPARAM)g_TreeView->GetSelTreeData();
		SHGetPathFromIDList(ptrItem->lpidl,szDest);
		op.AddDestFile(szDest);
		BOOL bSuccess, bAPICalled = FALSE, bAborted = FALSE;
		int  nAPIReturn = 0;
		
		bSuccess = op.Go (&bAPICalled, &nAPIReturn, &bAborted);
		
		GlobalFree(hglobal);
		delete szDest;
		
		//refresh
		if ( bSuccess && !bAborted )
		{
			g_TreeView->SendMessage(WM_REFRESHFILES,0,0);
			g_TreeView->SendMessage(WM_REFRESHFILES,(WPARAM)g_TreeView->m_hMoveSrc,0);
			g_ListView->SendMessage(WM_REFRESHFILES,0,0);
		}
	}
}
