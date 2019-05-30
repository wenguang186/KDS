/********************************************************************
created:	2005/06/03
created:	3:6:2005   13:22
filename: 	x:\Software\Mfc\Source\Controls\Buttons\RoundButton2.cpp
file path:	x:\Software\Mfc\Source\Controls\Buttons
file base:	RoundButton2
file ext:	cpp
author:		Markus Zocholl

purpose:	CRoundButton2 defines a universal Button-Control with the 
following features:

* Shape is a rounded Rectangle
* Button includes Border and Button-Face
* Many parameters to get an individual look
* Functions of Button to be en- or disabled:
- Button (disabled means a static control with userdefined styles)
- Hover
*********************************************************************/

#include "StdAfx.h"
#include <math.h>
#include <GdiPlus.h>
#include <GdiPlusPixelFormats.h>
#include ".\RoundButton2.h"
//#include "CreateGrayscaleIcon.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define BTN_STATE_NUM 2
/************************************************************************/
/* Construction and Destruction                                         */
/************************************************************************/
//! Construction
CRoundButton2::CRoundButton2(void):	
 m_rBtnSize(CRect(0, 0, 0, 0))
, m_bRedraw(true)
{
	m_tImage=NULL;
	m_tImagep=NULL;
	m_hIcon = NULL;
	m_bDeleteImage=true;
	m_textPos=0;
	m_tTextColor[0]= RGB(0, 0, 0);
	m_tTextColor[1]= RGB(0, 0, 0);
	m_nCheck=-1;
}

//! Destruction
CRoundButton2::~CRoundButton2(void)
{
	if (m_hIcon)
		DestroyIcon(m_hIcon);
	m_hIcon = NULL;
	if (m_tBtnFont.m_hObject != NULL)
		m_tBtnFont.DeleteObject();
	if(m_bDeleteImage)
	{
		delete m_tImage;
		delete m_tImagep;
	}
	m_Memdc.DeleteDC();
	
}

/************************************************************************/
/* public Functions                                                     */
/************************************************************************/

void CRoundButton2::SetStrTop(LPCTSTR strMark)
{
	m_strTop=strMark;
	m_bRedraw=true;
}
// Set Font of Button
bool CRoundButton2::SetFont(LOGFONT* _ptLogFont)
{
	if (_ptLogFont == NULL)
		return false;

	// Delete Font, if already given
	if (m_tBtnFont.m_hObject != NULL)
		m_tBtnFont.DeleteObject();

	// Create new Font
	m_tBtnFont.CreateFontIndirect(_ptLogFont);

	// Button should be redrawn
	m_bRedraw = true;

	return true;
}

//=============================================================================	
// Sets icon from resource id
CRoundButton2& CRoundButton2::SetIcon(UINT nIDResource, ICON_ALIGNMENT ia /*= LEFT*/)
//=============================================================================	
{
	HICON hIcon = (HICON) LoadImage(AfxGetInstanceHandle(),
		MAKEINTRESOURCE(nIDResource),
		IMAGE_ICON,
		0,
		0,
		0);

	return SetIcon(hIcon, ia);
}

