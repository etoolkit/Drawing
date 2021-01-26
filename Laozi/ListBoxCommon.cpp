#include "StdAfx.h"
#include "ListBoxCommon.h"
#include "Resource.h"

#define IDC_BUTTON_DeleteListItem 8888
CListBoxCommon::CListBoxCommon(void)
{
	m_CurrentIndex = -1;
}


CListBoxCommon::~CListBoxCommon(void)
{
}

BEGIN_MESSAGE_MAP(CListBoxCommon, CListBox)
	//{{AFX_MSG_MAP(COwnerDrawListBox) 
	ON_NOTIFY_EX_RANGE(TTN_NEEDTEXTW, 0, 0xFFFF, OnToolTipText)
	ON_NOTIFY_EX_RANGE(TTN_NEEDTEXTA, 0, 0xFFFF, OnToolTipText)
	ON_BN_CLICKED(IDC_BUTTON_DeleteListItem, OnBnDelClicked)
	//}}AFX_MSG_MAP
	ON_WM_MOUSEMOVE() 
	ON_WM_MOUSEWHEEL()
	ON_WM_LBUTTONDOWN()
	ON_WM_CTLCOLOR()
	ON_WM_CTLCOLOR_REFLECT()
	ON_WM_DROPFILES()
	ON_WM_DRAWITEM()
END_MESSAGE_MAP()

void CListBoxCommon::PreSubclassWindow() 
{
	// TODO: Add your specialized code here and/or call the base class
	ModifyStyle(0,WS_CLIPCHILDREN);
	CRect rc;
	GetClientRect(&rc);
	rc.bottom -= 16;
	rc.top=rc.bottom-16;

	m_btnDel.Create(L" ",SS_CENTER,rc,this,IDC_BUTTON_DeleteListItem);// may ID=2000

	//m_btnDel.SetBitmap((HBITMAP) ::LoadImage(::AfxGetInstanceHandle(),MAKEINTRESOURCE(IDB_BITMAP_DeleteItem),IMAGE_BITMAP,24,24,0));
	m_btnDel.SetImage(IDB_BITMAP_Delete_1,IDB_BITMAP_Delete_2,IDB_BITMAP_Delete_2,IDB_BITMAP_Delete_1);
//	m_btnDel.SetImage(L"image\\delete_1.bmp",L"image\\delete_2.bmp",L"image\\delete_2.bmp",L"image\\delete_1.bmp",18,18);
	m_btnDel.ShowWindow(SW_HIDE);
	EnableToolTips(TRUE);

	m_tt.Create(this);
	m_tt.Activate(TRUE);
	CWnd * pw = GetDlgItem(IDC_BUTTON_DeleteListItem);
	m_tt.AddTool(pw,L"删除该项");
	CListBox::PreSubclassWindow();
}
int CListBoxCommon::OnToolHitTest(CPoint point, TOOLINFO * pTI) const
{
	int row;
	RECT cellrect;   // cellrect		- to hold the bounding rect
	BOOL tmp = FALSE;
	row  = ItemFromPoint(point,tmp);  //we call the ItemFromPoint function to determine the row,
	//note that in NT this function may fail  use the ItemFromPointNT member function

	if ( row == -1 ) 
		return -1;

	//set up the TOOLINFO structure. GetItemRect(row,&cellrect);
	GetItemRect(row,&cellrect);
	pTI->rect = cellrect;
	pTI->hwnd = m_hWnd;
	pTI->uId = (UINT)((row));   //The 憉Id?is assigned a value according to the row value.
	pTI->lpszText = LPSTR_TEXTCALLBACK;

	return pTI->uId;

}

BOOL CListBoxCommon::OnToolTipText( UINT id, NMHDR * pNMHDR, LRESULT * pResult )
{
	// need to handle both ANSI and UNICODE versions of the message
	TOOLTIPTEXTA* pTTTA = (TOOLTIPTEXTA*)pNMHDR;
	TOOLTIPTEXTW* pTTTW = (TOOLTIPTEXTW*)pNMHDR;
	CString strTipText;
	UINT nID = pNMHDR->idFrom;

	 
	 GetText(nID,strTipText);
	//GetText( nID ,strTipText);

	SetDelBtn(nID);

	m_CurrentIndex = nID;

#ifndef _UNICODE
	if (pNMHDR->code == TTN_NEEDTEXTA)
		lstrcpyn(pTTTA->szText, strTipText, 80);
	else
		_mbstowcsz(pTTTW->szText, strTipText, 80);
#else
	if (pNMHDR->code == TTN_NEEDTEXTA)
		_wcstombsz(pTTTA->szText, strTipText, 80);
	else
		lstrcpyn(pTTTW->szText, strTipText, 80);
#endif
	*pResult = 0;

	return TRUE;    
}

