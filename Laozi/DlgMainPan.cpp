// DlgMainPan.cpp : 实现文件
//

#include "stdafx.h"
#include "Laozi.h"
#include "DlgMainPan.h"
#include "afxdialogex.h"
#include "ProcessMgr.h"
#include <afx.h>
#include <commdlg.h>
#include <Psapi.h>
#include "MyString.h"
#include "DlgBossKey.h"
#include "KeyHelper.h"



// CDlgMainPan 对话框

IMPLEMENT_DYNAMIC(CDlgMainPan, CDialogEx)

	CDlgMainPan::CDlgMainPan(CWnd* pParent /*=NULL*/)
	: CDlgMainBase()	
{
	m_bHideByProcess = theApp.m_config.m_guardInfo.bHideByProcess;
	m_bHideByWndCls = theApp.m_config.m_guardInfo.bHideByWndClass;
	m_bHideByWndText = theApp.m_config.m_guardInfo.bHideByWndText;
	m_bHideTray = theApp.m_config.m_guardInfo.bHideTray;
	m_hSelectedWnd = NULL;
}

CDlgMainPan::~CDlgMainPan()
{

}

void CDlgMainPan::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BUTTON_CHOOSEWND, m_btnChooseWnd);
	DDX_Control(pDX, IDC_LIST1, m_listProgram);
	DDX_Check(pDX, IDC_CHECK_HidePro, m_bHideByProcess);
	DDX_Check(pDX, IDC_CHECK_HideSeletedWnds, m_bHideByWndCls);
	DDX_Check(pDX, IDC_CHECK_HideContainsText, m_bHideByWndText);
	DDX_Check(pDX, IDC_CURSOR_CHOOSEWND, m_bHideTray);
	DDX_Control(pDX, IDC_LIST2, m_listHideWndCls);
	//DDX_Control(pDX, IDC_EDITtemp2, m_tbHideWndText); 
	DDX_Control(pDX, IDC_BTN_SelectWndOK, m_btnSelectWndOk);
	DDX_Control(pDX, IDC_BTN_SelectWndCancle, m_btnSelectWndCancel);
	DDX_Control(pDX, IDC_STATIC_SelectedWnd, m_lbSelectWnd);
	DDX_Control(pDX, IDC_BUTTON_SelectProgram, m_btnSelectProg);
	DDX_Control(pDX, IDC_BUTTON_HideWnd, m_btnHideWnd);
	DDX_Control(pDX, IDC_BTN_HotKey, m_btnHotKey);
}


BEGIN_MESSAGE_MAP(CDlgMainPan, CDialogEx)
	ON_WM_PAINT()
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_BUTTON_SelectProgram, &CDlgMainPan::OnBnClickedButtonSelectprogram)
	ON_BN_CLICKED(IDC_BUTTON_CHOOSEWND, &CDlgMainPan::OnBnClickedButtonChoosewnd)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_BUTTON_HideWnd, &CDlgMainPan::OnBnClickedButtonHidewnd)
	ON_BN_CLICKED(IDC_BTN_SelectWndOK, &CDlgMainPan::OnBnClickedBtnSelectwndok)
	ON_BN_CLICKED(IDC_BTN_SelectWndCancle, &CDlgMainPan::OnBnClickedBtnSelectwndcancle)
	ON_BN_CLICKED(IDC_CHECK_HidePro, &CDlgMainPan::OnBnClickedCheckHidepro)
	ON_BN_CLICKED(IDC_CHECK_HideSeletedWnds, &CDlgMainPan::OnBnClickedCheckHideseletedwnds)
	ON_BN_CLICKED(IDC_CHECK_HideContainsText, &CDlgMainPan::OnBnClickedCheckHidecontainstext)
	ON_BN_CLICKED(IDC_BTN_HotKey, &CDlgMainPan::OnBnClickedBtnHotkey)
	ON_WM_KEYDOWN()
END_MESSAGE_MAP()


