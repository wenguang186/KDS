#include "StdAfx.h"
//#include "POSClient.h"
#include "MultiLanguage.h"
#include <locale.h>
#include "RoundButton2.h"
#include "common.h"

CMultiLanguage::CMultiLanguage(void)
{
	m_bDefaultLang=TRUE;
}

CMultiLanguage::~CMultiLanguage(void)
{
}

void CMultiLanguage::Initialize()
{
	::GetPrivateProfileString(_T("POS"),_T("LANGUAGE"),_T("Default"),m_strLang.GetBuffer(40),40,_T(".\\config.ini"));
	m_strLang.ReleaseBuffer();
	Initialize(m_strLang);
	ReadButtonName(m_strLang);
}
void CMultiLanguage::Initialize(CString lang)
{
	CString line;
	TCHAR str[512];
	int i_type=0;
	line.Format(_T("Languages\\%s.lng"),lang);
	m_StringTable.clear();
	m_strLang=lang;
	FILE* fin=0;
#ifdef UNICODE
	_wfopen_s(&fin,line,_T("r,ccs=UNICODE"));
#else
	fopen_s(&fin,line,_T("r"));
#endif
	if (fin==NULL)
		return;
	m_bDefaultLang=FALSE;
	int fontsize;
	LOGFONT m_tLogFont;
	memset(&m_tLogFont,0,sizeof(LOGFONT));
	CStdioFile file(fin);
	while( file.ReadString(line) )
	{
		memset(str,0,512);
		line.TrimLeft();
		if(line.IsEmpty())
			continue;
		if (line==_T("[STRING_TABLE]"))
		{
			i_type=1;
			continue;
		}
		if (i_type==0)
		{
			if (line.Find(_T("DialogFontName"))>=0)
			{
#ifdef UNICODE
				swscanf_s(line,_T("DialogFontName=%[^\n]"),m_tLogFont.lfFaceName,32);
#else
				sscanf_s(line,_T("DialogFontName=%[^\n]"),m_tLogFont.lfFaceName,32);
#endif
			}
			else if (line.Find(_T("DialogFontSize"))>=0)
			{
#ifdef UNICODE
				swscanf_s(line,_T("DialogFontSize=%d"),&fontsize);
#else
				sscanf_s(line,_T("DialogFontSize=%d"),&fontsize);
#endif
			}
		}
		else if(i_type==1)
		{
			int resID=0;
#ifdef UNICODE
			swscanf_s(line,_T("%d=%[^\n]"),&resID,str,512);
#else
			sscanf_s(line,_T("%d=%[^\n]"),&resID,str,512);
#endif
			if (resID>0)
			{
				CString str2=str;
				str2.Replace(_T("\\n"),_T("\n"));
				str2.Replace(_T("\\r"),_T("\r"));
				str2.Replace(_T("\\t"),_T("\t"));
				m_StringTable.insert(pair<UINT,CString>(resID,str2));
				//AfxMessageBox(str2);
			}
		}
	}
	
	m_tLogFont.lfHeight	= ScaleY(fontsize);
	m_tLogFont.lfWeight=FW_NORMAL;
	m_dialogFont.DeleteObject();
	m_dialogFont.CreateFontIndirect(&m_tLogFont);
	fclose(fin);
}
void CMultiLanguage::ReadButtonName(CString lang)
{
	//初始化函数列表
	CString line;
	TCHAR str[512];
	CList<FUNCITEM, FUNCITEM&> *pList=NULL;
	BOOL bReset=FALSE;
	pair<map<CString,CList<FUNCITEM, FUNCITEM&>*>::iterator,bool> pit;
	FILE* fin=NULL;
	line.Format(_T("Languages\\FUNCTION_%s.cfg"),lang);
	_wfopen_s(&fin,line,_T("r,ccs=UNICODE"));
	if (fin==NULL)
	{
		return;
	}
	theFunc.clear();
	pageSet[_T("IDD_VIEWCHK")]=0;
	CStdioFile file(fin);
	while( file.ReadString(line) )
	{
		memset(str,0,512);
		line.TrimLeft();
		if(line.IsEmpty())
			continue;
		if (line.GetAt(0)=='[')
		{
			swscanf_s(line,_T("[%[^]]"),str,512);
			//查找继承的父类
			int index=line.Find('=');
			if (index>0)
			{
				CString parent=line.Right(line.GetLength()-index-1);
				parent.Trim();
				map<CString,CList<FUNCITEM, FUNCITEM&>*>::iterator iter;
				iter=theFunc.find(parent);
				if (iter!=theFunc.end())
				{//找到了继承
					bReset=TRUE;
					pList=iter->second;
					pit=theFunc.insert(pair<CString,CList<FUNCITEM, FUNCITEM&>*>(str,pList));
				}
			}
			else
			{
				bReset=FALSE;
				pList=new CList<FUNCITEM, FUNCITEM&>;
				theFunc.insert(pair<CString,CList<FUNCITEM, FUNCITEM&>*>(str,pList));
			}
			pageSet[str]=0;
		}
		else
		{
			FUNCITEM item;
			item.range=0;
			swscanf_s(line,_T("%s%d%d"),str,512,&item.funcid,&item.range);
			item.name=str;
			//查找末尾的注释
			int index=line.Find(_T("//"));
			if (index>0)
			{
				item.remark=line.Right(line.GetLength()-index-2);
				item.remark.Replace(_T("\\n"),_T("\n"));
			}
			if (bReset)
			{//拷贝一个新的列表
				bReset=FALSE;
				CList<FUNCITEM, FUNCITEM&> *oldList=pList;
				pList=new CList<FUNCITEM, FUNCITEM&>;
				POSITION pos2 = oldList->GetHeadPosition();
				while(pos2)
					pList->AddTail(oldList->GetNext(pos2));
				pit.first->second=pList;
			}
			if(pList!=NULL)
				pList->AddTail(item);
		}
	}
	fclose(fin);
}
BOOL CMultiLanguage::UpdatePage(CWnd* pWnd,CString pageName,BOOL bAways)
{
	if (bAways==FALSE&&pageSet[pageName]==1)
		return FALSE;
	map<CString,CList<FUNCITEM, FUNCITEM&>*>::iterator iter;
	iter=theFunc.find(pageName);
	if (iter!=theFunc.end())
	{
		CList<FUNCITEM, FUNCITEM&>* pList=iter->second;
		POSITION pos=pList->GetHeadPosition();
		while (pos)
		{
			POSITION pre=pos;
			FUNCITEM item=pList->GetNext(pos);
			CWnd* button=pWnd->GetDlgItem(item.funcid);
			if (button)
			{
				button->SetWindowText(item.remark);
				CRoundButton2* bt2=dynamic_cast<CRoundButton2*>(button);
				if(bt2)
					bt2->SetInvalid();
			}
		}
		//pWnd->Invalidate();
	}
	if(bAways==FALSE)
		pageSet[pageName]=1;
	return TRUE;
}
void CMultiLanguage::LoadString(UINT uResID, CString& strVal)
{
	map<UINT,CString>::iterator iter;
	iter=m_StringTable.find(uResID);
	if (iter==m_StringTable.end())
	{
		strVal.LoadString(uResID);
	}
	else
	{
		strVal=iter->second;
	}
}
void CMultiLanguage::LoadString(CString& strVal,UINT uResID)
{
	map<UINT,CString>::iterator iter;
	iter=m_StringTable.find(uResID);
	if (iter==m_StringTable.end())
	{
		strVal.LoadString(uResID);
	}
	else
	{
		strVal=iter->second;
	}
}
void CMultiLanguage::TranslateDlgItem(CWnd* pCtrl,UINT uResID)
{
	if (pCtrl)
	{
		CString str2;
		LoadString(uResID,str2);
		pCtrl->SetWindowText(str2);
		pCtrl->SetFont(&m_dialogFont);
	}
}
