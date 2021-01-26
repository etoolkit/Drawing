#pragma once
 #include <map>
#include "MouseSignalInfo.h"
 using namespace std;

class CMouseSignal:public CWnd
{
	
public:
	CMouseSignal(void);
	~CMouseSignal(void); 
   CMouseSignalInfo m_sigalInfo;
//DECLARE_MESSAGE_MAP()
	/*afx_msg*/ LRESULT    OnRMove(WPARAM wParam,LPARAM lParam);
	/*afx_msg*/ LRESULT    OnRPress(WPARAM wParam,LPARAM lParam);
	int SignalEvents(CMouseSignalInfo::EnumGesture* gesture);
protected:
	BOOL m_bHideSignal;
	short m_bSignalPos;
	CMouseSignalInfo::EnumGesture m_cMouseSignals[GestureElmCount];
	POINT m_pt;
	int		m_iSignalInfoStatus;
	COLORREF m_clr;
	BOOL m_bAutoSetting;
	UINT m_nLineWidth;
	map<CString,CMouseSignalInfo::EnumEvent> SignalEventSettings;
	ULONG_PTR m_nGdiplusToken;
	CString GetEventText(CMouseSignalInfo::EnumEvent e);
	CMouseSignalInfo::EnumEvent  GetEventByGesture(CMouseSignalInfo::EnumGesture  g[]);
	void AddGestureSet(CMouseSignalInfo::EnumEvent e, CMouseSignalInfo::EnumGesture s1 = CMouseSignalInfo::EnumGesture::None,
		CMouseSignalInfo::EnumGesture s2 = CMouseSignalInfo::EnumGesture::None,
		CMouseSignalInfo::EnumGesture s3 = CMouseSignalInfo::EnumGesture::None,
		CMouseSignalInfo::EnumGesture s4 = CMouseSignalInfo::EnumGesture::None,
		CMouseSignalInfo::EnumGesture s5 = CMouseSignalInfo::EnumGesture::None,
		CMouseSignalInfo::EnumGesture s6 = CMouseSignalInfo::EnumGesture::None,
		CMouseSignalInfo::EnumGesture s7 = CMouseSignalInfo::EnumGesture::None,
		CMouseSignalInfo::EnumGesture s8 = CMouseSignalInfo::EnumGesture::None);
public:
	DECLARE_MESSAGE_MAP()
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
};

