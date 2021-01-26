
// LaoziDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "Laozi.h"
#include "LaoziDlg.h"
#include "afxdialogex.h"
#include "ProcessMgr.h"
#include <vector>
#include <afxcoll.h>
#include "MyString.h"
#include "KeyHelper.h"
#include <GdiplusGraphics.h>
#pragma comment(lib,"gdiplus.lib")
#include "DlgPopBase.h"
#include "DlgHelp.h"
#include "Resource.h"
using namespace std;
using namespace Gdiplus;
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

typedef void (* INHook)(HWND hWnd); 
typedef void (* INHook1)(); 
typedef void (* UNHook)(); 
INHook DllInstallHook = NULL;
UNHook DllUNInstallHook = NULL;
// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDlgPopBase
{
public:
	CAboutDlg(CWnd* pParent /*=NULL*/);

	// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	// 实现
protected:
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg LRESULT OnNcHitTest(CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnBnClickedButtonAboutclose();
	afx_msg void OnBnClickedButtonAboutcopy();
	afx_msg void OnBnClickedMfclink2();
};

CAboutDlg::CAboutDlg(CWnd* pParent /*=NULL*/)
	: CDlgPopBase(CAboutDlg::IDD, pParent)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDlgPopBase::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BUTTON_AboutCLose, m_btnClose);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_CTLCOLOR()
	ON_WM_NCHITTEST()
	ON_WM_LBUTTONUP()
	ON_BN_CLICKED(IDC_BUTTON_AboutCLose, &CDlgPopBase::OnBnClickedButtonClose)
	ON_BN_CLICKED(IDC_BUTTON_AboutCopy, &CAboutDlg::OnBnClickedButtonAboutcopy)
	ON_BN_CLICKED(IDC_MFCLINK2, &CAboutDlg::OnBnClickedMfclink2)
END_MESSAGE_MAP()


// CLaoziDlg 对话框




CLaoziDlg::CLaoziDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CLaoziDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_bShowTaskbar = FALSE;
	m_brushBkrect = ::CreateSolidBrush(RGB(38,59,90));
	m_brushBkrectMid = ::CreateSolidBrush(RGB(221,228,228));
	 m_CurSel = 0;
	 m_pDlgMain = NULL;
	 m_pDlgWatch = NULL;	
}
CLaoziDlg::~CLaoziDlg()
{
	
	if(m_pDlgMain)
	{
		delete 	 m_pDlgMain;
		m_pDlgMain = NULL;		
	}
	if(m_pDlgWatch)
	{
		delete m_pDlgWatch;
		m_pDlgWatch = NULL;
	}

	GdiplusShutdown(m_nGdiplusToken);
	
}
void CLaoziDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BUTTON__CONFIG, m_ConfBtn);
	DDX_Control(pDX, IDC_BUTTON_MIN, m_MinBtn);
	DDX_Control(pDX, IDC_BUTTON_CLOSE, m_CloseBtn);
	DDX_Control(pDX, IDC_BUTTON_MAIN_PAN, m_MainPanBtn);
	DDX_Control(pDX, IDC_BUTTON_MAIN_PAN2, m_MainPanBtn1);
	DDX_Control(pDX, IDC_BUTTON_MAIN_PAN3, m_MainPanBtn2);
	DDX_Control(pDX, IDC_BUTTON_MAIN_PAN4, m_MainPanBtn3);
	DDX_Control(pDX, IDC_BUTTON_MAIN_PAN5, m_MainPanBtn4);
}

