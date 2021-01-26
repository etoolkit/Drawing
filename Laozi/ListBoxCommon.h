#pragma once
#include "SkinBtn.h"
#define  ListBoxCommonDelete 8002
class CListBoxCommon : public CListBox
{
public:
	CListBoxCommon(void);
	~CListBoxCommon(void);

public:
	/*virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	virtual void MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct);*/
	void SetHScroll();
	int OnToolHitTest(CPoint point, TOOLINFO * pTI) const;
	BOOL OnToolTipText( UINT id, NMHDR * pNMHDR, LRESULT * pResult );
protected:
	CSkinBtn m_btnDel;
	int m_CurrentIndex;
	CToolTipCtrl m_tt;
	void SetDelBtn(int nID);

	virtual void PreSubclassWindow();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnBnDelClicked();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg HBRUSH CtlColor(CDC* pDC, UINT /*nCtlColor*/);
	afx_msg void OnDropFiles(HDROP hDropInfo);
	afx_msg void OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct);
};

