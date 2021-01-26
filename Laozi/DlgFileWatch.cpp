// DlgFileWatch.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "DlgFileWatch.h"
#include "afxdialogex.h"
#include "..\Dirwatcher\FolderDialog.h"
#include <afxdlgs.h>
#include "Laozi.h"


// CDlgFileWatch �Ի���

IMPLEMENT_DYNAMIC(CDlgFileWatch, CDlgMainBase)

	CDlgFileWatch::CDlgFileWatch(CWnd* pParent /*=NULL*/)
	: CDlgMainBase(),
	m_DirChangeHandler( m_lstChanges )
{

}

CDlgFileWatch::~CDlgFileWatch()
{
}
BOOL CDlgFileWatch::OnInitDialog()
{
	CDlgMainBase::OnInitDialog();
 
	m_tt.AddTool(GetDlgItem(IDC_BTN_DirSelect),L"ѡ��Ҫ���ӵ�Ŀ¼");
	m_tt.AddTool(GetDlgItem(IDC_BTN_StartWatch),L"��ʼ����");
	m_tt.AddTool(GetDlgItem(IDC_BTNStopWatch),L"ֹͣ����");
	m_tt.AddTool(GetDlgItem(IDC_BTN_ExprotLog),L"����");
	m_tt.AddTool(GetDlgItem(IDC_BTN_Clearlog),L"���");

	((CButton*)GetDlgItem(IDC_CHECK_SUBDIR1))->SetCheck(theApp.m_config.m_watchInfo.fileWatch.bSubDir);
	for (int i = 0; i<theApp.m_config.m_watchInfo.fileWatch.watchDirArr.GetCount(); i++)
	{
		AddDir(theApp.m_config.m_watchInfo.fileWatch.watchDirArr.GetAt(i));
	}

	m_btnStartWatch.EnableWindow(TRUE);
	m_btnStopWatch.EnableWindow(FALSE);
	CRect tempUp,rcLst;
	m_lstDirs.GetWindowRect(rcLst);
	ScreenToClient(rcLst);
	tempUp.left = rcLst.right - (18 + 18 +5  + 6);//21
	tempUp.right = tempUp.left + 18 ;//21
	tempUp.top = rcLst.top -18 -2 ;
	tempUp.bottom = rcLst.top - 2 ;
	m_btnStartWatch.SetWindowText(_T(""));
	m_btnStartWatch.MoveWindow(tempUp,true);

	tempUp.left = rcLst.right - (18  + 6);//21
	tempUp.right = tempUp.left  +  18 ;//21
	tempUp.top = rcLst.top - 18 -2 ;
	tempUp.bottom = rcLst.top - 2 ;
	m_btnStopWatch.SetWindowText(_T(""));
	m_btnStopWatch.MoveWindow(tempUp,true);

	m_btnDirSelect.GetWindowRect(tempUp);
	ScreenToClient(tempUp);
	tempUp.right = tempUp.left +24;
	tempUp.bottom = tempUp.top + 24;
	m_btnDirSelect.SetWindowText(_T(""));
	m_btnDirSelect.MoveWindow(tempUp,true);

	m_btnExport.GetWindowRect(tempUp);
	ScreenToClient(tempUp);
	tempUp.right = tempUp.left +18;
	tempUp.bottom = tempUp.top + 18;
	m_btnExport.SetWindowText(_T(""));
	m_btnExport.MoveWindow(tempUp,true);


	tempUp.left = tempUp.right + 6;
	tempUp.right = tempUp.left +18;
	tempUp.bottom = tempUp.top + 18;
	m_btnClear.SetWindowText(_T(""));
	m_btnClear.MoveWindow(tempUp,true);

	m_btnStartWatch.SetImage(IDB_BITMAP_Start1,IDB_BITMAP_Start2,IDB_BITMAP_Start2,IDB_BITMAP_Start3);
	m_btnStopWatch.SetImage(IDB_BITMAP_Stop1, IDB_BITMAP_Stop2,IDB_BITMAP_Stop2,IDB_BITMAP_Stop3);
	m_btnDirSelect.SetImage(IDB_BITMAP_Search1,IDB_BITMAP_Search2,IDB_BITMAP_Search2,IDB_BITMAP_Search1);
	m_btnClear.SetImage(IDB_BITMAP_Clear1,IDB_BITMAP_Clear2,IDB_BITMAP_Clear2,IDB_BITMAP_Clear1);
	m_btnExport.SetImage(IDB_BITMAP_Export1,IDB_BITMAP_Export2,IDB_BITMAP_Export2,IDB_BITMAP_Export1);
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

void CDlgFileWatch::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_List_Log, m_lstChanges);
	DDX_Control(pDX, IDC_LIST_WatchDir, m_lstDirs);
	DDX_Control(pDX, IDC_BTN_DirSelect, m_btnDirSelect);
	DDX_Control(pDX, IDC_BTN_StartWatch, m_btnStartWatch);
	DDX_Control(pDX, IDC_BTNStopWatch, m_btnStopWatch);
	DDX_Control(pDX, IDC_BTN_Clearlog, m_btnClear);
	DDX_Control(pDX, IDC_BTN_ExprotLog, m_btnExport);
}