BEGIN_MESSAGE_MAP(CLaoziDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDCANCEL, &CLaoziDlg::OnBnClickedCancel)
	ON_MESSAGE(WM_HOTKEY,OnHotKey) 
	ON_WM_CLOSE(OnClose) 
	ON_WM_CREATE()
	ON_COMMAND(ID_MainShowHide, &CLaoziDlg::OnMainShowHide)
	ON_COMMAND(ID_SysExit, &CLaoziDlg::OnSysExit)
	ON_BN_CLICKED(IDC_BUTTON_MAIN_PAN, &CLaoziDlg::OnBnClickedButtonMainPan)
	ON_WM_NCHITTEST()
	ON_BN_CLICKED(IDC_BUTTON_MIN, &CLaoziDlg::OnBnClickedButtonMin)
	ON_BN_CLICKED(IDC_BUTTON_CLOSE, &CLaoziDlg::OnBnClickedButtonClose)
	ON_BN_CLICKED(IDC_BUTTON__CONFIG, &CLaoziDlg::OnBnClickedConfig)
	ON_BN_CLICKED(IDC_BUTTON_MAIN_PAN2, &CLaoziDlg::OnBnClickedButtonMainPan2)
	ON_BN_CLICKED(IDC_BUTTON_MAIN_PAN3, &CLaoziDlg::OnBnClickedButtonMainPan3)
	ON_BN_CLICKED(IDC_BUTTON_MAIN_PAN4, &CLaoziDlg::OnBnClickedButtonMainPan4)
	ON_BN_CLICKED(IDC_BUTTON_MAIN_PAN5, &CLaoziDlg::OnBnClickedButtonMainPan5)
	ON_COMMAND(IDM_ABOUTBOX, &CLaoziDlg::OnAboutbox)
	ON_COMMAND(ID_MENU_Regist, &CLaoziDlg::OnMenuRegist)
	//ON_MESSAGE(WM_MYHOOK_RMOVE,OnRMove)
	//ON_MESSAGE(WM_MYHOOK_RPRESS,OnRPress)

	ON_COMMAND(ID_MenuHelp, &CLaoziDlg::OnMenuhelp)
END_MESSAGE_MAP()


// CLaoziDlg 消息处理程序
BOOL CLaoziDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}
	
	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	
	 EnableToolTips(TRUE);
	 m_tt.Create(this);
	 m_tt.Activate(TRUE);
	//  m_tt.AddTool(m_,L"Check1lakjsfasfdasfd");
	ModifyStyle(0,WS_SYSMENU|WS_MINIMIZEBOX,0);

	CWnd * pwnd=GetDlgItem(IDD_LAOZI_DIALOG); 

	// m_hook.startMousehook(pwnd->GetSafeHwnd());

	SetDlgItemText(IDD_LAOZI_DIALOG,L"超级助手");


	HANDLE hMutex;
	//TCHAR tCurPath[MAX_PATH];
	//TCHAR szDLL[4096];
	//TCHAR szDate[MAX_PATH];
	HINSTANCE hModule;
	BOOL bError = FALSE;

	//hMutex = ::CreateMutex(NULL, FALSE, dllMutex.c_str());
	int argc(0);
	LPWSTR *argv=::CommandLineToArgvW(::GetCommandLineW(),&argc);   
	bool bDebug = false;
	bool bAuto=false;
	for (int i = 0; i < argc; i++)
	{
		if (StrCmp(argv[i],L"auto")==0)
		{
			bAuto=true;
		
		}
		else if(StrCmp(argv[i], L"debug")==0)
		{
			bDebug = true;
		}
	}  
	 if(!bDebug)
	{
		//_tcscpy(szDLL, theApp.m_modulePath);
		CString szDLL = theApp.m_modulePath+L"\\MyHookDll.dll";
		//_stprintf(szDLL,_T("%s\\MyHookDll.dll"), theApp.m_modulePath);
		// PathAppend(szDLL,L"MyHookDll.dll");
		hModule = ::LoadLibrary(szDLL);

		if (hModule != NULL)
		{	
			DllUNInstallHook = (UNHook)GetProcAddress(hModule, "UnInstallHook");
			DllInstallHook = (INHook)GetProcAddress(hModule, "InstallHook");
			
		}
		
		if(DllInstallHook!=NULL)
			DllInstallHook(m_hWnd); 
	 }
	 RegeistHKey();

	 CenterWindow();
	 GetClientRect(&m_rcClient);
	/*SetBackImage(0,_T("Image\\顶部背景.bmp"));
	SetBackImage(1,_T("Image\\状态.bmp"));
	SetBackImage(2,_T("image\\button_begin.BMP"));*/
	 SetBackImage(0,IDB_BITMAP_TopBack);
	 SetBackImage(1,IDB_BITMAP_Status);
	 SetBackImage(2,IDB_BITMAP_ButtonBegin);

	// 右上角的三个按妞
	m_ConfBtn.SetImage(IDB_BITMAP_Hide,IDB_BITMAP_Hide1,IDB_BITMAP_Hide2,IDB_BITMAP_Hide);
	CRect tempUp = m_rcClient;
	tempUp.left = m_rcClient.right - (32 + 32 +32  + 10);//21
	tempUp.right = tempUp.left + 32 ;//21
	tempUp.top = m_rcClient.top + 5;
	tempUp.bottom = m_rcClient.top + 24 ;
	m_ConfBtn.SetWindowText(_T(""));
	m_ConfBtn.MoveWindow(tempUp,true);
	m_MinBtn.SetImage(IDB_BITMAP_Minimize,IDB_BITMAP_Minimize1,IDB_BITMAP_Minimize2,IDB_BITMAP_Minimize);
	tempUp.left = m_rcClient.right - (32 + 32 + 10);//21
	tempUp.right = tempUp.left  +  32 ;//21
	tempUp.top = m_rcClient.top + 5;
	tempUp.bottom = m_rcClient.top + 24 ;
	m_MinBtn.SetWindowText(_T(""));
	m_MinBtn.MoveWindow(tempUp,true);

