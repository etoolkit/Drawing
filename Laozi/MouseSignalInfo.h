#pragma once
#include "Resource.h"
#define  GestureElmCount 8
#define WM_SIGNAL_INFO WM_USER+600
#define WM_INIT_SI WM_USER+700
// CMouseSignalInfo 对话框

class CMouseSignalInfo : public CDialogEx
{
	DECLARE_DYNAMIC(CMouseSignalInfo)
	enum EnumGesture
	{
		None=0,Left=1,Right=2,Up=3,Down=4
	};
	enum EnumEvent
	{
		NoneEvent,Close,Mini,Restore,ToRight, ToLeft, ToTop, ToBottom, ToCenter,Back,Redo,PageUp,PageDown,PageHome,PageEnd,CtrlTab,CtrlShiftTab,AltTab,AltShiftTab
	};
public:
	CMouseSignalInfo(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CMouseSignalInfo();

// 对话框数据
	enum { IDD = IDD_DIALOG_MouseSignalInfo };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	ULONG_PTR m_nGdiplusToken;
	CMouseSignalInfo::EnumGesture m_cMouseSignals[GestureElmCount];
	DECLARE_MESSAGE_MAP()
	afx_msg LRESULT    OnSignalInfo(WPARAM wParam,LPARAM lParam);
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
};
