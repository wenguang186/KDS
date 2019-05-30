// ListBoxEx.cpp : 实现文件
//

#include "stdafx.h"
#include "ListBoxEx2.h"
//#include "POSClient.h"

#ifdef FREE_VERSION
#define BG_COLOR RGB(255,255,255)
#else
#define BG_COLOR RGB(254,247,235)
#endif
// CListBoxEx

IMPLEMENT_DYNAMIC(CListBoxEx, CListBox)

CListBoxEx::CListBoxEx()
{
	m_bCanSelect=TRUE;
	memset(&m_tLogFont,0,sizeof(LOGFONT));
	m_tLogFont.lfHeight	= ScaleY(24);
	m_tLogFont.lfWeight=700;
	wcscpy_s(m_tLogFont.lfFaceName, _T("Microsoft YaHei"));

	m_brush.CreateSolidBrush(BG_COLOR);
}

CListBoxEx::~CListBoxEx()
{
}


BEGIN_MESSAGE_MAP(CListBoxEx, CListBox)
	ON_WM_MEASUREITEM_REFLECT()
	ON_WM_LBUTTONDOWN()
	ON_WM_ERASEBKGND()
	ON_WM_CTLCOLOR_REFLECT()
END_MESSAGE_MAP()



// CListBoxEx 消息处理程序



void CListBoxEx::DrawItem(LPDRAWITEMSTRUCT lpdis)
{

	if (lpdis->itemID < 0) 
		return;  
	if (lpdis->itemID >= (UINT)GetCount()) 
		return; 
	CDC* pDC = CDC::FromHandle(lpdis->hDC); 
	CRect rect(lpdis->rcItem); 

	CDC MemDC; 
	CBitmap MemBitmap; 

	MemDC.CreateCompatibleDC(pDC); 
	MemBitmap.CreateCompatibleBitmap(pDC, rect.Width(), rect.Height()); 
	CBitmap *pOldBit = MemDC.SelectObject(&MemBitmap); 

	CRect drRect(CPoint(0, 0), CSize(rect.Width(), rect.Height()));

	COLORREF clBk;
	if (lpdis->itemState & ODS_SELECTED) 
		clBk = ::GetSysColor(COLOR_HIGHLIGHT); 
	else 
		clBk = BG_COLOR;
	MemDC.FillSolidRect(&drRect,clBk); 
	
 	CFont   font;   
	m_tLogFont.lfHeight			= ScaleY(24);
	CString strText,subText;
	GetText(lpdis->itemID,strText);
	int preWidth=0;
	int index=0;
	//获取格式信息
	index=strText.Find('|',0);
	subText=strText.Left(index);
	strText.Delete(0,index+1);
	if(subText.GetLength()!=0)
	{
		m_tLogFont.lfHeight=ScaleY(_wtoi(subText));
		MemDC.SetTextColor(RGB(250,11,11));
		//rect.bottom=rect.top+m_tLogFont.lfHeight;
	}
	else
		MemDC.SetTextColor(RGB(24,111,223));
	if (lpdis->itemState & ODS_SELECTED)
	{
		MemDC.SetTextColor(RGB(250,250,250));
	}
	font.CreateFontIndirect(&m_tLogFont);
	MemDC.SelectObject(&font);
	for(int i=0;i<3;i++)
	{
		drRect.top=0;
		drRect.bottom=rect.Height();
		drRect.left=rect.left+preWidth;
		drRect.right=drRect.left+m_nSubWidth[i];
		preWidth=drRect.right;

		index=strText.Find('|',0);
		subText=strText.Left(index);
		strText.Delete(0,index+1);
		if(strText.GetLength()==1)
		{//只剩最后一个'|'
			drRect.right+=m_nSubWidth[i+1];
		}
		if(i==2)
			MemDC.DrawText(subText,-1,&drRect,DT_RIGHT);
		else
			MemDC.DrawText(subText,-1,&drRect,DT_LEFT | DT_EDITCONTROL | DT_WORDBREAK );
	}

#if 0
	OrderDetail* item=(OrderDetail*)GetItemDataPtr(lpdis->itemID);
	if (item&&item->n_seat_num>0)
	{
		WCHAR c=0x2474+item->n_seat_num-1;
		drRect.right=rect.right;
		MemDC.DrawText(&c,1,&drRect,DT_RIGHT);
	}
#endif

	pDC->BitBlt(rect.left, rect.top, rect.Width(), rect.Height(), 
		&MemDC, 0, 0, SRCCOPY); 

	MemBitmap.DeleteObject(); 
	MemDC.DeleteDC(); 
}
void CListBoxEx::MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct)
{
	CString strText;
	GetText(lpMeasureItemStruct->itemID, strText);
	int iBegin=strText.Find('|',0);
	CString subText=strText.Left(iBegin);
	int height=0;
	m_tLogFont.lfHeight	= ScaleY(24);
	if(subText.GetLength()!=0)
	{
		m_tLogFont.lfHeight	=ScaleY(_wtoi(subText));
	}
	CFont   font;
	font.CreateFontIndirect(&m_tLogFont);
	iBegin=strText.Find('|',iBegin+1);
	int iEnd=strText.Find('|',iBegin+1);
	subText=strText.Mid(iBegin+1,iEnd-iBegin-1);
	CRect rcLabel=CRect(0,0,m_nSubWidth[1],25);
	if (iEnd==strText.GetLength()-2)
	{
		rcLabel.right+=m_nSubWidth[2];
	}
	CDC *pDC = GetDC();
    CFont *pOldFont = pDC->SelectObject(&font);
	int itemHeight = pDC->DrawText( subText, -1, rcLabel, DT_CALCRECT| DT_EDITCONTROL | DT_WORDBREAK );
	pDC->SelectObject(pOldFont);
    ReleaseDC(pDC);
    lpMeasureItemStruct->itemHeight = itemHeight;
	if (lpMeasureItemStruct->itemHeight>96)
	{
		lpMeasureItemStruct->itemHeight==96;//最多显示4行
	}
}

void CListBoxEx::PreSubclassWindow()
{
	ModifyStyle(NULL, LBS_OWNERDRAWVARIABLE | LBS_HASSTRINGS);
	CListBox::PreSubclassWindow();
}

//设置点击的范围，只在item区域内有效
void CListBoxEx::OnLButtonDown(UINT nFlags, CPoint point)
{
	if (m_bCanSelect==FALSE)
	{
		return;
	}
	BOOL bOutside;
	int item=CListBox::ItemFromPoint(point,bOutside);
	RECT rect;
	GetItemRect(item,&rect);
	if(PtInRect(&rect,point))
	{
		CListBox::OnLButtonDown(nFlags, point);
	}
	else
	{
		SetFocus();
	}

}
BOOL CListBoxEx::OnEraseBkgnd(CDC* pDC) 
{
	CRect rcClient; 
	GetClientRect(&rcClient);
	pDC->FillSolidRect(rcClient,BG_COLOR);
	return TRUE;
}
BOOL CListBoxEx::PreTranslateMessage(MSG* pMsg)
{
	if(pMsg->message==WM_KEYDOWN)
	{
		return FALSE;
	}
	return CListBox::PreTranslateMessage(pMsg);
}

HBRUSH CListBoxEx::CtlColor(CDC* pDC, UINT /*nCtlColor*/)
{
	return m_brush;
}