m_CloseBtn.SetImage(IDB_BITMAP_CloseWnd,IDB_BITMAP_CloseWnd1,IDB_BITMAP_CloseWnd2,IDB_BITMAP_CloseWnd);
	tempUp.left = m_rcClient.right - (32 + 10);//21
	tempUp.right = tempUp.left  +  32 ;//21
	tempUp.top = m_rcClient.top + 5;
	tempUp.bottom = m_rcClient.top + 24 ;
	m_CloseBtn.SetWindowText(_T(""));
	m_CloseBtn.MoveWindow(tempUp,true);

	m_MainPanBtn.SetImage(IDB_BITMAP_Menu_1_2,IDB_BITMAP_Menu_1_1,IDB_BITMAP_Menu_1_0,IDB_BITMAP_Menu_1_0);
	m_MainPanBtn.SetWindowText(L"");
	m_MainPanBtn1.SetImage(IDB_BITMAP_Menu_2_2,IDB_BITMAP_Menu_2_1,IDB_BITMAP_Menu_2_0,IDB_BITMAP_Menu_2_0);
	m_MainPanBtn1.SetWindowText(L"");
	m_MainPanBtn2.SetImage(IDB_BITMAP_Menu_3_2,IDB_BITMAP_Menu_3_1,IDB_BITMAP_Menu_3_0,IDB_BITMAP_Menu_3_0);
	m_MainPanBtn2.SetWindowText(L"");
	m_MainPanBtn3.SetImage(IDB_BITMAP_Menu_4_2,IDB_BITMAP_Menu_4_1,IDB_BITMAP_Menu_4_0,IDB_BITMAP_Menu_4_0);
	m_MainPanBtn3.SetWindowText(L"");
	m_MainPanBtn4.SetImage(IDB_BITMAP_Menu_5_2,IDB_BITMAP_Menu_5_1,IDB_BITMAP_Menu_5_0,IDB_BITMAP_Menu_5_0);
	m_MainPanBtn4.SetWindowText(L"");

	CSkinBtn* pBtnall[5]={&m_MainPanBtn,&m_MainPanBtn1,&m_MainPanBtn2,&m_MainPanBtn3,&m_MainPanBtn4};

	CRect rcBtn;
	memset(rcBtn,0x00,sizeof(rcBtn));
	for( int i = 0; i< 5; i++)
	{
		rcBtn.left  = m_rcClient.left + i*79 + (i + 1)*10;
		rcBtn.right =   rcBtn.left + 79;
		rcBtn.top    = 25;
		rcBtn.bottom    = rcBtn.top + 85;
		pBtnall[i]->MoveWindow(rcBtn,FALSE);
	}
	m_MainPanBtn.SetCurrentShow(1);
	m_CurSel = 0;
	OnChangeSelDlg();



	/*Weather_Info wi;
	SearchInfo si;
	char *html=NULL; 
	strcpy(wi.city,"nanjing");
	if(wea.initsocket())
		if(	wea.initargs(si,wi.city))
			html = wea.GetFile(&si);

	wea.GetWeather(wi,html);*/
	GdiplusStartupInput m_gdiInput;

	GdiplusStartup(&m_nGdiplusToken,&m_gdiInput,NULL);

	CString city = theApp.m_config.m_sCity;
	//m_weather.ThreadGet(city);
	CRect rt1(0,0,0,0);
	
	m_mouseSignal.m_sigalInfo.Create(IDD_DIALOG_MouseSignalInfo,0);

	CoInitializeEx(NULL, COINIT_MULTITHREADED);
	if(bAuto)
	{
		SetWindowPos(&CWnd::wndNoTopMost,0,0,0,0,SWP_HIDEWINDOW); 
		ModifyStyleEx(WS_EX_APPWINDOW,WS_EX_TOOLWINDOW);
		ShowWindow(SW_HIDE);
	}

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}
int CLaoziDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialogEx::OnCreate(lpCreateStruct) == -1)
		return -1;
	SetWindowText(L"超级助手");
	m_winMgr.NotifyIcon(this->m_hWnd);
	m_winMgr.SetupRegRun(L"auto");
	return 0;
}
void CLaoziDlg::SetBackImage(int index,UINT id)
{
	m_BitmapBack[index].LoadBitmap(id);
//	SetImage(m_BitmapBack[index], strName);
	SetImageSize(&m_BitmapBack[index], m_szBitmapBack[index]);
}

