
// LaoziDlg.h : ͷ�ļ�
//

#pragma once

#include "../MyHookDll/Myhook.h"
#include "MyWinMgr.h"
#include "MsgProcess.h"
#include "afxwin.h"
#include "SkinBtn.h"
#include "DlgMainPan.h"
#include "DlgFileWatch.h"
#include "Weather.h"
#include "MouseSignal.h"


// CLaoziDlg �Ի���
class CLaoziDlg : public CDialogEx
{
// ����
public:
	CLaoziDlg(CWnd* pParent = NULL);	// ��׼���캯��
	~CLaoziDlg();
// �Ի�������
	enum { IDD = IDD_LAOZI_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;
	
	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();

private:
	UINT HotKeyId;
	UINT HotKeyId2;
	CMyWinMgr m_winMgr;
	CMsgProcess m_MsgProcess;	
	BOOL m_bShowTaskbar; 
	
	CBitmap m_BitmapBack[3];
	CSize   m_szBitmapBack[3];
	HBRUSH m_brushBkrectMid;
	HBRUSH m_brushBkrect;
	ULONG_PTR m_nGdiplusToken;
	long     _icur;        // ��ǰMOUSE״̬
	int m_CurSel;
	CDlgMainPan* m_pDlgMain;
	CDlgFileWatch* m_pDlgWatch;
	CToolTipCtrl m_tt;
	CRect m_rcClient;
	CMouseSignal m_mouseSignal;
	

	CWeather m_weather;
	void RegeistHKey();
	void SetBackImage(int index,UINT id);
	void DrawRangeImage(CBitmap *pBitmap, CDC *pDC, CRect rc);
	void SetImageSize(CBitmap *pBitmap, CSize &sz);
	void SetImage(CBitmap &bitmap, UINT ID);
	void ChangeWindowRgn(CDC *pDC);
	void OnChangeSelDlg();
	void SaveCfg(); 
public:
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnBnClickedCancel();
	afx_msg LRESULT OnHotKey(WPARAM wParam, LPARAM lParam);
	afx_msg void OnClose();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);	
	DECLARE_MESSAGE_MAP()
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	afx_msg void OnMainShowHide();
	afx_msg void OnSysExit();
	afx_msg void OnBnClickedButtonMainPan();
	CSkinBtn m_ConfBtn;
	CSkinBtn m_MinBtn;
	CSkinBtn m_CloseBtn;
	CSkinBtn m_MainPanBtn;
	CSkinBtn m_MainPanBtn1;
	CSkinBtn m_MainPanBtn2;
	CSkinBtn m_MainPanBtn3;
	CSkinBtn m_MainPanBtn4;
	afx_msg LRESULT OnNcHitTest(CPoint point);
	afx_msg void OnBnClickedButtonMin();
	afx_msg void OnBnClickedButtonClose();
	afx_msg void OnBnClickedConfig();
	afx_msg void OnBnClickedButtonMainPan2();
	afx_msg void OnBnClickedButtonMainPan3();
	afx_msg void OnBnClickedButtonMainPan4();
	afx_msg void OnBnClickedButtonMainPan5();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnAboutbox();
	afx_msg void OnMenuRegist();
	afx_msg void OnMenuhelp();
};