// CDlgMainPan 消息处理程序
BOOL CDlgMainPan::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	m_tt.Create(this);
	m_tt.Activate(TRUE);
	CWnd * pw = GetDlgItem(IDC_BUTTON_CHOOSEWND);
	m_tt.AddTool(pw,L"按住鼠标左键以选择窗口");
	m_brush.CreateSolidBrush(RGB(221,228,238));  
	m_listProgram.SetTextPosition(COwnerDrawListBox::ITEM_LEFT);
	m_listProgram.SetIconPosition(COwnerDrawListBox::ITEM_LEFT);	
	m_btnChooseWnd.SetTargetHwnd(GetSafeHwnd());
	SetDlgItemText(IDC_STATIC_HotKey,theApp.m_config.m_sHotKeyHideWnd);
	for (int i = 0;i < theApp.m_config.m_guardInfo.hidedProcessNameArr.GetCount(); i++)
	{
		AddListProgram( theApp.m_config.m_guardInfo.hidedProcessNameArr.GetAt(i));
	}
	vector<HideWndInfo>::iterator w;  
	for (w = theApp.m_config.m_guardInfo.hideWinCls.begin(); w != theApp.m_config.m_guardInfo.hideWinCls.end();w++)  
	{
		HideWndInfo *h  = new HideWndInfo ;		  
		h->sTitle = w->sTitle;
		h->clsName = w->clsName;
		h->sFileName =w->sFileName;
		AddListWndClass(h);
	}
	for (int i = 0;i < theApp.m_config.m_guardInfo.hidedWndText.GetCount(); i++)
	{

	}
	m_listHideWndCls.GetWindowRect(&m_list2Rect);
	ScreenToClient(m_list2Rect);
	m_btnSelectProg.SetImage(IDB_BITMAP_Button_1,IDB_BITMAP_Button_2,IDB_BITMAP_Button_2,IDB_BITMAP_Button_1);
	m_btnHideWnd.SetImage(IDB_BITMAP_Button_1,IDB_BITMAP_Button_2,IDB_BITMAP_Button_2,IDB_BITMAP_Button_1);
	m_btnHotKey.SetImage(IDB_BITMAP_Button_1,IDB_BITMAP_Button_2,IDB_BITMAP_Button_2,IDB_BITMAP_Button_1);
	m_btnSelectWndOk.SetImage(IDB_BITMAP_Button_1,IDB_BITMAP_Button_2,IDB_BITMAP_Button_2,IDB_BITMAP_Button_1);
	m_btnSelectWndCancel.SetImage(IDB_BITMAP_Button_1,IDB_BITMAP_Button_2,IDB_BITMAP_Button_2,IDB_BITMAP_Button_1);
	// DragAcceptFiles();
	UpdateData(FALSE);
	SetStatus();
	return TRUE;
}
void CDlgMainPan::AddListProgram(LPCTSTR file)
{
	if(file == L"")
		return;
	SHFILEINFO FileInfo;
	DWORD_PTR dwRet = ::SHGetFileInfo(file, 0, &FileInfo, sizeof(SHFILEINFO), SHGFI_ICON);
	int n = 0;
	CDWordArray arr;
	n = m_listProgram.FindItem(file, &arr);
	int m =  m_listProgram.GetCurSel();
	m_listProgram.SetSel(m,FALSE);
	if(n==0)
	{
		HICON h = ::ExtractIcon(NULL,file,0);
		m_listProgram.Add(file, h );
	}
	else
		m_listProgram.SetCurSel(arr.GetAt(0));
}


int  CDlgMainPan::AddListWndClass(HideWndInfo* info)
{//info.clsName = L"ddddddddddd";
	int m = m_listHideWndCls.GetCount();
	for (int i=0;i<m;i++)
	{
		HideWndInfo* h =(HideWndInfo*) m_listHideWndCls.GetItemDataPtr(i); 

		if(*h == *info) 
		{
			m_listHideWndCls.SetCurSel(i);
			return -1;
		}
	}
	int n = m_listHideWndCls.AddString(info->sTitle);
	m_listHideWndCls.SetItemDataPtr(n, info);
	return n;
}

