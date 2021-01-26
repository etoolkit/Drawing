// ExplorerListView.cpp : implementation file
//

#include "stdafx.h"
#include "CMyExplorer.h"
#include "ExplorerListView.h"
#include "ShellContextMenu.h"
#include "ExplorerTreeView.h"
#include "ShellFileOp.h"
#include <commctrl.h>
#include <string> 
#include <sys\stat.h>
using namespace std;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

int g_nThreadNum=0;
CExplorerListView * g_ListView = NULL;
extern CExplorerTreeView * g_TreeView;
HANDLE g_hThread[2000];
struct tagItemFileInfo
{
	int iItem;
	CString sPath;
	INT64 nSize;
	CListCtrl* pList;
};
/////////////////////////////////////////////////////////////////////////////
// CExplorerListView

IMPLEMENT_DYNCREATE(CExplorerListView, CListView)

CExplorerListView::CExplorerListView()
{
	m_pListCtrl = &GetListCtrl();

	m_dViewType = LVS_LIST;

	g_ListView = this;
	m_BStop=false;
}

CExplorerListView::~CExplorerListView()
{
//	m_pListCtrl->SetImageList(NULL,LVSIL_SMALL);
//	m_pListCtrl->SetImageList(NULL,LVSIL_NORMAL);
}


BEGIN_MESSAGE_MAP(CExplorerListView, CListView)
	//{{AFX_MSG_MAP(CExplorerListView)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_NOTIFY_REFLECT(NM_RCLICK, OnRclick)
	ON_NOTIFY_REFLECT(NM_DBLCLK, OnDblclk)
	ON_NOTIFY_REFLECT(LVN_BEGINDRAG, OnBegindrag)
	ON_WM_MENUSELECT()
	ON_NOTIFY_REFLECT(LVN_ENDLABELEDIT, OnEndlabeledit)
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_REFRESHFILES,OnRefreshList)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CExplorerListView drawing

void CExplorerListView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: add draw code here
	
}

/////////////////////////////////////////////////////////////////////////////
// CExplorerListView diagnostics

#ifdef _DEBUG
void CExplorerListView::AssertValid() const
{
	CListView::AssertValid();
}

void CExplorerListView::Dump(CDumpContext& dc) const
{
	CListView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CExplorerListView message handlers

int CExplorerListView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CListView::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here
	//修改属性
	ModifyStyle(0,LVS_EDITLABELS|LVS_SHOWSELALWAYS|LVS_AUTOARRANGE);

	//设置菜单
	SetListViewType(LVS_ICON);

	//注册拖拽
	m_ListTarget.Register(this);

	//初始化系统图标列表
	InitImageList();

	if ( g_TreeView )
	{
		HTREEITEM hSelect = g_TreeView->m_pTreeCtrl->GetSelectedItem();
		LPTREEPARAM ptrItem = (LPTREEPARAM)g_TreeView->m_pTreeCtrl->GetItemData(hSelect);
		SetListContext(ptrItem->lpsf,ptrItem->lpidl);
	}

	CListCtrl& m_list = GetListCtrl();//得到内置的listctrl引用

	LONG lStyle;

	lStyle = GetWindowLong(m_list.m_hWnd, GWL_STYLE);//获取当前窗口风格

	lStyle &= ~LVS_TYPEMASK; //清除显示方式位

	lStyle |= LVS_REPORT; //设置报表风格

	SetWindowLong(m_list.m_hWnd, GWL_STYLE, lStyle); //设置窗口风格

	DWORD dwStyle = m_list.GetExtendedStyle();
	dwStyle |= LVS_REPORT;
	m_list.SetExtendedStyle(dwStyle);

	int nColumnCount = m_pListCtrl->GetHeaderCtrl()->GetItemCount();	 
	while (m_pListCtrl->DeleteColumn(0));
	m_pListCtrl->InsertColumn (0, _T("名称"),LVCFMT_LEFT,200);
	m_pListCtrl->InsertColumn (1, _T("大小"),LVCFMT_LEFT,100);
	return 0;
}

void CExplorerListView::OnDestroy() 
{
	CListView::OnDestroy();
	
	// TODO: Add your message handler code here
	
}

