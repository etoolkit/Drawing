#include "StdAfx.h"
#include "MouseSignal.h"
#include "../MyHookDll/Myhook.h"
#include <gdiplus.h>
#include <GdiplusGraphics.h>
#pragma comment(lib,"gdiplus.lib")
#include "Laozi.h"
#include <afxcontrolbarutil.h>
using namespace Gdiplus;
//BEGIN_MESSAGE_MAP(CMouseSignal, CWnd)
//	/*ON_MESSAGE(WM_MYHOOK_RMOVE,OnRMove)
//	ON_MESSAGE(WM_MYHOOK_RPRESS,OnRPress)*/
//END_MESSAGE_MAP()

CMouseSignal::CMouseSignal(void)
{ 
	m_bAutoSetting = FALSE;
	m_bHideSignal = FALSE;
	GdiplusStartupInput m_gdiInput;

	GdiplusStartup(&m_nGdiplusToken,&m_gdiInput,NULL);
	AddGestureSet(CMouseSignalInfo::EnumEvent::Close, CMouseSignalInfo::EnumGesture::Down,CMouseSignalInfo::EnumGesture::Right);
	AddGestureSet(CMouseSignalInfo::EnumEvent::Back, CMouseSignalInfo::EnumGesture::Down,CMouseSignalInfo::EnumGesture::Left);
	AddGestureSet(CMouseSignalInfo::EnumEvent::CtrlTab, CMouseSignalInfo::EnumGesture::Up,CMouseSignalInfo::EnumGesture::Right);
	AddGestureSet(CMouseSignalInfo::EnumEvent::CtrlShiftTab, CMouseSignalInfo::EnumGesture::Up,CMouseSignalInfo::EnumGesture::Left);
	AddGestureSet(CMouseSignalInfo::EnumEvent::PageUp, CMouseSignalInfo::EnumGesture::Up);
	AddGestureSet(CMouseSignalInfo::EnumEvent::PageDown, CMouseSignalInfo::EnumGesture::Down);
	AddGestureSet(CMouseSignalInfo::EnumEvent::PageHome,  CMouseSignalInfo::EnumGesture::Right, CMouseSignalInfo::EnumGesture::Up);
	AddGestureSet(CMouseSignalInfo::EnumEvent::PageEnd, CMouseSignalInfo::EnumGesture::Right, CMouseSignalInfo::EnumGesture::Down);
	AddGestureSet(CMouseSignalInfo::EnumEvent::Redo, CMouseSignalInfo::EnumGesture::Right,CMouseSignalInfo::EnumGesture::Left);
//	AddGestureSet(CMouseSignalInfo::EnumEvent::, CMouseSignalInfo::EnumGesture::Down,CMouseSignalInfo::EnumGesture::Right);
}


CMouseSignal::~CMouseSignal(void)
{
	GdiplusShutdown(m_nGdiplusToken);
}
HDC g_hScreenDc=0, hMemDc=0;
CDC *dpc=0;
HBITMAP hbitmap=0;