void  CDlgMainPan::GetCfg(guardInfo &info)
{

	int n = m_listProgram.GetCount();
	info.hidedProcessNameArr.RemoveAll();
	for (int i=0; i<n; i++)
	{
		CString s = m_listProgram.GetItemText(i);
		info.hidedProcessNameArr.Add(s);
	}
	info.hideWinCls.clear();
	n= m_listHideWndCls.GetCount();
	for (int i=0; i<n; i++)
	{
		HideWndInfo* s = (HideWndInfo*)m_listHideWndCls.GetItemDataPtr(i);
		info.hideWinCls.push_back(*s);
	}

	info.bHideByProcess = m_bHideByProcess;
	info.bHideByWndClass = m_bHideByWndCls;
	info.bHideByWndText = m_bHideByWndText;
	 
}
void CDlgMainPan::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	CRect rcClient,rcBtn,rcScroll,rcPic;
	GetClientRect(&rcClient);

	//内存画图//////////////////////////
	CDC szMemDC;
	szMemDC.CreateCompatibleDC(&dc);

	CBitmap btScreen;
	btScreen.CreateCompatibleBitmap(&dc, rcClient.Width(), rcClient.Height());

	szMemDC.SelectObject(&btScreen);
	btScreen.DeleteObject();
	//szMemDC.Rectangle(rcClient);
	//szMemDC.Draw3dRect(&rcClient,RGB(0,100,200),RGB(255,255,255));

	szMemDC.FillSolidRect( &rcClient ,RGB(255,255,255));//RGB( 150,150,150));


	szMemDC.FillSolidRect( &rcClient ,RGB(100,150,238));//RGB( 150,150,150));


	szMemDC.FillSolidRect( &rcClient ,RGB(221,228,238));//RGB( 150,150,150));

	dc.BitBlt(rcClient.left, rcClient.top, rcClient.Width(), rcClient.Height(), &szMemDC, 0, 0, SRCCOPY);




	//画边框
	szMemDC.MoveTo(rcClient.right-1,rcClient.top);
	szMemDC.LineTo(rcClient.right-1,rcClient.bottom);

	//dc.BitBlt(rcClient.left, rcClient.top, rcClient.Width(), rcClient.Height(), &szMemDC, 0, 0, SRCCOPY);

	szMemDC.DeleteDC();
}

BOOL CDlgMainPan::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
	if (NULL != m_tt.GetSafeHwnd())
	{
		m_tt.RelayEvent(pMsg);
	}
	if(pMsg->message==WM_KEYDOWN&&pMsg->wParam==VK_RETURN)    
		return TRUE; 
	if(pMsg->message==WM_KEYDOWN&&pMsg->wParam==VK_ESCAPE)    
		return TRUE; 
	return CDialogEx::PreTranslateMessage(pMsg);
}


HBRUSH CDlgMainPan::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  在此更改 DC 的任何特性

	if( nCtlColor == CTLCOLOR_STATIC )        
	{      
		pDC->SetBkMode(TRANSPARENT);   //设置背景透明   
		hbr =    HBRUSH(GetStockObject(NULL_BRUSH));  
	}   

	//if(nCtlColor == CTLCOLOR_BTN)
	{
		UINT id = pWnd->GetDlgCtrlID();
		if(id==IDC_CHECK_HideTray || id  == IDC_CHECK_HidePro || id == IDC_CHECK_HideContainsText || id == IDC_CHECK_HideSeletedWnds/* || id == IDC_BUTTON4*/)
		{
			CRect rc;
			pWnd->GetWindowRect(&rc);
			ScreenToClient(&rc);

			CDC* dc = GetDC();

			pDC->BitBlt(0,0,rc.Width(),rc.Height(),dc,rc.left,rc.top,SRCCOPY);	//把父窗口背景图片先画到按钮上

			ReleaseDC(dc);

			hbr = (HBRUSH) ::GetStockObject(NULL_BRUSH);
		}
		else if (id == IDC_STATIC)
		{
			pDC->SetTextColor(RGB(255,255,255));

			//pDC->SetBkMode(TRANSPARENT); 
			return m_brush;
		}

		if(id == IDC_LIST1 || id == IDC_LIST2 || id == IDC_LIST3)
		{
			/*if(m_bHideByProcess)
				pDC ->SetBkColor( RGB(190,190,220) ) ;
			else
				pDC ->SetBkColor( RGB(235,235,255) ) ;*/
			
		//	hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
		}
 
	}


	// TODO:  如果默认的不是所需画笔，则返回另一个画笔
	return hbr;
}