void CLaoziDlg::SaveCfg()
{	 
	if(m_pDlgMain!=NULL)	
	{
		m_pDlgMain->GetCfg(theApp.m_config.m_guardInfo);		
	}
	if (m_pDlgWatch != NULL)
	{
		m_pDlgWatch->GetCfg(theApp.m_config.m_watchInfo);
	}
	theApp.m_config.Save();
}

//载入硬盘文件位图
void CLaoziDlg::SetImage(CBitmap &bitmap, UINT id /*CString strName*/)
{
	HBITMAP hBitmap = NULL;
	//strName = "..\\Image\\顶部背景.bmp";
    
	//hBitmap = (HBITMAP)::LoadImage(NULL, strName, IMAGE_BITMAP, 0, 0, LR_DEFAULTSIZE|LR_LOADFROMFILE);
	//ASSERT(hBitmap);

	if(hBitmap == NULL)
	{
		CString str;
		//AfxMessageBox(_T("Not Open SetImage Bitmap"));
		//PostQuitMessage(0);
	}

	if (bitmap.m_hObject)
		bitmap.Detach();

	bitmap.Attach(hBitmap);
}

//保存位图尺寸
void CLaoziDlg::SetImageSize(CBitmap *pBitmap, CSize &sz)
{
	BITMAP bm;
	pBitmap->GetBitmap(&bm);

	sz = CSize(bm.bmWidth, bm.bmHeight);
}
void CLaoziDlg::RegeistHKey()
{	
	try
	{
		bool b = CKeyHelper::RegisetHotkey(m_hWnd, theApp.m_hotKeyIdHideWnd, theApp.m_config.m_sHotKeyHideWnd);
		if(!b)
			MessageBox(L"热键"+theApp.m_config.m_sHotKeyHideWnd+"注册失败，可能与其它热键冲突",L"Error");
		HotKeyId2 = GlobalAddAtom(L"HotKeyId2") - 0xC000;;
		HotKeyId = GlobalAddAtom(L"HotKeyId") - 0xC000;;
		//b=RegisterHotKey(m_hWnd, HotKeyId2,0,VK_F8);
		//b=RegisterHotKey(m_hWnd, HotKeyId,0,VK_F9);
		int a = 0;
	}
	catch (CException* e)
	{
	}
	
}

LRESULT CLaoziDlg::OnHotKey(WPARAM wParam, LPARAM lParam)
{ 
	if(wParam == theApp.m_hotKeyIdHideWnd)
	{	
	//	m_bShowTaskbar = !m_bShowTaskbar;
	//	m_winMgr.ShowHideTaskBar(m_bShowTaskbar);
		m_pDlgMain->HideWnd();
	}
	else if(wParam == HotKeyId)
	{
		CString ss ;
		m_pDlgWatch-> GetDlgItemText(IDC_EDIT2,ss);
		ss= L"xyz1a";
		ss.MakeUpper();
		UnregisterHotKey(m_hWnd, HotKeyId);
		for (int i=0;i<ss.GetLength();i++)
		{
			WCHAR c = (WCHAR)ss.GetAt(i);
			Sleep(10);
			keybd_event(c,0,0,0);
		} 
		RegisterHotKey(m_hWnd, HotKeyId,0,VK_F9);
	}
	else if(wParam == HotKeyId2)
	{
		CString ss ;
		m_pDlgWatch-> GetDlgItemText(IDC_EDIT1,ss);
		ss = L"12345678";
		UnregisterHotKey(m_hWnd, HotKeyId2);
     	for (int i=0;i<ss.GetLength();i++)
		{
			keybd_event(ss.GetAt(i),0,0,0);
			Sleep(2);
		}
		Sleep(500);
		//keybd_event(VK_RETURN,0,0,0);
		//
		RegisterHotKey(m_hWnd, HotKeyId2,0,VK_F8);
		return 0;
		// m_ProcessMgr.ProcessList();
		/*int i=0;
		static bool bshow=true;
		CWnd   *pPrevHwnd = CWnd::GetDesktopWindow()-> GetWindow(GW_CHILD); 
		while(*pPrevHwnd)
		{ 
			CString str;
			i++;
			 
			str = CProcessMgr::GetProcessFileName(pPrevHwnd->m_hWnd);
			if(::IsWindowVisible(pPrevHwnd->m_hWnd))
			{
				
			}
			pPrevHwnd=pPrevHwnd-> GetWindow(GW_HWNDNEXT); 
		}*/
		CDWordArray pIdArr;
		TCHAR s[256];
		static BOOL bHide = TRUE;
		static vector<HWND> hiededWnds;
		if(bHide)
		{
			if(CProcessMgr::GetProcessIdByText(pIdArr, L"qq")>0)
			{
				for (int i=0; i<pIdArr.GetCount();i++)
				{
					vector<HWND> wnds= CProcessMgr::EnumWnds(pIdArr.GetAt(i));
					vector<HWND>::iterator w;  
					for (w = wnds.begin(); w != wnds.end();w++)  
					{  
						::GetWindowText(*w,s,256);
						if(::IsWindowVisible(*w))
						{
							::ShowWindow(*w,SW_HIDE);
							hiededWnds.push_back(*w);
						}							
					}
				}
			}
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
		}
		bHide = !bHide;
	}
	return 0; 
} 
void CLaoziDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout(this);
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