BEGIN_MESSAGE_MAP(CDlgFileWatch, CDialogEx)
	ON_BN_CLICKED(IDC_BTN_StartWatch, &CDlgFileWatch::OnBnClickedBtnStartwatch)
	ON_BN_CLICKED(IDC_BTN_DirSelect, &CDlgFileWatch::OnBnClickedBtnDirselect)
	ON_BN_CLICKED(IDC_BTNStopWatch, &CDlgFileWatch::OnBnClickedBtnstopwatch)
	ON_BN_CLICKED(IDC_BTN_Clearlog, &CDlgFileWatch::OnBnClickedBtnClearlog)
	ON_BN_CLICKED(IDC_BTN_ExprotLog, &CDlgFileWatch::OnBnClickedBtnExprotlog) 
	ON_WM_DROPFILES()
	ON_WM_PAINT()
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()


CString GetLastErrorMessageString(DWORD dwLastError )
{
	LPVOID lpMsgBuf = NULL;
	FormatMessage( FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM, NULL,
		dwLastError, 
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
		(LPTSTR) &lpMsgBuf, 0, NULL );

	CString str = (LPCTSTR)lpMsgBuf;

	if( lpMsgBuf != NULL )
		LocalFree(lpMsgBuf);	

	return str;
}

WORD CDlgFileWatch::GetChangeFlags1()
{

	DWORD dwChangeNotifyFlags = 0UL;
	/*FLAG_INFO arFlagInfo[]={
	{IDC_CHECK_FILE_NAME1,		FILE_NOTIFY_CHANGE_FILE_NAME},
	{IDC_CHECK_DIR_NAME1,		FILE_NOTIFY_CHANGE_DIR_NAME},
	{IDC_CHECK_LAST_WRITE1,		FILE_NOTIFY_CHANGE_LAST_WRITE},
	{IDC_CHECK_LAST_ACCESS1,	FILE_NOTIFY_CHANGE_LAST_ACCESS},
	{IDC_CHECK_SIZE1,			FILE_NOTIFY_CHANGE_SIZE},
	{IDC_CHECK_ATTRIBUTES1,		FILE_NOTIFY_CHANGE_ATTRIBUTES},
	{IDC_CHECK_LAST_WRITE1,		FILE_NOTIFY_CHANGE_LAST_WRITE},
	{IDC_CHECK_LAST_ACCESS1,	FILE_NOTIFY_CHANGE_LAST_ACCESS},
	{IDC_CHECK_CREATION1,		FILE_NOTIFY_CHANGE_CREATION},
	{IDC_CHECK_SECURITY1,		FILE_NOTIFY_CHANGE_SECURITY}
	};

	for(int i = 0; i < sizeof(arFlagInfo)/sizeof(arFlagInfo[0]); ++i)
	{
	dwChangeNotifyFlags |= (IsDlgButtonChecked(arFlagInfo[i].nCtrlId) == BST_CHECKED)? arFlagInfo[i].nFlag : 0;
	}*/
	dwChangeNotifyFlags = FILE_NOTIFY_CHANGE_FILE_NAME|FILE_NOTIFY_CHANGE_DIR_NAME|FILE_NOTIFY_CHANGE_LAST_WRITE|
		FILE_NOTIFY_CHANGE_LAST_ACCESS|FILE_NOTIFY_CHANGE_SIZE|FILE_NOTIFY_CHANGE_ATTRIBUTES|FILE_NOTIFY_CHANGE_LAST_WRITE
		|FILE_NOTIFY_CHANGE_LAST_ACCESS|FILE_NOTIFY_CHANGE_CREATION|FILE_NOTIFY_CHANGE_SECURITY;
	return dwChangeNotifyFlags;
}

