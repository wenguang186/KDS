#include "stdafx.h"
#include "resource.h"
#include <vector>
using namespace std;
#include "MultiLanguage.h"

CMultiLanguage	g_lang;
//g_lang.Initialize();

BOOL __VectorCmp(vector<INT> &vec_1, vector<INT> &vec_2)
{
	unsigned int size_1 = vec_1.size();  
	unsigned int size_2 = vec_2.size();  

	if( size_1 != size_2 )
	{  
		return FALSE;  
	} 
	 
	for (vector<int>::iterator ite1 = vec_1.begin(), ite2 = vec_2.begin();  
		ite1 != vec_1.end(), ite2 != vec_2.end();  
		ite1++, ite2++  
		)
	{  
		if( *ite1 != *ite2 )
		{  
			return FALSE;  
			break;  
		}  
	}  	  

	return TRUE;
}

void  POSMessageBox(INT nStrId)
{
	CString str;
	//str.LoadString(nStrId);
	g_lang.LoadString(str, nStrId);
	//AfxMessageBox(str);
	
	if (g_lang.m_strLang.Compare(_T("简体中文")) == 0)
	{
		// 参考网址 http://blog.csdn.net/guoyk1990/article/details/44337249
		MessageBoxEx(NULL, str, _T("KDS"), MB_OK, MAKELANGID(LANG_CHINESE, SUBLANG_CHINESE_SIMPLIFIED));
	}
	else
	{
		MessageBoxEx(NULL, str, _T("KDS"), MB_OK, MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US));
	}
}

void  POSMessageBox(CString &strMsg)
{
	//AfxMessageBox(strMsg);
	if (g_lang.m_strLang.Compare(_T("简体中文")) == 0)
	{
		// 参考网址 http://blog.csdn.net/guoyk1990/article/details/44337249
		MessageBoxEx(NULL, strMsg, _T("KDS"), MB_OK, MAKELANGID(LANG_CHINESE, SUBLANG_CHINESE_SIMPLIFIED));
	}
	else
	{
		MessageBoxEx(NULL, strMsg, _T("KDS"), MB_OK, MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US));
	}
}

int ScaleX(int pt) 
{
	int _dpiY=96;
	int _dpiX=96;

	HDC hdc = GetDC(NULL); 
	if (hdc) 
	{ 
		_dpiX = GetDeviceCaps(hdc, LOGPIXELSX); 
		_dpiY = GetDeviceCaps(hdc, LOGPIXELSY); 
		ReleaseDC(NULL, hdc); 
	}

	return MulDiv(pt, _dpiX, 96);
} 
int ScaleY(int pt) 
{
	int _dpiY=96;
	int _dpiX=96;

	HDC hdc = GetDC(NULL); 
	if (hdc) 
	{ 
		_dpiX = GetDeviceCaps(hdc, LOGPIXELSX); 
		_dpiY = GetDeviceCaps(hdc, LOGPIXELSY); 
		ReleaseDC(NULL, hdc); 
	}

	return MulDiv(pt, _dpiY, 96); 
} 