void CExplorerListView::OnDblclk(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	NMITEMACTIVATE * nmia = (NMITEMACTIVATE*)pNMHDR;

	CPoint point(nmia->ptAction);
	int nCurItem = m_pListCtrl->HitTest(point);
	if ( nCurItem==-1 )
		return;

	LPLISTPARAM pliParam = (LPLISTPARAM)m_pListCtrl->GetItemData(nCurItem);
	
	if ( pliParam->uType>0 )
	{
		SHELLEXECUTEINFO si;
		ZeroMemory(&si,sizeof(si));
		si.cbSize = sizeof(si);
		si.hwnd = GetSafeHwnd();
		si.nShow = SW_SHOW;
		si.lpIDList = (LPVOID)pliParam->lpidl;
		si.fMask  = SEE_MASK_INVOKEIDLIST;
		ShellExecuteEx(&si);
	}
	else
	{
//		SetListContext(pliParam->lpsf,pliParam->lpidl);
		g_TreeView->SetTreeContext(pliParam->lpsf,pliParam->lpidl);
	}
	*pResult = 0;
}

void CExplorerListView::OnRclick(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	NMITEMACTIVATE * nmia = (NMITEMACTIVATE *) pNMHDR;
	
	POSITION pos = m_pListCtrl->GetFirstSelectedItemPosition();
	if ( pos==NULL )
	{
		//没选中时

		return;
	}

	char * szPath = new char[MAX_PATH];
	CStringArray sPathArray;
	while (pos)
	{
		ZeroMemory(szPath,MAX_PATH);
		int nItem = m_pListCtrl->GetNextSelectedItem(pos);
		LPLISTPARAM pliParam =(LPLISTPARAM) m_pListCtrl->GetItemData(nItem);
		SHGetPathFromIDList(pliParam->lpidl,szPath);
		sPathArray.Add(szPath);
	}
	delete szPath;
	
	CShellContextMenu scm;        
	scm.SetObjects(sPathArray);    
	CPoint point (nmia->ptAction);
	m_pListCtrl->ClientToScreen(&point);
	UINT idCommand = scm.ShowContextMenu(this, point);

	switch( idCommand )
	{
	case 18: //chose delete
		{
			g_TreeView->SendMessage(WM_REFRESHFILES,(WPARAM)g_TreeView->m_hSelItem,0);
			SendMessage(WM_REFRESHFILES,0,0);
			break;
		}
	case 27:  //chose paste
		{
			break;
		}
	}

 	*pResult = 0;
}