void CDlgFileWatch::GetCfg(WatchInfo &info)
{
	info.fileWatch.bSubDir = IsDlgButtonChecked(IDC_CHECK_SUBDIR1) == BST_CHECKED;
	info.fileWatch.watchDirArr.RemoveAll();
	for (int i=0; i<m_lstDirs.GetCount(); i++)
	{
		CString s; m_lstDirs.GetText(i,s);
		info.fileWatch.watchDirArr.Add(s);
	}
	
}

int CDlgFileWatch::AddDir(CString dir)
{
	int m = m_lstDirs.GetCount();
	for (int i=0;i<m;i++)
	{	
		CString s;m_lstDirs.GetText(i,s);
		if(s.CompareNoCase(dir) == 0) 
		{
			m_lstDirs.SetCurSel(i);
			return -1;
		}
	}
	int n = m_lstDirs.AddString(dir);
	m_lstDirs.SetHScroll();
	return n;
}

void CDlgFileWatch::OnBnClickedBtnStartwatch()
{

	DWORD dwChangeFilter = GetChangeFlags1();
	BOOL bWatchSubDir = (BOOL)(IsDlgButtonChecked(IDC_CHECK_SUBDIR1) == BST_CHECKED);
	m_DirWatcher.UnwatchAllDirectories();
	if( dwChangeFilter == 0 ){
		MessageBox(_T("You must select one or more change filters in order to monitor a directory"));
		return;
	}
	if( m_DirWatcher.GetFilterFlags() & CDirectoryChangeWatcher::FILTERS_DONT_USE_FILTERS )
	{
		m_strIncludeFilter1.Empty();
		m_strExcludeFilter1.Empty();
	}
	else
	{
		/*GetDlgItemText(IDC_EDIT_INCLUDE_FILTER1, m_strIncludeFilter1);
		GetDlgItemText(IDC_EDIT_EXCLUDE_FILTER1, m_strExcludeFilter1);
		*/
		if( m_strIncludeFilter1 == _T("*.*") )
		{
			MessageBox(_T("For performance reasons, don't use *.* as the include filter. An empty string means the same thing."));
			//SetDlgItemText(IDC_EDIT_INCLUDE_FILTER1, _T(""));
			m_strIncludeFilter1.Empty();
		}
	}
	if(m_lstDirs.GetCount() == 0)
	{
		MessageBox(_T("��ѡ��Ҫ���ӵ�Ŀ¼"));
		return;
	}
	for (int i = 0; i < m_lstDirs.GetCount(); i++)
	{
		CString sDir;
		m_lstDirs.GetText(i,sDir);
		if( m_DirWatcher.IsWatchingDirectory( sDir) )
			m_DirWatcher.UnwatchDirectory( sDir );

		//	GetDlgItemText(IDC_EDIT_DIR_TO_MONITOR, m_strDirectoryToMonitor);



		DWORD dwWatch = 0;
		if( ERROR_SUCCESS != (dwWatch = m_DirWatcher.WatchDirectory(sDir, 
			dwChangeFilter,
			&m_DirChangeHandler,
			bWatchSubDir,
			m_strIncludeFilter1,
			m_strExcludeFilter1)) )
		{
			MessageBox(_T("Failed to start watch:\n") + GetLastErrorMessageString( dwWatch ) );
		}
		else
		{
			 m_btnStartWatch.EnableWindow(FALSE);
			 m_btnStopWatch.EnableWindow(TRUE);
		}
	}


}

void CDlgFileWatch::OnBnClickedBtnDirselect()
{
	//GetDlgItemText(IDC_EDIT_DIR_TO_MONITOR, m_strDirectoryToMonitor);

	CFolderDialog dlg(_T("Select the Directory you wish to monitor..."), m_strDirectoryToMonitor, this);

	if( dlg.DoModal() == IDOK )
	{
		 AddDir(dlg.GetFolderPath());
	}
}

void CDlgFileWatch::OnBnClickedBtnstopwatch()
{
	m_DirWatcher.UnwatchAllDirectories();
	m_btnStartWatch.EnableWindow(TRUE);
	m_btnStopWatch.EnableWindow(FALSE);
}

void CDlgFileWatch::OnBnClickedBtnClearlog()
{
	m_lstChanges.ResetContent();
}


