#include "StdAfx.h"
#include "BtnSelectWnd.h"
#include <commdlg.h>
#include "Resource.h"
#include <winuser.h>

CBtnSelectWnd::CBtnSelectWnd(void)
{
	bAiming = FALSE;
	hOld = NULL;
	threadId = 0;
	m_hTargetWnd = NULL;
	/*SetBitmap( LoadBitmap(AfxGetInstanceHandle(),   
		MAKEINTRESOURCE(IDB_BMP_SELECTWND)));*/
}

void CBtnSelectWnd::SetTargetHwnd(HWND hwnd)
{
	m_hTargetWnd = hwnd;OnGetDlgCode();
}
CBtnSelectWnd::~CBtnSelectWnd(void)
{
}
BEGIN_MESSAGE_MAP(CBtnSelectWnd, CButton)
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_GETDLGCODE()
END_MESSAGE_MAP()
 
void CBtnSelectWnd::InvertDrawFrame(HWND hWndPoint) //反画窗口的边框
{
	if(hWndPoint<0)
		return ;

	RECT rectFrame;
	//窗口的位置
	::GetWindowRect(hWndPoint,&rectFrame);
	//Map the screen coordinate to the current window.
	//将窗口左上点置为（0，0）后，窗口四个角点的坐标
	rectFrame.right -=rectFrame.left;
	rectFrame.bottom-=rectFrame.top;
	//!!sequence
	rectFrame.left = 0;rectFrame.top = 0;

	//paint frame
	HDC hdc;
	hdc = ::GetWindowDC(hWndPoint); //Hdc of the pointed window area.
	SetROP2(hdc,R2_NOT);//Pixel is the inverse of the screen color.
	HPEN hPen;
	int  iPenWidth = 3;
	hPen =CreatePen(PS_INSIDEFRAME,iPenWidth,RGB(0,0,0));
	SelectObject(hdc,hPen);
	SelectObject(hdc,GetStockObject(NULL_BRUSH));
	Rectangle(hdc,rectFrame.left,rectFrame.top,rectFrame.right,rectFrame.bottom);
	::ReleaseDC(hWndPoint,hdc);
	DeleteObject(hPen);

	
}
//根据屏幕上的点，取得该点下面的窗口句柄
HWND CBtnSelectWnd::GetWndFromPoint(POINT pt)
{
	HWND hwndFocus,hTmp;
	//据说这个系统函数实现了那个功能
	//但好像还得迭代一下才能找到包含该点的最小的子窗口
	hwndFocus= ::WindowFromPoint(pt);

	//于是就迭代了一下
	::ScreenToClient(hwndFocus,&pt);
	hTmp = ChildWindowFromPointEx(hwndFocus,pt,CWP_ALL);

	while(hTmp != NULL && hTmp != hwndFocus)
	{//如果点下面的窗口中还有包含该点的子窗口，并且该子窗口不是当前窗口的话，
		//继续缩小范围
		hwndFocus = hTmp;
		::ScreenToClient(hwndFocus,&pt);
		hTmp = ChildWindowFromPointEx(hwndFocus,pt,CWP_ALL);
	}

	return hwndFocus;
}
void CBtnSelectWnd::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	if(bAiming)
	{
		POINT pt;  //鼠标位置
		GetCursorPos(&pt); //screen pos
		HWND hWndPoint = this->GetWndFromPoint(pt);//得到鼠标下面的窗口句柄
		if(hOld != hWndPoint && hWndPoint != m_hWnd)//是否是旧的窗口句柄
		{
			//如果鼠标到了新窗口的话

			//刷掉旧窗口的边
			InvertDrawFrame(hOld);//Refresh the invalid frame.
			//更新旧窗口句柄
			hOld = hWndPoint;
			//获得鼠标所指向新窗口的线程ID
			threadId = GetWindowThreadProcessId(hOld,NULL);
			//更新主窗口的信息显示
			//InvalidateRect(hwndDlg,NULL,1);//Refresh the text output of the dialog.
			/////////////////draw frame
			//画出新窗口边框
			InvertDrawFrame(hWndPoint);
			if(m_hTargetWnd!=NULL)
				::SendMessage(m_hTargetWnd,MSG_SLECTEWND,(WPARAM)hWndPoint,NULL);
		}
	}
	CButton::OnMouseMove(nFlags, point);
}


