// DlgMainBase.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "DlgMainBase.h"
#include "afxdialogex.h"


// CDlgMainBase �Ի���

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


// CDlgMainBase ��Ϣ�������


BOOL CDlgMainBase::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	m_tt.Create(this);
	m_tt.Activate(TRUE);
	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	m_brushBackground.CreateSolidBrush(RGB(221,228,238));  
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
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
	
	//���߿�
	szMemDC.MoveTo(rcClient.right-1,rcClient.top);
	szMemDC.LineTo(rcClient.right-1,rcClient.bottom);

	//dc.BitBlt(rcClient.left, rcClient.top, rcClient.Width(), rcClient.Height(), &szMemDC, 0, 0, SRCCOPY);
	
	szMemDC.DeleteDC();
}


BOOL CDlgMainBase::PreTranslateMessage(MSG* pMsg)
{
	// TODO: �ڴ����ר�ô����/����û���
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

	// TODO:  �ڴ˸��� DC ���κ�����
WCHAR s[256];
	GetClassName(pWnd->m_hWnd, s,256);
	if( nCtlColor == CTLCOLOR_STATIC )        
	{      
		pDC->SetBkMode(TRANSPARENT);   //���ñ���͸��   
		hbr =    HBRUSH(GetStockObject(NULL_BRUSH));  
	}   
	//else if(nCtlColor == CTLCOLOR_BTN)   
	
	else if(StrCmp(s,L"Button")==0)
	{
		CRect rc;
		pWnd->GetWindowRect(&rc);
		ScreenToClient(&rc);

		CDC* dc = GetDC();

		pDC->BitBlt(0,0,rc.Width(),rc.Height(),dc,rc.left,rc.top,SRCCOPY);	//�Ѹ����ڱ���ͼƬ�Ȼ�����ť��

		ReleaseDC(dc);

		hbr = (HBRUSH) ::GetStockObject(NULL_BRUSH);
		return (HBRUSH)GetStockObject(NULL_BRUSH);  
	}
	
	// TODO:  ���Ĭ�ϵĲ������軭�ʣ��򷵻���һ������
	return hbr;
}
