#pragma once
#include <map>
using namespace std;

struct FUNCITEM 
{
	CString name;
	CString remark;
	int funcid;
	int range;
};

class CMultiLanguage
{
public:
	//int m_nCodePage;
	BOOL m_bDefaultLang;
	CFont m_dialogFont;
	CString m_strLang;
	std::map<CString,int> pageSet;
	CMultiLanguage(void);
	~CMultiLanguage(void);
	void Initialize();
	void Initialize(CString lang);
	void ReadButtonName(CString lang);
	BOOL UpdatePage(CWnd* pWnd,CString pageName,BOOL bAways=FALSE);
	void LoadString(UINT uResID, CString& strVal);
	void LoadString(CString& strVal,UINT uResID);
	void TranslateDlgItem(CWnd* pWnd,UINT uResID);
protected:
	std::map<UINT,CString> m_StringTable;
	std::map<CString,CList<FUNCITEM, FUNCITEM&>*> theFunc;
};