void CDlgMainPan::OnBnClickedButtonSelectprogram()
{ 
	CStringArray strArr;
	//int n = CProcessMgr::FindSoft(strArr);
	TCHAR   szPath[MAX_PATH],szPath1[MAX_PATH];
	SHGetSpecialFolderPath(NULL,szPath, CSIDL_COMMON_PROGRAMS ,FALSE);
	SHGetSpecialFolderPath(NULL,szPath1, CSIDL_COMMON_STARTUP ,FALSE);
	CString file;
	//wcscat(szPath,L"\\");
	//TCHAR szBuffer[MAX_PATH] = {0}; 
	////wcscpy(szBuffer, szPath);
	//OPENFILENAME ofn= {0}; 
	//ofn.lStructSize = sizeof(ofn); 
	//ofn.hwndOwner = m_hWnd; 
	//ofn.lpstrFilter = _T("Exe文件(*.exe)\0*.exe\0所有文件(*.*)\0*.*\0");//要选择的文件后缀 
	//ofn.lpstrInitialDir = szPath;//默认的文件路径 
	//ofn.lpstrFile = szBuffer;//存放文件的缓冲区 
	//ofn.nMaxFile = sizeof(szBuffer)/sizeof(*szBuffer); 
	//ofn.nFilterIndex = 0; 
	//ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_EXPLORER ;//标志如果是多选要加上OFN_ALLOWMULTISELECT
	//BOOL bSel = GetOpenFileName(&ofn); 
	CFileDialog dlg(TRUE,NULL,szPath1,OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT /* | OFN_NOCHANGEDIR*/,TEXT("Exe文件(*.exe)|*.exe||"),this);
	dlg.GetOFN().lpstrInitialDir  = szPath;
	if(dlg.DoModal()==IDOK)
	{
		file =  dlg.GetPathName();
	}
	if(file == "")
		return;

	AddListProgram(file);


	//CFileFind find;
	//CString fdPath;
	//BOOL bf = find.FindFile(szPath);
	//while(bf)
	//{                        
	//	bf = find.FindNextFile();
	//	if(!find.IsDots())
	//	{
	//		fdPath=find.GetFilePath();
	//		if (find.IsDirectory())
	//		{
	//			//如果是文件夹，递归，继续往下找                        
	//			//getFolderDayFile(fdPath, arrStrFile);
	//		}
	//		else
	//		{
	//			//如果是文件,判断是否是*.txt
	//			 
	//		}
	//	}
	//}
	//find.Close();
}


void CDlgMainPan::OnBnClickedButtonChoosewnd()
{
	// TODO: 在此添加控件通知处理程序代码
}


void CDlgMainPan::Destroy()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	m_listProgram.Destroy();
	int m = m_listHideWndCls.GetCount();
	for (int i=0; i<m; i++)
	{
		HideWndInfo* h =(HideWndInfo*) m_listHideWndCls.GetItemDataPtr(i); 
		delete h;
		//m_listHideWndCls.DeleteString(i);
	} 
}


void CDlgMainPan::OnBnClickedButtonHidewnd()
{
	HideWnd();
}


