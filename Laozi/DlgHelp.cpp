// DlgHelp.cpp : 实现文件
//

#include "stdafx.h"
#include "DlgHelp.h"
#include "afxdialogex.h"


// CDlgHelp 对话框

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


// CDlgHelp 消息处理程序


void CDlgHelp::OnPaint()
{
	//CPaintDC dc(this); // device context for painting
	// TODO: 在此处添加消息处理程序代码
	// 不为绘图消息调用 
	CDlgPopBase::OnPaint();
}


HBRUSH CDlgHelp::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  在此更改 DC 的任何特性

	// TODO:  如果默认的不是所需画笔，则返回另一个画笔
	return hbr;
}


void CDlgHelp::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	CDialogEx::OnLButtonUp(nFlags, point);
}


LRESULT CDlgHelp::OnNcHitTest(CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	return CDialogEx::OnNcHitTest(point);
}


void CDlgHelp::OnBnClickedButtonClose()
{
	 OnBnClickedButtonClose();
}


BOOL CDlgHelp::OnInitDialog()
{
	CDlgPopBase::OnInitDialog();

	// TODO:  在此添加额外的初始化
	SetWindowText(L"帮助");
	SetDlgItemText(IDC_STATIC_HELP, L"可通三种方式设置要隐藏的窗口\n1.选择程序。\n    必须勾选“隐藏以下程序的窗口”，否则无效，\n    点击“选择程序按钮”，选择要隐藏的程序。\n    如果删除，选中列表某一行，将出现关闭图标，点击该图标即可删除\n2.通过鼠标选择窗口\n    同样勾选“隐藏选择的窗口”一栏\n    点击该标题后面的环形图标，按住左键不放，拖到要隐藏的窗口，松开鼠标即可选中。\n    此时对应列表中显示选中的窗口标题和类名，然后点击后面的确定即可添加。\n    删除方式同上\n3.通过设定窗口标题\n    勾选“窗体中包含以下文字”\n    可设定任意多个标题，每个标题之间用\";\"隔开，注意是英文分号。\n    所有包含其中标题的窗口都讲被隐藏。\n\n默认快捷键是Alt+1，可通自定义快捷键\n\n\n发送邮件至ajiework@163.com获取更多帮助");

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}
