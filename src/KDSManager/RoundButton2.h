/********************************************************************
	created:	2005/06/03
	created:	3:6:2005   13:21
	filename: 	x:\Software\Mfc\Source\Controls\Buttons\RoundButton2.h
	file path:	x:\Software\Mfc\Source\Controls\Buttons
	file base:	RoundButton2
	file ext:	h
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

#pragma once
#include "afxwin.h"
#include <GdiPlus.h>

class CRoundButton2 :
	public CButton
{
public:
	enum ICON_ALIGNMENT
	{
		CENTER = 0,
		LEFT,
		RIGHT,
		UP,
		DOWN,
		RIGHTDOWN
	};
	CString m_strBottom;//ÓÒÏÂ½Ç±ê×¢
	int m_textPos;
	//! Constructor
	CRoundButton2(void);
	//! Destructor
	~CRoundButton2(void);

	/************************************************************************/
	/* Functions for Design of Button                                       */
	/************************************************************************/
	void SetStrTop(LPCTSTR strMark);
	//! Set Font of Button
	bool SetFont(LOGFONT* _ptLogFont);
	//! Set Color of Caption
	bool SetTextColor(COLORREF	_ptTextColor);
	bool SetTextColor(COLORREF _ptTextColor,COLORREF _ptPressColor);
	CRoundButton2& SetIcon(UINT nIDResource, ICON_ALIGNMENT ia = LEFT);
	CRoundButton2& SetIcon(HICON hIcon, ICON_ALIGNMENT ia = LEFT,BOOL bDestroy = TRUE);
//	BOOL SetImage(HBITMAP hBitmapIn,HBITMAP hBitmapOut,BOOL bDestroy=TRUE);
	BOOL SetImages(Gdiplus::Image* hBitmapIn,Gdiplus::Image* hBitmapOut,bool bDestroy=true);
	void SetImage(CString path);
	static Gdiplus::Image* CreateDarkerBitmap(Gdiplus::Image* hBitmap);
	static COLORREF DarkenColor(COLORREF crColor, double dFactor);
	DWORD SetCheck(int nCheck, BOOL bRepaint = TRUE);
	void SetBkGnd(CDC* pDC,BOOL bForce);
	void ClearBkGnd();
	void SetInvalid();
	/************************************************************************/
	/* Message-Map of Control                                               */
	/************************************************************************/
	DECLARE_MESSAGE_MAP()
protected:

	/************************************************************************/
	/* Own Drawing-Functions                                                */
	/************************************************************************/
	//! Draw Caption on Button
	void DrawButtonCaption(CDC *_pDC,LPCTSTR sCaption,CRect& rect);
	//! PreSubclass-Function
	virtual void PreSubclassWindow();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	void DrawIcon(BOOL bHasText,CRect& rectText);
	void PrepareIconRect(BOOL bHasText,CRect& rectText,DWORD dwWidth,DWORD dwHeight,CRect& rectImage);
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	virtual LRESULT DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam);
private:
	//! Size of Button-Images
	CRect	m_rBtnSize;
	//! Font for Caption
	CFont	m_tBtnFont;
	//! Color Scheme of Caption
	COLORREF	m_tTextColor[2];
	HICON			m_hIcon;
	ICON_ALIGNMENT	m_eIconAlignment;
	Gdiplus::Image* m_tImage;
	Gdiplus::Image* m_tImagep;
	CDC m_Memdc;
	CDC	m_dcBk;
	bool	m_bRedraw;
	bool	m_bDeleteImage;
	CString m_strTop;
	int m_nCheck;
};