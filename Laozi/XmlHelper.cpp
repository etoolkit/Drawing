#include "StdAfx.h"
#include ".\xmlhelper.h"

//#include "htcDetect.h"

#include <atlbase.h>
#include <comutil.h>
#include <shlwapi.h>

#pragma comment(lib,"msxml2.lib")
//#pragma comment(lib,"C:\\Program Files\\MSXML 6.0\\lib\\msxml6.lib")
#pragma comment(lib,"comsupp.lib")

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//extern ChtcDetectApp theApp;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CXmlHelper::CXmlHelper()
{
	m_pXMLDoc=NULL;
	m_bReady=FALSE;

	::CoInitialize(NULL);
}

CXmlHelper::~CXmlHelper()
{
	if (m_pXMLDoc!=NULL)
	{
		m_bReady=FALSE;
		m_pXMLDoc=NULL;
	}
	//	::CoUninitialize();
}

void CXmlHelper::ShowXmlError()
{
	try
	{
		if (m_pXMLDoc !=NULL)
		{
			CComPtr<IXMLDOMParseError> parse_error;
			m_pXMLDoc->get_parseError(&parse_error);
			CComBSTR error_reason;
			parse_error->get_reason(&error_reason);
		}
	}
	catch (CException* e)
	{
		//theApp.LogIt(_T("Error: ShowXmlError()"));
	}
}

BOOL CXmlHelper::SetNodeText(IXMLDOMNode* node, CString sValue)
{
	try
	{
		if( HasChildren(node) )
		{
			// 下面有子结点 
			ASSERT( FALSE );
			return FALSE;
		}

		HRESULT hr = S_OK;
		hr = node->put_text( _bstr_t(sValue) );
		ASSERT( SUCCEEDED(hr) );

		return SUCCEEDED(hr);
	}
	catch (CException* e)
	{
		//theApp.LogIt(_T("Error: SetNodeText()"));
		return FALSE;
	}
}



//-------------------------------------------------------------------------
// Function Name    :HasChildren
// Parameter(s)     :
// Return           :
// Memo             :是否有子结点
//-------------------------------------------------------------------------
BOOL CXmlHelper::HasChildren(IXMLDOMNode* node)
{
	try
	{
		BOOL bHasChildren = FALSE;

		HRESULT hr = S_OK;
		CComPtr<IXMLDOMNodeList> pNodeList = NULL;
		hr = node->get_childNodes(&pNodeList);
		ASSERT( SUCCEEDED(hr) );

		if (SUCCEEDED(hr) && pNodeList)
		{
			long count = 0;
			hr = pNodeList->get_length(&count);

			for( int i = 0; i < count && !bHasChildren; i++)
			{
				CComPtr<IXMLDOMNode> pNode = NULL;
				hr = pNodeList->get_item(i, &pNode);
				if (SUCCEEDED(hr) && pNode)
				{
					DOMNodeType NodeType;
					hr = pNode->get_nodeType(&NodeType);
					if( NodeType == NODE_ELEMENT )
					{
						bHasChildren = TRUE;
					}
				}
			}
		}

		return bHasChildren;
	}
	catch (CException* e)
	{
//		theApp.LogIt(_T("Error: HasChildren()"));
		return FALSE;
	}
}

BOOL CXmlHelper::RemoveChildren(IXMLDOMNode *parent)
{
	try
	{
		ASSERT( parent != NULL );

		HRESULT hr = S_OK;
		CComPtr<IXMLDOMNodeList> pNodeList = NULL;
		hr = parent->get_childNodes(&pNodeList);
		if (SUCCEEDED(hr) && pNodeList)
		{
			long count = 0;
			hr = pNodeList->get_length(&count);
			for( int i = count - 1; i >=0; i--)//这儿要用Downto,不然的话，后面的会删除不了，
			{
				CComPtr<IXMLDOMNode> pNode = NULL;
				hr = pNodeList->get_item(i, &pNode);
				if (SUCCEEDED(hr) && pNode)
				{
					DOMNodeType NodeType;
					hr = pNode->get_nodeType(&NodeType);
					if( NodeType == NODE_ELEMENT )
					{
						CComPtr<IXMLDOMNode> pOldNode = NULL;
						hr = parent->removeChild(pNode,&pOldNode);
						if (SUCCEEDED(hr) && pOldNode)
						{
						}
					}

				}
			}

		}
		return TRUE;
	}
	catch (CException* e)
	{
//		theApp.LogIt(_T("Error: RemoveChildren()"));
		return FALSE;
	}
}