void CLaoziDlg::OnPaint()
{   //ShowWindow(SW_HIDE);
	//if (IsIconic())
	//{
	//	CPaintDC dc(this); // 用于绘制的设备上下文

	//	SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

	//	// 使图标在工作区矩形中居中
	//	int cxIcon = GetSystemMetrics(SM_CXICON);
	//	int cyIcon = GetSystemMetrics(SM_CYICON);
	//	CRect rect;
	//	GetClientRect(&rect);
	//	int x = (rect.Width() - cxIcon + 1) / 2;
	//	int y = (rect.Height() - cyIcon + 1) / 2;

	//	// 绘制图标
	//	dc.DrawIcon(x, y, m_hIcon);
	//}
	//else
	//{
	//	CDialogEx::OnPaint();
	//}


	CPaintDC dc(this); // device context for painting

	CRect rcClient;
	GetClientRect(&rcClient);


	//内存画图//////////////////////////
	CDC szMemDC;
	szMemDC.CreateCompatibleDC(&dc);

	CBitmap btScreen;
	btScreen.CreateCompatibleBitmap(&dc, rcClient.Width(), rcClient.Height());

	szMemDC.SelectObject(&btScreen);
	btScreen.DeleteObject();


	HGDIOBJ oldBrush=szMemDC.SelectObject(m_brushBkrect);
	szMemDC.Rectangle(rcClient);
	szMemDC.SelectObject(oldBrush);


	CRect rcMid = rcClient;
	rcMid.left = rcClient.left + 10;
	rcMid.top = rcClient.top + 140;
	rcMid.bottom = rcClient.bottom -25;
	rcMid.right = rcClient.right - 10;


	oldBrush=szMemDC.SelectObject(m_brushBkrectMid);
	szMemDC.Rectangle(rcMid);
	szMemDC.SelectObject(oldBrush);


	//头部主图背景
	CRect rcTop = rcClient;
	rcTop.bottom =   100;
	DrawRangeImage(&m_BitmapBack[0],&szMemDC,rcTop);

	//底部主图背景
	CRect rcBottom = rcClient;
	rcBottom.top =   rcClient.bottom - 30;
	DrawRangeImage(&m_BitmapBack[1],&szMemDC,rcBottom);
	szMemDC.SetTextColor(RGB(225,225,225));
	szMemDC.SetBkMode(TRANSPARENT);
	//szMemDC.TextOut(10,rcClient.bottom-23,m_strVersion);


	//szMemDC.TextOut(300,rcClient.bottom-23,m_strscroll);


	////当前登陆状态提示
	//if(m_strloginStatus.GetLength() > 0)
	//{
	//	CSize   cs=szMemDC.GetTextExtent(m_strloginStatus);//获得字符串长度 

	//	szMemDC.TextOut(rcClient.right - cs.cx-10,rcClient.bottom-23,m_strloginStatus);
	//}

	// 画按妞

	szMemDC.SetTextColor(RGB(255,250,200));
	szMemDC.SetBkMode(TRANSPARENT);
	szMemDC.TextOut(8,5,_T("超级助手"));


	szMemDC.SetTextColor(RGB(187,190,200));
	szMemDC.SetBkMode(TRANSPARENT);

	//CFont font;
	//font.CreatePointFont(90,L"宋体");
	//CFont* pOldFont = szMemDC.SelectObject(&font);
	//szMemDC.SelectObject(&font);
	//szMemDC.TextOut(rcClient.right - 100,rcClient.top + 40,m_weather. m_strInfo[1].m_strNew_yesNoRain);
	//szMemDC.TextOut(rcClient.right - 100,rcClient.top + 60,m_weather. m_strInfo[1].m_strNew_Temperature);
	//szMemDC.TextOut(rcClient.right - 100,rcClient.top + 80,m_weather. m_strInfo[1].m_strNew_Wind);
	//szMemDC.SelectObject(&pOldFont);
	//Graphics graphics( szMemDC.m_hDC);
	//Image image(_T("c:\\new.png"), TRUE);
	//graphics.DrawImage(&image, rcClient.right - 180,rcClient.top + 30);
	//font.DeleteObject();
	//pOldFont->DeleteObject();
	//创建不规则窗体
	//ChangeWindowRgn(&szMemDC);
	//画边框
	CPen pen;	pen.CreatePen(PS_SOLID,1,RGB(100,145,200));
	szMemDC.SelectObject(pen);
	szMemDC.MoveTo(rcClient.left,rcClient.top);
	szMemDC.LineTo(rcClient.left,rcClient.bottom);
	szMemDC.MoveTo(rcClient.right-1,rcClient.top);
	szMemDC.LineTo(rcClient.right-1,rcClient.bottom);
	szMemDC.MoveTo(rcClient.left,rcClient.bottom-1);
	szMemDC.LineTo(rcClient.right,rcClient.bottom-1);

	dc.BitBlt(rcClient.left, rcClient.top, rcClient.Width(), rcClient.Height(), &szMemDC, 0, 0, SRCCOPY);
	dc.BitBlt(rcClient.left, rcClient.top, rcClient.Width(), rcClient.Height(), &szMemDC, 0, 0, SRCCOPY);



	szMemDC.DeleteDC();
}
//平铺画位图
void CLaoziDlg::DrawRangeImage(CBitmap *pBitmap, CDC *pDC, CRect rc)
{
	CDC MemDC;
	BITMAP bm;
	pBitmap->GetBitmap(&bm);

	int li_Width = bm.bmWidth;
	int li_Height = bm.bmHeight;


	MemDC.CreateCompatibleDC(pDC);
	CBitmap* pOldBitmap = MemDC.SelectObject(pBitmap);

	int x=rc.left;
	int y=rc.top;
	while (y < (rc.Height()+rc.top)) 
	{
		while(x < (rc.Width()+rc.left)) 
		{
			pDC->BitBlt(x, y, li_Width, li_Height, &MemDC, 0, 0, SRCCOPY);
			x += li_Width;
		}
		x = rc.left;
		y += li_Height;
	}

	MemDC.SelectObject(pOldBitmap);
	MemDC.DeleteDC();
}

