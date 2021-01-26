// DlgPopBase.cpp : 实现文件
//

#include "stdafx.h"
#include "DlgPopBase.h" 
//#define IDC_POP_BUTTON_Close 6151
#include "Resource.h"
// CDlgPopBase 对话框

IMPLEMENT_DYNAMIC(CDlgPopBase, CDialogEx)

CDlgPopBase::CDlgPopBase(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgPopBase::IDD, pParent)
{

}
CDlgPopBase::CDlgPopBase(UINT nIDTemplate,CWnd* pParent /*=NULL*/)
	: CDialogEx(nIDTemplate, pParent)
{

}

CDlgPopBase::~CDlgPopBase()
{
}

void CDlgPopBase::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
//	DDX_Control(pDX, IDC_POPBase_BUTTON_CLOSE, m_btnClose);

//	DDX_Control(pDX, IDC_BUTTON_asdfas, m_btnClose);
}


BEGIN_MESSAGE_MAP(CDlgPopBase, CDialogEx)
	ON_WM_PAINT()
	ON_WM_CTLCOLOR()
	ON_WM_NCHITTEST()
	//ON_BN_CLICKED(IDC_POP_BUTTON_Close, &CDlgPopBase::OnBnCloseClicked)
	ON_WM_LBUTTONUP()
	ON_BN_CLICKED(IDC_POPBase_BUTTON_CLOSE, &CDlgPopBase::OnBnClickedButtonClose)
//	ON_BN_CLICKED(IDC_BUTTON_asdfas, &CDlgPopBase::OnBnClickedButtonasdfas)
END_MESSAGE_MAP()


// CDlgPopBase 消息处理程序
BOOL CDlgPopBase::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	HBITMAP hBitmap = NULL;
	//strName = "..\\Image\\顶部背景.bmp";
	//hBitmap = (HBITMAP)::LoadImage(NULL, _T("Image\\顶部背景2.bmp"), IMAGE_BITMAP, 0, 0, LR_DEFAULTSIZE|LR_LOADFROMFILE);

	//ASSERT(hBitmap);

	if(hBitmap == NULL)
	{
		CString str;
		//AfxMessageBox(_T("Not Open SetImage Bitmap"));
		//PostQuitMessage(0);
	}

	if (m_BmpTop.m_hObject)
		m_BmpTop.Detach();

	//m_BmpTop.Attach(hBitmap);
	m_BmpTop.LoadBitmapW(IDB_BITMAP_TopBack2);
	CRect tempUp ,rcClient;
	GetClientRect(&rcClient);
	//m_btnClose.Create(L" ",SS_CENTER,tempUp,this,IDC_POPBase_BUTTON_CLOSE);// may ID=2000
	m_btnClose.SetImage(IDB_BITMAP_CloseWnd,IDB_BITMAP_CloseWnd1,IDB_BITMAP_CloseWnd2,IDB_BITMAP_CloseWnd);
	tempUp.left = rcClient.right - (34 + 5);//21
	tempUp.right = tempUp.left  +  34 ;//21
	tempUp.top = rcClient.top + 2;
	tempUp.bottom = rcClient.top + 24 ;
	m_btnClose.SetWindowText(_T(""));
	m_btnClose.MoveWindow(tempUp,true);
	m_btnClose.ShowWindow(SW_SHOW);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CDlgPopBase::OnPaint()
{
	CPaintDC dc(this); 
	CRect rcClient;
	GetClientRect(&rcClient);
	CDC szMemDC;
	szMemDC.CreateCompatibleDC(&dc);
	CBitmap btScreen;
	btScreen.CreateCompatibleBitmap(&dc, rcClient.Width(), rcClient.Height());

	szMemDC.SelectObject(&btScreen);
	btScreen.DeleteObject();

	szMemDC.FillSolidRect( &rcClient ,RGB(221,228,238));//RGB( 150,150,150));

	dc.BitBlt(rcClient.left, rcClient.top, rcClient.Width(), rcClient.Height(), &szMemDC, 0, 0, SRCCOPY);

	CRect rcTop = rcClient;
	rcTop.bottom =   25;
	 DrawRangeImage(&m_BmpTop, &szMemDC,rcTop);


	//画边框
	 CPen pen;	pen.CreatePen(PS_SOLID,1,RGB(100,145,200));
	 szMemDC.SelectObject(pen);
	 szMemDC.MoveTo(rcClient.left,rcClient.top);
	 szMemDC.LineTo(rcClient.left,rcClient.bottom);
	szMemDC.MoveTo(rcClient.right-1,rcClient.top);
	szMemDC.LineTo(rcClient.right-1,rcClient.bottom);
	szMemDC.MoveTo(rcClient.left,rcClient.bottom-1);
	szMemDC.LineTo(rcClient.right,rcClient.bottom-1);
	szMemDC.SetTextColor(RGB(255,250,200));
 
	szMemDC.SetBkMode(TRANSPARENT);
	CString t ;
	GetWindowText(t);
	szMemDC.TextOut(8,5,t);
	dc.BitBlt(rcClient.left, rcClient.top, rcClient.Width(), rcClient.Height(), &szMemDC, 0, 0, SRCCOPY);

	szMemDC.DeleteDC();
}