LRESULT    CMouseSignal::OnRMove(WPARAM wParam,LPARAM lParam)
{
	static CMouseSignalInfo::EnumGesture SignalBuffer=CMouseSignalInfo::None;
	if(!g_hScreenDc && !m_bHideSignal) return 0;
	//if(!m_bHideSignal)
	{
		m_nLineWidth = 5;
		CPen pen(PS_SOLID, m_nLineWidth, m_clr);
		dpc->SelectObject(&pen);
		dpc->MoveTo(m_pt.x, m_pt.y);
		dpc->LineTo(wParam, lParam);
		//Beep(2000,30);
	}
	if(m_bSignalPos<7 && abs((int)(lParam-m_pt.y))+abs((int)(wParam-m_pt.x)) > 3)
	{
		if(abs((int)(lParam-m_pt.y))<abs((int)(wParam-m_pt.x)))//不是左就是右
		{
			if (wParam>(UINT)m_pt.x)//右
			{
				if(m_bSignalPos==-1) m_cMouseSignals[++m_bSignalPos]= CMouseSignalInfo::Right;
				else if(SignalBuffer!=CMouseSignalInfo::Right) SignalBuffer=CMouseSignalInfo::Right;
				else if(m_cMouseSignals[m_bSignalPos]!=CMouseSignalInfo::Right) m_cMouseSignals[++m_bSignalPos]=CMouseSignalInfo::Right;
			}
			else//左?
			{
				if(m_bSignalPos==-1) m_cMouseSignals[++m_bSignalPos]=CMouseSignalInfo::Left;
				else if(SignalBuffer!=CMouseSignalInfo::Left) SignalBuffer=CMouseSignalInfo::Left;
				else if(m_cMouseSignals[m_bSignalPos]!=CMouseSignalInfo::Left) m_cMouseSignals[++m_bSignalPos]=CMouseSignalInfo::Left;
			}
		}
		else//上下
		{
			if(lParam>m_pt.y)//下
			{
				if(m_bSignalPos==-1) m_cMouseSignals[++m_bSignalPos]=CMouseSignalInfo::Down;
				else if(SignalBuffer!=CMouseSignalInfo::Down) SignalBuffer=CMouseSignalInfo::Down;
				else if(m_cMouseSignals[m_bSignalPos]!=CMouseSignalInfo::Down) m_cMouseSignals[++m_bSignalPos]=CMouseSignalInfo::Down;
			}
			else//上?
			{
				if(m_bSignalPos==-1) m_cMouseSignals[++m_bSignalPos]=CMouseSignalInfo::Up;
				else if(SignalBuffer!=CMouseSignalInfo::Up) SignalBuffer=CMouseSignalInfo::Up;
				else if(m_cMouseSignals[m_bSignalPos]!=CMouseSignalInfo::Up) m_cMouseSignals[++m_bSignalPos]=CMouseSignalInfo::Up;
			}
		}

	}
	m_pt.x=wParam;
	m_pt.y=lParam;
	char tem[9]={0};
	memcpy(tem,m_cMouseSignals,8);
	RECT rc_tag;
	int scrMidX = GetSystemMetrics(SM_CXSCREEN) /2;
	int scrMidY = GetSystemMetrics(SM_CYSCREEN)  / 2;
	rc_tag.left= scrMidX; 
	 rc_tag.top= scrMidY;
	rc_tag.right =rc_tag.left + 45;
	rc_tag.bottom = rc_tag.top  +45;
	/*Graphics graphics( dpc->m_hDC);

	for (int i=0; i<GestureElmCount; i++)
	{
		if(m_cMouseSignals[i] == CMouseSignalInfo::Up)
		{	
			Image image(theApp.m_modulePath +_T("\\Image\\ArrowUp.png"), TRUE);
			graphics.DrawImage(&image, rc_tag.left,rc_tag.top + 30);
		}
		else if(m_cMouseSignals[i] == CMouseSignalInfo::Down)
		{	
			Image image(theApp.m_modulePath +_T("\\Image\\ArrowDown.png"), TRUE);
			graphics.DrawImage(&image, rc_tag.left,rc_tag.top + 30);
		}
		else if(m_cMouseSignals[i] == CMouseSignalInfo::Left)
		{	
			Image image(theApp.m_modulePath +_T("\\Image\\ArrowLeft.png"), TRUE);
			graphics.DrawImage(&image, rc_tag.left,rc_tag.top + 30);
		}
		else if(m_cMouseSignals[i] == CMouseSignalInfo::Right)
		{	
			Image image(theApp.m_modulePath +_T("\\Image\\ArrowRight.png"), TRUE);
			graphics.DrawImage(&image, rc_tag.left,rc_tag.top + 30);
		}
		rc_tag.left += 45;
	}*/
	int num = 0;
	for (int i=0; i<GestureElmCount; i++)
	{
		if(m_cMouseSignals[i] != CMouseSignalInfo::None)
			num ++;
	}

	HPEN hPen = CreatePen(PS_NULL, 1, NULL);    //注意：无边框画笔
	SelectObject(dpc->m_hDC, hPen);
	CBrush brush;//(RGB(0,162,232));
	brush.CreateSolidBrush(RGB(0,162,232));
	dpc->SelectObject(brush);
	
	dpc->SetBkMode(TRANSPARENT);
	CString showTag = GetEventText(GetEventByGesture(m_cMouseSignals));
 
	CSize szText=dpc->GetTextExtent(showTag);
	szText.cx += 10;
	//::GetTextExtentPoint32(dpc->m_hDC,showTag,strlen((LPCSTR)(LPCWSTR)showTag),&szText);
	int nleft = scrMidX - szText.cx/2;
	rc_tag.left = nleft;
	rc_tag.right = rc_tag.left + szText.cx;
	rc_tag.top = scrMidY + 70;
	rc_tag.bottom = scrMidY + 100;
	CRgn rgn;rgn.CreateRectRgnIndirect(&rc_tag);
	// dpc->FillRgn(&rgn, &brush);
	
	dpc->Rectangle(nleft,scrMidY+70,nleft + szText.cx, scrMidY + 100);
	rc_tag.left = nleft+5;rc_tag.bottom = scrMidY + 100;
	rc_tag.top = scrMidY+70+5;rc_tag.right = nleft + szText.cx;  
	dpc->SetTextColor(RGB(255,255,255));

 ///::InvalidateRect(NULL, &rc_tag, FALSE);
	dpc->DrawText(showTag,&rc_tag,0);
	
	//if(!m_sigalInfo.GetSafeHwnd())
	//	m_sigalInfo.Create(IDD_DIALOG_MouseSignalInfo,0);
	m_sigalInfo.ShowWindow(SW_SHOW);
	::SendMessage(m_sigalInfo.GetSafeHwnd(),WM_SIGNAL_INFO,(WPARAM)m_cMouseSignals,NULL);
	tem[8]=0;
	// 	sprintf(tem, "%c%c%c%c%c%c%c%c",m_cMouseSignals[0],m_cMouseSignals[1],m_cMouseSignals[2],m_cMouseSignals[3],
	// 			m_cMouseSignals[4],m_cMouseSignals[5],m_cMouseSignals[6],m_cMouseSignals[7]);
//	::SendMessage(theApp.m_dlgInfo->m_hWnd, WM_SIGNAL_INFO, m_iSignalInfoStatus, m_lbMouseSignals.FindString(-1,tem));
	//主要用于 m_iSignalInfoStatus == 1 时
	//将 list box 控件转化为变量了……请以后将取地址函数换成直接引用
	return 0;
}
LRESULT    CMouseSignal::OnRPress(WPARAM wParam,LPARAM lParam)
{
	if(wParam!=0)//Down
	{
		if(!m_bHideSignal)
		{
			g_hScreenDc=CreateDC(L"DISPLAY",NULL,NULL,NULL);//::GetDC(0);
			// 			if (!hMemDc)
			hMemDc=CreateCompatibleDC(g_hScreenDc);
			if (!hbitmap)
				hbitmap=CreateCompatibleBitmap(g_hScreenDc,GetDeviceCaps(g_hScreenDc,HORZRES),GetDeviceCaps(g_hScreenDc,VERTRES));
			dpc=CDC::FromHandle(g_hScreenDc);
			SelectObject(hMemDc,hbitmap);
			BitBlt(hMemDc,0,0,GetDeviceCaps(g_hScreenDc,HORZRES),GetDeviceCaps(g_hScreenDc,VERTRES),g_hScreenDc,0,0,SRCCOPY);//?
		}
		//if (m_bRandomColor)
		{
			m_clr=RGB(rand()%256,rand()%256,rand()%256);
		}
		m_pt.x=wParam;
		m_pt.y=lParam;
		for (byte i=0;i<8;i++)
		{
			m_cMouseSignals[i]=CMouseSignalInfo::None;
		}
		m_bSignalPos=-1;
	//	theApp.m_dlgInfo->m_bDrawingSignals=TRUE;
	}
	else//up
	{
		if(!m_bHideSignal)
		{
			SelectObject(hMemDc,hbitmap);
			BitBlt(g_hScreenDc,0,0,GetDeviceCaps(g_hScreenDc,HORZRES),GetDeviceCaps(g_hScreenDc,VERTRES),hMemDc,0,0,SRCCOPY);
			DeleteDC(g_hScreenDc);	g_hScreenDc=NULL; dpc=NULL;
			DeleteDC(hMemDc);		hMemDc=NULL;
					DeleteObject(hbitmap);	hbitmap=NULL;
			//theApp.m_dlgInfo->Invalidate();
		}
		m_bSignalPos=-1;
		if(m_bAutoSetting)
		{
			for(short i=0;i<GestureElmCount;i++)
			{
				switch(m_cMouseSignals[i])
				{
				case 'U':
					//OnSelectUp();
					break;
				case 'D':
					//OnSelectDown();
					break;
				case 'L':
					//OnSelectLeft();
					break;
				case 'R':
					//OnSelectRight();
					break;
				default:
					break;
				}
			}
			//m_BTN_AutoIdentification.ShowWindow(SW_SHOW);
			m_bAutoSetting=FALSE;
			//TrayBalloon("自动设定鼠标手势(完成)", "手势已经识别完成，请核对手势列表看看是否心目中的手势。",NIIF_INFO);	
			return 0;
		}
		/*	char tem[512]={0};
		memcpy(tem,m_cMouseSignals,8);
		sprintf(tem,"%s|",tem);*/
		// 		sprintf(tem, "%c%c%c%c%c%c%c%c|",m_cMouseSignals[0],m_cMouseSignals[1],m_cMouseSignals[2],m_cMouseSignals[3],
		// 			m_cMouseSignals[4],m_cMouseSignals[5],m_cMouseSignals[6],m_cMouseSignals[7]);
		// 		CListBox *pLB = (CListBox*)GetDlgItem(IDC_LIST_MOUSE_SIGNALS);
		int nIndex=-1;
		m_sigalInfo.ShowWindow(SW_HIDE);
		//AfxMessageBox(tem);
	//	if(LB_ERR != (nIndex = m_lbMouseSignals.FindString(-1, tem)))
		{
			//m_lbMouseSignals.GetText(nIndex, tem);
			//m_strLastSignal=tem;//即刻结束消息
			//m_bEventHandled=FALSE;
			SignalEvents(m_cMouseSignals);
		}
		// 		::SendMessage(theApp.m_dlgInfo->m_hWnd, WM_SIGNAL_INFO, m_iSignalInfoStatus, nIndex);
		//TRACE("%d\n",IsWindow(m_dlgInfo->m_hWnd));
		//消息显示 //主要用于 m_iSignalInfoStatus == 2 时

		// 		MessageBox(tem);
		//theApp.m_dlgInfo->m_bDrawingSignals=FALSE;
	}
	
	//m_sigalInfo.CloseWindow();
	return 0;
}
int CMouseSignal::SignalEvents(CMouseSignalInfo::EnumGesture* gesture)
{
	

	CString str=L"";
	for (int i=0;i<GestureElmCount;i++)
	{
		char s[10];
		itoa( (int)gesture[i],s,10);
		str+=s;
	}
	map<CString ,CMouseSignalInfo::EnumEvent >::iterator l_it;; 
	l_it = SignalEventSettings.find(str);
	if(l_it == SignalEventSettings.end())
		return -1;
	switch(l_it->second)
	{
	case CMouseSignalInfo::Close:
		keybd_event( VK_MENU, 0x45, KEYEVENTF_EXTENDEDKEY | 0, 0 );
		keybd_event( VK_F4, 0x45, KEYEVENTF_EXTENDEDKEY | 0, 0 );
		keybd_event( VK_MENU, 0x45, KEYEVENTF_EXTENDEDKEY | KEYEVENTF_KEYUP, 0 );//松开Alt
		break;
	case CMouseSignalInfo::Mini:
		{
		HWND hFGWnd=::GetForegroundWindow();
			if(::IsWindow(hFGWnd) && hFGWnd!=m_hWnd)
				CWnd::FromHandle(hFGWnd)->ShowWindow(SW_MINIMIZE);
		}
		break;		
	case CMouseSignalInfo::Back:
		keybd_event( VK_BACK, 0x45, KEYEVENTF_EXTENDEDKEY | 0, 0 );
		break;
	case CMouseSignalInfo::Redo:
		keybd_event( VK_CONTROL, 0x45, KEYEVENTF_EXTENDEDKEY | 0, 0 );
		keybd_event( 'Z', 0x45, KEYEVENTF_EXTENDEDKEY | 0, 0 );
		keybd_event( VK_CONTROL, 0x45, KEYEVENTF_EXTENDEDKEY | KEYEVENTF_KEYUP, 0 );//
		break;
	case CMouseSignalInfo::PageUp:
		keybd_event( VK_PRIOR, 0x45, KEYEVENTF_EXTENDEDKEY | 0, 0 );
		break;
	case CMouseSignalInfo::PageDown:
		keybd_event( VK_NEXT, 0x45, KEYEVENTF_EXTENDEDKEY | 0, 0 );
		break;
	case CMouseSignalInfo::PageHome:
		keybd_event( VK_CONTROL, 0x45, KEYEVENTF_EXTENDEDKEY | 0, 0 );
		keybd_event( VK_HOME, 0x45, KEYEVENTF_EXTENDEDKEY | 0, 0 );
		keybd_event( VK_CONTROL, 0x45, KEYEVENTF_EXTENDEDKEY | KEYEVENTF_KEYUP, 0 );//
		break;
	case CMouseSignalInfo::PageEnd:
		keybd_event( VK_CONTROL, 0x45, KEYEVENTF_EXTENDEDKEY | 0, 0 );
		keybd_event( VK_END, 0x45, KEYEVENTF_EXTENDEDKEY | 0, 0 );
		keybd_event( VK_CONTROL, 0x45, KEYEVENTF_EXTENDEDKEY | KEYEVENTF_KEYUP, 0 );//
		break;
	case CMouseSignalInfo::CtrlTab:
		keybd_event( VK_CONTROL, 0x45, KEYEVENTF_EXTENDEDKEY | 0, 0 );
		keybd_event( VK_TAB, 0x45, KEYEVENTF_EXTENDEDKEY | 0, 0 );
		keybd_event( VK_CONTROL, 0x45, KEYEVENTF_EXTENDEDKEY | KEYEVENTF_KEYUP, 0 );//
		break;
	case CMouseSignalInfo::CtrlShiftTab:
		keybd_event( VK_CONTROL, 0x45, KEYEVENTF_EXTENDEDKEY | 0, 0 );
		keybd_event( VK_SHIFT, 0x45, KEYEVENTF_EXTENDEDKEY | 0, 0 );
		keybd_event( VK_TAB, 0x45, KEYEVENTF_EXTENDEDKEY | 0, 0 );
		keybd_event( VK_SHIFT, 0x45, KEYEVENTF_EXTENDEDKEY | KEYEVENTF_KEYUP, 0 );//
		keybd_event( VK_CONTROL, 0x45, KEYEVENTF_EXTENDEDKEY | KEYEVENTF_KEYUP, 0 );//
		break;
	}
	return 0;
}BEGIN_MESSAGE_MAP(CMouseSignal, CWnd)
	ON_WM_CREATE()
	END_MESSAGE_MAP()


