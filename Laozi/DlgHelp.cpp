// DlgHelp.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "DlgHelp.h"
#include "afxdialogex.h"


// CDlgHelp �Ի���

IMPLEMENT_DYNAMIC(CDlgHelp, CDlgPopBase)

CDlgHelp::CDlgHelp(CWnd* pParent /*=NULL*/)
	: CDlgPopBase(CDlgHelp::IDD, pParent)
{
	
	}

CDlgHelp::~CDlgHelp()
{
}

void CDlgHelp::DoDataExchange(CDataExchange* pDX)
{
	CDlgPopBase::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BUTTONHELP_CLOSE, m_btnClose);
}


BEGIN_MESSAGE_MAP(CDlgHelp, CDlgPopBase)
	ON_WM_PAINT()
	/*ON_WM_CTLCOLOR()
	ON_WM_LBUTTONUP()
	ON_WM_NCHITTEST()*/
	ON_BN_CLICKED(IDC_BUTTONHELP_CLOSE, &CDlgPopBase::OnBnClickedButtonClose)
END_MESSAGE_MAP()


// CDlgHelp ��Ϣ�������


void CDlgHelp::OnPaint()
{
	//CPaintDC dc(this); // device context for painting
	// TODO: �ڴ˴������Ϣ����������
	// ��Ϊ��ͼ��Ϣ���� 
	CDlgPopBase::OnPaint();
}


HBRUSH CDlgHelp::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  �ڴ˸��� DC ���κ�����

	// TODO:  ���Ĭ�ϵĲ������軭�ʣ��򷵻���һ������
	return hbr;
}


void CDlgHelp::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

	CDialogEx::OnLButtonUp(nFlags, point);
}


LRESULT CDlgHelp::OnNcHitTest(CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

	return CDialogEx::OnNcHitTest(point);
}


void CDlgHelp::OnBnClickedButtonClose()
{
	 OnBnClickedButtonClose();
}


BOOL CDlgHelp::OnInitDialog()
{
	CDlgPopBase::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	SetWindowText(L"����");
	SetDlgItemText(IDC_STATIC_HELP, L"��ͨ���ַ�ʽ����Ҫ���صĴ���\n1.ѡ�����\n    ���빴ѡ���������³���Ĵ��ڡ���������Ч��\n    �����ѡ�����ť����ѡ��Ҫ���صĳ���\n    ���ɾ����ѡ���б�ĳһ�У������ֹر�ͼ�꣬�����ͼ�꼴��ɾ��\n2.ͨ�����ѡ�񴰿�\n    ͬ����ѡ������ѡ��Ĵ��ڡ�һ��\n    ����ñ������Ļ���ͼ�꣬��ס������ţ��ϵ�Ҫ���صĴ��ڣ��ɿ���꼴��ѡ�С�\n    ��ʱ��Ӧ�б�����ʾѡ�еĴ��ڱ����������Ȼ���������ȷ��������ӡ�\n    ɾ����ʽͬ��\n3.ͨ���趨���ڱ���\n    ��ѡ�������а����������֡�\n    ���趨���������⣬ÿ������֮����\";\"������ע����Ӣ�ķֺš�\n    ���а������б���Ĵ��ڶ��������ء�\n\nĬ�Ͽ�ݼ���Alt+1����ͨ�Զ����ݼ�\n\n\n�����ʼ���ajiework@163.com��ȡ�������");

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}
