// dllmain.h : 模块类的声明。

class CMyATLModule : public ATL::CAtlDllModuleT< CMyATLModule >
{
public :
	DECLARE_LIBID(LIBID_MyATLLib)
	DECLARE_REGISTRY_APPID_RESOURCEID(IDR_MYATL, "{E3885FF4-CF08-4788-8E01-A11B0CF7CDFA}")
};

extern class CMyATLModule _AtlModule;
