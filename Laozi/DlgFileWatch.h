#pragma once
#include "Resource.h"
#include "..\Dirwatcher\DirectoryChanges.h"
#include "afxwin.h"
#include "ListBoxCommon.h"
#include "MyConfig.h"
#include "DlgMainBase.h"


// CDlgFileWatch 对话框
class CDirectoryChangeHandler_ListBox : public CDirectoryChangeHandler
{
public:
	CDirectoryChangeHandler_ListBox(CListBoxCommon & list_box)
		: CDirectoryChangeHandler(),
		m_listBox( list_box ){}
protected:
	//These functions are called when the directory to watch has had a change made to it
	void On_FileAdded(const CString & strFileName)
	{
		SYSTEMTIME sys; 
		GetLocalTime( &sys ); 
		CString s; 
		if(GetFileAttributes (strFileName) == FILE_ATTRIBUTE_DIRECTORY)
			s.Format(L"%2d/%02d/%02d %02d:%02d:%02d 新增文件:%s",sys.wYear,sys.wMonth,sys.wDay,sys.wHour,sys.wMinute, sys.wSecond, strFileName);
		else
			s.Format(L"%2d/%02d/%02d %02d:%02d:%02d 新增文件:%s",sys.wYear,sys.wMonth,sys.wDay,sys.wHour,sys.wMinute, sys.wSecond, strFileName);

		m_listBox.AddString( s);
		m_listBox.SetHScroll();
	}
	void On_FileRemoved(const CString & strFileName)
	{
		SYSTEMTIME sys; 
		GetLocalTime( &sys ); 
		CString s; 
		int a = GetFileAttributes (strFileName);
		if(GetFileAttributes (strFileName) == FILE_ATTRIBUTE_DIRECTORY)
			s.Format(L"%2d/%02d/%02d %02d:%02d:%02d 删除文件:%s",sys.wYear,sys.wMonth,sys.wDay,sys.wHour,sys.wMinute, sys.wSecond, strFileName);
		else
			s.Format(L"%2d/%02d/%02d %02d:%02d:%02d 删除文件:%s",sys.wYear,sys.wMonth,sys.wDay,sys.wHour,sys.wMinute, sys.wSecond, strFileName);

		m_listBox.AddString( s);	
		m_listBox.SetHScroll();
	}
	void On_FileModified(const CString & strFileName)
	{
		SYSTEMTIME sys; 
		GetLocalTime( &sys ); 
		CString s; 
; 
		if(GetFileAttributes (strFileName) == FILE_ATTRIBUTE_DIRECTORY)
		{
			return;
			s.Format(L"%2d/%02d/%02d %02d:%02d:%02d 修改文件:%s",sys.wYear,sys.wMonth,sys.wDay,sys.wHour,sys.wMinute, sys.wSecond, strFileName);
		}
		else
			s.Format(L"%2d/%02d/%02d %02d:%02d:%02d 修改文件:%s",sys.wYear,sys.wMonth,sys.wDay,sys.wHour,sys.wMinute, sys.wSecond, strFileName);

		m_listBox.AddString( s);
		m_listBox.SetHScroll();
	}
	void On_FileNameChanged(const CString & strOldFileName, const CString & strNewFileName)
	{
		SYSTEMTIME sys; 
		GetLocalTime( &sys ); 
		CString s; 
		if(GetFileAttributes (strNewFileName) == FILE_ATTRIBUTE_DIRECTORY)
			s.Format(L"%2d/%02d/%02d %02d:%02d:%02d 文件重命名:%s -> %s",sys.wYear,sys.wMonth,sys.wDay,sys.wHour,sys.wMinute, sys.wSecond, strOldFileName, strNewFileName);
		else
			s.Format(L"%2d/%02d/%02d %02d:%02d:%02d 文件重命名:%s -> %s",sys.wYear,sys.wMonth,sys.wDay,sys.wHour,sys.wMinute, sys.wSecond, strOldFileName, strNewFileName);

		m_listBox.AddString( s);	
		m_listBox.SetHScroll();
	}

