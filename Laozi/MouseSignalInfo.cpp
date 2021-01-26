// MouseSignalInfo.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "MouseSignalInfo.h"
#include "afxdialogex.h"
#include <GdiplusGraphics.h>
#pragma comment(lib,"gdiplus.lib")
#include <gdiplus.h>
#include "Laozi.h"
using namespace Gdiplus;
// CMouseSignalInfo �Ի���

IMPLEMENT_DYNAMIC(CMouseSignalInfo, CDialogEx)

	CMouseSignalInfo::CMouseSignalInfo(CWnd* pParent /*=NULL*/)
	: CDialogEx(CMouseSignalInfo::IDD, pParent)
{
	GdiplusStartupInput m_gdiInput;

	GdiplusStartup(&m_nGdiplusToken,&m_gdiInput,NULL);

}

CMouseSignalInfo::~CMouseSignalInfo()
{
	GdiplusShutdown(m_nGdiplusToken);
}

void CMouseSignalInfo::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CMouseSignalInfo, CDialogEx)
	ON_MESSAGE(WM_SIGNAL_INFO,OnSignalInfo)
	ON_WM_PAINT()
END_MESSAGE_MAP()


// CMouseSignalInfo ��Ϣ�������
LRESULT    CMouseSignalInfo::OnSignalInfo(WPARAM wParam,LPARAM lParam)
{
	memcpy(m_cMouseSignals,(const void *)wParam,sizeof(m_cMouseSignals));
	CPaintDC dc(this); // device context for painting

	//�ڴ滭ͼ//////////////////////////
	CDC szMemDC;
	szMemDC.CreateCompatibleDC(&dc);
	szMemDC.SetBkMode(TRANSPARENT);
	Graphics graphics( szMemDC.m_hDC);
	Image image(_T("c:\\new.png"), TRUE);
	graphics.DrawImage(&image, 0,0);

	CFont font;
	font.CreatePointFont(90,L"����");
	CFont* pOldFont = szMemDC.SelectObject(&font);
	szMemDC.SelectObject(&font);

	//szMemDC.TextOut(rcClient.right - 100,rcClient.top + 40,m_weather. m_strInfo[1].m_strNew_yesNoRain);
	font.DeleteObject();
	pOldFont->DeleteObject();



	// TODO: �ڴ˴������Ϣ����������
	CRect rcClient;
	GetClientRect(&rcClient);

	//�ڴ滭ͼ//////////////////////////

	//CBitmap btScreen;
	//btScreen.CreateCompatibleBitmap(&dc, rcClient.Width(), rcClient.Height());

	//szMemDC.SelectObject(&btScreen);
	//btScreen.DeleteObject();
	//Graphics graphics( szMemDC.m_hDC);
	//Image image(_T("c:\\new.png"), TRUE);
	//graphics.DrawImage(&image, 0,0);

	//CFont font;
	//font.CreatePointFont(90,L"����");
	//CFont* pOldFont = szMemDC.SelectObject(&font);
	//szMemDC.SelectObject(&font);

	////szMemDC.TextOut(rcClient.right - 100,rcClient.top + 40,m_weather. m_strInfo[1].m_strNew_yesNoRain);
	//font.DeleteObject();
	//pOldFont->DeleteObject();

	//HBITMAP hBitmap =  (HBITMAP)::LoadImage(::AfxGetInstanceHandle(), _T("Image\\ArrowUp.bmp"), IMAGE_BITMAP, 0, 0, LR_DEFAULTSIZE|LR_LOADFROMFILE);
	//BITMAP bm;
	//GetObject(hBitmap,sizeof(bm),(LPTSTR)&bm);
	//CBitmap bitmap; 
	//bitmap.Attach(hBitmap);
	//CBitmap* pOldBitmap = szMemDC.SelectObject(&bitmap);
	//dc.BitBlt(0, 0,  bm.bmWidth,  bm.bmHeight, &szMemDC, 0, 0, SRCCOPY);
	//szMemDC.SelectObject(pOldBitmap);
	//szMemDC.DeleteDC();

	RECT rc_tag;rc_tag.left = 0;rc_tag.top = 0;
	for (int i=0; i<GestureElmCount; i++)
	{
		if(m_cMouseSignals[i] != CMouseSignalInfo::None)
			rc_tag.left += 45;
	}
	MoveWindow(GetSystemMetrics(SM_CXSCREEN) /2 - rc_tag.left /2,GetSystemMetrics(SM_CYSCREEN) /2,rc_tag.left,45,FALSE);
	Invalidate();
	//if (bAnimating || 0 == wParam || 0 == m_pLb)
	//{
	//	return 0;
	//}
	//if(' ' != m_pSignalList[m_cSignalPos])/*lParam!=m_iLBIndex*/
	//{
	//	for(;m_cSignalPos<7;m_cSignalPos++)//ʹѭ������m_cSignalPos==7;
	//	{
	//		if(' '==m_pSignalList[m_cSignalPos])
	//			break;
	//	}

	//	ChangeTips(lParam);//��������ģ��
	//	if (2==wParam && IsWindowVisible()) //��̬�Ļ�ÿ�ζ����´��ڴ�С
	//	{
	//		static CRect rectNow;
	//		GetWindowRect(&rectNow);
	//		//ClientToScreen(&rectNow);
	//		SetWindowPos(&wndTopMost, theApp.m_activeRegion.Width()-rectNow.Width(), 
	//			theApp.m_activeRegion.Height() - rectNow.Height(),0, 0, SWP_NOSIZE|SWP_NOOWNERZORDER|SWP_NOACTIVATE);
	//		// 			SetWindowPos(&wndTopMost, GetSystemMetrics(SM_CXSCREEN)-rectNow.Width(), 
	//		// 				GetSystemMetrics(SM_CYSCREEN) - rectNow.Height()-m_rectOf_Shell_TrayWnd.Height(),
	//		// 				0, 0, SWP_NOSIZE|SWP_NOOWNERZORDER|SWP_NOACTIVATE);
	//	}
	//	m_iLBIndex=lParam;
	//}
	//m_iStatus=wParam;
	//switch(wParam)
	//{
	//case 1://��̬��ʾ
	//	//������(��̬��ʾ)ģ��
	//	::GetCursorPos(&m_ptCursor);
	//	if (!IsWindowVisible())
	//	{
	//		::SetWindowPos(m_hWnd, HWND_TOPMOST, m_ptCursor.x+15, m_ptCursor.y+15/*m_ptCursor.y-m_myOriginRect.Height()/2*/,
	//			0, 0, SWP_NOSIZE|SWP_SHOWWINDOW|SWP_NOOWNERZORDER|SWP_NOACTIVATE);
	//	}
	//	else
	//		::SetWindowPos(m_hWnd, HWND_TOPMOST, m_ptCursor.x+15, m_ptCursor.y+15/*m_ptCursor.y-m_myOriginRect.Height()/2*/,
	//		0, 0, SWP_NOSIZE|SWP_NOOWNERZORDER|SWP_NOACTIVATE|SWP_NOZORDER);
	//	// 		SetTimer(1,100, NULL);
	//	//TRACE("%d,%d:%d\n",m_ptCursor.x, m_ptCursor.y, IsWindowVisible());
	//	break;
	//case 2://���½���ʾ
	//	if (!IsWindowVisible() && m_bDrawingSignals)//!GetAsyncKeyState(VK_RBUTTON) //ֻ������ʱ��Ч
	//	{
	//		static CRect rectNow;
	//		GetWindowRect(&rectNow);
	//		//ClientToScreen(&rectNow);
	//		SetWindowPos(&wndTopMost, theApp.m_activeRegion.Width()-rectNow.Width(), 
	//			theApp.m_activeRegion.Height() - rectNow.Height(), 0, 0, SWP_NOSIZE|SWP_NOOWNERZORDER|SWP_NOACTIVATE);
	//		// 			SetWindowPos(&wndTopMost, GetSystemMetrics(SM_CXSCREEN)-rectNow.Width(), 
	//		// 				GetSystemMetrics(SM_CYSCREEN) - rectNow.Height()-m_rectOf_Shell_TrayWnd.Height(),
	//		// 				0, 0, SWP_NOSIZE|SWP_NOOWNERZORDER|SWP_NOACTIVATE);

	//		// 			SetWindowPos(NULL, m_rectOf_Shell_TrayWnd.Width()-rectNow.Width(), m_rectOf_Shell_TrayWnd.top - m_myOriginRect.Height(),
	//		// 				0, 0, SWP_NOSIZE|SWP_NOOWNERZORDER|SWP_NOACTIVATE|SWP_NOZORDER);
	//		// 			KillTimer(1);
	//		bAnimating=TRUE;

	//		HANDLE hNewDlg;
	//		DWORD WINAPI AnimateThreat(LPVOID m_hWnd);
	//		hNewDlg=::CreateThread(NULL,0,AnimateThreat,m_hWnd,0,NULL);//Animate in sub thread
	//		CloseHandle(hNewDlg);
	//		//AnimateWindow(m_hWnd, 200, AW_SLIDE|AW_VER_NEGATIVE);
	//		// 			SetTimer(2, 1000, NULL);
	//		SetTimer(3, 300, NULL);
	//	}
	//	break;
	//default:
	//	break;
	//}
	//m_msCount=0;
	//Beep(2000,200);
	return 0;
}


