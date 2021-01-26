#include "StdAfx.h"
#include "Weather.h"
#include <afxinet.h>
#include "URLToEncode.h"


CWeather::CWeather(void)
{
}


CWeather::~CWeather(void)
{
}


const char* weather_tag="class=\"weather\">"; 
const char* temp_tag="class=\"weatheren\">"; 
const char* wind_tag="class=\"big-cn\">"; 
bool CWeather::GetWeather(Weather_Info&wi,char *html) 
{ 
    if(html==NULL) 
        return false; 
    char *temp=NULL; 
	if((temp=strstr(html,weather_tag))==NULL) 
		return false; 

	temp+=strlen(weather_tag); 
	memcpy(wi.weather,temp,strstr(temp,"<")-temp);

	if((temp=strstr(html,temp_tag))==NULL) 
        return false; 
    temp+=strlen(temp_tag); 
  //  memcpy(wi.temp,temp,strlen(temp)-strlen(strstr(temp," <"))); 
memcpy(wi.temp,temp,strstr(temp,"<")-temp);
    if((temp=strstr(temp,wind_tag))==NULL) 
        return false; 
    temp+=strlen(wind_tag); 
memcpy(wi.wind,temp,strstr(temp,"<")-temp);
    //memcpy(wi.wind,temp,strlen(temp)-strlen(strstr(temp," <"))); 
    return true; 
} 
bool CWeather::initsocket() 
{ 
    WORD wVersionRequested; 
    WSADATA wsaData; 
    int err; 
    wVersionRequested = MAKEWORD( 2, 2 ); 
    err = WSAStartup( wVersionRequested, &wsaData ); 
    if ( err != 0 ) { 
        return false; 
    } 
    
    if ( LOBYTE( wsaData.wVersion ) != 2 || 
        HIBYTE( wsaData.wVersion ) != 2 ) { 
        WSACleanup( ); 
        return false; 
    } 
    return true; 
    
} 

const char* WURL="http://weather.tq121.com.cn/mapanel/index1.php?city=%s"; 
const char *HTTP_STR="http://"; 
const char* HTTP_REQUEST_HEADER=
"GET %s HTTP/1.1\r\nAccept:*/*\r\n\ Accept-Language:zh-cn\r\n\User-Agent: Mozilla/4.0 (compatible; MSIE 5.01; Windows NT 5.0)\r\n\Host:%s\r\n\r\n"; 

bool CWeather::initargs(SearchInfo &outinfo,const char*city) 
{ 
    memset(&outinfo,0,sizeof(SearchInfo));
    
    char *temp=new char[256]; 
    memset(temp,0,256); 
    sprintf(temp,WURL,city); 
    printf(temp); 
    temp=(strstr(temp,HTTP_STR)!=NULL)?temp+strlen(HTTP_STR):temp;
    strcpy(outinfo.filename,strstr(temp,"/")!=NULL?strstr(temp,"/"):"/");
    int length=strstr(temp,"/")==NULL?strlen(temp):(strlen(temp)-strlen(strstr(temp,"/"))); 

    memcpy(outinfo.host,temp,length);
    if((temp=strstr(outinfo.host,":"))!=NULL)
    { 
        temp++; 
        outinfo.port=atoi(temp); 
    } 
    else 
    { 
        outinfo.port=80; 
    } 
    delete temp; 
    return true; 
    
} 

