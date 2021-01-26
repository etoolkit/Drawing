#pragma once
#include "DlgPopBase.h"


// CDlgHelp 对话框

class CDlgHelp : public CDlgPopBase
{
	DECLARE_DYNAMIC(CDlgHelp)

public:
	CDlgHelp(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgHelp();

// 对话框数据
	enum { IDD = IDD_DIALOG_Help };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg LRESULT OnNcHitTest(CPoint point);
	afx_msg void OnBnClickedButtonClose();
	virtual BOOL OnInitDialog();
};