	void On_WatchStarted(DWORD dwError, const CString & strDirectoryName)
	{
		SYSTEMTIME sys; 
		GetLocalTime( &sys ); 
		CString s; 	
		if( dwError == 0 )
			s.Format(L"%2d/%02d/%02d %02d:%02d:%02d 开始监控:%s",sys.wYear,sys.wMonth,sys.wDay,sys.wHour,sys.wMinute, sys.wSecond, strDirectoryName);
		else
			s.Format(L"%2d/%02d/%02d %02d:%02d:%02d 监控失败:%s",sys.wYear,sys.wMonth,sys.wDay,sys.wHour,sys.wMinute, sys.wSecond, strDirectoryName);
		m_listBox.AddString( s);	
		m_listBox.SetHScroll();
	}
	void On_WatchStopped(const CString & strDirectoryName)
	{
		//
		//	NOTE:  Be careful ! On_WatchStopped can be called AFTER this list box has
		//	been destoroyed.  Be carefull whenever interacting w/ windows with this function
		//	because the window may not be valid when this function is executed for the last time.
		//
		//
		SYSTEMTIME sys; 
		GetLocalTime( &sys ); 
		CString s; 		
		if( ::IsWindow( m_listBox.GetSafeHwnd() ) )
		{
			s.Format(L"%2d/%02d/%02d %02d:%02d:%02d 停止监控:%s",sys.wYear,sys.wMonth,sys.wDay,sys.wHour,sys.wMinute, sys.wSecond, strDirectoryName);
			m_listBox.AddString( s);
			m_listBox.SetHScroll();
		}
		else
		{
			TRACE(_T("NOTE: m_listBox is not a valid window! This has probably been called as the result of program shutdown sometime after CDirWatcherDlg::OnBtnMonitor2() was used to watch a direcotry.\n"));
			TRACE(_T("NOTE: Be sure that you are careful in your implementation of this function to watch out for the same condition if you are dealing w/ windows.\n"));
			//			MessageBeep(static_cast<UINT>(-1));
			//			MessageBeep(static_cast<UINT>(-1));
			//			MessageBeep(static_cast<UINT>(-1));

			//
			//	See the comments in DirectoryChanges.h for the CDirectoryChangeHandler::On_WatchStopped() function.
			//
			//
		}

	}

	CListBoxCommon & m_listBox;

};
class CDlgFileWatch : public CDlgMainBase
{
	DECLARE_DYNAMIC(CDlgFileWatch)

public:
	CDlgFileWatch(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgFileWatch();

// 对话框数据
	enum { IDD = IDD_DLG_FileWatcher };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedBtnStartwatch();

private:
	WORD GetChangeFlags1();

	CDirectoryChangeWatcher			m_DirWatcher;
	CDirectoryChangeHandler_ListBox m_DirChangeHandler;
		CString	m_strDirectoryToMonitor;
		CString	m_strIncludeFilter1;
		CString	m_strExcludeFilter1;
			
	//CDirectoryChangeHandler_ListBox m_DirChangeHandler;
public:
	CListBoxCommon m_lstChanges;
	CListBoxCommon m_lstDirs;
	CSkinBtn m_btnDirSelect;
	int AddDir(CString dir);
	void GetCfg(WatchInfo &info);
	afx_msg void OnBnClickedBtnDirselect();
	afx_msg void OnBnClickedBtnstopwatch();
	CSkinBtn m_btnStartWatch;
	CSkinBtn m_btnStopWatch;
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedBtnClearlog();
	CSkinBtn m_btnClear;
	CSkinBtn m_btnExport;
	afx_msg void OnBnClickedBtnExprotlog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnDropFiles(HDROP hDropInfo);
	afx_msg void OnPaint();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
};