void CBtnSelectWnd::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	RECT rectCtrl;
	
	POINT pt;  //鼠标点击位置
	::GetCursorPos(&pt);
	HWND hwnd = AfxGetMainWnd()->GetSafeHwnd();
	//if(pt.x>rectCtrl.left && pt.x<rectCtrl.right //如果鼠标左键击中靶子图标
	//	&& pt.y>rectCtrl.top && pt.y<rectCtrl.bottom)//hit bitmap
	{
		//恢复初始状态
		hOld = NULL;
		threadId = 0;
		//show red point 
		::SetClassLong(hwnd,GCL_HICON,
			(LONG)LoadIcon((HINSTANCE)GetWindowLong(hwnd,GWL_HINSTANCE),(LPCTSTR)IDB_BMP_SELECTWND));
		//setCursor to own cursor
		//将鼠标光标改为靶子
		::SetCursor(
			LoadCursor(
			(HINSTANCE)GetWindowLong(hwnd,GWL_HINSTANCE),
			(LPCTSTR)IDC_CURSOR_CHOOSEWND
			)
			);
		//Set blank bitmap to IDC_TARGET
		//将原来主窗口中的靶子置成空白图
		::SendMessage(::GetDlgItem(m_hWnd,IDB_BMP_SELECTWND),STM_SETIMAGE,IMAGE_BITMAP,
			(LPARAM)LoadBitmap((HINSTANCE)GetWindowLong(m_hWnd,GWL_HINSTANCE),MAKEINTRESOURCE(IDB_BMP_SELECTWND2)));
		HBITMAP hBmp=::LoadBitmap(AfxGetInstanceHandle(),  MAKEINTRESOURCE(IDB_BMP_SELECTWND2));   
		SetBitmap(hBmp);
		if(m_hTargetWnd!=NULL)
			::SendMessage(m_hTargetWnd,MSG_SLECTEWNDBegin,NULL,NULL);
		//capture mouse
		::SetCapture(hwnd);
		//set aiming state
		//鼠标左键按住时即为aiming state = true
		bAiming = TRUE;
	}
	::InvalidateRect(hwnd,NULL,1);
	CButton::OnLButtonDown(nFlags, point);
}


void CBtnSelectWnd::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if(bAiming)
	{
		//擦掉最后一个窗口的边框
		InvertDrawFrame(hOld);//erase the last frame
		//释放鼠标
		::ReleaseCapture();

		//还原鼠标指针
		::SetCursor(LoadCursor(NULL,IDC_ARROW));
		//还原主窗口中的靶子图标
	/*	::SendMessage(::GetDlgItem(m_hWnd,IDB_BMP_SELECTWND),STM_SETIMAGE,IMAGE_BITMAP,
			(LPARAM)LoadBitmap((HINSTANCE)GetWindowLong(m_hWnd,GWL_HINSTANCE),MAKEINTRESOURCE(IDB_BMP_SELECTWND2)));
		*/
		HBITMAP hBmp=::LoadBitmap(AfxGetInstanceHandle(),  MAKEINTRESOURCE(IDB_BMP_SELECTWND));   
		SetBitmap(hBmp);
		//主窗口标题显示黄灯
		/*SetClassLong(hwndDlg,GCL_HICON,
			(LONG)LoadIcon((HINSTANCE)GetWindowLong(hwndDlg,GWL_HINSTANCE),(LPCTSTR)IDI_ICON1));*/
		//全局状态置假
		bAiming = FALSE;
		if(m_hTargetWnd!=NULL)
			::SendMessage(m_hTargetWnd,MSG_SLECTEDWND,(WPARAM)hOld,NULL);
	}
	CButton::OnLButtonUp(nFlags, point);
}


 

 

UINT CBtnSelectWnd::OnGetDlgCode()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	static BOOL b = TRUE;
	if(b)
	{
		ModifyStyle(0,BS_BITMAP);
		SetBitmap( LoadBitmap(AfxGetInstanceHandle(),   
			MAKEINTRESOURCE(IDB_BMP_SELECTWND)));
		b = FALSE;
	}
	//::SendMessage(m_hWnd,STM_SETIMAGE,IMAGE_BITMAP,
	//	(LPARAM)LoadBitmap((HINSTANCE)GetWindowLong(m_hWnd,GWL_HINSTANCE),MAKEINTRESOURCE(IDB_BMP_SELECTWND2)));

	return CButton::OnGetDlgCode();
}