int CMouseSignal::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	//m_sigalInfo.Create(IDD_DIALOG_MouseSignalInfo,0);
	// TODO:  在此添加您专用的创建代码

	return 0;
}

CMouseSignalInfo::EnumEvent CMouseSignal::GetEventByGesture(CMouseSignalInfo::EnumGesture  g[])
{
	CString str=L"";
	for (int i=0;i<GestureElmCount;i++)
	{
		char s[10];
		itoa( (int)g[i],s,10);
		str+=s;
	}
	map<CString ,CMouseSignalInfo::EnumEvent >::iterator l_it;; 
	l_it = SignalEventSettings.find(str);
	if(l_it == SignalEventSettings.end())
		return  CMouseSignalInfo::EnumEvent::NoneEvent;
	return l_it->second;
}
CString CMouseSignal::GetEventText(CMouseSignalInfo::EnumEvent e)
{
	switch(e)
	{
	case CMouseSignalInfo::Close:
		return L" 关闭程序 ";
	case CMouseSignalInfo::Back:
		return L"    后退    ";
	case CMouseSignalInfo::Redo:
		return L"    撤销    ";
	case CMouseSignalInfo::PageUp:
		return L" 向上翻页 ";
	case CMouseSignalInfo::PageDown:
		return L" 向下翻页 ";
	case CMouseSignalInfo::PageHome:
		return L"到页面顶部";
	case CMouseSignalInfo::PageEnd:
		return L"到页面底部";
	case CMouseSignalInfo::ToRight:
		return L" 窗口右靠 ";
	case CMouseSignalInfo::ToCenter:
		return L" 窗口居中 ";
	case CMouseSignalInfo::CtrlShiftTab:
		return L"前一个标签";
	case CMouseSignalInfo::CtrlTab:
		return L"后一个标签";
	case CMouseSignalInfo::Mini:
		return L"最小化窗口"; 
	case CMouseSignalInfo::Restore:
		return L" 还原窗口 "; 
	default:
		return L" 无效动作 ";
	}
}