void CExplorerListView::OnBegindrag(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	g_TreeView->m_hMoveSrc = g_TreeView->m_hSelItem;

	POSITION pos = m_pListCtrl->GetFirstSelectedItemPosition();
	if ( pos==NULL )
		return;
	
	char * szSrc = new char[MAX_PATH];
	CStringArray sDragArray;
	while(pos)
	{
		int nItem = m_pListCtrl->GetNextSelectedItem(pos);
		LPLISTPARAM pliItem = (LPLISTPARAM)m_pListCtrl->GetItemData(nItem);
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
	
	COleDataSource *poleSourceObj = new COleDataSource;
	poleSourceObj->CacheGlobalData(CF_HDROP,hgblData);
	DROPEFFECT dropeffect = poleSourceObj->DoDragDrop();
	
	*pResult = 0;
}

void CExplorerListView::OnEndlabeledit(NMHDR* pNMHDR, LRESULT* pResult) 
{
	LV_DISPINFO* pDispInfo = (LV_DISPINFO*)pNMHDR;
	// TODO: Add your control notification handler code here
	LV_ITEM lvItem = pDispInfo->item;
	LPLISTPARAM pliParam = (LPLISTPARAM)lvItem.lParam;
	char * szSource = new char[MAX_PATH];
	SHGetPathFromIDList(pliParam->lpidl,szSource);
	
	LPTREEPARAM ptrParam = (LPTREEPARAM)g_TreeView->GetSelTreeData();
	char * szDest = new char[MAX_PATH];
	SHGetPathFromIDList(ptrParam->lpidl,szDest);
	strcat(szDest,lvItem.pszText);
	
	CShellFileOp op;
	op.SetOperationFlags(FO_MOVE, this, FOF_NOCONFIRMMKDIR);
	op.AddDestFile(szDest);
	op.AddSourceFile(szSource);
	
	BOOL bSuccess, bAPICalled = FALSE, bAborted = FALSE;
	int  nAPIReturn = 0;
	bSuccess = op.Go ( &bAPICalled, &nAPIReturn, &bAborted );
	if ( bSuccess && !bAborted )
	{
		g_TreeView->SendMessage(WM_REFRESHFILES,(WPARAM)g_TreeView->m_hSelItem,0);
		*pResult = 1;
	}
	else
	{
		*pResult = 0;
	}
}

long CExplorerListView::OnRefreshList(WPARAM wParam, LPARAM lParam)
{
	if ( g_TreeView )
	{
		LPTREEPARAM ltrParam = (LPTREEPARAM)g_TreeView->GetSelTreeData();
		SetListContext(ltrParam->lpsf,ltrParam->lpidl);
	}
	return 0;
}

BOOL CExplorerListView::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if ( pMsg->message==WM_RENDERFORMAT )
	{
		long l = pMsg->lParam;
	}
	if ( pMsg->message==WM_KEYDOWN )
	{
		HWND hWnd = this->GetSafeHwnd();
		switch ( pMsg->wParam )
		{
		case 0x43:     //Ctrl+C
			{
				short ctrl = GetKeyState(VK_CONTROL);
				if ( HIBYTE(ctrl)!=0xFF )
					break;
				
				GetParent()->PostMessage(WM_COMMAND,MAKEWPARAM(0,ID_EDIT_COPY),0);
				break;
			}
		case 0x58:     //Ctrl+X
			{ 
				short ctrl = GetKeyState(VK_CONTROL);
				if ( HIBYTE(ctrl)!=0xFF )
					break;
				
				GetParent()->PostMessage(WM_COMMAND,MAKEWPARAM(0,ID_EDIT_CUT),0);
				break;
			}
		case 0x56:     //Ctrl+V
			{
				short ctrl = GetKeyState(VK_CONTROL);
				if ( HIBYTE(ctrl)!=0xFF )
					break;

				GetParent()->PostMessage(WM_COMMAND,MAKEWPARAM(0,ID_EDIT_PASTE),0);
				break;
			}
		case 0x2e:       //DEL
			{
				CShellFileOp op;
				short lShift = GetKeyState(VK_LSHIFT);
				short rShift = GetKeyState(VK_RSHIFT);
				if ( HIBYTE(lShift)==0xFF || HIBYTE(rShift)==0xFF )
					op.SetOperationFlags(FO_DELETE, this,NULL);
				else
					op.SetOperationFlags(FO_DELETE, this,FOF_ALLOWUNDO);
				
				
				POSITION pos = m_pListCtrl->GetFirstSelectedItemPosition();
				if ( pos==NULL )
					return true;
				char * szSrc = new char[MAX_PATH];
				while(pos)
				{
					int nItem = m_pListCtrl->GetNextSelectedItem(pos);
					LPLISTPARAM pliItem = (LPLISTPARAM)g_ListView->m_pListCtrl->GetItemData(nItem);
					
					ZeroMemory(szSrc,MAX_PATH);
					SHGetPathFromIDList(pliItem->lpidl,szSrc);
					
					op.AddSourceFile(szSrc);
				}
				delete szSrc;

				BOOL bSuccess, bAPICalled = FALSE, bAborted = FALSE;
				int  nAPIReturn = 0;
				bSuccess = op.Go(&bAPICalled, &nAPIReturn, &bAborted);
				
				//refresh
				if ( bSuccess&&!bAborted )
				{
					g_TreeView->SendMessage(WM_REFRESHFILES,(WPARAM)g_TreeView->m_hSelItem,0);
					g_ListView->SendMessage(WM_REFRESHFILES,0,0);
				}
				break;
			}
		}
	}
	if (pMsg->message==WM_DROPFILES)
	{
		short ctrl = HIWORD(pMsg->wParam);
		return 0;
	}
	return CListView::PreTranslateMessage(pMsg);
}


//////////////////////////////////////////////////
// method
///////////////////////////////////////////////////