void CLaoziDlg::ChangeWindowRgn(CDC *pDC)
{
	COLORREF col = RGB(255,255,255);
	CRect rcClient;
	GetClientRect (rcClient);
	CRgn rgn;
	rgn.CreateRoundRectRgn (1, 1, rcClient.Width(), rcClient.Height(),6,6);

	SetWindowRgn (rgn, TRUE);
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CLaoziDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);	
}

void CLaoziDlg::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	CDialogEx::OnCancel();
}

void CLaoziDlg::OnClose()
{
	
	ShowWindow(SW_HIDE);
	//ModifyStyleEx(WS_EX_APPWINDOW,WS_EX_TOOLWINDOW);
	// ModifyStyleEx(WS_EX_TOOLWINDOW, WS_EX_APPWINDOW);
	 
}




LRESULT CLaoziDlg::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{ 
	switch(message){    
	case MYWM_NOTIFYICON:      
		//如果是用户定义的消息   
		if(lParam==WM_LBUTTONDBLCLK)   
		{   
			 //鼠标双击时主窗口出现   
			 AfxGetApp()->m_pMainWnd->ShowWindow(SW_HIDE);  
			 OnMainShowHide();
		}   
		else if(lParam==WM_RBUTTONDOWN){      
			 //鼠标右键单击弹出菜单   
			 CMenu menu;   
			  menu.LoadMenu(IDR_MENU1);      
			   //载入事先定义的菜单   
			   CMenu* pMenu=menu.GetSubMenu(0);   
			    CPoint pos;   
				 GetCursorPos(&pos);   
				  SetForegroundWindow(); 
				  pMenu->TrackPopupMenu(TPM_LEFTALIGN|TPM_RIGHTBUTTON,pos.x,pos.y,AfxGetMainWnd());   
		}   
		break;   
	  case WM_SYSCOMMAND:      
		  //如果是系统消息   
		  switch(wParam){  
		  case SC_MINIMIZE:  
			  //接收到最小化消息时主窗口隐藏   
			//  ShowWindow(SW_HIDE);   
			//  return 0;  
			  break;  
		  case SC_CLOSE :  
			 // ::Shell_NotifyIcon(NIM_DELETE,&m_nid); //关闭时删除系统托盘图标  
			  break;  
		  }   
		  break;   
		
	  case WM_HOTKEY:
		 
		  break;
	  case WM_MYHOOK_RMOVE:
		  m_mouseSignal.OnRMove(wParam,lParam);
		  break;
	  case WM_MYHOOK_RPRESS:		
		  {
		   m_mouseSignal.OnRPress(wParam,lParam);
		  break;
		  }
	  case WM_MYHOOK_XWHEEL:
		  if(lParam==1)
			  keybd_event( VK_PRIOR, 0x45, KEYEVENTF_EXTENDEDKEY | 0, 0 );
		 else
			keybd_event( VK_NEXT, 0x45, KEYEVENTF_EXTENDEDKEY | 0, 0 );
		  break;
	  default:
		  m_MsgProcess.ProcessMsg(message,wParam,lParam);
		  break;
	 }   

	return CDialogEx::WindowProc(message, wParam, lParam);
}


