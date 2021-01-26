// OwnerDrawListBox.cpp : implementation file
//

#include "stdafx.h"
#include "OwnerDrawListBox.h"
#include "Resource.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// COwnerDrawListBox
#define IDC_BUTTON_DeleteListItem 8888
COwnerDrawListBox::COwnerDrawListBox()
{
	bMouseWheel = FALSE;
	iconSize = 40;
}

COwnerDrawListBox::~COwnerDrawListBox()
{
	//Destroy();
}


BEGIN_MESSAGE_MAP(COwnerDrawListBox, CListBox)
	//{{AFX_MSG_MAP(COwnerDrawListBox)
	ON_WM_DROPFILES()
	ON_NOTIFY_EX_RANGE(TTN_NEEDTEXTW, 0, 0xFFFF, OnToolTipText)
	ON_NOTIFY_EX_RANGE(TTN_NEEDTEXTA, 0, 0xFFFF, OnToolTipText)
	ON_BN_CLICKED(IDC_BUTTON_DeleteListItem, OnBnDelClicked)
	//}}AFX_MSG_MAP
	ON_WM_MOUSEMOVE()
	ON_WM_TIMER()
	ON_WM_MOUSEWHEEL()
	ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COwnerDrawListBox message handlers

// Delete the Pointer which holds the List Box Data Items
void COwnerDrawListBox::Destroy()
{
	if(!GetSafeHwnd())
		return;
	int ItemCount;
	ItemCount = GetCount();

	if (ItemCount != 0)
		for (int i = 0; i < ItemCount; i++)
		{
			m_pListBoxDataItems = (DataItems*) GetItemDataPtr(i);
			delete m_pListBoxDataItems;
		}
}
CString COwnerDrawListBox::GetItemText(int nindex)
{
	DataItems* d = (DataItems*) GetItemDataPtr(nindex);
	return d->strItemData;
}

int COwnerDrawListBox::FindItem(CString text, CDWordArray *resultArr /* = NULL */)
{
	int n = 0;
	for (int i=0; i<GetCount(); i++)
	{
		if(GetItemText(i) == text)
		{
			n++;
			if(resultArr!=NULL)
				resultArr->Add(i);
		}
	}
	return n;
}
void COwnerDrawListBox::PreSubclassWindow() 
{
	// TODO: Add your specialized code here and/or call the base class
	ModifyStyle(0,WS_CLIPCHILDREN);
	CRect rc;
	GetClientRect(&rc);
	rc.bottom -= 16;
	rc.top=rc.bottom-16;

	m_btnDel.Create(L" ",SS_CENTER,rc,this,IDC_BUTTON_DeleteListItem);// may ID=2000
	 
	//m_btnDel.SetBitmap((HBITMAP) ::LoadImage(::AfxGetInstanceHandle(),MAKEINTRESOURCE(IDB_BITMAP_DeleteItem),IMAGE_BITMAP,24,24,0));
//	m_btnDel.SetImage(L"image\\delete_1.bmp",L"image\\delete_2.bmp",L"image\\delete_2.bmp",L"image\\delete_1.bmp",18,18);
	m_btnDel.SetImage(IDB_BITMAP_Delete_1,IDB_BITMAP_Delete_2,IDB_BITMAP_Delete_2,IDB_BITMAP_Delete_1);
	m_btnDel.ShowWindow(SW_HIDE);
	EnableToolTips(TRUE);
	
	m_tt.Create(this);
	m_tt.Activate(TRUE);
	CWnd * pw = GetDlgItem(IDC_BUTTON_DeleteListItem);
	m_tt.AddTool(pw,L"删除该项");
	CListBox::PreSubclassWindow();
}

int COwnerDrawListBox::OnToolHitTest(CPoint point, TOOLINFO * pTI) const
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

