#include "StdAfx.h"
#include "MyConfig.h"
#include <direct.h>


CMyConfig::CMyConfig(void)
{
}


CMyConfig::~CMyConfig(void)
{
}
int CMyConfig::Save()
{
	TCHAR iniPath[MAX_PATH];
	_tcscpy(iniPath,m_xmlPath);
	char str[MAX_PATH];
	int size=WideCharToMultiByte(CP_ACP,0,iniPath,-1,NULL,0,NULL,NULL);
  WideCharToMultiByte(CP_ACP,0,iniPath,MAX_PATH,str,size,NULL,NULL);
	int n=mkdir(str);
	 
	PathAppend(iniPath, L"Config.ini");
	WritePrivateProfileString(L"Common",L"City", m_sCity,iniPath);
	WritePrivateProfileString(L"HotKey",L"HideWnd", m_sHotKeyHideWnd,iniPath);

	TCHAR xmlPath[MAX_PATH];
	_tcscpy(xmlPath,m_xmlPath);
	PathAppend(xmlPath,_T("config.xml"));
	m_xmlHelper.CreateXMLDocument(L"Laozi");
	CComPtr<IXMLDOMNode> nodeBase;
	CComPtr<IXMLDOMNode> nodeGuard;
	m_xmlHelper.GetNode(L"Laozi",&nodeBase);
	m_xmlHelper.CreateAppendNode(nodeBase ,L"Guard",&nodeGuard);
	CComPtr<IXMLDOMNode> node2 = NULL;

	m_xmlHelper.CreateAppendNode(nodeGuard,L"IsHideByProcess",&node2);
	m_xmlHelper.SetNodeText(node2, m_guardInfo.bHideByProcess? L"1":L"0");
	node2 = NULL;
	m_xmlHelper.CreateAppendNode(nodeGuard,L"IsHideByWndClass",&node2);
	m_xmlHelper.SetNodeText(node2, m_guardInfo.bHideByWndClass? L"1":L"0");
	node2 = NULL;
	m_xmlHelper.CreateAppendNode(nodeGuard,L"IsHideByWndText",&node2);
	m_xmlHelper.SetNodeText(node2, m_guardInfo.bHideByWndText? L"1":L"0");

	for(int i=0;i<m_guardInfo.hidedProcessNameArr.GetCount();i++)
	{	
		node2 = NULL;
		m_xmlHelper.CreateAppendNode(nodeGuard,L"HideProcess",&node2);
		m_xmlHelper.SetNodeText(node2,m_guardInfo.hidedProcessNameArr.GetAt(i));
	}
	
	vector<HideWndInfo>::iterator w;  
	for (w = m_guardInfo.hideWinCls.begin(); w != m_guardInfo.hideWinCls.end();w++)  
	{
		node2 = NULL;
		m_xmlHelper.CreateAppendNode(nodeGuard,L"HideWndClass",&node2);
		m_xmlHelper.SetNodeText(node2,w->sFileName);
		m_xmlHelper.SetNodeAttrib(node2, L"Class",w->clsName);
		m_xmlHelper.SetNodeAttrib(node2,L"Title",w->sTitle);
	}
	for(int i=0;i<m_guardInfo.hidedWndText.GetCount();i++)
	{	
		node2 = NULL;
		m_xmlHelper.CreateAppendNode(nodeGuard,L"HideWndText",&node2);
		m_xmlHelper.SetNodeText(node2,m_guardInfo.hidedWndText.GetAt(i));
	}

	CComPtr<IXMLDOMNode> nodeFileWatch; 
	m_xmlHelper.CreateAppendNode(nodeBase ,L"FileWatch",&nodeFileWatch);
	node2 = NULL;
	m_xmlHelper.CreateAppendNode(nodeFileWatch,L"IsSubDir",&node2);
	m_xmlHelper.SetNodeText(node2, m_watchInfo.fileWatch.bSubDir? L"1":L"0");
	for(int i=0;i<m_watchInfo.fileWatch.watchDirArr.GetCount();i++)
	{	
		node2 = NULL;
		m_xmlHelper.CreateAppendNode(nodeFileWatch,L"WatchDir",&node2);
		m_xmlHelper.SetNodeText(node2,m_watchInfo.fileWatch.watchDirArr.GetAt(i));
	}
	m_xmlHelper.SaveXml(xmlPath);

	

	return 0;
}
int CMyConfig::ReadConfig(LPCTSTR path)
{
	_tcscpy(m_xmlPath  , path);
	TCHAR xmlPath[MAX_PATH];
	_tcscpy(xmlPath,path);
	PathAppend(xmlPath,_T("\\config.xml"));
	ReadXmlCfg(xmlPath);
	TCHAR iniPath[MAX_PATH];
	_tcscpy(iniPath,path);
	PathAppend(iniPath, L"Config.ini");
	TCHAR hotKeyHideWnd[256],tcity[256];
	GetPrivateProfileString(L"HotKey",L"HideWnd",L"Alt+1",hotKeyHideWnd,sizeof(hotKeyHideWnd),iniPath);
	GetPrivateProfileString(L"Common",L"City",L"",tcity,sizeof(tcity),iniPath);
	m_sHotKeyHideWnd = hotKeyHideWnd;
	m_sCity = tcity;
	return 0;
}

