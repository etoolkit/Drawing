#pragma once
#include "Resource.h"


// CDlgMainBase 对话框

class CDlgMainBase : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgMainBase)

public:
	CDlgMainBase(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgMainBase();

// 对话框数据
	enum { IDD = IDD_DLG_MainBase };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL OnInitDialog();

protected:
	CToolTipCtrl m_tt;
		CBrush  m_brushBackground;
public:
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
};