BOOL CDlgMainPan::OnWndMsg(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult)
{
	// TODO: 在此添加专用代码和/或调用基类
	static TCHAR cls[MAX_PATH],text[MAX_PATH];

	static  CRect initLbRect(0,0,0,0) ;

	switch(message)
	{
	case MSG_SLECTEWNDBegin:
		OnBnClickedBtnSelectwndcancle();
		break;
	case  MSG_SLECTEWND:
		if(initLbRect.left ==0 && initLbRect.right==0)
		{
			m_lbSelectWnd.GetWindowRect(&initLbRect);
			ScreenToClient(initLbRect);
		}
		else
		{

		}
		if(IsWindow((HWND)wParam))
		{		
			m_hSelectedWnd = (HWND)wParam;
			::GetClassName(m_hSelectedWnd,cls,sizeof(cls));
			::GetWindowTextW(m_hSelectedWnd,text,sizeof(text));
			m_lbSelectWnd.MoveWindow(initLbRect);	
			m_lbSelectWnd.ShowWindow(TRUE);
			SetDlgItemText(IDC_STATIC_SelectedWnd,text);
			TCHAR PassWord[255];
			int iLength = ::SendMessage(m_hSelectedWnd, WM_GETTEXTLENGTH, 0, 0) ;
			::SendMessage(m_hSelectedWnd, WM_GETTEXT, (WPARAM)(iLength + 1), (LPARAM)PassWord) ;
			CString  s;
			CString stext(text);
			if(CString(PassWord).IsEmpty())
				stext+=L"_Pwd:"+ CString(PassWord);
			if(stext.IsEmpty() )
				s.Format(L"无标题[%s]",cls);
			else
				s.Format(L"%s[%s]",text,cls);
			SetDlgItemText(IDC_STATIC_SelectedWnd, 	s);
		}
		else
		{
			m_hSelectedWnd = NULL;
			SetDlgItemText(IDC_STATIC_SelectedWnd,L"正在抓取窗口...");		
		}


		RedrawWindow();
		break;
	case  MSG_SLECTEDWND:

		/*if(!m_btnSelectWndOk.IsWindowVisible())
		{
		m_btnSelectWndOk.ShowWindow(TRUE);
		m_btnSelectWndCancel.ShowWindow(TRUE);		 
		CRect rc;	
		m_listHideWndCls.GetWindowRect(&rc);	
		ScreenToClient(rc);
		m_btnSelectWndOk.MoveWindow(rc.right-120,rc.top-20,50,20);
		m_btnSelectWndCancel.MoveWindow(rc.right-50,rc.top-20,50,20);
		m_lbSelectWnd.MoveWindow(rc.left,rc.top,300, 20);
		rc.top+=20;
		m_listHideWndCls.MoveWindow(rc);
		}*/
		if((HWND)wParam != NULL)
		{
			m_hSelectedWnd = (HWND)wParam;
			CRect rc;	
			m_listHideWndCls.GetWindowRect(&rc);
			ScreenToClient(rc);
			m_lbSelectWnd.ShowWindow(TRUE);
			m_btnSelectWndOk.ShowWindow(TRUE);
			m_btnSelectWndCancel.ShowWindow(TRUE);
			m_lbSelectWnd.MoveWindow(rc.left,rc.top-20,200,20);
			m_btnSelectWndOk.MoveWindow(rc.right-98,rc.top-5,45,20);
			m_btnSelectWndCancel.MoveWindow(rc.right-50,rc.top-5,45,20);
			m_lbSelectWnd.MoveWindow(rc.left,rc.top,300, 20);

			m_listHideWndCls.MoveWindow(m_list2Rect.left,m_list2Rect.top + 20,m_list2Rect.right - m_list2Rect.left,m_list2Rect.bottom - m_list2Rect.top-20,TRUE);
		}
		else
			SetDlgItemText(IDC_STATIC_SelectedWnd,L"");	
		RedrawWindow();
		break;

	case ListBoxCommonDelete:
		HideWndInfo* pInfo = (HideWndInfo*)m_listHideWndCls.GetItemDataPtr((UINT)wParam);
		delete pInfo;
		break;
	}
	return CDialogEx::OnWndMsg(message, wParam, lParam, pResult);
}


void CDlgMainPan::OnBnClickedBtnSelectwndok()
{
	TCHAR title[MAX_PATH],file[MAX_PATH],cls[MAX_PATH];
	m_lbSelectWnd.GetWindowTextW(title,sizeof(title)); 
	::GetClassName(m_hSelectedWnd,cls,sizeof(cls));
	DWORD proId=0;
	HANDLE hProcess=NULL;
	GetWindowThreadProcessId(m_hSelectedWnd, &proId);
	hProcess = OpenProcess(PROCESS_ALL_ACCESS,FALSE, proId);
	HMODULE hModule=0;
	::GetModuleFileNameEx(hProcess, hModule,file, sizeof(file));
	CloseHandle(hProcess);	
	HideWndInfo *h  = new HideWndInfo ;
	h->sTitle = title;
	h->clsName = cls;
	h->sFileName = file;
	AddListWndClass(h);
	OnBnClickedBtnSelectwndcancle();
	// TODO: 在此添加控件通知处理程序代码
}


