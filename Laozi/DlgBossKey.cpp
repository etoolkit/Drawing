// DlgBossKey.cpp : 实现文件
//

#include "stdafx.h"
#include "Laozi.h"
#include "DlgBossKey.h"
#include "afxdialogex.h"
#include "KeyHelper.h"
#include "DlgPopBase.h"


// CDlgBossKey 对话框

IMPLEMENT_DYNAMIC(CDlgBossKey, CDlgPopBase)

CDlgBossKey::CDlgBossKey(CWnd* pParent /*=NULL*/)
	: CDlgPopBase(CDlgBossKey::IDD, pParent)
	, m_sHotKey(_T(""))
{

}

CDlgBossKey::~CDlgBossKey()
{
}

void CDlgBossKey::DoDataExchange(CDataExchange* pDX)
{
	CDlgPopBase::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_sHotKey);
	DDX_Control(pDX, IDC_HotDlg_BUTTON_CLOSE, m_btnClose);
}


BEGIN_MESSAGE_MAP(CDlgBossKey, CDlgPopBase)
	ON_WM_KEYDOWN()
	ON_BN_CLICKED(IDOK, &CDlgBossKey::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CDlgBossKey::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_HotDlg_BUTTON_CLOSE, &CDlgPopBase::OnBnClickedButtonClose)
	ON_WM_PAINT()
END_MESSAGE_MAP()


// CDlgBossKey 消息处理程序


void CDlgBossKey::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

 
	CDialogEx::OnKeyDown(nChar, nRepCnt, nFlags);
}


void CDlgBossKey::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	CDialogEx::OnOK();
}


void CDlgBossKey::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	CDialogEx::OnCancel();
}


BOOL CDlgBossKey::OnInitDialog()
{
	CDlgPopBase::OnInitDialog();
	SetWindowText(L"设置热键");
	// TODO:  在此添加额外的初始化
	UpdateData(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


BOOL CDlgBossKey::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
   static 	bool bValidate = true;
	if(pMsg->message == WM_KEYDOWN || pMsg->message == WM_SYSKEYDOWN)
	{
		CString s ;	
		TCHAR c = (UINT)pMsg->wParam;
		UINT n = pMsg->wParam;
		bValidate = true;
		if(n == VK_ESCAPE)
		{
			SetDlgItemText(IDC_EDIT1,L"无");
			return TRUE;
		}
		if(GetKeyState(VK_CONTROL)& 0x80)
		{ 
			s +=  "Ctrl";
		}
		if(GetKeyState(VK_MENU)& 0x80)
		{
			if(s !="")
				s+= "+";
			s += "Alt";
		}
		if(GetKeyState(VK_SHIFT)&0x80)
		{
			if(s !="")
				s+= "+";
			s += "Shift";
		}
		if((GetKeyState(VK_LWIN) || GetKeyState(VK_RWIN))&0x80)
		{
			if(s !="")
				s+= "+";
			s += "Win";
		}
		if(s !="")
			s+= " + ";
		      
		 if(n == VK_CONTROL || n == VK_SHIFT || n== VK_MENU || n == VK_LWIN || n == VK_RWIN)
		 {
			 bValidate = false;
		 }
		else
			s += CKeyHelper::GetKeyStr(pMsg->wParam);
		//else if(n  >= VK_F1 && n <= VK_F12)
		//{		
		//	 s.Format(L"%sF%d",s,pMsg->wParam - VK_F1 + 1);
		//} 
		//else if(n>=VK_NUMPAD0 && n <= VK_NUMPAD9)
		//{
		//	 s.Format(L"%sNum %d",s,pMsg->wParam - VK_NUMPAD0);
		//}
		//else if((n>=0x41 && n <=0x5A)/*||(n>=0x61 && n < 0x7A)*/ || (n>=0x30 && n <= 0x39))
		//{
		//	s+= c;
		//}
		//
		//else
		//{
		//	switch(n)
		//	{
		//	case  VK_UP :
		//		s += "Up";break;
		//	case  VK_LEFT: 
		//		s += "Left";break;
		//	case  VK_DOWN :
		//		s += "Down";break;
		//	case  VK_RIGHT :
		//		s += "Right";break;

		//	case VK_OEM_COMMA:
		//		s +=",";break;
		//	case  VK_OEM_PERIOD:
		//		s +=".";break;
		//	case  VK_OEM_MINUS:
		//		s +="-";break;
		//	case VK_OEM_1:
		//		s += ";";break;
		//	case VK_OEM_2:
		//		s += "/";break;
		//	case  VK_OEM_3:
		//		s += "`";break;
		//	case VK_OEM_4:
		//		s += "[";break;
		//	case VK_OEM_5:
		//		s += "\\";break;
		//	case  VK_OEM_6:
		//		s += "]";break;
		//	case  VK_OEM_7:
		//		s += "'";break;
		//	case  VK_OEM_NEC_EQUAL:
		//	case VK_OEM_PLUS:
		//		s += '=';break;;
		//	case  VK_HOME:
		//		s += "Home";break;
		//	case  VK_END:
		//		s += "End";break;
		//	case  VK_INSERT:
		//		s += "Insert";break;
		//	case  VK_DELETE:
		//		s += "Delete";break;
		//	case  VK_PRIOR:
		//		s += "PageUp";break;
		//	case   VK_NEXT:
		//		s += "PageDown";break;
		//	case  VK_NUMLOCK:
		//		s +="NumLock";break;
		//	case   VK_DECIMAL:
		//		s += "NumDel";break;
		//	case   VK_DIVIDE:
		//		s += "Num/";break;
		//	case   VK_CLEAR:
		//		s += "Num5";break;
		//	case   VK_MULTIPLY:
		//		s += "Num *";break;
		//	case   VK_SUBTRACT:
		//		s += "Num-";break;
		//	case   VK_ADD:
		//		s += "Num+";break;
		//	case   VK_PAUSE:
		//		s += "Pause";break;
		//	case   VK_SNAPSHOT:
		//		s += "Snapshot";break;
		//	case   VK_SCROLL:
		//		s += "Scroll";break;
		//	default:
		//		s = "";break;
		//	}
		//}
		//
		
		m_sHotKey = s;
		SetDlgItemText(IDC_EDIT1,s);
		pMsg->wParam  = '\0';
	//  return 1;
	}
	if(pMsg->message == WM_KEYUP || pMsg->message == WM_SYSKEYUP)
	{ 
		if(m_sHotKey==""  || !bValidate /*|| (m_sHotKey.GetAt(m_sHotKey.GetLength()-1) == '+' )*/)
		{	
			m_sHotKey = "";
			SetDlgItemText(IDC_EDIT1,L"无");
		}
	}
	return CDlgPopBase::PreTranslateMessage(pMsg);
}


void CDlgBossKey::OnPaint()
{
//	CPaintDC dc(this); // device context for painting
	// TODO: 在此处添加消息处理程序代码
	// 不为绘图消息调用 
	CDlgPopBase::OnPaint();
}
