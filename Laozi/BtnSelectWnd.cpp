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
 
void CBtnSelectWnd::InvertDrawFrame(HWND hWndPoint) //�������ڵı߿�
{
	if(hWndPoint<0)
		return ;

	RECT rectFrame;
	//���ڵ�λ��
	::GetWindowRect(hWndPoint,&rectFrame);
	//Map the screen coordinate to the current window.
	//���������ϵ���Ϊ��0��0���󣬴����ĸ��ǵ������
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
//������Ļ�ϵĵ㣬ȡ�øõ�����Ĵ��ھ��
HWND CBtnSelectWnd::GetWndFromPoint(POINT pt)
{
	HWND hwndFocus,hTmp;
	//��˵���ϵͳ����ʵ�����Ǹ�����
	//�����񻹵õ���һ�²����ҵ������õ����С���Ӵ���
	hwndFocus= ::WindowFromPoint(pt);

	//���Ǿ͵�����һ��
	::ScreenToClient(hwndFocus,&pt);
	hTmp = ChildWindowFromPointEx(hwndFocus,pt,CWP_ALL);

	while(hTmp != NULL && hTmp != hwndFocus)
	{//���������Ĵ����л��а����õ���Ӵ��ڣ����Ҹ��Ӵ��ڲ��ǵ�ǰ���ڵĻ���
		//������С��Χ
		hwndFocus = hTmp;
		::ScreenToClient(hwndFocus,&pt);
		hTmp = ChildWindowFromPointEx(hwndFocus,pt,CWP_ALL);
	}

	return hwndFocus;
}
void CBtnSelectWnd::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

	if(bAiming)
	{
		POINT pt;  //���λ��
		GetCursorPos(&pt); //screen pos
		HWND hWndPoint = this->GetWndFromPoint(pt);//�õ��������Ĵ��ھ��
		if(hOld != hWndPoint && hWndPoint != m_hWnd)//�Ƿ��ǾɵĴ��ھ��
		{
			//�����굽���´��ڵĻ�

			//ˢ���ɴ��ڵı�
			InvertDrawFrame(hOld);//Refresh the invalid frame.
			//���¾ɴ��ھ��
			hOld = hWndPoint;
			//��������ָ���´��ڵ��߳�ID
			threadId = GetWindowThreadProcessId(hOld,NULL);
			//���������ڵ���Ϣ��ʾ
			//InvalidateRect(hwndDlg,NULL,1);//Refresh the text output of the dialog.
			/////////////////draw frame
			//�����´��ڱ߿�
			InvertDrawFrame(hWndPoint);
			if(m_hTargetWnd!=NULL)
				::SendMessage(m_hTargetWnd,MSG_SLECTEWND,(WPARAM)hWndPoint,NULL);
		}
	}
	CButton::OnMouseMove(nFlags, point);
}


void CBtnSelectWnd::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	RECT rectCtrl;
	
	POINT pt;  //�����λ��
	::GetCursorPos(&pt);
	HWND hwnd = AfxGetMainWnd()->GetSafeHwnd();
	//if(pt.x>rectCtrl.left && pt.x<rectCtrl.right //������������а���ͼ��
	//	&& pt.y>rectCtrl.top && pt.y<rectCtrl.bottom)//hit bitmap
	{
		//�ָ���ʼ״̬
		hOld = NULL;
		threadId = 0;
		//show red point 
		::SetClassLong(hwnd,GCL_HICON,
			(LONG)LoadIcon((HINSTANCE)GetWindowLong(hwnd,GWL_HINSTANCE),(LPCTSTR)IDB_BMP_SELECTWND));
		//setCursor to own cursor
		//��������Ϊ����
		::SetCursor(
			LoadCursor(
			(HINSTANCE)GetWindowLong(hwnd,GWL_HINSTANCE),
			(LPCTSTR)IDC_CURSOR_CHOOSEWND
			)
			);
		//Set blank bitmap to IDC_TARGET
		//��ԭ���������еİ����óɿհ�ͼ
		::SendMessage(::GetDlgItem(m_hWnd,IDB_BMP_SELECTWND),STM_SETIMAGE,IMAGE_BITMAP,
			(LPARAM)LoadBitmap((HINSTANCE)GetWindowLong(m_hWnd,GWL_HINSTANCE),MAKEINTRESOURCE(IDB_BMP_SELECTWND2)));
		HBITMAP hBmp=::LoadBitmap(AfxGetInstanceHandle(),  MAKEINTRESOURCE(IDB_BMP_SELECTWND2));   
		SetBitmap(hBmp);
		if(m_hTargetWnd!=NULL)
			::SendMessage(m_hTargetWnd,MSG_SLECTEWNDBegin,NULL,NULL);
		//capture mouse
		::SetCapture(hwnd);
		//set aiming state
		//��������סʱ��Ϊaiming state = true
		bAiming = TRUE;
	}
	::InvalidateRect(hwnd,NULL,1);
	CButton::OnLButtonDown(nFlags, point);
}


void CBtnSelectWnd::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	if(bAiming)
	{
		//�������һ�����ڵı߿�
		InvertDrawFrame(hOld);//erase the last frame
		//�ͷ����
		::ReleaseCapture();

		//��ԭ���ָ��
		::SetCursor(LoadCursor(NULL,IDC_ARROW));
		//��ԭ�������еİ���ͼ��
	/*	::SendMessage(::GetDlgItem(m_hWnd,IDB_BMP_SELECTWND),STM_SETIMAGE,IMAGE_BITMAP,
			(LPARAM)LoadBitmap((HINSTANCE)GetWindowLong(m_hWnd,GWL_HINSTANCE),MAKEINTRESOURCE(IDB_BMP_SELECTWND2)));
		*/
		HBITMAP hBmp=::LoadBitmap(AfxGetInstanceHandle(),  MAKEINTRESOURCE(IDB_BMP_SELECTWND));   
		SetBitmap(hBmp);
		//�����ڱ�����ʾ�Ƶ�
		/*SetClassLong(hwndDlg,GCL_HICON,
			(LONG)LoadIcon((HINSTANCE)GetWindowLong(hwndDlg,GWL_HINSTANCE),(LPCTSTR)IDI_ICON1));*/
		//ȫ��״̬�ü�
		bAiming = FALSE;
		if(m_hTargetWnd!=NULL)
			::SendMessage(m_hTargetWnd,MSG_SLECTEDWND,(WPARAM)hOld,NULL);
	}
	CButton::OnLButtonUp(nFlags, point);
}


 

 

UINT CBtnSelectWnd::OnGetDlgCode()
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
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