void CListBoxCommon::SetDelBtn( int nID)
{
	if ( nID == -1 ) 
	{
		if(m_btnDel.IsWindowVisible())
			m_btnDel.ShowWindow(SW_HIDE);
	}
	else
	{
		if(nID !=m_CurrentIndex)
		{	
			RECT rc; 
			GetItemRect(nID,&rc);	 
			RECT rc2;
			m_btnDel.GetWindowRect(&rc2);
			int h = rc2.bottom - rc2.top;
			h = 18;
			//cellrect.right += rc.right;	cellrect.bottom += rc.bottom;
			//if(point.x>rc.left&& point.x<rc.right&&point.y>rc.top&&point.y<rc.bottom)
			{
				m_btnDel.MoveWindow(rc.right -30,rc.top + (rc.bottom-rc.top)/2-h/2,h,h);
				m_btnDel.ShowWindow(SW_SHOW);
			}

		}
	}
}
void CListBoxCommon::OnBnDelClicked()
{	
	if(m_CurrentIndex>-1)
	{
		/*void *d = (void*) GetItemDataPtr(m_CurrentIndex);
		if(d )
		{
		delete d ;
		d  = NULL;
		}*/
		::SendMessage(GetParent()->m_hWnd, ListBoxCommonDelete, m_CurrentIndex,NULL);
		this->DeleteString(m_CurrentIndex);
	}
	m_CurrentIndex = -1;
	m_btnDel.ShowWindow(SW_HIDE);
}

void CListBoxCommon::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	int row;
	RECT rc;   // cellrect		- to hold the bounding rect
	BOOL tmp = FALSE;
	row  = ItemFromPoint(point,tmp);  //we call the ItemFromPoint function to determine the row,
	//note that in NT this function may fail  use the ItemFromPointNT member function
	 
	/*if ( row == -1 ) 
	{
		if(m_btnDel.IsWindowVisible())
			m_btnDel.ShowWindow(SW_HIDE);
	}*/
	//else
	//{
	//	if(row !=m_CurrentIndex)
	//	{	
	//		GetItemRect(row,&rc);	
	//		//cellrect.right += rc.right;	cellrect.bottom += rc.bottom;
	//		if(point.x>rc.left&& point.x<rc.right&&point.y>rc.top&&point.y<rc.bottom)
	//		{
	//			m_btnDel.MoveWindow(rc.right -30,rc.top + (rc.bottom-rc.top)/2-14,28,28);
	//			m_btnDel.ShowWindow(SW_SHOW);
	//		}
	//		else
	//		{
	//			m_CurrentIndex = -1;
	//			if(m_btnDel.IsWindowVisible())
	//				m_btnDel.ShowWindow(SW_HIDE);
	//		}
	//	}
	//}

	//m_CurrentIndex = row;

	GetItemRect(row,&rc);	
	//cellrect.right += rc.right;	cellrect.bottom += rc.bottom;
	if(point.x>rc.left&& point.x<rc.right&&point.y>rc.top&&point.y<rc.bottom)
	{
		
	}
	else
	{
		m_CurrentIndex = -1;
		if(m_btnDel.IsWindowVisible())
			m_btnDel.ShowWindow(SW_HIDE);
	}
	CListBox::OnMouseMove(nFlags, point);
}


BOOL CListBoxCommon::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
	if (NULL != m_tt.GetSafeHwnd())
	{
		m_tt.RelayEvent(pMsg);
	}
	
	return CListBox::PreTranslateMessage(pMsg);
}
BOOL CListBoxCommon::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	 
	SetTimer(100, 500, NULL); //100ms
	if(m_btnDel.IsWindowVisible())
		m_btnDel.ShowWindow(SW_HIDE);
	int n = GetSelCount();
	n = GetCurSel();

	SetSel(GetCurSel(),FALSE);

	return CListBox::OnMouseWheel(nFlags, zDelta, pt);
}