DWORD CXmlHelper::InitNewXml( LPCTSTR name, IXMLDOMElement **retNode)
{
	try
	{
		DWORD ret = ERROR_UNKNOWN_FEATURE;
		VARIANT_BOOL isSc;

		HRESULT hr = S_FALSE;
		if (m_pXMLDoc != NULL)
		{
			m_pXMLDoc.Release();
			//m_pXMLDoc = NULL;
		}

		hr = m_pXMLDoc.CoCreateInstance(CLSID_DOMDocument);
		if (SUCCEEDED(hr) )
		{
			if (name!=NULL && _tcslen(name)>0 && retNode)
			{
				hr = m_pXMLDoc->loadXML(_bstr_t(name), &isSc);
				ret = m_pXMLDoc->get_documentElement(retNode);

			}
		}else{
//			theApp.LogIt(L"CoCreateInstance return false.");
		}

		return ret;
	}
	catch (CException* e)
	{
//		theApp.LogIt(_T("Error: InitNewXml()"));
		return -1;
	}
}

void CXmlHelper::SaveXml(LPCTSTR xmlFileName)
{
	try
	{
		LPCTSTR file = xmlFileName;
		if(file == NULL)
			file = m_fileName;
		HRESULT res=S_OK;
		int retry=10;
		do 
		{
			res=m_pXMLDoc->save(_variant_t(file));
			if (res!=S_OK)
			{
////				theApp.LogIt(_T("Save XML fail: error=%d and will retry after 500ms\n"),res);
				Sleep(500);
			}
		} while(res!=S_OK && retry-->0);
	}
	catch (CException* e)
	{
///		theApp.LogIt(_T("Error: SaveXml()"));
	}
}


DWORD CXmlHelper::GetNodeList(LPCTSTR szPath, IXMLDOMNodeList **resultList, IXMLDOMNode* node)
{
	try
	{
		DWORD ret = ERROR_SUCCESS;
		if (szPath && _tcslen(szPath)>0 && resultList)
		{
			CComPtr<IXMLDOMNode> pNode=NULL;
			if (node!=NULL)
			{
				pNode=node;
				//			node->AddRef();
			}
			else
			{
				ret = m_pXMLDoc->QueryInterface(IID_IXMLDOMNode, (void**)&pNode);
			}
			if (pNode)
			{
				CComBSTR path = szPath;
				ret = pNode->selectNodes(path,resultList);
				//pNode.Release();
				//pNode=NULL;
			}
		}
		else ret = ERROR_INVALID_PARAMETER;

		return ret;
	}
	catch (CException* e)
	{
//		theApp.LogIt(_T("Error: GetNodeList()"));
		return -1;
	}
}

