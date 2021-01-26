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
		////当前当前感冒指数
		//CString m_strNew_Rheum;
		//当前时间
		CString m_strNew_Time;
		//当前小图标
		CString m_strNewUrl;
		//是否有雨
		CString m_strNew_yesNoRain; 
		//温度
		CString m_strNew_Temperature;
		//风
		CString m_strNew_Wind;
	};
	strInfo m_strInfo[3];

private:
	//原始数据
	CString wData;
	//可以用数据类型实现
	//当前城市
	CString m_City;
	//当前图片地址
	CString m_strNewDigUrl;
	//当前城市 雨和温度
	CString m_strNew_Rain;
	//当前当前感冒指数
	CString m_strNew_Rheum;

	CBitmap m_SkinBitmap;

	CString strURL;
};



