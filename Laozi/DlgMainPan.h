#pragma once
#include "btnselectwnd.h"
#include "afxwin.h"
#include "OwnerDrawListBox.h"
#include "MyConfig.h"
#include "ListBoxCommon.h"
#include "SkinBtn.h"
#include "DlgMainBase.h"

// CDlgMainPan �Ի���

class CDlgMainPan : public CDlgMainBase
{
	DECLARE_DYNAMIC(CDlgMainPan)

public:
	CDlgMainPan(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgMainPan();

// �Ի�������
	enum { IDD = IDD_Dlg_MainPan };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	void SetStatus();
	
	DECLARE_MESSAGE_MAP()

protected:
	afx_msg void OnPaint();
	CBtnSelectWnd m_btnChooseWnd;

	CToolTipCtrl m_tt;
	CBrush  m_brush;
	CRect m_list2Rect; // IDC_LIST2 ListBox�ĳ�ʼλ��
	CDWordArray m_HideWndClsArr;
	void AddListProgram(LPCTSTR file);
	int  AddListWndClass(HideWndInfo* str);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
public:
	afx_msg void OnBnClickedButtonSelectprogram();
	afx_msg void OnBnClickedButtonChoosewnd();	
  void Destroy(); 	
	virtual BOOL OnWndMsg(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult);
	afx_msg void OnBnClickedBtnSelectwndok();
	afx_msg void OnBnClickedBtnSelectwndcancle();	
	afx_msg void OnBnClickedButtonHidewnd();	

	void GetCfg(guardInfo &info);
	void HideWnd();

public: 
	COwnerDrawListBox m_listProgram;
	BOOL m_bHideByWndCls;
	BOOL m_bHideByWndText;
	BOOL m_bHideTray;
	BOOL m_bHideByProcess;
	HWND m_hSelectedWnd;
	CListBoxCommon m_listHideWndCls;
	CEdit m_tbHideWndText;
	CSkinBtn m_btnSelectWndOk;
	CSkinBtn m_btnSelectWndCancel;
	CSkinBtn m_btnSelectProg;
	CSkinBtn m_btnHideWnd;
	CSkinBtn m_btnHotKey;
	CStatic m_lbSelectWnd;
	afx_msg void OnBnClickedCheckHidepro();
	afx_msg void OnBnClickedCheckHideseletedwnds();
	afx_msg void OnBnClickedCheckHidecontainstext();
	afx_msg void OnBnClickedBtnHotkey();
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	
};