DWORD CXmlHelper::CreateXMLDocument(LPCTSTR lpRoot)
{
	try
	{
		DWORD ret=S_OK;
		::CoInitialize(NULL);
		if (m_pXMLDoc!=NULL)
			m_pXMLDoc=NULL;

		ret = m_pXMLDoc.CoCreateInstance(CLSID_DOMDocument);
		if (ret==S_OK)
		{
			CComBSTR sTarget (L"xml"), 
				sData (L"version=\"1.0\" encoding=\"UTF-8\"");
			CComPtr<IXMLDOMNode> pOutNode;
			CComPtr<IXMLDOMProcessingInstruction> pProcessingInstruction ;
			ret=m_pXMLDoc->createProcessingInstruction(sTarget, sData, &pProcessingInstruction);
			if (ret==S_OK)
			{
				ret = m_pXMLDoc->appendChild (pProcessingInstruction, &pOutNode);
				if (lpRoot!=NULL)
				{
					CComPtr<IXMLDOMNode> pResult;
					ret=CreateAppendNode(m_pXMLDoc,lpRoot,&pResult);
				}
			}
		}else{
////			theApp.LogIt(L"CoCreateInstance return false.");
		}

		return ret;
	}
	catch (CException* e)
	{
//		theApp.LogIt(_T("Error: CreateXMLDocument()"));
		return -1;
	}
}

DWORD CXmlHelper::LoadFromXmlString(LPCTSTR xmlString)
{
	try
	{
		DWORD ret = ERROR_SUCCESS;
		//TRACE_INFO(_T("CXmlHelper::LoadFromXmlString: %s\n"), xmlString);
		::CoInitialize(NULL);
		if (xmlString && _tclen(xmlString)>0)
		{
			if (m_pXMLDoc!=NULL)
			{
				m_pXMLDoc=NULL;
			}

			ret = m_pXMLDoc.CoCreateInstance(CLSID_DOMDocument);
			if (ret==S_OK) 
			{
				VARIANT_BOOL isSuccessful=VARIANT_FALSE;
				ret = m_pXMLDoc->loadXML(_bstr_t(xmlString),&isSuccessful);
				if (ret==S_OK && isSuccessful==VARIANT_TRUE)
				{
					m_bReady=TRUE;
				}
				else
				{
					ShowXmlError();
					m_pXMLDoc=NULL;
				}
			}else{
//				theApp.LogIt(L"CoCreateInstance return false.");
			}
		}
		else ret = ERROR_INVALID_PARAMETER;
		//TRACE_INFO(_T("CXmlHelper::LoadFromXmlString: return %d\n"), ret);
		return ret;
	}
	catch (CException* e)
	{
//		theApp.LogIt(_T("Error: LoadFromXmlString()"));
		return -1;
	}	
}

DWORD CXmlHelper::LoadFromFile(LPCTSTR xmlFilename)
{
	try
	{
		DWORD ret = ERROR_SUCCESS;
		m_fileName = xmlFilename;
		//TRACE_INFO(_T("CXmlHelper::LoadFromFile: %s\n"), xmlFilename);
		if (xmlFilename && _tclen(xmlFilename)>0)
		{
			if (m_pXMLDoc!=NULL)
			{
				m_pXMLDoc=NULL;
			}

			COleVariant name;
			name.SetString(xmlFilename,VT_BSTR);
			//ret = CoCreateInstance(CLSID_DOMDocument, NULL, CLSCTX_INPROC_SERVER, IID_IXMLDOMDocument, (void**)&m_pXMLDoc);
			// need MSXML4.dll
			::CoInitialize(NULL);
			//ret = CoCreateInstance(, NULL, CLSCTX_INPROC_SERVER, IID_IXMLDOMDocument, (void**)&m_pXMLDoc);
			ret = m_pXMLDoc.CoCreateInstance(CLSID_DOMDocument);
			if (ret==S_OK) 
			{
				VARIANT_BOOL isSuccessful=VARIANT_FALSE;
				ret = m_pXMLDoc->load(name,&isSuccessful);
				if (ret==S_OK && isSuccessful==VARIANT_TRUE)
				{
					m_bReady=TRUE;
				}
				else
				{
					ShowXmlError();
					m_pXMLDoc=NULL;
					ret=ERROR_OPEN_FAILED;
				}
			}else{
//				theApp.LogIt(L"CoCreateInstance return false.");
			}
		}
		else ret = ERROR_INVALID_PARAMETER;
		//TRACE_INFO(_T("CXmlHelper::LoadFromFile: return %d\n"), ret);
		return ret;	
	}
	catch (CException* e)
	{
//		theApp.LogIt(_T("Error: LoadFromFile()"));
		return -1;
	}
}