//=============================================================================	
// Sets icon from HICON
CRoundButton2& CRoundButton2::SetIcon(HICON hIcon, ICON_ALIGNMENT ia /*= LEFT*/,BOOL bDestroy)
//=============================================================================	
{
	if (bDestroy&&m_hIcon)
		DestroyIcon(m_hIcon);

	m_hIcon = hIcon;
	m_eIconAlignment = ia;
	m_bRedraw=true;
	Invalidate();
	return *this;
}
//=============================================================================	
void CRoundButton2::PrepareIconRect( BOOL bHasText,
									 CRect& rectText,
									 DWORD dwWidth,
									 DWORD dwHeight,
									 CRect& rectImage)
{
	rectImage.CopyRect(&rectText);
	int diff=15;
	if (bHasText)
	{
		// image must be placed just inside the focus rect
		int nTextWidth = rectText.right - rectText.left;
		if (m_eIconAlignment == CRoundButton2::RIGHT)
		{
			rectText.right = nTextWidth - dwWidth-diff;
			rectImage.left = rectText.right;
			// center image vertically
			int nImageHeight = rectImage.bottom - rectImage.top;
			rectImage.top += ((nImageHeight - dwHeight)/2);
		}
		else if (m_eIconAlignment == CRoundButton2::DOWN)
		{
			HGDIOBJ hOldFont;
			if (m_tBtnFont.m_hObject == NULL)
			{
				CWnd* pParentWnd=GetParent();
				hOldFont=m_Memdc.SelectObject(pParentWnd->GetFont());
			}
			else
				hOldFont=m_Memdc.SelectObject(&m_tBtnFont);
			CString sCaption;
			this->GetWindowText(sCaption);
			CRect temp=rectText;
			int height = m_Memdc.DrawText(sCaption,temp,DT_CENTER | DT_WORDBREAK | DT_CALCRECT | DT_EDITCONTROL); // 获得文本高度
			rectImage.left=(nTextWidth-dwWidth)/2;
			int nImageHeight = rectImage.bottom - rectImage.top;
			rectImage.top += (nImageHeight-height)/2+height;
		}
		else if (m_eIconAlignment == CRoundButton2::UP)
		{
			rectImage.top = 10;
			rectImage.bottom =dwHeight+10;
			rectImage.left=(nTextWidth-dwWidth)/2;
			rectText.top=rectImage.bottom;
		}
		else if (m_eIconAlignment == CRoundButton2::CENTER)
		{
			// center image horizontally
			int nImageWidth = rectImage.right - rectImage.left;
			rectImage.left += ((nImageWidth - dwWidth)/2);
			// center image vertically
			int nImageHeight = rectImage.bottom - rectImage.top;
			rectImage.top += ((nImageHeight - dwHeight)/2);
		}
		else if (m_eIconAlignment == CRoundButton2::RIGHTDOWN)
		{//右下
			rectText.right = nTextWidth;
			rectImage.left = nTextWidth-dwWidth;
			int nImageHeight = rectImage.bottom - rectImage.top;
			rectImage.top += nImageHeight - dwHeight;
		}
		else//默认靠左
		{
			//rectText.right = nTextWidth - 3;
			rectText.left  =dwWidth+diff;
			rectImage.right = rectText.left;
			rectImage.left=diff;
			// center image vertically
			int nImageHeight = rectImage.bottom - rectImage.top;
			rectImage.top += ((nImageHeight - dwHeight)/2);
		}
	}
	else	// no text
	{
		// center image horizontally
		int nImageWidth = rectImage.right - rectImage.left;
		rectImage.left += ((nImageWidth - dwWidth)/2);
		// center image vertically
		int nImageHeight = rectImage.bottom - rectImage.top;
		rectImage.top += ((nImageHeight - dwHeight)/2);
	}
}
//=============================================================================	
void CRoundButton2::DrawIcon(BOOL bHasText,CRect& rectText)
{
	if (m_hIcon)
	{
		DWORD dwWidth = 48;		// assume 32x32
		DWORD dwHeight = 48;
		ICONINFO iconinfo;
		if (GetIconInfo(m_hIcon, &iconinfo))
		{
			CBitmap* pBitmap = CBitmap::FromHandle(iconinfo.hbmColor);
			if (pBitmap)
			{
				BITMAP bm;
				pBitmap->GetBitmap(&bm);
				dwWidth = bm.bmWidth;
				dwHeight = bm.bmHeight;
			}

			if (iconinfo.hbmColor)
				::DeleteObject(iconinfo.hbmColor);
			if (iconinfo.hbmMask) 
				::DeleteObject(iconinfo.hbmMask);
		}

		CRect rectImage(m_rBtnSize);
		PrepareIconRect(bHasText,rectText,dwWidth, dwHeight, rectImage);
		for (int nState = 0; nState < BTN_STATE_NUM; nState++)
		{
			CPoint point;
			point=CPoint(rectImage.left,nState *m_rBtnSize.Height()+rectImage.top);
			m_Memdc.DrawState(point,
				CSize(rectImage.right - rectImage.left, rectImage.bottom - rectImage.top),
				m_hIcon,
				DSS_NORMAL|DST_ICON,
				(CBrush *) NULL);
		}
	}
}
//! Set Color of Caption
bool CRoundButton2::SetTextColor(COLORREF _ptTextColor)
{
	m_tTextColor[0]=_ptTextColor;
	m_tTextColor[1]=_ptTextColor;
	// Button should be redrawn
	m_bRedraw = true;
	return true;
}
bool CRoundButton2::SetTextColor(COLORREF _ptTextColor,COLORREF _ptPressColor)
{
	m_tTextColor[0]=_ptTextColor;
	m_tTextColor[1]=_ptPressColor;
	// Button should be redrawn
	m_bRedraw = true;
	return true;
}
/************************************************************************/
/* Own Drawing-Functions                                                */
/************************************************************************/