BOOL CMouseSignalInfo::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	::SetWindowPos(this->m_hWnd, HWND_TOPMOST, -1, -1, -1, -1, SWP_NOMOVE | SWP_NOSIZE); 

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}


void CMouseSignalInfo::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: �ڴ˴������Ϣ����������
	CRect rcClient;
	GetClientRect(&rcClient);

	//�ڴ滭ͼ//////////////////////////
	CDC szMemDC;
	szMemDC.CreateCompatibleDC(&dc);
	szMemDC.SetBkMode(TRANSPARENT);

	CBitmap btScreen;
	btScreen.CreateCompatibleBitmap(&dc, rcClient.Width(), rcClient.Height());

	szMemDC.SelectObject(&btScreen);
	btScreen.DeleteObject();
	//Graphics graphics( szMemDC.m_hDC);
	//Image image(_T("c:\\new.png"), TRUE);
	//graphics.DrawImage(&image, 0,0);

	//CFont font;
	//font.CreatePointFont(90,L"����");
	//CFont* pOldFont = szMemDC.SelectObject(&font);
	//szMemDC.SelectObject(&font);

	////szMemDC.TextOut(rcClient.right - 100,rcClient.top + 40,m_weather. m_strInfo[1].m_strNew_yesNoRain);
	//font.DeleteObject();
	//pOldFont->DeleteObject();
	int x = 0;
	for (int i=0; i<GestureElmCount; i++)
	{
		HBITMAP hBitmap =  NULL;
		BITMAP bm;
		switch(m_cMouseSignals[i])
		{
		case  CMouseSignalInfo::None:
			break;
		case  CMouseSignalInfo::Left:
			hBitmap =  (HBITMAP)::LoadImage(::AfxGetInstanceHandle(), _T("Image\\ArrowLeft.bmp"), IMAGE_BITMAP, 0, 0, LR_DEFAULTSIZE|LR_LOADFROMFILE);
			break;
		case  CMouseSignalInfo::Right:
			hBitmap =  (HBITMAP)::LoadImage(::AfxGetInstanceHandle(), _T("Image\\ArrowRight.bmp"), IMAGE_BITMAP, 0, 0, LR_DEFAULTSIZE|LR_LOADFROMFILE);
			break;
		case  CMouseSignalInfo::Up:
			hBitmap =  (HBITMAP)::LoadImage(::AfxGetInstanceHandle(), _T("Image\\ArrowUp.bmp"), IMAGE_BITMAP, 0, 0, LR_DEFAULTSIZE|LR_LOADFROMFILE);
			break;
		case  CMouseSignalInfo::Down:
			hBitmap =  (HBITMAP)::LoadImage(::AfxGetInstanceHandle(), _T("Image\\ArrowDown.bmp"), IMAGE_BITMAP, 0, 0, LR_DEFAULTSIZE|LR_LOADFROMFILE);
			break;
		}
		GetObject(hBitmap,sizeof(bm),(LPTSTR)&bm);
		CBitmap bitmap; 
		bitmap.Attach(hBitmap);
		CBitmap* pOldBitmap = szMemDC.SelectObject(&bitmap);		
		dc.BitBlt(x, 0,  bm.bmWidth,  bm.bmHeight, &szMemDC, 0, 0, SRCCOPY);
		if(m_cMouseSignals[i] != CMouseSignalInfo::None)
			x += bm.bmWidth;	
		szMemDC.SelectObject(pOldBitmap);
	}

	szMemDC.DeleteDC();
	// ��Ϊ��ͼ��Ϣ���� CDialogEx::OnPaint()
}