void CExplorerListView::InitImageList()
{	
	HIMAGELIST himlSmall;
    HIMAGELIST himlLarge;
    SHFILEINFO sfi;

    himlSmall = (HIMAGELIST)SHGetFileInfo((LPCTSTR)_T("C:\\"), 
                                           0,
                                           &sfi, 
                                           sizeof(SHFILEINFO), 
                                           SHGFI_SYSICONINDEX | SHGFI_SMALLICON);

    himlLarge = (HIMAGELIST)SHGetFileInfo((LPCTSTR)_T("C:\\"), 
                                           0,
                                           &sfi, 
                                           sizeof(SHFILEINFO), 
                                           SHGFI_SYSICONINDEX | SHGFI_LARGEICON);

    if (himlSmall && himlLarge)
    {
        ::SendMessage(GetSafeHwnd(), LVM_SETIMAGELIST, (WPARAM)LVSIL_SMALL,
            (LPARAM)himlSmall);
        ::SendMessage(GetSafeHwnd(), LVM_SETIMAGELIST, (WPARAM)LVSIL_NORMAL,
            (LPARAM)himlLarge);
    }

}	

void CExplorerListView::SetListContext(IShellFolder *psfParent, LPITEMIDLIST pidlParent)
{
//	m_pListCtrl->DeleteAllItems();

	DeleteAllItems();

	IEnumIDList  *peidl;
	LPITEMIDLIST pidl, pidlAbs;
	IShellFolder *psf = NULL;

	HRESULT hr = psfParent->EnumObjects(NULL,SHCONTF_FOLDERS ,&peidl);

	if ( NOERROR!=hr )
		return;
	while (NOERROR == hr)
	{
		if(m_BStop)
			break;
		hr = peidl->Next(1, &pidl, NULL);
		if (hr == S_FALSE || pidl == NULL)
			break;
		pidlAbs = ConcatPidl(pidlParent,pidl);
		psfParent->BindToObject(pidl, NULL, IID_IShellFolder, (void**)&psf);

		SHFILEINFO sfi;
		SHGetFileInfo((LPCTSTR)pidlAbs, 
			0,
			&sfi, 
			sizeof(SHFILEINFO), 
			SHGFI_PIDL|SHGFI_SYSICONINDEX|SHGFI_SMALLICON|
			SHGFI_DISPLAYNAME|SHGFI_ATTRIBUTES);

		LVITEM lvItem;
		memset(&lvItem,0,sizeof(lvItem));
	/*	lvItem.mask     |= LVIF_TEXT|LVIF_IMAGE|LVIF_PARAM ;
		lvItem.iItem     = m_pListCtrl->GetItemCount();
		lvItem.iImage    = sfi.iIcon;
		lvItem.pszText   = sfi.szDisplayName;*/

		LPLISTPARAM pliParam = (LPLISTPARAM)m_pMalloc->Alloc(sizeof(LISTPARAM));
		pliParam->lpsf    = psf;
		pliParam->lpidl   = pidlAbs;
		pliParam->uType   = 0;
	
		strcpy(pliParam->szName,sfi.szDisplayName);
		lvItem.lParam = (LPARAM)pliParam;
		SetItems(sfi, pliParam);
	
		m_pMalloc->Free(pidl);
	}
	peidl->Release();    //若不Release掉文件会处于被操作的状态

	if ( g_TreeView )
	{
		//我的电脑的子项会得到文件和非文件两套
		//过滤掉文件的项
		if ( ComparePidls(g_TreeView->m_psfDesk, g_TreeView->m_pidlMyComputer, pidlParent) )
			return;
	}

	hr = psfParent->EnumObjects(NULL,SHCONTF_NONFOLDERS ,&peidl);
	if ( NOERROR!=hr )
		return;
	while (NOERROR == hr)
	{
		hr = peidl->Next(1, &pidl, NULL);
		if (hr == S_FALSE || pidl == NULL)
			break;
		pidlAbs = ConcatPidl(pidlParent,pidl);
		psfParent->BindToObject(pidl, NULL, IID_IShellFolder, (void**)&psf);

		SHFILEINFO sfi;
		SHGetFileInfo((LPCTSTR)pidlAbs, 
			0,
			&sfi, 
			sizeof(SHFILEINFO), 
			SHGFI_PIDL|SHGFI_SYSICONINDEX|SHGFI_SMALLICON|SHGFI_DISPLAYNAME);


		LPLISTPARAM pliParam = (LPLISTPARAM)m_pMalloc->Alloc(sizeof(LISTPARAM));
		pliParam->lpsf    = psf;
		pliParam->lpidl   = pidlAbs;
		pliParam->uType   = 1;
		strcpy(pliParam->szName,sfi.szDisplayName);
		SetItems(sfi, pliParam);
	}
	peidl->Release();

	//m_pListCtrl->SortItems(SortListView, (DWORD)this);
}
#define S_IFMT	0xF000	/* File type mask */
#define S_IFDIR 0x4000 /* Directory */