//! Draw-Item-Function
/*! This Function is called each time, the Button needs a redraw
*/
void CRoundButton2::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	CDC* pDC = CDC::FromHandle(lpDrawItemStruct->hDC);
	CString strCaption;
	GetWindowText(strCaption);
	m_rBtnSize = lpDrawItemStruct->rcItem;
	CRect rectText = lpDrawItemStruct->rcItem;
	if (m_bRedraw)
	{
		if(m_Memdc.m_hDC==NULL)
			m_Memdc.CreateCompatibleDC(pDC);
		CBitmap bmp;
		bmp.CreateCompatibleBitmap(pDC, m_rBtnSize.Width(), m_rBtnSize.Height()*BTN_STATE_NUM);
		m_Memdc.SelectObject(&bmp);
		bmp.DeleteObject();
		if(m_tImage && m_tImage->GetPixelFormat()& PixelFormatAlpha)
		{//含有透明图层
			if (m_dcBk.m_hDC == NULL)
			{
				CRect rect;
				GetWindowRect(rect);
				GetParent()->ScreenToClient(rect);
				CClientDC clDC(GetParent());
				m_dcBk.CreateCompatibleDC(&clDC);
				bmp.CreateCompatibleBitmap(&clDC, m_rBtnSize.Width(), m_rBtnSize.Height());
				m_dcBk.SelectObject(&bmp);
				m_dcBk.BitBlt(0, 0, m_rBtnSize.Width(), m_rBtnSize.Height(), &clDC, rect.left, rect.top, SRCCOPY);
				bmp.DeleteObject();
			}
			m_Memdc.BitBlt(0, 0, m_rBtnSize.Width(), m_rBtnSize.Height(), &m_dcBk, 0, 0, SRCCOPY);
			m_Memdc.BitBlt(0, m_rBtnSize.Height(), m_rBtnSize.Width(), m_rBtnSize.Height(), &m_dcBk, 0, 0, SRCCOPY);
		}
		Gdiplus::Graphics graphics(m_Memdc.GetSafeHdc());
		graphics.DrawImage(m_tImage,0,0,m_rBtnSize.Width(), m_rBtnSize.Height());
		graphics.DrawImage(m_tImagep,0,m_rBtnSize.Height(),m_rBtnSize.Width(), m_rBtnSize.Height());
		
		BOOL bHasText=TRUE;
		if(strCaption.GetLength()==0)
			bHasText=FALSE;
		DrawIcon(bHasText,rectText);
		// Draw Button-Caption
		DrawButtonCaption(&m_Memdc,strCaption,rectText);
		m_bRedraw=false;
	}
	int nButtonState = 0;
	if ((lpDrawItemStruct->itemState & ODS_SELECTED) == ODS_SELECTED)
		nButtonState = 1;
	if(m_nCheck>=0)
	{
		nButtonState=m_nCheck;
	}
	// Copy correct Bitmap to Screen
	pDC->BitBlt(0,0,m_rBtnSize.Width(),m_rBtnSize.Height(),&m_Memdc,0,m_rBtnSize.Height() * nButtonState,SRCCOPY);
}


