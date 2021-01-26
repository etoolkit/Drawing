
// LaoziDlg.h : 头文件
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


// CLaoziDlg 对话框
class CLaoziDlg : public CDialogEx
{
// 构造
public:
	CLaoziDlg(CWnd* pParent = NULL);	// 标准构造函数
	~CLaoziDlg();
// 对话框数据
	enum { IDD = IDD_LAOZI_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;
	
	// 生成的消息映射函数
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
	long     _icur;        // 当前MOUSE状态
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