#define S_ISDIR(mode) ((mode& S_IFMT) == S_IFDIR ) //Is Directory 
bool IsDir(const CString& strCurrentFileOrDirectory)
{
	/*WIN32_FIND_DATA wfdata;
	HANDLE hfile = FindFirstFile(strCurrentFileOrDirectory,&wfdata);
	if(hfile==INVALID_HANDLE_VALUE)
	{
	if(PathFindFileName(strCurrentFileOrDirectory)==CString(wfdata.cFileName))
	return false;
	}*/
	struct _stat fileStat;

	if(_tstat(strCurrentFileOrDirectory,&fileStat) == 0)
	{
		return S_ISDIR(fileStat.st_mode);
	}
	else
		return false;
}

void FindFiles(CString folder, INT64 &allSize)
{
	HANDLE hfile;//查找文件句柄
	WIN32_FIND_DATA wfdata;//文件信息结构
	BOOL IsOver=FALSE;//查找过程结束标志
	CString strname;//文件名
	CString strfull;//全路径

	// hfile = FindFirstFile((LPCSTR)("*"),&wfdata);//查找第一个文件
	CString sf=folder;
	folder = folder + L"\\*";

	hfile = FindFirstFile(folder,&wfdata);//查找第一个文件

	//if(hfile==(HANDLE)-1)
	//{
	//	hfile = FindFirstFile(sf,&wfdata);//查找第一个文件

	//	if(PathFindFileName(sf)==CString(wfdata.cFileName))
	//	{

	//		allSize+= ((INT64)wfdata.nFileSizeHigh * (MAXDWORD+1)) + (INT64)wfdata.nFileSizeLow;
	//	}
	//	return;
	//}
	//else 
		if (hfile==INVALID_HANDLE_VALUE) //查找结束
	{
			//if(!IsDir(sf))
			{

				 FindFirstFile(sf,&wfdata);
				 allSize+= ((INT64)wfdata.nFileSizeHigh * (MAXDWORD+1)) + (INT64)wfdata.nFileSizeLow;
			}
		return;
	} 
	do
	{
		CString strCurrentFileOrDirectory(sf);
		strCurrentFileOrDirectory += "\\";
		strCurrentFileOrDirectory += wfdata.cFileName;

		//allSize+= ((INT64)wfdata.nFileSizeHigh * (MAXDWORD+1)) + (INT64)wfdata.nFileSizeLow;
		//if(0 != _tcsicmp(wfdata.cFileName,_T(".")) && 
		//	0 != _tcsicmp(wfdata.cFileName,_T("..")))
		//FindFiles(strCurrentFileOrDirectory,allSize);
		//else
		//{
		//	 
		//}
		
		//continue;

		//We are doing a Depth First Search	
		if(IsDir(strCurrentFileOrDirectory))
		{
			if(0 != _tcsicmp(wfdata.cFileName,_T(".")) && 
				0 != _tcsicmp(wfdata.cFileName,_T("..")))
			{	
				//_tprintf(_T("\n\nTraversing directory %s\n"),strCurrentFileOrDirectory );
				if(!(wfdata.dwFileAttributes &FILE_ATTRIBUTE_REPARSE_POINT))
				FindFiles(strCurrentFileOrDirectory,allSize);
			}
		}
		else
		{	//else we need to enumerate files
			//TRACE("\n%s",wfdata.cFileName );
			allSize+= ((INT64)wfdata.nFileSizeHigh * (MAXDWORD+1)) + (INT64)wfdata.nFileSizeLow;
		}

	}while(::FindNextFile(hfile,&wfdata));
 

	//while(!IsOver)
	//{
	//	strname.Format("%s",wfdata.cFileName);//获取找到的文件名
	//	folder.TrimRight("*");
	//	strfull=folder + strname;          //全路径

	//	if ((wfdata.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY)
	//		&&(wfdata.cFileName[0]!='.'))//是有效文件夹
	//	{
	//		// cout<<wfdata.cFileName<<endl;
	//		FindFiles(strfull ,allSize);//递归遍历
	//	}
	//	else if (wfdata.cFileName[0]!='.')//是有效文件
	//	{	

	//		allSize+= ((INT64)wfdata.nFileSizeHigh * (MAXDWORD+1)) + (INT64)wfdata.nFileSizeLow;


	//		//	TRACE("Path:%s,Size:%lld,Total:%lld\n",sf,len, allSize);
	//		// cout<<strfull<<endl;
	//		// if(strTemp.Compare(".txt")==0)
	//		// {    
	//		//  m_FileList.AddString(strfull);//将.txt文件添加到列表控件
	//		// }
	//	}
	//	IsOver=!FindNextFile(hfile,&wfdata);//查找下一个
	//}
	FindClose(hfile);//关闭查找文件句柄

}
void FindFiles0(CString folder, INT64 &allSize)
{
	if(g_ListView->m_BStop)
		return;
	HANDLE hfile;//查找文件句柄
	WIN32_FIND_DATA wfdata;//文件信息结构
	BOOL IsOver=FALSE;//查找过程结束标志
	CString strname;//文件名
	CString strfull;//全路径
	
	// hfile = FindFirstFile((LPCSTR)("*"),&wfdata);//查找第一个文件
	CString sf=folder;
	if(sf.Trim().IsEmpty())
		return;
	folder = folder + L"\\*";
	 
	hfile = FindFirstFile(folder,&wfdata);//查找第一个文件
 
	//if(hfile==(HANDLE)-1)
	//{
	//	hfile = FindFirstFile(sf,&wfdata);//查找第一个文件
	//	
	//	if(PathFindFileName(sf)==CString(wfdata.cFileName))
	//	{
	//			
	//		allSize+= ((INT64)wfdata.nFileSizeHigh * (MAXDWORD+1)) + (INT64)wfdata.nFileSizeLow;
	//	 
	//	 
	//	}
	//	return;
	//}
	//else 
		if (hfile==INVALID_HANDLE_VALUE) //查找结束
	{
		FindFirstFile(sf,&wfdata);
		allSize+= ((INT64)wfdata.nFileSizeHigh * (MAXDWORD+1)) + (INT64)wfdata.nFileSizeLow;
		IsOver=TRUE;		
	}
	while(!IsOver&&!g_ListView->m_BStop)
	{
		strname.Format("%s",wfdata.cFileName);//获取找到的文件名
     	folder.TrimRight("*");
		strfull=folder + strname;          //全路径
		
		if ((wfdata.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY) && (0 != _tcsicmp(wfdata.cFileName,_T(".")) && 
				0 != _tcsicmp(wfdata.cFileName,_T("..")))	/*&&(wfdata.cFileName[0]!='.')*/)//是有效文件夹
		{
			if(!(wfdata.dwFileAttributes &FILE_ATTRIBUTE_REPARSE_POINT))
			 
			// cout<<wfdata.cFileName<<endl;
			 FindFiles0(strfull ,allSize);//递归遍历
		}
		else if (wfdata.cFileName[0]!='.')//是有效文件
		{	
		
			allSize+= ((INT64)wfdata.nFileSizeHigh * (MAXDWORD+1)) + (INT64)wfdata.nFileSizeLow;
		
				
		//	TRACE("Path:%s,Size:%lld,Total:%lld\n",sf,len, allSize);
			// cout<<strfull<<endl;
			// if(strTemp.Compare(".txt")==0)
			// {    
			//  m_FileList.AddString(strfull);//将.txt文件添加到列表控件
			// }
		}
		IsOver=!FindNextFile(hfile,&wfdata);//查找下一个
	}
	FindClose(hfile);//关闭查找文件句柄
 
}