void CLaoziDlg::OnMainShowHide()
{
	RECT rc;GetClientRect(&rc);
	if(!IsWindowVisible() || rc.right == 0)
	{		
		ShowWindow(SW_SHOW);
		SetWindowPos(&CWnd::wndNoTopMost,m_rcClient.left,m_rcClient.top,m_rcClient.right-m_rcClient.left,m_rcClient.bottom-m_rcClient.top,SWP_SHOWWINDOW); 
		
		CenterWindow();ModifyStyleEx(WS_EX_TOOLWINDOW,WS_EX_APPWINDOW);
	}
	else
		ShowWindow(SW_HIDE);
	
}


void CLaoziDlg::OnSysExit()
{	
	UnregisterHotKey(this->m_hWnd, HotKeyId);
	SaveCfg();
	if (m_pDlgMain!=NULL)
	{
		m_pDlgMain->Destroy();
	}
	 CDialog::OnCancel(); 

	// TODO: 在此添加命令处理程序代码
}


LRESULT CLaoziDlg::OnNcHitTest(CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	ScreenToClient(&point);

	RECT rtWindow;
	GetClientRect(&rtWindow);

	long wndHeight = rtWindow.bottom - rtWindow.top;
	long wndWidth = rtWindow.right - rtWindow.left;


	_icur = 0;
	RECT rcW = {0,0,wndWidth,100};
	if(::PtInRect(&rcW,point))
	{  
		return HTCAPTION;  // 在拖动范围内
	}

	return CDialogEx::OnNcHitTest(point);
}


void CLaoziDlg::OnBnClickedButtonMin()
{
	this->ShowWindow(SW_MINIMIZE);
}


void CLaoziDlg::OnBnClickedButtonClose()
{ 
	OnClose();
}

void CLaoziDlg::OnChangeSelDlg()
{
	CSkinBtn* pBtnall[5]={&m_MainPanBtn,&m_MainPanBtn1,&m_MainPanBtn2,&m_MainPanBtn3,&m_MainPanBtn4};
	for(long i = 0; i< 5; i++)
	{
		pBtnall[i]->SetCurrentShow(0);
		pBtnall[i]->Invalidate(FALSE);
	}
	pBtnall[m_CurSel]->SetCurrentShow(1);
	pBtnall[m_CurSel]->Invalidate(FALSE);

	CRect rcClient;
	GetClientRect(&rcClient);
	if(m_pDlgMain)
		m_pDlgMain->ShowWindow(SW_HIDE);
	if(m_pDlgWatch)
		m_pDlgWatch->ShowWindow(SW_HIDE);
	if(m_CurSel == 0)
	{
		if(m_pDlgMain == NULL)
		{
			m_pDlgMain=new CDlgMainPan();
			m_pDlgMain->Create(IDD_Dlg_MainPan,this);			
			m_pDlgMain->MoveWindow(rcClient.left + 2,112 , rcClient.right - rcClient.left  -4, 408);
			m_pDlgMain->MoveWindow(rcClient.left + 1,110 , rcClient.right - rcClient.left  -1, 478);
			//m_pDlgMain->MoveWindow(rcClient.left + 1,25 , rcClient.right - rcClient.left  -2, 478);
		}
		m_pDlgMain->ShowWindow(SW_SHOW);
	}
	else if(m_CurSel == 1)
	{
		if(m_pDlgWatch == NULL)
		{
			m_pDlgWatch=new CDlgFileWatch();
			m_pDlgWatch->Create(IDD_DLG_FileWatcher,this);			
			m_pDlgWatch->MoveWindow(rcClient.left + 2,112 , rcClient.right - rcClient.left  -4, 408);
			m_pDlgWatch->MoveWindow(rcClient.left + 1,110 , rcClient.right - rcClient.left  -1, 408);

		}
		m_pDlgWatch->ShowWindow(SW_SHOW);
	}
}
void CLaoziDlg::OnBnClickedConfig()
{
	CMenu menu;   
	menu.LoadMenu(IDR_MENU_CONFIG);      
	//载入事先定义的菜单   
	CMenu* pMenu=menu.GetSubMenu(0);   
	CPoint pos;   
	GetCursorPos(&pos);   
	SetForegroundWindow(); 
	pMenu->TrackPopupMenu(TPM_LEFTALIGN|TPM_RIGHTBUTTON,pos.x-15,pos.y+10,AfxGetMainWnd());   
}