void CDlgMainPan::OnBnClickedBtnSelectwndcancle()
{
	m_hSelectedWnd = NULL;
	SetDlgItemText(IDC_STATIC_SelectedWnd,L"");			 
	//rc.bottom+=20;
	m_listHideWndCls.MoveWindow(m_list2Rect);
	m_btnSelectWndOk.ShowWindow(FALSE);
	m_btnSelectWndCancel.ShowWindow(FALSE);		
	m_lbSelectWnd.ShowWindow(FALSE);
}


void CDlgMainPan::HideWnd()
{
	CDWordArray pIdArr;
	TCHAR s[256];
	static BOOL bHide = TRUE;
	static vector<HWND> hiededWnds;
	vector<HWND> wnds;
	if(bHide)
	{
		hiededWnds.clear();
		if(m_bHideByProcess)
		{
			for (int i = 0; i < m_listProgram.GetCount(); i++)
			{
				CString s1 = m_listProgram.GetItemText(i);				
				s1=s1.Mid(s1.ReverseFind( L'\\ ')+1); 
				 CDWordArray pIdTempArr;
				CProcessMgr::GetProcessIdByText(pIdTempArr, s1,true);
				for (int j = 0; j < pIdTempArr.GetCount(); j++)
				{
					wnds= CProcessMgr::EnumWnds(pIdTempArr.GetAt(j));
					vector<HWND>::iterator w;  
					for (w = wnds.begin(); w != wnds.end();w++)  
					{  
						if(::IsWindowVisible(*w))
						{
						//	::GetWindowText(*w,s, 200);
							::ShowWindow(*w,SW_HIDE);
							hiededWnds.push_back(*w);
						}							
					}
					
				}

			}
		}

		if(m_bHideByWndText)
		{	 
			TCHAR winTitles[1024];
			GetDlgItemText(IDC_EDIT_WinText,winTitles,sizeof(winTitles));
			CStringArray winTitileArr;
			CString s =CString(winTitles);
			s.Replace(L"；",L";");
			Split(s , winTitileArr, ';');
		    if(winTitileArr.GetCount()>0)
			{	
				CProcessMgr::GetProcessIdByText(pIdArr,L"");
			//	if(CProcessMgr::GetProcessIdByText(pIdArr,stitle)>0)
				{
					for (int j=0; j<pIdArr.GetCount();j++)
					{	
						wnds= CProcessMgr::EnumWnds(pIdArr.GetAt(j),&winTitileArr,false);
					//	for (int i = 0; i < winTitileArr.GetCount(); i++)
					{
					//	CString stitle =  winTitileArr.GetAt(i);
					//	HWND hhh=::FindWindowEx(0,hhh, NULL,L"超级");
					//	stitle.Trim();
					
						vector<HWND>::iterator w;  
						for (w = wnds.begin(); w != wnds.end();w++)  
						{  
					        if(!::IsWindowVisible(*w))
								continue;
							::ShowWindow(*w,SW_HIDE);
							hiededWnds.push_back(*w);
						//	TCHAR tmp[1024];
						//	::GetWindowText(*w,tmp,sizeof(tmp));						  
						//	if(CString(tmp).MakeLower().Find(stitle.MakeLower())>=0)							 
						////	if(CString(tmp).CompareNoCase(stitle)>0)
						//	{ 
						//		if(*w  !=GetDlgItem(IDC_EDIT_WinText)->m_hWnd)
						//		{  
						//			::ShowWindow(*w,SW_HIDE);
						//			hiededWnds.push_back(*w);
						//			 break;
						//		}	
						//	}
							 
						}
					}

					}
				}
			}		

		}
		

		if(m_bHideByWndCls)
		{
			for (int i = 0; i < m_listHideWndCls.GetCount(); i++)
			{
				HideWndInfo *p = (HideWndInfo*)m_listHideWndCls.GetItemDataPtr(i);
				HWND hwnd = ::FindWindow(p->clsName,NULL);
				
				// 遍历子窗体
				CDWordArray allPro;
				CProcessMgr::GetProcessIdByText(allPro,L"");
				for (int j = 0; j < allPro.GetCount(); j++)
				{
					TCHAR fileName[MAX_PATH];
					//CString fileName = CProcessMgr::GetProcessFileName(allPro.GetAt(j));	
					HANDLE hProcess=NULL; 
					hProcess = OpenProcess(PROCESS_ALL_ACCESS,FALSE, allPro.GetAt(j));
					if(hProcess == NULL)
						continue;
					HMODULE hModule=0;
					GetModuleFileNameEx(hProcess, hModule,(LPWSTR)fileName, sizeof(fileName));
					CloseHandle(hProcess);

					if( StrCmp(fileName , p->sFileName)==0)
					{
						vector<HWND> tmpwnds = CProcessMgr::EnumWnds(allPro.GetAt(j));
						vector<HWND>::iterator w;  
						for (w = tmpwnds.begin(); w != tmpwnds.end();w++)  
						{  
							TCHAR stmp[1024];
							GetClassName(*w, stmp, sizeof(stmp));
							if(StrCmp(p->clsName, stmp)==0)
							{
								//::GetWindowText(*w,s, 200);
								if(::IsWindowVisible(*w))
								{
									::ShowWindow(*w,SW_HIDE);
									hiededWnds.push_back(*w);
								}	
							}
						}
						
					}
				}



				/*CString s1 = CProcessMgr::GetProcessFileName(hwnd);
				if(hwnd != NULL)
				{	::GetWindowText(hwnd,s, 200);
					if(StrCmp(s1 , p->sFileName)==0)
					{
						if(::IsWindowVisible(hwnd))
						{
							::ShowWindow(hwnd,SW_HIDE);
							hiededWnds.push_back(hwnd);
						}		
					}				
				}*/
			}
		}

		SetDlgItemText(IDC_BUTTON_HideWnd, L"显示");
	}
	else
	{
		vector<HWND>::iterator w;  
		for (w = hiededWnds.begin(); w != hiededWnds.end();w++)  
		{  
			if(!::IsWindowVisible(*w))
			{
				::ShowWindow(*w,SW_SHOW);				 
			}			
		}
		hiededWnds.clear(); 
		SetDlgItemText(IDC_BUTTON_HideWnd, L"隐藏");
		BringWindowToTop();
	}

	bHide = !bHide;

}


