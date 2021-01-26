#pragma once
#include "XmlHelper.h"
#include <vector>
using namespace std;

struct HideWndInfo
{ 
	CString  clsName; 
	CString sTitle;
	CString sFileName;
	bool& operator == (HideWndInfo &info)
	{
		bool b = _tcscmp(clsName, info.clsName)==0 
			&&  _tcscmp(sTitle, info.sTitle)==0 
			&&  _tcscmp(sFileName, info.sFileName)==0 ;
		return b;
	};

};
 
struct guardInfo
{
	bool bHideByProcess;
	bool bHideByWndClass;
	bool bHideByWndText;
	bool bHideTray;
	bool bHideTaskbar;
	CStringArray hidedProcessNameArr;
	CStringArray hidedWndText;
	vector< HideWndInfo> hideWinCls;
	


	guardInfo()
	{
		bHideByProcess = true;
		bHideByWndClass = true;
		bHideByWndText = true;
		bHideTaskbar = true;
	};

	guardInfo& operator =(guardInfo &guard)
	{
		if(&guard == this)
			return *this;
		bHideByProcess = guard.bHideByProcess;
		bHideByWndClass = guard.bHideByWndClass;
		bHideByWndText = guard.bHideByWndText;
		bHideTaskbar = guard.bHideTaskbar;
		hidedProcessNameArr.Copy(guard.hidedProcessNameArr); 
		hidedWndText.Copy(guard.hidedWndText);
		hideWinCls = guard.hideWinCls;
		return *this;
	};
};
struct FileWatchInfo
{
	CStringArray watchDirArr;
	bool bSubDir;
	FileWatchInfo()
	{
		bSubDir = true;
	}
	FileWatchInfo& operator = (FileWatchInfo &info)
	{
		if(&info == this)
			return *this;
		bSubDir = info.bSubDir;
		watchDirArr.Copy(info.watchDirArr);
		return *this;
	};
};

struct WatchInfo
{
	FileWatchInfo fileWatch;
	WatchInfo& operator = (WatchInfo &info)
	{
		if(&info == this)
			return *this;
		fileWatch = info.fileWatch;
		return *this;
	};
};

class CMyConfig
{
public:
	CMyConfig(void);
	~CMyConfig(void);
public:
	guardInfo m_guardInfo;
	WatchInfo m_watchInfo;
	CString m_sHotKeyHideWnd;
	CString m_sCity;
	int ReadConfig(LPCTSTR path);
	int ReadXmlCfg(LPCTSTR fileName);
	
	int Save();

private:
	CXmlHelper m_xmlHelper;
	TCHAR m_xmlPath[MAX_PATH];
};