char* CWeather::GetFile(const SearchInfo * psi) 
{ 
    if(psi==NULL) 
    { 
        return NULL; 
    } 
    unsigned long serverip=0;
    if((serverip=inet_addr(psi->host))==INADDR_NONE)
    { 
        hostent *phst=gethostbyname(psi->host); 
        if(phst==NULL)
            return NULL; 
      
        if(phst->h_addr_list[0]!=0)
        { 
            memcpy(&serverip,phst->h_addr_list[0],phst->h_length); 
           
        } 
        
    } 
    SOCKET s=socket(AF_INET,SOCK_STREAM,0); 
    if(s==INVALID_SOCKET)
    { 
        printf("Create socket Error!Error Code:%d\n",WSAGetLastError()); 
        return NULL; 
    } 
    SOCKADDR_IN server_addr;
    server_addr.sin_addr.S_un.S_addr=serverip; 
    server_addr.sin_family=AF_INET; 
    server_addr.sin_port=htons(psi->port); 
    memset(server_addr.sin_zero,0,sizeof(server_addr.sin_zero)); 
    
    if(SOCKET_ERROR==connect(s,(const sockaddr*)&server_addr,sizeof(SOCKADDR_IN))) 
    { 
        printf("Connect Server Error!Error Code:%d\n",WSAGetLastError()); 
        closesocket(s); 
        
        return NULL; 
    } 
    char buffer_sendmsg[256]={0}; 
    sprintf(buffer_sendmsg,HTTP_REQUEST_HEADER,psi->filename,psi->host);
  //  printf("\n�����ʽ:%s\n",buffer_sendmsg); 
    if(send(s,buffer_sendmsg,256,0)==SOCKET_ERROR)
    { 
        printf("\n������������,������˳�\n"); 
closesocket(s); 
system("pause"); 
getchar();
        return NULL; 
    } 
    
    
    int len=0; 
    char buffer_recv[1024]={0};
    
    int sumlen=0;
    char *html=(char*)malloc(sizeof(char)*1);
    
    while((len=recv(s,buffer_recv,1024,0))!=0) 
    { 
        if(len==SOCKET_ERROR) 
        { 
            printf("�����޻ظ�,������˳�\n"); 
            closesocket(s); 
            system("pause"); 
 getchar(); 
            return NULL; 
        } 
        sumlen=strlen(html); 
     
//	printf(buffer_recv);

        if((html=(char*)realloc((void*)html,sumlen+sizeof(char)*len))!=NULL) 
        { 
            memset(html+sumlen-1,0,len);
            strcat(html,buffer_recv);
        } 
        memset(buffer_recv,0,1024); 
    } 
    
    closesocket(s); 
    closesocket(s); 
    
    return strlen(html)==0?NULL:html; 
} 
void CWeather::PrintWeather(const Weather_Info* pwi) 
{ 
    //system("cls"); 
    printf("\n"); 
    printf("\n"); 
    printf("\n"); 
    printf("          \t  ����Ԥ��  \n"); 
    printf(" \t=====================================\n"); 
    printf("  \t����    : %s\n",pwi->city); 
    printf("  \t------------------------------------\n"); 
    printf("  \t�������: %s\n",pwi->weather); 
    printf("  \t------------------------------------\n"); 
    printf("  \t����    : %s\n",pwi->temp); 
    printf("  \t------------------------------------\n"); 
    printf("  \t����    : %s\n",pwi->wind); 
    printf(" \t======================================\n"); 
printf(" \t��Ȩ����:һ������ \n"); 
} 