void CDlgMainPan::OnBnClickedCheckHidepro()
{
	UpdateData(TRUE);
	SetStatus();
}


void CDlgMainPan::OnBnClickedCheckHideseletedwnds()
{
	UpdateData(TRUE);
	SetStatus();
}


void CDlgMainPan::OnBnClickedCheckHidecontainstext()
{
	UpdateData(TRUE);
	SetStatus();
}

void CDlgMainPan::OnBnClickedBtnHotkey()
{
	 CDlgBossKey d ; 
	 d.m_sHotKey = theApp.m_config.m_sHotKeyHideWnd;
	if(d.DoModal() == IDOK)
	{
		if(theApp.m_config.m_sHotKeyHideWnd != d.m_sHotKey)
		{
			UnregisterHotKey(theApp.GetMainWnd()->m_hWnd,theApp.m_hotKeyIdHideWnd);
			theApp.m_config.m_sHotKeyHideWnd = d.m_sHotKey;
			bool b = CKeyHelper::RegisetHotkey(theApp.GetMainWnd()->m_hWnd,theApp.m_hotKeyIdHideWnd,d.m_sHotKey);
			if(!b)
				MessageBox(L"热键"+theApp.m_config.m_sHotKeyHideWnd+"注册失败，可能与其它热键冲突",L"Error");
			SetDlgItemText(IDC_STATIC_HotKey,theApp.m_config.m_sHotKeyHideWnd);
			Invalidate();
		}
	}
}


void CDlgMainPan::SetStatus()
{
	if(m_bHideByProcess)
	{
		m_listProgram.EnableWindow(TRUE);
		m_listProgram.Invalidate();
		m_listProgram.UpdateWindow();
	}
	else
		m_listProgram.EnableWindow(FALSE);

	if(m_bHideByWndCls)
		m_listHideWndCls.EnableWindow(TRUE);
	else
		m_listHideWndCls.EnableWindow(FALSE);

	if(m_bHideByWndText)
		::EnableWindow(GetDlgItem(IDC_EDIT_WinText)->m_hWnd,TRUE);
	else
		::EnableWindow(GetDlgItem(IDC_EDIT_WinText)->m_hWnd,FALSE);

	
}



void CDlgMainPan::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	CDialogEx::OnKeyDown(nChar, nRepCnt, nFlags);
}