BOOL COwnerDrawListBox::OnToolTipText( UINT id, NMHDR * pNMHDR, LRESULT * pResult )
{
	// need to handle both ANSI and UNICODE versions of the message
	TOOLTIPTEXTA* pTTTA = (TOOLTIPTEXTA*)pNMHDR;
	TOOLTIPTEXTW* pTTTW = (TOOLTIPTEXTW*)pNMHDR;
	CString strTipText;
	UINT nID = pNMHDR->idFrom;

	DataItems* item= (DataItems*) GetItemDataPtr(nID);
	strTipText = item->strItemData;
	//GetText( nID ,strTipText);
 
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

void COwnerDrawListBox::SetDelBtn( int nID)
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

// This is where is handle the Drag and Drop Event
void COwnerDrawListBox::OnDropFiles(HDROP hDropInfo)
{
	TCHAR strFileName[MAX_PATH], strDrive[MAX_PATH], strDir[MAX_PATH], strExt[MAX_PATH];
	TCHAR szFilePath[MAX_PATH];

	::DragQueryFile (hDropInfo, 0, szFilePath, sizeof (szFilePath));

	HICON h = ::ExtractIcon(NULL,szFilePath,0);

	// This function splits the whole path into Drive, Dir, File Name and Extension
	::_wsplitpath(szFilePath, strDrive, strDir, strFileName, strExt);

	if (h)
	{
		// After getting the the Icon handle and the Icon file name,
		// Add them to the list box
		Add(strFileName,h);
	}

	SetCurSel(this->GetCount()-1);

	::DragFinish(hDropInfo);
}

void COwnerDrawListBox::MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct) 
{
	// Changes the Width and the Height of item in the list box	
	lpMeasureItemStruct->itemHeight = iconSize;
	RECT rc; 
	//GetItemRect(nID,&rc);	
//	lpMeasureItemStruct->itemWidth =rc.right-rc.le ;
}