//-------------------------------------------------------------------------
// Function Name    :IsFileExist	[static]
// Parameter(s)     :CString strFilePath	文件路径和名称
// Return           :BOOL
// Memo             :判断文件是否存在
//-------------------------------------------------------------------------
BOOL CXmlHelper::IsFileExist(CString strFilePath)
{
	try
	{
		//return _taccess(strFilePath, 0) == 0;
		return PathFileExists(strFilePath);
	}
	catch (CException* e)
	{
//		theApp.LogIt(_T("Error: IsFileExist()"));
	}
}

//int nlabelIndex;
//CString sHandsetId;
//CString sEventName;
//enumOP	op;
//
//CString returnstring;		//不用了，从icss.xml中读取
//CString nsProfile;			//不用了，从icss.xml中读取
//enumModule nModule;

DWORD CXmlHelper::GetExeMessage(LPCTSTR xml, PEXEMESSAGE exemsg)
{
	try
	{
		DWORD ret = ERROR_SUCCESS;
		ret = LoadFromXmlString(xml);
		if (ret == ERROR_SUCCESS)
		{
			CString str;
			ret = GetNodeTextValue(PARAM_LABEL,str,NULL); 
			if (ret != ERROR_SUCCESS) return ret;
			exemsg->nlabelIndex = _ttoi(str);

			ret = GetNodeTextValue(PARAM_HANDSET,exemsg->sHandsetId,NULL); 
			//if (ret != ERROR_SUCCESS) return ret;

			ret = GetNodeTextValue(PARAM_EVENTNAME,exemsg->sEventName,NULL); 
			//if (ret != ERROR_SUCCESS) return ret;

			str.Empty();
			ret = GetNodeTextValue(PARAM_OPERATION,str,NULL); 
			if (ret != ERROR_SUCCESS) return ret;
			if (str.CompareNoCase(STR_SETPARAM) == 0)
			{
				exemsg->op = OP_SETPARAM;
			}
			else if (str.CompareNoCase(STR_GETSTATUS) == 0)
			{
				exemsg->op = OP_GETSTATUS;
			}
			else if (str.CompareNoCase(STR_USERCANCEL) == 0)
			{
				exemsg->op = OP_USERCANCEL;
			}
			else 
				exemsg->op = OP_UNKNOWN;

			str.Empty();
			ret = GetNodeTextValue(PARAM_MODULE,str,NULL); 
			if (ret == ERROR_SUCCESS)
			{
				if (str.CompareNoCase(STR_DRIVERUPDATE) == 0)
				{
					exemsg->nModule = MODUDLE_DRIVERUPDATE;
				}
				else if (str.CompareNoCase(STR_PHONEDETECT) == 0)
				{
					exemsg->nModule = MODULE_PHONEDTECT;
				}
				else
					exemsg->nModule = MODULE_UNKNOWN;
			}
		}
		return ret;
	}
	catch (CException* e)
	{
//		theApp.LogIt(_T("Error: GetExeMessage()"));
		return -1;
	}
}


//void CXmlHelper::DumpXml()
//{
//#if NDEBUG
//	return;
//#endif
//	if (m_bReady && m_pXMLDoc!=NULL)
//	{
//		TCHAR tempPath[MAX_PATH]={0};
//		if(GetTempPath(MAX_PATH,tempPath)>0)
//		{
//			TCHAR tempFilename[MAX_PATH]={0};
//			if(GetTempFileName(tempPath,_T("xml"),0,tempFilename)>0)
//			{
//				_variant_t path=tempFilename;
//				m_pXMLDoc->save(path);
//				TRACE_INFO(_T("DumpXml to %s\n"), tempFilename);
//			}
//		}
//	}
//}