//! Draw Caption on Button
void CRoundButton2::DrawButtonCaption(CDC *_pDC,LPCTSTR sCaption,CRect& rect)
{
	// Select Transparency for Background
	_pDC->SetBkMode(TRANSPARENT);
	CFont* captionFont=NULL;
	if (m_tBtnFont.m_hObject == NULL)
	{
		captionFont=GetParent()->GetFont();
	}
	else
		captionFont=&m_tBtnFont;
	static CFont smallFont;
	if(smallFont.m_hObject==NULL)
		smallFont.CreatePointFont(90,_T("Microsoft YaHei"));
	for (int nState = 0; nState < BTN_STATE_NUM; nState++)
	{
		_pDC->SetTextColor(m_tTextColor[nState]);
		CRect temp ;
		CRect temp_raw=CRect(rect.left, nState * m_rBtnSize.Height() + rect.top, rect.right, nState * m_rBtnSize.Height() + rect.bottom);
		//画脚注
		if (!m_strTop.IsEmpty())
		{
			_pDC->SelectObject(&smallFont);
			temp = CRect(rect.left, nState * m_rBtnSize.Height() + rect.top, rect.right-5, nState * m_rBtnSize.Height() + rect.bottom);
			_pDC->DrawText(m_strTop,temp,DT_RIGHT| DT_TOP| DT_EDITCONTROL | DT_WORDBREAK);
			temp_raw.right-=8;
		}
		if (!m_strBottom.IsEmpty())
		{
			_pDC->SelectObject(&smallFont);
			temp = CRect(rect.left, nState * m_rBtnSize.Height() + rect.top, rect.right-5, nState * m_rBtnSize.Height() + rect.bottom);
			_pDC->DrawText(m_strBottom,temp,DT_RIGHT| DT_BOTTOM| DT_SINGLELINE);
			temp_raw.bottom-=8;
		}
		temp=temp_raw;
		_pDC->SelectObject(captionFont);
		if(m_textPos==1)
		{
			_pDC->DrawText(sCaption,temp,DT_TOP| DT_CENTER | DT_EDITCONTROL | DT_WORDBREAK);
		}
		else if(m_textPos==2)
		{
			_pDC->DrawText(sCaption,temp,DT_BOTTOM| DT_CENTER | DT_SINGLELINE);
		}
		else
		{
			int height = _pDC->DrawText(sCaption,temp,DT_CENTER | DT_WORDBREAK | DT_CALCRECT | DT_EDITCONTROL); // 获得文本高度
			if(temp_raw.Height() > height)
				temp_raw.DeflateRect(0,(temp_raw.Height() - height)/2); // 改变rect才能垂直居中
			_pDC->DrawText(sCaption,temp_raw,DT_CENTER | DT_EDITCONTROL | DT_WORDBREAK);
		}
		
	}
}

/************************************************************************/
/* Overwritten Functions for Init and Draw of Button                    */
/************************************************************************/

//! Presubclass-Window-Function
void CRoundButton2::PreSubclassWindow()
{
#ifdef _DEBUG
	// We really should be only sub classing a button control
	TCHAR buffer[255];
	GetClassName (m_hWnd, buffer, sizeof(buffer) / sizeof(TCHAR));
	ASSERT (CString (buffer) == _T("Button"));
#endif

	// Make the button owner-drawn
	ModifyStyle (0x0FL, BS_OWNERDRAW | BS_AUTOCHECKBOX, SWP_FRAMECHANGED);

	CButton::PreSubclassWindow();
}
void CRoundButton2::ClearBkGnd()
{
	m_dcBk.DeleteDC();
	m_dcBk.m_hDC=NULL;
	m_bRedraw=true;
}
void CRoundButton2::SetInvalid()
{
	m_bRedraw=true;
	Invalidate();
}
void CRoundButton2::SetBkGnd(CDC* pDC,BOOL bForce)
{
	if(bForce==FALSE&&m_dcBk.m_hDC!=NULL)
		return;
	CBitmap bmp;
	CRect rect;
	GetWindowRect(rect);
	GetParent()->ScreenToClient(rect);
	m_dcBk.DeleteDC();
	m_dcBk.CreateCompatibleDC(pDC);
	bmp.CreateCompatibleBitmap(pDC, rect.Width(), rect.Height());
	m_dcBk.SelectObject(&bmp);
	m_dcBk.BitBlt(0, 0, rect.Width(), rect.Height(), pDC, rect.left, rect.top, SRCCOPY);
	bmp.DeleteObject();
	m_bRedraw=true;
	//UpdateWindow();
	Invalidate();
}
BOOL CRoundButton2::OnEraseBkgnd(CDC* pDC) 
{
	return TRUE;
}
BEGIN_MESSAGE_MAP(CRoundButton2, CButton)
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()

