#pragma once
struct Weather_Info 
{ 
	char city[50]; 
	char weather[50]; 
	char temp[50]; 
	char wind[50]; 
}; 
struct SearchInfo
	{ 
		char host[256];
		unsigned int port;
		char filename[256];
		char outfile[50];
	}; 


class CWeather
{
	
public:
	CWeather(void);
	~CWeather(void);
	bool GetWeather(Weather_Info&wi,char *html);
	bool initsocket();
	void GetUrls(char *html);
	bool initargs(SearchInfo &outinfo,const char*city) ;
	bool initargs(SearchInfo &outinfo,int argc,char **args);
	void PrintWeather(const Weather_Info* pwi);
	char* GetFile(const SearchInfo * psi);

	void ThreadGet(CString sCity);
	void SavePng(CString strUrl,CString strPath);
	CString FindStr(CString &str,CString strFind1,CString strFind2,bool yesNo);

	struct strInfo 
	{
		////��ǰ��ǰ��ðָ��
		//CString m_strNew_Rheum;
		//��ǰʱ��
		CString m_strNew_Time;
		//��ǰСͼ��
		CString m_strNewUrl;
		//�Ƿ�����
		CString m_strNew_yesNoRain; 
		//�¶�
		CString m_strNew_Temperature;
		//��
		CString m_strNew_Wind;
	};
	strInfo m_strInfo[3];

private:
	//ԭʼ����
	CString wData;
	//��������������ʵ��
	//��ǰ����
	CString m_City;
	//��ǰͼƬ��ַ
	CString m_strNewDigUrl;
	//��ǰ���� ����¶�
	CString m_strNew_Rain;
	//��ǰ��ǰ��ðָ��
	CString m_strNew_Rheum;

	CBitmap m_SkinBitmap;

	CString strURL;
};