DWORD CXmlHelper::GetNodeTextValue(LPCTSTR szPath, CString &value, IXMLDOMNode *node)
{
	try
	{
		DWORD ret = ERROR_SUCCESS;
		//TRACE_INFO(_T("CXmlHelper::GetNodeTextValue: path=%s\n"), szPath);
		if (szPath && _tcslen(szPath)>0)
		{
			CComPtr<IXMLDOMNode> pNode;
			if (node!=NULL)
			{
				pNode=node;
				node->AddRef();
			}
			else
			{
				ret = m_pXMLDoc->QueryInterface(IID_IXMLDOMNode, (void**)&pNode);
			}
			if (pNode)
			{
				_bstr_t path(szPath);
				CComPtr<IXMLDOMNode> pIXMLDOMNode=NULL;
				ret = pNode->selectSingleNode(path,&pIXMLDOMNode);
				if (SUCCEEDED(ret) && pIXMLDOMNode)
				{
					VARIANT_BOOL haschild;
					ret=pIXMLDOMNode->hasChildNodes(&haschild);
					if (SUCCEEDED(ret) && haschild==VARIANT_TRUE)
					{
						CComPtr<IXMLDOMNode> child=NULL;
						ret=pIXMLDOMNode->get_firstChild(&child);
						if (SUCCEEDED(ret) && child)
						{
							VARIANT v;
							ret=child->get_nodeValue(&v);
							if (SUCCEEDED(ret))
							{
								_bstr_t b(v);
								value=(wchar_t*)(b);
							}
						}
					}
				}
			}
		}
		else ret = ERROR_INVALID_PARAMETER;
		//TRACE_INFO(_T("CXmlHelper::GetNodeTextValue: return (%d) with value=%s\n"), ret, value);
		return ret;	
	}
	catch (CException* e)
	{
//		theApp.LogIt(_T("Error: GetNodeTextValue()"));
		return -1;
	}
}

DWORD CXmlHelper::GetNodeTextArray(LPCTSTR szPath, CStringArray &value, IXMLDOMNode* node)
{
	try
	{
		DWORD ret = ERROR_SUCCESS;
		//TRACE_INFO(_T("CXmlHelper::GetNodeTextArray: path=%s\n"), szPath);
		if (szPath && _tcslen(szPath)>0)
		{
			CComPtr<IXMLDOMNodeList> lists = NULL;
			ret = GetNodeList(szPath, &lists, node);
			if (lists)
			{
				long count=0;
				lists->get_length(&count);

				for(long i  = 0; i < count; i++)
				{
					// find unique one
					CComPtr<IXMLDOMNode> node = NULL;
					if (lists->get_item(i,&node)==ERROR_SUCCESS)
					{
						//VARIANT v;
						BSTR bstr=NULL;
						ret = node->get_text(&bstr);
						if (SUCCEEDED(ret) && bstr!=NULL)
						{
							CString s((BSTR)bstr);
							value.Add(s);
							SysFreeString(bstr);
						}
					}
				}
			}
		}
		else 
			ret = ERROR_INVALID_PARAMETER;
		//TRACE_INFO(_T("CXmlHelper::GetNodeTextArray: return (%d)\r\n"), ret);
		return ret;	
	}
	catch (CException* e)
	{
//		theApp.LogIt(_T("Error: GetNodeTextArray()"));
		return -1;
	}
}


DWORD CXmlHelper::GetNode(LPCTSTR szPath, IXMLDOMNode **resultNode)
{
	try
	{
		if(m_pXMLDoc==NULL)
			return ERROR_INVALID_PARAMETER;
		DWORD ret = ERROR_SUCCESS;
		if (szPath && _tcslen(szPath)>0 && resultNode)
		{
			CComBSTR path =szPath;
			ret = m_pXMLDoc->selectSingleNode(path,resultNode);
		}
		else ret = ERROR_INVALID_PARAMETER;
		return ret;
	}
	catch (CException* e)
	{
//		theApp.LogIt(_T("Error: GetNode()"));
		return -1;
	}
}