LRESULT CRoundButton2::DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	if (message == WM_LBUTTONDBLCLK)
	{
		message = WM_LBUTTONDOWN;
	}
	return CButton::DefWindowProc(message, wParam, lParam);
}

void CRoundButton2::SetImage(CString path)
{
	Gdiplus::Image* img1=Gdiplus::Image::FromFile(path);
	if(img1)
	{
		path.Replace(_T("."),_T("_."));
		Gdiplus::Image* img2=Gdiplus::Image::FromFile(path);
		SetImages(img1,img2);
	}
}
BOOL CRoundButton2::SetImages(Gdiplus::Image* hBitmapIn,Gdiplus::Image* hBitmapOut,bool bDestroy)
{
	m_bDeleteImage=bDestroy;
	if (bDestroy)
	{
		delete m_tImage;
		delete m_tImagep;
	}
	if (hBitmapIn)
	{
		m_tImage = hBitmapIn;
		if (hBitmapOut&&hBitmapOut->GetLastStatus()==Gdiplus::Ok)
		{
			m_tImagep=hBitmapOut;
		}
		else
		{
			m_tImagep = CreateDarkerBitmap(hBitmapIn);
		}
	}
// 	if(bDestroy)
// 		RedrawWindow();
	m_bRedraw=true;
	return TRUE;
} // End of SetBitmaps
DWORD CRoundButton2::SetCheck(int nCheck, BOOL bRepaint)
{
	if (nCheck == 0)
		m_nCheck = 0;
	else 
		m_nCheck = 1;
	if (bRepaint)
		Invalidate();
	//return BTNST_OK;
	return 1;
}

Gdiplus::Image* CRoundButton2::CreateDarkerBitmap(Gdiplus::Image* originalImage)
{
	if(originalImage==NULL)
		return NULL;
	Gdiplus::Image* clonedImage=originalImage->Clone();
	Gdiplus::ColorMatrix cm =
	{
		1.5, 0, 0, 0, 0,
		0, 1.5, 0, 0, 0,
		0, 0, 1.5, 0, 0,
		0, 0, 0, 1, 0,
		-0.3, -0.3, -0.3, 0, 1
	};
	Gdiplus::ImageAttributes imageAttributes;
	imageAttributes.SetColorMatrix(&cm);
	Gdiplus::Rect destRect(0, 0, originalImage->GetWidth(), originalImage->GetHeight());
	Gdiplus::Graphics graphics(clonedImage);
	graphics.DrawImage(originalImage,destRect,0,0,originalImage->GetWidth(), originalImage->GetHeight(),Gdiplus::UnitPixel,&imageAttributes);
	return clonedImage;
		
} // End of CreateDarkerBitmap

COLORREF CRoundButton2::DarkenColor(COLORREF crColor, double dFactor)
{
	if (dFactor > 0.0 && dFactor <= 1.0)
	{
		BYTE red,green,blue,lightred,lightgreen,lightblue;
		red = GetRValue(crColor);
		green = GetGValue(crColor);
		blue = GetBValue(crColor);
		lightred = (BYTE)(red-(dFactor * red));
		lightgreen = (BYTE)(green-(dFactor * green));
		lightblue = (BYTE)(blue-(dFactor * blue));
		crColor = RGB(lightred,lightgreen,lightblue);
	} // if

	return crColor;
} // End of DarkenColor