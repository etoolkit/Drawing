#pragma once
#include "Resource.h"


// CDlgMainBase �Ի���

class CDlgMainBase : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgMainBase)

public:
	CDlgMainBase(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgMainBase();

// �Ի�������
	enum { IDD = IDD_DLG_MainBase };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

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