DWORD WINAPI ThreadMethod(  LPVOID lpParameter)
{
	 
	tagItemFileInfo* info=(tagItemFileInfo*)lpParameter;
	INT64 nSize=0;
	CString sPath = info->sPath;
	if(g_ListView->m_BStop)
	{
	 
		delete info;
		return 0;
	 }
	FindFiles0(sPath,nSize);
	float fSize;
	CString s;
	if(nSize>1024*1024*1024)	
	{
		fSize = (double) nSize / (1024*1024*1024);
		s.Format("%.3fGB", fSize);
	}
	else if(nSize>1024*1024)	
	{
		fSize = (double)nSize / (1024*1024);
		s.Format("%.3fMB", fSize);
	}
	else if(nSize>1024)	
	{
		fSize = (double)nSize / (1024);
		s.Format("%.3fKB", fSize);
	}
	else 
		s.Format("%dB", nSize);
	if(!g_ListView->m_BStop)	 
		info->pList->SetItemText(info->iItem, 1,s );
	//info->pList->SortItems(g_ListView->SortListView, (DWORD)g_ListView);
	delete info;
 
	return 0;

}

void CExplorerListView::SetItems(SHFILEINFO &sfi,LPLISTPARAM &pliParam)
{

	LVITEM lvItem;
	memset(&lvItem,0,sizeof(lvItem));
	lvItem.mask     |= LVIF_TEXT|LVIF_IMAGE|LVIF_PARAM ;
	lvItem.iItem     = m_pListCtrl->GetItemCount();
	lvItem.iImage    = sfi.iIcon;
	lvItem.pszText   = sfi.szDisplayName;

	lvItem.lParam = (LPARAM)pliParam;

	int nItem = m_pListCtrl->InsertItem(&lvItem);
	if(m_pListCtrl->GetExtendedStyle()&LVS_REPORT )
	{	CString s;INT64 nSize(0);
		char szPath[MAX_PATH];
		SHGetPathFromIDList(pliParam->lpidl,szPath);
		if(CString(szPath).IsEmpty() )
		{
			m_pListCtrl->SetItemText(lvItem.iItem, 1,"" );
			return;
		}
		if(IsDir(szPath))
		{
			m_pListCtrl->SetItemText(lvItem.iItem, 1,"等待计算..." );
			if(m_BStop)
				 return;
			DWORD dwRet = 0;
			int   nIndex = 0;
			DWORD dwThreadID=0;//保存线程ID		
			int nCount= g_nThreadNum;
			if(g_nThreadNum>10000)
			{
				while(1)
				{
					dwRet = WaitForMultipleObjects(nCount,g_hThread,false,INFINITE);
					switch(dwRet)
					{
					case WAIT_TIMEOUT:	break;
					case WAIT_FAILED:
						return ;
					default:
						{
							nIndex = dwRet - WAIT_OBJECT_0;
							tagItemFileInfo *info=new tagItemFileInfo;
							info->sPath = szPath;
							info->pList = m_pListCtrl;
							info->iItem=lvItem.iItem;
							g_hThread[nIndex++] =CreateThread(0,0,ThreadMethod,info,0,&dwThreadID);
						//	processhan(nIndex++);
							//同时检测其他的事件
							while(nIndex < nCount)
							{
								dwRet = WaitForMultipleObjects(nCount - nIndex,&g_hThread[nIndex],false,0);
								switch(dwRet)
								{
								case WAIT_TIMEOUT:
									nIndex = nCount; //退出检测,因为没有被触发的对象了.
									break;
								case WAIT_FAILED: return ;
								default:
									{
										nIndex = dwRet - WAIT_OBJECT_0;
										tagItemFileInfo *info=new tagItemFileInfo;
										info->sPath = szPath;
										info->pList = m_pListCtrl;
										info->iItem=lvItem.iItem;
										g_hThread[nIndex++] =CreateThread(0,0,ThreadMethod,info,0,&dwThreadID);
									}
									break;
								}
							}
						}
						break;
					}
				}
				return ;			 
			
			}
			tagItemFileInfo *info=new tagItemFileInfo;
			info->sPath = szPath;
			info->pList = m_pListCtrl;
			info->iItem=lvItem.iItem;
			m_pListCtrl->SetItemText(lvItem.iItem, 1,"正在计算..." );

			if(g_nThreadNum>=2000)
			{ 
				//m_pListCtrl->SetItemText(lvItem.iItem, 1,"停止计算" );
				ThreadMethod(info);
				return;
			}	 
			//g_hThread[g_nThreadNum++] =CreateThread(0,0,ThreadMethod,info,0,&dwThreadID);//创建线程
		g_hThread[g_nThreadNum++] =AfxBeginThread((AFX_THREADPROC)ThreadMethod,info);//创建线程
			//CloseHandle(hThread);	
			return;
		}
		//if((GetFileAttributes(sfi.szDisplayName)   &  FILE_ATTRIBUTE_DIRECTORY )!=0)
		{
			
			//FindFiles( szPath,nSize);	
			WIN32_FIND_DATA wfdata;//文件信息结构
			FindFirstFile(szPath,&wfdata);
			nSize = ((INT64)wfdata.nFileSizeHigh * (MAXDWORD+1)) + (INT64)wfdata.nFileSizeLow;
		}
		float fSize;
		if(nSize>1024*1024*1024)	
		{
			fSize = (double) nSize / (1024*1024*1024);
			s.Format("%.3fGB", fSize);
		}
		else if(nSize>1024*1024)	
		{
			fSize = (double)nSize / (1024*1024);
			s.Format("%.3fMB", fSize);
		}
		else if(nSize>1024)	
		{
			fSize = (double)nSize / (1024);
			s.Format("%.3fKB", fSize);
		}
		else 
			s.Format("%dB", nSize);
		m_pListCtrl->SetItemText(lvItem.iItem, 1,s );
	}
	//m_pListCtrl->InsertItem(&lvItem);
	//m_pListCtrl->SetItemText(lvItem.iItem,0,sfi.szDisplayName);

}
int CALLBACK CExplorerListView::SortListView(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort)
{
	CListCtrl* pListCtrl = (CListCtrl*)lParamSort;
	LPLISTPARAM     pliParam1 = (LPLISTPARAM)lParam1;
	LPLISTPARAM     pliParam2 = (LPLISTPARAM)lParam2;
	if ( pliParam1->uType==pliParam2->uType )
	{
		return strcmp(pliParam1->szName, pliParam2->szName);
	}
	else if (pliParam1->uType<pliParam2->uType)
	{
		return -1;
	}
	else
	{
		return 1;
	}
}