void COwnerDrawListBox::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct) 
{

	if(GetCount()==0)
		return;

	CDC* pDC = CDC::FromHandle(lpDrawItemStruct->hDC);

	CRect *rectIcon = NULL, *rectText = NULL;

	COLORREF crOldTextColor = pDC->GetTextColor();
	COLORREF crOldBkColor = pDC->GetBkColor();

	m_rectListBoxItem = lpDrawItemStruct->rcItem;

	// Returns the required struct which holds the Data (Text ,Icon) to be drawn
	DataItems *pListDataItem = (DataItems *)GetItemDataPtr(lpDrawItemStruct->itemID);

	// Setting the Text and icon Position in the List Box

	// Checking if the User choose to set the text position as he supposed to do so,
	// then see whether if the user has set the icon to the right position or not
	// If yes the text and the icon rects are set to different position
	// if no the text and the icon rects are set to the same position

	// Checking if the User choose to set the Icon position as he supposed to do so,
	// then see whether if the user has set the text to the right position or not
	// If yes the text and the icon rects are set to different position
	// if no the text and the icon rects are set to the same position

	if (m_TextPos == ITEM_RIGHT )
	{
		if (m_IconPos == ITEM_LEFT)
		{
			// Set the Text on the right and the Icon on the left, if the user chooses so
			rectIcon = new CRect(m_rectListBoxItem.left,m_rectListBoxItem.top,m_rectListBoxItem.left+iconSize,m_rectListBoxItem.top+iconSize); 
			rectText = new CRect(rectIcon->right-iconSize,m_rectListBoxItem.top,m_rectListBoxItem.right-20,m_rectListBoxItem.bottom);	
		}else
		{
			// If the user set the icon position on the right as he set the text,
			// draw the text and the icon on the same side
			rectIcon = new CRect(m_rectListBoxItem.right - iconSize,m_rectListBoxItem.top,m_rectListBoxItem.right,m_rectListBoxItem.bottom);
			rectText = new CRect(m_rectListBoxItem.left+iconSize,m_rectListBoxItem.top,m_rectListBoxItem.right-20,m_rectListBoxItem.bottom);
		}


	}else if (m_TextPos == ITEM_LEFT )
	{
		if (m_IconPos == ITEM_RIGHT)
		{
			// Set the Text on the left and the Icon on the right, if the user chooses so
			rectText = new CRect(m_rectListBoxItem.left+10,m_rectListBoxItem.top,m_rectListBoxItem.left+100,m_rectListBoxItem.bottom);
			rectIcon = new CRect(rectText->right-iconSize,m_rectListBoxItem.top,m_rectListBoxItem.right,m_rectListBoxItem.bottom);
		}else
		{
			// If the user set the icon position on the left as he set the text,
			// draw the text and the icon on the same side
			rectText = new CRect(m_rectListBoxItem.left+iconSize,m_rectListBoxItem.top,m_rectListBoxItem.right+0,m_rectListBoxItem.bottom);
			rectIcon = new CRect(m_rectListBoxItem.left+0,m_rectListBoxItem.top,m_rectListBoxItem.left+iconSize,m_rectListBoxItem.bottom);
		}


	}else if ( m_IconPos == ITEM_RIGHT)
	{
		if ( m_TextPos == ITEM_LEFT)
		{
			// Set the Text on the left and the Icon on the right, if the user chooses so
			rectText = new CRect(m_rectListBoxItem.left+10,m_rectListBoxItem.top,m_rectListBoxItem.left+100,m_rectListBoxItem.bottom);
			rectIcon = new CRect(rectText->right,m_rectListBoxItem.top,m_rectListBoxItem.right,m_rectListBoxItem.bottom);
		}
		else
		{
			// If the user set the icon position on the left as he set the text,
			// draw the text and the icon on the same side
			rectIcon = new CRect(m_rectListBoxItem.left+100,m_rectListBoxItem.top,m_rectListBoxItem.right,m_rectListBoxItem.bottom);
			rectText = new CRect(m_rectListBoxItem.left+iconSize,m_rectListBoxItem.top,m_rectListBoxItem.right-20,m_rectListBoxItem.bottom);
		}
	}
	else if ( m_IconPos == ITEM_LEFT)
	{
		if ( m_TextPos == ITEM_RIGHT)
		{
			// Set the text on the right and the icon on the left, if the user chooses so
			rectIcon = new CRect(m_rectListBoxItem.left,m_rectListBoxItem.top,m_rectListBoxItem.left+iconSize,m_rectListBoxItem.top+iconSize); 
			rectText = new CRect(rectIcon->right-iconSize,m_rectListBoxItem.top,m_rectListBoxItem.right-20,m_rectListBoxItem.bottom);	
		}
		else
		{
			// If the user set the icon position on the right as he set the text,
			// draw the text and the icon on the same side
			rectText = new CRect(m_rectListBoxItem.left+10,m_rectListBoxItem.top,m_rectListBoxItem.left+100,m_rectListBoxItem.bottom);
			rectIcon = new CRect(m_rectListBoxItem.left+10,m_rectListBoxItem.top,m_rectListBoxItem.left+100,m_rectListBoxItem.bottom);

		}
	}
	else
	{
		// If the user didnt set the Icon or the Text positions, draw them to the default positions
		// The text default position is on the right and the Icon default position is on the left
		rectIcon = new CRect(m_rectListBoxItem.left,m_rectListBoxItem.top,m_rectListBoxItem.left+iconSize,m_rectListBoxItem.bottom); 
		rectText = new CRect(rectIcon->right-iconSize,m_rectListBoxItem.top,m_rectListBoxItem.right-20,m_rectListBoxItem.bottom);	
	}	


	// If the List box has the focus,
	// draw around the first item the focus rect, which is a black dotted rectangle
	if((lpDrawItemStruct->itemAction | ODA_FOCUS) &&
		(lpDrawItemStruct->itemState & ODS_FOCUS))

	{
		CRect rect(m_rectListBoxItem);
		pDC->DrawFocusRect(m_rectListBoxItem);

	}
	// If the user has selected an item from the list box, or the selection has been changed
	// draw the item of the list box and fill it with the color of selected item
	else if ((lpDrawItemStruct->itemAction | ODA_SELECT) &&
		(lpDrawItemStruct->itemState & ODS_SELECTED))

	{	
		CRect rect(m_rectListBoxItem);

		// Fill the item rect with the highlight blue color
		pDC->FillSolidRect(&lpDrawItemStruct->rcItem,::GetSysColor(COLOR_HIGHLIGHT));		
		// Set the color of the background of the text rect
		pDC->SetBkColor(::GetSysColor(COLOR_HIGHLIGHT));
		// Set the color of the text
		pDC->SetTextColor(::GetSysColor(COLOR_HIGHLIGHTTEXT));	

		// If the user has chosen to set the text and the icon position, draw both
		// If the user has chosen to set the text position only, draw the text only
		// If the user has chosen to set the icon position only, draw the icon only
		if ( m_TextPos == ITEM_LEFT )
		{
			pDC->DrawText(pListDataItem->strItemData,wcslen(pListDataItem->strItemData),rectText,DT_VCENTER|DT_SINGLELINE|DT_VCENTER);

			if (m_IconPos == ITEM_RIGHT)
			{
				pDC->DrawIcon(rectIcon->left,rectIcon->top,pListDataItem->hIcon);
			}
			else if (m_IconPos == ITEM_LEFT )
			{
				pDC->DrawIcon(rectIcon->left,rectIcon->top,pListDataItem->hIcon);
			}
		}
		else if (m_TextPos == ITEM_RIGHT)
		{
			pDC->DrawText(pListDataItem->strItemData,wcslen(pListDataItem->strItemData),rectText,DT_RIGHT|DT_SINGLELINE|DT_VCENTER);

			if(m_IconPos == ITEM_LEFT)
			{
				pDC->DrawIcon(rectIcon->left,rectIcon->top,pListDataItem->hIcon);
			}
			else if ( m_IconPos == ITEM_RIGHT )
			{
				pDC->DrawIcon(rectIcon->left,rectIcon->top,pListDataItem->hIcon);
			}
		}
		else if ( m_IconPos == ITEM_RIGHT )
		{
			pDC->DrawIcon(rectIcon->left,rectIcon->top,pListDataItem->hIcon);

			if ( m_TextPos == ITEM_LEFT)
				pDC->DrawText(pListDataItem->strItemData,wcslen(pListDataItem->strItemData),rectText,DT_VCENTER|DT_SINGLELINE|DT_VCENTER);
			else if ( m_TextPos == ITEM_RIGHT)
				pDC->DrawText(pListDataItem->strItemData,wcslen(pListDataItem->strItemData),rectText, DT_RIGHT|DT_SINGLELINE|DT_VCENTER);

		}
		else if ( m_IconPos == ITEM_LEFT)
		{
			pDC->DrawIcon(rectIcon->left,rectIcon->top,pListDataItem->hIcon);

			if ( m_TextPos == ITEM_RIGHT)
				pDC->DrawText(pListDataItem->strItemData,wcslen(pListDataItem->strItemData),rectText, DT_RIGHT|DT_SINGLELINE|DT_VCENTER);
			else if ( m_TextPos == ITEM_LEFT)
				pDC->DrawText(pListDataItem->strItemData,wcslen(pListDataItem->strItemData),rectText,DT_VCENTER|DT_SINGLELINE|DT_VCENTER);
		}
		else
		{
			pDC->DrawIcon(rectIcon->left,rectIcon->top,pListDataItem->hIcon);
			pDC->DrawText(pListDataItem->strItemData,wcslen(pListDataItem->strItemData),rectText, DT_RIGHT|DT_SINGLELINE|DT_VCENTER);
		}

	}
	// Draw the icons by the default colors as no item is selected
	else
	{
		// Fill the item rect with white
		pDC->FillSolidRect(&lpDrawItemStruct->rcItem, crOldBkColor);

		// If the user has chosen to set the text and the icon position, draw both
		// If the user has chosen to set the text position only, draw the text only
		// If the user has chosen to set the icon position only, draw the icon only

		if ( m_TextPos == ITEM_LEFT )
		{
			pDC->DrawText(pListDataItem->strItemData,wcslen(pListDataItem->strItemData),rectText,DT_VCENTER|DT_SINGLELINE|DT_VCENTER);

			if (m_IconPos == ITEM_RIGHT)
			{
				pDC->DrawIcon(rectIcon->left,rectIcon->top,pListDataItem->hIcon);
			}
			else if (m_IconPos == ITEM_LEFT )
			{
				pDC->DrawIcon(rectIcon->left,rectIcon->top,pListDataItem->hIcon);
			}
		}
		else if (m_TextPos == ITEM_RIGHT)
		{
			pDC->DrawText(pListDataItem->strItemData,wcslen(pListDataItem->strItemData),rectText,DT_RIGHT|DT_SINGLELINE|DT_VCENTER);

			if(m_IconPos == ITEM_LEFT)
			{
				pDC->DrawIcon(rectIcon->left,rectIcon->top,pListDataItem->hIcon);
			}
			else if ( m_IconPos == ITEM_RIGHT )
			{
				pDC->DrawIcon(rectIcon->left,rectIcon->top,pListDataItem->hIcon);
			}
		}
		else if ( m_IconPos == ITEM_RIGHT )
		{
			pDC->DrawIcon(rectIcon->left,rectIcon->top,pListDataItem->hIcon);

			if ( m_TextPos == ITEM_LEFT)
				pDC->DrawText(pListDataItem->strItemData,wcslen(pListDataItem->strItemData),rectText,DT_VCENTER|DT_SINGLELINE|DT_VCENTER);
			else if ( m_TextPos == ITEM_RIGHT)
				pDC->DrawText(pListDataItem->strItemData,wcslen(pListDataItem->strItemData),rectText, DT_RIGHT|DT_SINGLELINE|DT_VCENTER);

		}
		else if ( m_IconPos == ITEM_LEFT)
		{
			pDC->DrawIcon(rectIcon->left,rectIcon->top,pListDataItem->hIcon);

			if ( m_TextPos == ITEM_RIGHT)
				pDC->DrawText(pListDataItem->strItemData,wcslen(pListDataItem->strItemData),rectText, DT_RIGHT|DT_SINGLELINE|DT_VCENTER);
			else if ( m_TextPos == ITEM_LEFT)
				pDC->DrawText(pListDataItem->strItemData,wcslen(pListDataItem->strItemData),rectText,DT_VCENTER|DT_SINGLELINE|DT_VCENTER);
		}
		else
		{
			pDC->DrawIcon(rectIcon->left,rectIcon->top,pListDataItem->hIcon);
			pDC->DrawText(pListDataItem->strItemData,wcslen(pListDataItem->strItemData),rectText, DT_RIGHT|DT_SINGLELINE|DT_VCENTER);
		}


	}
	//if(pDC->GetSafeHdc())
	//	pDC->Detach();

	if(rectText)
		delete rectText;
	if(rectIcon)
		delete rectIcon;
}

