// DlgMainBase.cpp : 实现文件
//

#include "stdafx.h"
#include "DlgMainBase.h"
#include "afxdialogex.h"


// CDlgMainBase 对话框

IMPLEMENT_DYNAMIC(CDlgMainBase, CDialogEx)

CDlgMainBase::CDlgMainBase(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgMainBase::IDD, pParent)
{

}

CDlgMainBase::~CDlgMainBase()
{
}

void CDlgMainBase::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgMainBase, CDialogEx)
	ON_WM_PAINT()
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()


// CDlgMainBase 消息处理程序


BOOL CDlgMainBase::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	m_tt.Create(this);
	m_tt.Activate(TRUE);
	// TODO:  在此添加额外的初始化
	m_brushBackground.CreateSolidBrush(RGB(221,228,238));  
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void CDlgMainBase::OnPaint()
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
	
	//画边框
	szMemDC.MoveTo(rcClient.right-1,rcClient.top);
	szMemDC.LineTo(rcClient.right-1,rcClient.bottom);

	//dc.BitBlt(rcClient.left, rcClient.top, rcClient.Width(), rcClient.Height(), &szMemDC, 0, 0, SRCCOPY);
	
	szMemDC.DeleteDC();
}


BOOL CDlgMainBase::PreTranslateMessage(MSG* pMsg)
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


HBRUSH CDlgMainBase::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
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
