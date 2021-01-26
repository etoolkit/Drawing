#pragma once
#include "DlgPopBase.h"
#include "DlgMainBase.h"


// CDlgBossKey �Ի���

class CDlgBossKey : public CDlgPopBase
{
	DECLARE_DYNAMIC(CDlgBossKey)

public:
	CDlgBossKey(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgBossKey();

// �Ի�������
	enum { IDD = IDD_DIALOG2 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	CString m_sHotKey;
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnPaint();
};