void CDlgPopBase::DrawRangeImage(CBitmap *pBitmap, CDC *pDC, CRect rc)
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
	if(li_Width>rc.Width())
		li_Width = rc.Width();
	if(li_Height>rc.Height())
		li_Height = rc.Height();
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



BOOL CDlgPopBase::PreTranslateMessage(MSG* pMsg)
{
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


HBRUSH CDlgPopBase::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  在此更改 DC 的任何特性
WCHAR s[256];
	GetClassName(pWnd->m_hWnd, s,256);
	if( nCtlColor == CTLCOLOR_STATIC )        
	{      
		pDC->SetBkMode(TRANSPARENT);   //设置背景透明   
		hbr =    HBRUSH(GetStockObject(NULL_BRUSH));  
	}   
	//else if(nCtlColor == CTLCOLOR_BTN)   
	
	else if(StrCmp(s,L"Button")==0)
	{
		CRect rc;
		pWnd->GetWindowRect(&rc);
		ScreenToClient(&rc);

		CDC* dc = GetDC();

		pDC->BitBlt(0,0,rc.Width(),rc.Height(),dc,rc.left,rc.top,SRCCOPY);	//把父窗口背景图片先画到按钮上

		ReleaseDC(dc);

		hbr = (HBRUSH) ::GetStockObject(NULL_BRUSH);
		return (HBRUSH)GetStockObject(NULL_BRUSH);  
	}
	
	// TODO:  如果默认的不是所需画笔，则返回另一个画笔
	return hbr;
}


LRESULT CDlgPopBase::OnNcHitTest(CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	ScreenToClient(&point);

	RECT rtWindow;
	GetClientRect(&rtWindow);

	long wndHeight = rtWindow.bottom - rtWindow.top;
	long wndWidth = rtWindow.right - rtWindow.left;
	
	RECT rcW = {0,0,wndWidth-30,25};

	RECT rcBtnClose;
	m_btnClose.GetClientRect(&rcBtnClose);
	 
	if(::PtInRect(&rcW,point)&&!::PtInRect(&rcBtnClose,point))
	{ 		
		return HTCAPTION;  // 在拖动范围内
	}
	return CDialogEx::OnNcHitTest(point);
}

void CDlgPopBase:: OnBnCloseClicked()	
{
  
}


void CDlgPopBase::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	RECT rcBtnClose;
	m_btnClose.GetClientRect(&rcBtnClose);
	if(::PtInRect(&rcBtnClose,point))
		CloseWindow();
	CDialogEx::OnLButtonUp(nFlags, point);
}


void CDlgPopBase::OnBnClickedButtonClose()
{
	 this->OnCancel();
}


void CDlgPopBase::OnBnClickedButtonasdfas()
{
	// TODO: 在此添加控件通知处理程序代码
	
}