void CDlgFileWatch::OnBnClickedBtnExprotlog()
{
	TCHAR   szPath[MAX_PATH],szPath1[MAX_PATH];
	SHGetSpecialFolderPath(NULL,szPath, CSIDL_DESKTOP ,FALSE);
	_tcscat(szPath, L"\\filewatch.txt"); 

	CFileDialog dlg(TRUE,NULL,szPath,OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT /* | OFN_NOCHANGEDIR*/,TEXT("�ļ��ļ�(*.txt)|*.txt||"),this);
		dlg.GetOFN().lpstrInitialDir  = szPath;
	CString sfile;
	if(dlg.DoModal()==IDOK)
	{
		sfile =  dlg.GetPathName();
	}
	if(sfile == "")
		return;

	CFile file;
	if(PathFileExists( sfile))
	{
		if(MessageBox(L"�ļ��Ѵ��ڣ�Ҫ�滻��",L"��ʾ", MB_OKCANCEL) == IDOK)
		{
		
		}
		else
			return;
	}
	if(file.Open(sfile,CFile::modeCreate|CFile::modeWrite|CFile::shareDenyRead))  
	{    
		file.SeekToBegin(); //�����ļ���ͷ    

		file.Write("\xff\xfe", 2); //Unicode�����txt�ļ���0xff��0xfe�����ֽ���Ϊ��ͷ��ǡ�   

		CString s;
		for(int i=0; i<m_lstChanges.GetCount();i++)
		{
			CString tmp ; m_lstChanges.GetText(i,tmp);
			s += tmp+ L"\r\n";
		}
		file.Write(s , s.GetLength()*2); //д��ʵ������    

		file.Close(); //�ر��ļ�    
		MessageBox(L"                �����ɹ�      ",L"��ʾ");
	}
	else
		MessageBox(L"                ����ʧ��    ",L"��ʾ");
}


BOOL CDlgFileWatch::PreTranslateMessage(MSG* pMsg)
{
	// TODO: �ڴ����ר�ô����/����û���
	

	return CDlgMainBase::PreTranslateMessage(pMsg);
}


void CDlgFileWatch::OnDropFiles(HDROP hDropInfo)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	TCHAR strFileName[MAX_PATH], strDrive[MAX_PATH], strDir[MAX_PATH], strExt[MAX_PATH];
	TCHAR szFilePath[MAX_PATH];

	::DragQueryFile (hDropInfo, 0, szFilePath, sizeof (szFilePath));

	//HICON h = ::ExtractIcon(NULL,szFilePath,0); 
	if(GetFileAttributes (szFilePath) != FILE_ATTRIBUTE_DIRECTORY)
	{
		MessageBox(L"������һ����Ч��Ŀ¼");
		return;
	}
	// This function splits the whole path into Drive, Dir, File Name and Extension
	::_wsplitpath(szFilePath, strDrive, strDir, strFileName, strExt);

	//if (h)
	{
		// After getting the the Icon handle and the Icon file name,
		// Add them to the list box
		AddDir(szFilePath);
	}

	m_lstDirs.SetCurSel(m_lstDirs.GetCount()-1);

	::DragFinish(hDropInfo);
	CDlgMainBase::OnDropFiles(hDropInfo);
}


void CDlgFileWatch::OnPaint()
{
	//CPaintDC dc(this); // device context for painting
	// TODO: �ڴ˴������Ϣ����������
	// ��Ϊ��ͼ��Ϣ���� CDlgMainBase::OnPaint()
	CDlgMainBase::OnPaint();
}


HBRUSH CDlgFileWatch::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDlgMainBase::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  �ڴ˸��� DC ���κ�����
	UINT id = pWnd->GetDlgCtrlID();
	if(id==IDC_CHECK_SUBDIR1)
	{
		CRect rc;
		pWnd->GetWindowRect(&rc);
		ScreenToClient(&rc);

		CDC* dc = GetDC();

		pDC->BitBlt(0,0,rc.Width(),rc.Height(),dc,rc.left,rc.top,SRCCOPY);	//�Ѹ����ڱ���ͼƬ�Ȼ�����ť��

		ReleaseDC(dc);

		hbr = (HBRUSH) ::GetStockObject(NULL_BRUSH);
	}
	// TODO:  ���Ĭ�ϵĲ������軭�ʣ��򷵻���һ������
	return hbr;
}