void CWeather::ThreadGet(CString city)
{

	strURL  = city;
	//-------------------------------------------------------------------------
	//���ֽڵ����ֽ�
	DWORD dwNum = WideCharToMultiByte(CP_OEMCP,NULL,strURL,-1,NULL,0,NULL,FALSE);
	char psText[1024];
	
	WideCharToMultiByte(CP_OEMCP,NULL,strURL,-1,psText,dwNum,NULL,FALSE);

	//���ֵ�URL���� ��л���۾�de�ǵ���
	CStringA strTurl=psText;
	CString strCoding;
	CURLTOEncode Encode;
	strCoding=Encode.URLEncode(strTurl);

	//-------------------------------------------------------------------------

	if (strURL == "")
	{
		strURL = _T("http://www.soso.com/tb.q?cid=");
	}
	else
	{
		strURL = _T("http://www.soso.com/tb.q?cid=tb.tq&cin=&city=") + strCoding;
	}
		
	CInternetSession mySession(NULL,0);
	CHttpFile *httpFile;
	httpFile = (CHttpFile *)mySession.OpenURL(strURL);
	if (!httpFile)
	{
		//MessageBox(_T("��ȡ������Ϣ����"),_T(""),MB_OK);
	}
	DWORD dwStatus;
	DWORD dwBuffLen = sizeof(dwStatus);
	BOOL bSuccess;
	bSuccess = httpFile->QueryInfo(HTTP_QUERY_STATUS_CODE|HTTP_QUERY_FLAG_NUMBER,&dwStatus,&dwBuffLen);
	CString str;
	if (dwStatus >= 200 && dwStatus < 300)
	{
		CString strTemp;
		while (httpFile->ReadString(strTemp))
		{
			str += strTemp;	
		}
	}
	httpFile->Close();
	mySession.Close();
	delete httpFile;	
	//uft-8 ת unicode
	WCHAR *wword;
	DWORD num = MultiByteToWideChar(CP_UTF8,0,(LPCSTR)str.GetBuffer(),-1,NULL,0);
	wword = (WCHAR*)calloc(num,sizeof(WCHAR));  
	memset(wword, 0, num*sizeof(WCHAR)); 
	MultiByteToWideChar(CP_ACP,0,(LPCSTR)str.GetBuffer(),-1,wword,num);
	str.ReleaseBuffer();
	wData = wword;
	free(wword);

	m_City = FindStr(wData,_T("<city><![CDATA["),_T("]]></city>"),true);
	//��ͼƬurl
	m_strNewDigUrl = FindStr(wData,_T("</a></div><ul><li><img src=\""),_T("\" align=\"absmiddle\" onload=\"setPng(this,60,50);\"></li>"),true);
	m_strNew_Rain = FindStr(wData,_T("class=\"fs_14\">"),_T("</strong>"),true);
	m_strNew_Rheum = FindStr(wData,_T("<br />"),_T("</li></ul>"),true);

	
	//��ǰʱ��  �������
	//-----------------������õ�����ɾ��  ���ݻ���� �����Ҫȡ�� ���漸�в��Ҵ���͵øĸ�
	m_strInfo[0].m_strNew_Time = FindStr(wData,_T("<div class=\"today\">"),_T("</div><div class=\"weather\">"),true);
	//��ǰСͼ��
	m_strInfo[0].m_strNewUrl = FindStr(wData,_T("<img src=\""),_T("\" onload=\"setPng"),true);
	//�Ƿ�����
	m_strInfo[0].m_strNew_yesNoRain = FindStr(wData,_T("&nbsp;"),_T("<div"),true);
	//�¶�
	m_strInfo[0].m_strNew_Temperature = FindStr(wData,_T("class=\"pd_l\">"),_T("<"),true);
	//��
	m_strInfo[0].m_strNew_Wind = FindStr(wData,_T("br/>"),_T("</div></div></li><li>"),true);
	//��ǰʱ��  �������

	m_strInfo[1].m_strNew_Time = FindStr(wData,_T("<div class=\"today\">"),_T("</div><div class=\"weather\">"),true);
	m_strInfo[1].m_strNewUrl = FindStr(wData,_T("<img src=\""),_T("\" onload=\"setPng"),true);
	m_strInfo[1].m_strNew_yesNoRain = FindStr(wData,_T("&nbsp;"),_T("<div"),true);
	m_strInfo[1].m_strNew_Temperature = FindStr(wData,_T("class=\"pd_l\">"),_T("<"),true);
	m_strInfo[1].m_strNew_Wind = FindStr(wData,_T("br/>"),_T("</div></div>"),true);


	m_strInfo[2].m_strNew_Time = FindStr(wData,_T("<div class=\"today\">"),_T("</div><div class=\"weather\">"),true);
	m_strInfo[2].m_strNewUrl = FindStr(wData,_T("<img src=\""),_T("\" onload=\"setPng"),true);
	m_strInfo[2].m_strNew_yesNoRain = FindStr(wData,_T("&nbsp;"),_T("<div"),true);
	m_strInfo[2].m_strNew_Temperature = FindStr(wData,_T("class=\"pd_l\">"),_T("<"),true);
	m_strInfo[2].m_strNew_Wind = FindStr(wData,_T("br/>"),_T("</div></div>"),true);

	//�����籣��ͼƬ������
	SavePng(m_strNewDigUrl,_T("c:\\new.png"));
	//SavePng(m_strInfo[0].m_strNewUrl,_T("c:\\tody.png"));
	SavePng(m_strInfo[1].m_strNewUrl,_T("c:\\tomorrow.png"));
	SavePng(m_strInfo[2].m_strNewUrl,_T("c:\\postnatal.png"));

	//DrawPng(_T("c:\\new.png"),20,20);
	
	
}
//��ȡ������Դ  ��л (��)�ᴳ�����ṩ�Ĳο�����
void CWeather::SavePng(CString strUrl,CString strPath)
{
	CInternetSession Session;
	CString strServerName,strObject;
	DWORD dwSvrType;
	INTERNET_PORT nPort;
	AfxParseURL(strUrl,dwSvrType,strServerName,strObject,nPort);
	const TCHAR szHeaders[] = _T("application/x-shockwave-flash, image/gif, image/x-xbitmap, image/jpeg, image/pjpeg, application/vnd.ms-excel, application/vnd.ms-powerpoint, application/msword, application/xaml+xml, application/vnd.ms-xpsdocument, application/x-ms-xbap, application/x-ms-application, */*");
	CHttpConnection *pConnection = Session.GetHttpConnection(strServerName,nPort);
	if (!pConnection)
	{
		//MessageBox(_T("���ӷ�����ʧ��"));
	}
	CHttpFile *pHttpFile;
	try
	{
		pHttpFile = (CHttpFile *)pConnection->OpenRequest(
			CHttpConnection::HTTP_VERB_GET,
			strObject,
			NULL,1,NULL,NULL,
			INTERNET_FLAG_NO_COOKIES|INTERNET_FLAG_KEEP_CONNECTION|INTERNET_FLAG_NO_AUTO_REDIRECT
			);
	}
	catch(CException *e)
	{
		pHttpFile = NULL;
		throw;
	}
	if (pHttpFile)
	{
		DWORD dwStatus = 0;
		BOOL bSuccess;
		try
		{
			pHttpFile->AddRequestHeaders(szHeaders);
			pHttpFile->SendRequest();
			bSuccess = pHttpFile->QueryInfoStatusCode(dwStatus);
		}
		catch(...)
		{
			delete pHttpFile;
			delete pConnection;
			return;
		}
		char strarray[1024];
		if (bSuccess && (dwStatus >= 200 && dwStatus <=300))
		{
			//CString sFileName = GetUrlName
			CString strContentType = _T("");
			//text/html
			pHttpFile->QueryInfo(HTTP_QUERY_CONTENT_TYPE,strContentType);
			CFile NetFile;
			if (NetFile.Open(strPath,CFile::modeWrite|CFile::modeCreate))
			{
				int nCount = 0;
				nCount = pHttpFile->Read(strarray,1023);
				while (nCount>0)
				{
					NetFile.Write(strarray,nCount);
					nCount = pHttpFile->Read(strarray,1023);
				}
				NetFile.Close();
			}

		}

	}
	pHttpFile->Close();
	delete pHttpFile;
	
}
//Ѱ���ַ������� yesNO�Ƿ�Ҫɾ���ҵ���ǰ�������ַ�
CString CWeather::FindStr(CString &str,CString strFind1,CString strFind2,bool yesNo)
{
	int nFirst;
	int nEnd;
	CString strTemp;
	nFirst = str.Find(strFind1);
	nEnd = str.Find(strFind2);
	strTemp = str.Mid(nFirst+_tcslen(strFind1),nEnd-nFirst-_tcslen(strFind1));
	if (yesNo)
	{
		str = str.Right(_tcslen(str)-(nEnd + _tcslen(strFind2)));
	}
	
	return strTemp;
}