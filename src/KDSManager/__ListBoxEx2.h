#pragma once
/*********************************************************************
*Copyright (c) 2011 Coolroid Co.Ltd.
*DESCRIPTION��
*		�Ի��listbox�ؼ������Զ���ÿ�еĸ߶ȣ���ʾ2���ַ��ȡ�
*AUTHOR ��zhangyi
*HISTORY��
*		author time    version   desc

*********************************************************************/

#define ScaleY(X) X

#define LIST_COL_NUM 2

class CListBoxEx : public CListBox
{
	DECLARE_DYNAMIC(CListBoxEx)

public:
	CListBoxEx();
	virtual ~CListBoxEx();
	int m_nSubWidth[3];
	BOOL m_bCanSelect;
	CBrush m_brush;
protected:
	DECLARE_MESSAGE_MAP()
	LOGFONT m_tLogFont;
public:
	virtual void DrawItem(LPDRAWITEMSTRUCT /*lpDrawItemStruct*/);
	void MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct);
protected:
	virtual void PreSubclassWindow();
public:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL OnEraseBkgnd(CDC* pDC) ;
	afx_msg HBRUSH CtlColor(CDC* /*pDC*/, UINT /*nCtlColor*/);
};


