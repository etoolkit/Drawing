#pragma once
#include "Resource.h"
#include "SkinBtn.h"
#include "afxwin.h"


// CDlgPopBase 对话框

class CDlgPopBase : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgPopBase)

public:
	CDlgPopBase(CWnd* pParent = NULL);   // 标准构造函数
	CDlgPopBase(UINT nIDTemplate, CWnd *pParent = NULL);
	virtual ~CDlgPopBase();

// 对话框数据
	enum { IDD = IDD_DLG_PopBase };

protected:
	CToolTipCtrl m_tt;
	CBitmap m_BmpTop; 
	 afx_msg void OnBnCloseClicked();
public:
	afx_msg void OnPaint();
	virtual BOOL OnInitDialog();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	void DrawRangeImage(CBitmap *pBitmap, CDC *pDC, CRect rc);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	DECLARE_MESSAGE_MAP()
	afx_msg LRESULT OnNcHitTest(CPoint point);
	
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	CSkinBtn m_btnClose;
	afx_msg void OnBnClickedButtonClose();
	CButton m_btnTest;
	afx_msg void OnBnClickedButtonasdfas();
	CButton m_btndfdsadasd;
};