int CExplorerListView::SetListViewType(DWORD viewType)
{
	if ( m_dViewType==viewType )
		return viewType;
	
	int old = m_dViewType;

	CMenu * pMainMenu = AfxGetApp()->GetMainWnd()->GetMenu();
	CMenu * pMenu     = pMainMenu->GetSubMenu(2);
	
	switch(viewType)
	{
	case LVS_ICON:	
		pMenu->CheckMenuRadioItem(3,5,3,MF_BYPOSITION);
		break;
	case LVS_SMALLICON:
		pMenu->CheckMenuRadioItem(3,5,4,MF_BYPOSITION);
		break;
	case LVS_LIST:	
		pMenu->CheckMenuRadioItem(3,5,5,MF_BYPOSITION);
		break;
	}
	ModifyStyle(m_dViewType, viewType);

	m_dViewType = viewType;

	return old;
}


void CExplorerListView::OnMenuSelect(UINT nItemID, UINT nFlags, HMENU hSysMenu)
{
	CWnd::OnMenuSelect(nItemID, nFlags, hSysMenu);
}

void CExplorerListView::DeleteAllItems()
{
	int nCount = m_pListCtrl->GetItemCount();
	for ( int i=0 ; i<nCount ; i++ )
	{
		LPLISTPARAM pliParam = (LPLISTPARAM)m_pListCtrl->GetItemData(i);
		m_pMalloc->Free(pliParam->lpidl);
		m_pMalloc->Free(pliParam);
	}
	m_pListCtrl->DeleteAllItems();
}