int CMyConfig::ReadXmlCfg(LPCTSTR fileName)
{

	if(PathFileExists(fileName))
	{
		m_xmlHelper.LoadFromFile(fileName);
		CComPtr<IXMLDOMNodeList> nodes = NULL;
		CComPtr<IXMLDOMNode> node = NULL;
		int ret = m_xmlHelper.GetNode(L"Laozi/Guard",&node);
		if (ret==S_OK )
		{
			long count=0;		
			CComPtr<IXMLDOMNode> node1;
			CString str;
			guardInfo guard;
			nodes = NULL;
			
			m_xmlHelper.GetNodeTextValue(L"IsHideByProcess",str,node);
			guard.bHideByProcess = str == L"1" ? true : false;
			m_xmlHelper.GetNodeTextValue(L"IsHideByWndClass",str,node);
			guard.bHideByWndClass = str == L"1" ? true : false;
			m_xmlHelper.GetNodeTextValue(L"IsHideByWndText",str,node);
			guard.bHideByWndText = str == L"1" ? true : false;

			m_xmlHelper.GetNodeList(L"HideProcess",&nodes,node);
			ret=nodes->get_length(&count);
			for(long i=0; i<count; i++)
			{ 		
				node1 = NULL;
				nodes->get_item(i,&node1);			
				m_xmlHelper.GetNodeTextValue(L".",str,node1);				
				guard.hidedProcessNameArr.Add(str);						 
			}
			nodes = NULL;
			m_xmlHelper.GetNodeList(L"HideWndClass",&nodes,node);
			ret=nodes->get_length(&count);
			for(long i=0; i<count; i++)
			{ 
				HideWndInfo w ;		
				node1 = NULL;
				nodes->get_item(i,&node1);			
				m_xmlHelper.GetNodeTextValue(L".",str,node1);
				w.sFileName = str;
				m_xmlHelper.GetNodeAttribeValue(L".",L"Class",str,node1);
				w.clsName = str;
				m_xmlHelper.GetNodeAttribeValue(L".",L"Title",str,node1);
				w.sTitle = str;
				 
				guard.hideWinCls.push_back( w);						 
			}
			nodes = NULL;
			m_xmlHelper.GetNodeList(L"HideWndText",&nodes,node);
			ret=nodes->get_length(&count);
			for(long i=0; i<count; i++)
			{ 	
				node1 = NULL;
				nodes->get_item(i,&node1);			
				m_xmlHelper.GetNodeTextValue(L".",str,node1);				
				guard.hidedWndText.Add(str);						 
			}
			m_guardInfo = guard;		
		}

		node = NULL;
		ret = m_xmlHelper.GetNode(L"Laozi/FileWatch",&node);
		if (ret==S_OK )
		{
			long count=0;		
			CComPtr<IXMLDOMNode> node1;
			CString str;
			nodes = NULL;

			m_xmlHelper.GetNodeList(L"WatchDir",&nodes,node);
			ret=nodes->get_length(&count);
			for(long i=0; i<count; i++)
			{ 		
				node1 = NULL;
				nodes->get_item(i,&node1);			
				m_xmlHelper.GetNodeTextValue(L".",str,node1);				
				m_watchInfo.fileWatch.watchDirArr.Add(str);						 
			}
		}

	}
	else
	{	
		m_xmlHelper.CreateXMLDocument(L"");		
		return -1;
	} 

	return 0;
}
