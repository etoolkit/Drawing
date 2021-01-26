#pragma once
#define  MSG_SLECTEWND WM_USER+3020
#define  MSG_SLECTEDWND WM_USER+3021
#define  MSG_SLECTEWNDBegin WM_USER+3022
class CBtnSelectWnd : public CButton
{
public:

	void SetTargetHwnd(HWND hwnd);
	CBtnSelectWnd(void);
	~CBtnSelectWnd(void);
	
	DECLARE_MESSAGE_MAP()
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	
protected:
	void InvertDrawFrame(HWND hWndPoint); //反画窗口的边框
	
	HWND GetWndFromPoint(POINT pt); //根据屏幕上的点，取得该点下面的窗口句柄


protected:
	BOOL bAiming;
	HWND hOld;	 
	DWORD threadId;
	HWND m_hTargetWnd;
public:  
	afx_msg UINT OnGetDlgCode();
};

