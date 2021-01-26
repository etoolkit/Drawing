#pragma once

#include <MsXml2.h>
//#include <msxml6.h>

typedef enum
{
	MODUDLE_DRIVERUPDATE = 0,
	MODULE_PHONEDTECT,
	//MODULE_TASK,

	MODULE_UNKNOWN,
}enumModule;


typedef enum
{
	OP_SETPARAM = 0,
	OP_GETSTATUS,
	OP_USERCANCEL,
	//OP_START,
	//OP_END,

	OP_UNKNOWN,
}enumOP;


#define PARAM_LABEL				_T("//param/label")
#define PARAM_HANDSET			_T("//param/handsetid")
#define PARAM_MODULE			_T("//param/module")
#define PARAM_OPERATION			_T("//param/operation")
#define PARAM_EVENTNAME			_T("//param/eventname")

#define STR_DRIVERUPDATE		_T("driverupdate")
#define STR_PHONEDETECT			_T("phonedetect")
#define STR_SETPARAM			_T("setparam")
#define STR_GETSTATUS			_T("getstatus")
#define STR_USERCANCEL			_T("usercancel")

//<param>
//	<label></label>
//	<handsetid></handsetid>
//	<module>driverupdate<module>  //or phonedetect 
//	<operation>setparam</operation>  //getstatus   
//	<eventname></eventname>		//eventname	 if operation is status, then Only care the label node
//</param>

typedef struct _tagExeMessage 
{
	int nlabelIndex;
	CString sHandsetId;
	CString sEventName;
	enumOP	op;
	
	CString returnstring;		//不用了，从icss.xml中读取
	CString nsProfile;			//不用了，从icss.xml中读取
	enumModule nModule;
	_tagExeMessage()
	{
		Clear();
	};
	void Clear()
	{
		op = OP_UNKNOWN;
		nlabelIndex = 0;
		sHandsetId.Empty();
		sEventName.Empty();
		returnstring.Empty();
		nsProfile.Empty();
		nModule = MODULE_UNKNOWN;
	};

}EXEMESSAGE, *PEXEMESSAGE;
/*
		/ 从根节点选取 
		// 从匹配选择的当前节点选择文档中的节点，而不考虑它们的位置 
*/
class CXmlHelper  
{
public:
	DWORD CreateXMLDocument(LPCTSTR  lpRoot);
	DWORD GetNodeAttribeValue(LPCTSTR szPath, LPCTSTR szAttribName, CString &value, IXMLDOMNode* node);
	DWORD SetNodeAttrib(IXMLDOMNode* node, LPCTSTR attribName, LPCTSTR attribValue);
	DWORD CreateAppendNode(IXMLDOMNode* parent, LPCTSTR name, IXMLDOMNode** resultNode);
	DWORD GetNode(LPCTSTR szPath, IXMLDOMNode **resultNode);
	DWORD GetNodeList(LPCTSTR szPath, IXMLDOMNodeList** resultList, IXMLDOMNode* node=NULL);

	DWORD LoadFromXmlString(LPCTSTR xmlString);
	DWORD GetNodeTextValue(LPCTSTR szPath, CString &value, IXMLDOMNode* node);
	DWORD GetNodeTextArray(LPCTSTR szPath, CStringArray &value, IXMLDOMNode* node);
	//void DumpXml();
	DWORD LoadFromFile(LPCTSTR xmlFilename);
	DWORD InitNewXml( LPCTSTR name, IXMLDOMElement **retNode);
	void SaveXml(LPCTSTR xmlFileName = NULL);
	BOOL SetNodeText(IXMLDOMNode* node, CString sValue);
	BOOL HasChildren(IXMLDOMNode* node);
	BOOL RemoveChildren(IXMLDOMNode *parent);
	BOOL IsReady(void);

	CXmlHelper();
	virtual ~CXmlHelper();


	DWORD GetExeMessage(LPCTSTR xml, PEXEMESSAGE exemsg);

	static BOOL IsFileExist(CString strFilePath);
protected:
	//void printXmlNodeNameAndAttributesAndText(IXMLDOMNode* node, IXMLDOMNode* value, int deep, CString& line);
	//void PrintXmlNodeClose(IXMLDOMNode* node, int deep,CString& line);
	//void PrintXmlNodeAttributes(IXMLDOMNode* node, CString& line);
	//void PrintXmlNodeNameAndAttributes(IXMLDOMNode* node, BOOL open, int deep, CString& line);
	//void PrintXmlNode(IXMLDOMNode* node, int deep);

	void ShowXmlError();
	CString m_fileName;
	CComPtr<IXMLDOMDocument> m_pXMLDoc;
	BOOL m_bReady;
	CCriticalSection m_csSave;
public:
	
};