void CListBoxCommon::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	RECT rc;   // cellrect		- to hold the bounding rect
	BOOL tmp = FALSE;
	int n  = ItemFromPoint(point,tmp);
	SetDelBtn(n);
	m_CurrentIndex = n;
	CListBox::OnLButtonDown(nFlags, point);
}


HBRUSH CListBoxCommon::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CListBox::OnCtlColor(pDC, pWnd, nCtlColor);

	
	// TODO:  如果默认的不是所需画笔，则返回另一个画笔
	return hbr;
}


HBRUSH CListBoxCommon::CtlColor(CDC* pDC, UINT /*nCtlColor*/)
{
	// TODO:  在此更改 DC 的任何特性
	if(EnableWindow())
		pDC->SetBkColor (RGB(255,255,255)); 
	else
		pDC->SetBkColor (RGB(230,235,23)); 
	// TODO:  如果不应调用父级的处理程序，则返回非 null 画笔

	return NULL;
}

void CListBoxCommon::SetHScroll()
{
	CDC* dc = GetDC();
	SIZE s;
	int i;
	TCHAR str[1024];
	HDC hdc = dc->m_hDC; // 获得相关设备句柄
	for(i = 0; i < GetCount(); i++)
	{
		GetText(i, str);
		GetTextExtentPoint32(hdc, str, lstrlen(str), &s); // 获取字符串的像素大小
		// 如果新的字符串宽度大于先前的水平滚动条宽度，则重新设置滚动条宽度
		// IDC_LISTBOX为m_listBox的资源ID
	/*	if (s.cx > (LONG)SendDlgItemMessage(IDC_LISTBOX, LB_GETHORIZONTALEXTENT, 0, 0))
			SendDlgItemMessage(IDC_LISTBOX, LB_SETHORIZONTALEXTENT, (WPARAM)s.cx, 0);*/
		if (s.cx > (LONG)SendMessage(LB_GETHORIZONTALEXTENT, 0, 0))
			SendMessage(LB_SETHORIZONTALEXTENT, (WPARAM)s.cx, 0);
	}
	ReleaseDC(dc);
}

void CListBoxCommon::OnDropFiles(HDROP hDropInfo)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	CListBox::OnDropFiles(hDropInfo);
}


void CListBoxCommon::OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CDC dc;  
	dc.Attach(lpDrawItemStruct->hDC);  

	// Save these value to restore them when done drawing.  
	COLORREF crOldTextColor = dc.GetTextColor();  
	COLORREF crOldBkColor = dc.GetBkColor();  

	// If this item is selected, set the background color   
	// and the text color to appropriate values. Also, erase  
	// rect by filling it with the background color.  
	if ((lpDrawItemStruct->itemAction | ODA_SELECT) &&  
		(lpDrawItemStruct->itemState & ODS_SELECTED))  
	{  
		dc.SetTextColor(RGB(20,20,20));  
		dc.SetBkColor(RGB(220,220,237));  
		dc.FillSolidRect(&lpDrawItemStruct->rcItem, RGB(220,220,237));  
	}  
	else  
	{  
		dc.SetTextColor(RGB(255,255,255));  
		dc.SetBkColor(RGB(50,50,250));  
		dc.FillSolidRect(&lpDrawItemStruct->rcItem, RGB(50,50,250));  
	}  

	lpDrawItemStruct->rcItem.left += 5;  
	// Draw the text.  
	/*if(lpDrawItemStruct->itemID <= GetCount()-1)
	{
		CString text;GetText(lpDrawItemStruct->itemID,text);
		dc.DrawText(text, text.GetLength(), &lpDrawItemStruct->rcItem, DT_WORDBREAK);  
	}*/
	// Reset the background color and the text color back to their  
	// original values.  
	//dc.SetTextColor(crOldTextColor);  
	//dc.SetBkColor(crOldBkColor);  

	dc.Detach();   
	CListBox::OnDrawItem(nIDCtl, lpDrawItemStruct);
}