void COwnerDrawListBox::SetTextPosition(Item_Pos Txt_Pos)
{
	m_TextPos = Txt_Pos;
}

void COwnerDrawListBox::SetIconPosition(Item_Pos Ico_Pos)
{
	m_IconPos = Ico_Pos;
}

void COwnerDrawListBox::Add(CString strItemName, HICON hDataIcon)
{
	m_pListBoxDataItems = new DataItems;
	//hDataIcon = SetIcon(hDataIcon,FALSE);
	m_pListBoxDataItems->hIcon = hDataIcon;
	m_pListBoxDataItems->strItemData = strItemName;

	int index = AddString(L"");
	SetItemDataPtr(index, m_pListBoxDataItems); 
}
void COwnerDrawListBox::OnBnDelClicked()
{	
	if(m_CurrentIndex>-1)
	{
		
		m_pListBoxDataItems = (DataItems*) GetItemDataPtr(m_CurrentIndex);
		if(m_pListBoxDataItems)
		{
			delete m_pListBoxDataItems;
			m_pListBoxDataItems = NULL;
		}
		this->DeleteString(m_CurrentIndex);
	}
	m_CurrentIndex = -1;
	m_btnDel.ShowWindow(SW_HIDE);
}

void COwnerDrawListBox::OnMouseMove(UINT nFlags, CPoint point)
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


BOOL COwnerDrawListBox::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
	if (NULL != m_tt.GetSafeHwnd())
	{
		m_tt.RelayEvent(pMsg);
	}
	
	return CListBox::PreTranslateMessage(pMsg);
}


void COwnerDrawListBox::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if(nIDEvent == 100)
	{
		bMouseWheel = FALSE;
		 KillTimer(100);
	}
	CListBox::OnTimer(nIDEvent);
}


BOOL COwnerDrawListBox::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	bMouseWheel = TRUE; 
	SetTimer(100, 500, NULL); //100ms
	if(m_btnDel.IsWindowVisible())
		m_btnDel.ShowWindow(SW_HIDE);
	int n = GetSelCount();
	n = GetCurSel();
	 
	SetSel(GetCurSel(),FALSE);
	 
	return CListBox::OnMouseWheel(nFlags, zDelta, pt);
}
void COwnerDrawListBox::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	RECT rc;   // cellrect		- to hold the bounding rect
	BOOL tmp = FALSE;
	int n  = ItemFromPoint(point,tmp);
	SetDelBtn(n);
	m_CurrentIndex = n;
	CListBox::OnLButtonDown(nFlags, point);
}