DWORD CXmlHelper::CreateAppendNode(IXMLDOMNode *parent, LPCTSTR name, IXMLDOMNode** resultNode)
{
	try
	{
		DWORD ret=ERROR_SUCCESS;

		if (parent!=NULL && name!=NULL && _tcslen(name)>0 && resultNode)
		{
			_variant_t v(long(1));
			CComPtr<IXMLDOMNode> node = NULL;
			CComBSTR strName = name;
			if ((ret=m_pXMLDoc->createNode(v,strName,NULL,&node))==ERROR_SUCCESS && node!=NULL)
			{
				ret = parent->appendChild(node,resultNode);
			}
		}
		return ret;
	}
	catch (CException* e)
	{
//		theApp.LogIt(_T("Error: CreateAppendNode()"));
		return -1;
	}
}



DWORD CXmlHelper::SetNodeAttrib(IXMLDOMNode *node, LPCTSTR attribName, LPCTSTR attribValue)
{
	try
	{
		DWORD ret = ERROR_SUCCESS;
		if (node!=NULL && attribName && attribValue && _tcslen(attribValue)>0 && _tcslen(attribName)>0)
		{
			CComPtr<IXMLDOMElement> element=NULL;
			ret = node->QueryInterface(IID_IXMLDOMElement,(void**)&element);
			if (SUCCEEDED(ret) && element!=NULL)
			{
				_variant_t v(attribValue);
				_bstr_t b(attribName);
				element->setAttribute(b,v);
			}
		}
		else ret = ERROR_INVALID_PARAMETER;
		return ret;
	}
	catch (CException* e)
	{
//		theApp.LogIt(_T("Error: SetNodeAttrib()"));
		return -1;
	}
}

DWORD CXmlHelper::GetNodeAttribeValue(LPCTSTR szPath, LPCTSTR szAttribName, CString &value, IXMLDOMNode *node)
{
	try
	{
		DWORD ret = ERROR_SUCCESS;
		//TRACE_INFO(_T("CXmlHelper::GetNodeAttribeValue: path=%s, attrib=%s\n"), szPath, szAttribName);
		if (szPath && _tcslen(szPath)>0)
		{
			CComPtr<IXMLDOMNode> pNode=NULL;
			if (node!=NULL)
			{
				pNode=node;
				//			node->AddRef();
			}
			else
			{
				ret = m_pXMLDoc->QueryInterface(IID_IXMLDOMNode, (void**)&pNode);
			}
			if (pNode!=NULL)
			{
				CComBSTR path =szPath;
				CComPtr<IXMLDOMNode> pIXMLDOMNode=NULL;
				ret = pNode->selectSingleNode(path,&pIXMLDOMNode);
				if (SUCCEEDED(ret) && pIXMLDOMNode)
				{
					CComPtr<IXMLDOMNamedNodeMap> pIXMLDOMNamedNodeMap=NULL;
					ret=pIXMLDOMNode->get_attributes(&pIXMLDOMNamedNodeMap);
					if (SUCCEEDED(ret) && pIXMLDOMNamedNodeMap)
					{
						CComPtr<IXMLDOMNode> attrNode;
						CComBSTR attrib = szAttribName;
						ret=pIXMLDOMNamedNodeMap->getNamedItem(attrib,&attrNode);
						if (SUCCEEDED(ret) && attrNode)
						{
							CComVariant v;
							ret=attrNode->get_nodeValue(&v);
							if (SUCCEEDED(ret))
							{
								value = ::SysAllocString(v.bstrVal);
							}
						}
					}
				}
				//pNode.Release();
				//pNode=NULL;
			}
		}
		else ret = ERROR_INVALID_PARAMETER;
		//TRACE_INFO(_T("CXmlHelper::GetNodeAttribeValue: return(%d) with value=%s\n"), ret, value);
		return ret;
	}
	catch (CException* e)
	{
//		theApp.LogIt(_T("Error: GetNodeAttribeValue()"));
		return -1;
	}
}