void CMouseSignal::AddGestureSet(CMouseSignalInfo::EnumEvent e, CMouseSignalInfo::EnumGesture s1 /* = CMouseSignalInfo::EnumGesture::None */, CMouseSignalInfo::EnumGesture s2 /* = CMouseSignalInfo::EnumGesture::None */, CMouseSignalInfo::EnumGesture s3 /* = CMouseSignalInfo::EnumGesture::None */, CMouseSignalInfo::EnumGesture s4 /* = CMouseSignalInfo::EnumGesture::None */, CMouseSignalInfo::EnumGesture s5 /* = CMouseSignalInfo::EnumGesture::None */, CMouseSignalInfo::EnumGesture s6 /* = CMouseSignalInfo::EnumGesture::None */, CMouseSignalInfo::EnumGesture s7 /* = CMouseSignalInfo::EnumGesture::None */, CMouseSignalInfo::EnumGesture s8 /* = CMouseSignalInfo::EnumGesture::None */)
{
	CString s = L"";
	char c[10];
	itoa( (int)s1,c,10);
	s += c;
	itoa( (int)s2,c,10);
	s += c;
	itoa( (int)s3,c,10);
	s += c;
	itoa( (int)s4,c,10);
	s += c;
	itoa( (int)s5,c,10);
	s += c;
	itoa( (int)s6,c,10);
	s += c;
	itoa( (int)s7,c,10);
	s += c;
	itoa( (int)s8,c,10);
	s += c;
	if(SignalEventSettings.find(s) == SignalEventSettings.end())
		SignalEventSettings.insert(pair<CString,CMouseSignalInfo::EnumEvent>(s,e));

}