void CLaoziDlg::OnBnClickedButtonMainPan()
{
	m_CurSel = 0;
	OnChangeSelDlg();
	
}



void CLaoziDlg::OnBnClickedButtonMainPan2()
{
	m_CurSel = 1;
	OnChangeSelDlg();
}


void CLaoziDlg::OnBnClickedButtonMainPan3()
{
	m_CurSel = 2;
	OnChangeSelDlg();
}


void CLaoziDlg::OnBnClickedButtonMainPan4()
{
	m_CurSel = 3;
	OnChangeSelDlg();
}


void CLaoziDlg::OnBnClickedButtonMainPan5()
{
	m_CurSel = 4;
	OnChangeSelDlg();
}


BOOL CAboutDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
	/*if(pMsg->message==WM_KEYDOWN&&pMsg->wParam==VK_RETURN)    
	return TRUE; 
	if(pMsg->message==WM_KEYDOWN&&pMsg->wParam==VK_ESCAPE)    
	return TRUE; */
	return CDlgPopBase::PreTranslateMessage(pMsg); 
}


BOOL CLaoziDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
	if(pMsg->message==WM_KEYDOWN&&pMsg->wParam==VK_RETURN)    
		return TRUE; 
	if(pMsg->message==WM_KEYDOWN&&pMsg->wParam==VK_ESCAPE)    
		return TRUE; 
	return CDialogEx::PreTranslateMessage(pMsg);
}


BOOL CAboutDlg::OnInitDialog()
{
	CDlgPopBase::OnInitDialog();
	SetWindowText(L"关于");
	// TODO:  在此添加额外的初始化

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void CAboutDlg::OnPaint()
{
	//CPaintDC dc(this); // device context for painting
	// TODO: 在此处添加消息处理程序代码
	// 不为绘图消息调用 
	CDlgPopBase::OnPaint();
}

void CLaoziDlg::OnAboutbox()
{
	CAboutDlg dlgAbout(this);
	//dlgAbout.Create(IDD_DIALOG2,this);
	dlgAbout.DoModal();
	
}



HBRUSH CAboutDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDlgPopBase::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  在此更改 DC 的任何特性

	// TODO:  如果默认的不是所需画笔，则返回另一个画笔
	return hbr;
}


LRESULT CAboutDlg::OnNcHitTest(CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	return CDlgPopBase::OnNcHitTest(point);
}


void CAboutDlg::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	CDlgPopBase::OnLButtonUp(nFlags, point);
}


void CAboutDlg::OnBnClickedButtonAboutclose()
{
	 OnBnClickedButtonClose();
}


void CLaoziDlg::OnMenuRegist()
{
	MessageBox(L"软件已经是注册版本",L"注册");
}


void CLaoziDlg::OnMenuhelp()
{
	 CDlgHelp dlg;
	 dlg.DoModal();
}


void CAboutDlg::OnBnClickedButtonAboutcopy()
{
	// TODO: 在此添加控件通知处理程序代码
	CopyToClipboard("ajiework@163.com",50);
	MessageBox(L"已复制“ajiework@163.com”到剪贴板");
}


void CAboutDlg::OnBnClickedMfclink2()
{
	CopyToClipboard("419512058",50);
	MessageBox(L"已复制“419512058”到剪贴板");
}
