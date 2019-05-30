#pragma once
#include "datamodel.h"
#include "common.h"
#include "RoundButton2.h"
#include "atlimage.h"
#include "afxwin.h"
#include <algorithm>
using namespace std;

// CDlgItemDetailNew2 对话框

class CDlgItemDetailNew2 : public CDialog
{
	DECLARE_DYNAMIC(CDlgItemDetailNew2)

public:
	CDlgItemDetailNew2(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgItemDetailNew2();

// 对话框数据
	enum { IDD = IDD_DLG_ITEM_DETAILNEW2 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);

	DECLARE_MESSAGE_MAP()
public:
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnPaint();
	afx_msg void OnStnClickedStaticItemName1();
	afx_msg LRESULT OnNcHitTest(CPoint point);
	
public:
	CBaseData* GetDataPtr();
	void ResetShowState(KDS_ShowType showType);
	void ClearData();
	void RefreshData();

public:	
	CDataOrderDetail m_data;
	KDS_ShowType showType;
	
public:
	CStatic m_eat_type;
	CStatic m_itemName1;
	CStatic m_itemName2;
	CStatic m_description;
	CStatic m_quantity;
	CStatic m_waitTime;
	CStatic m_tableName;
	CStatic m_waitPicture;
	CStatic m_selectPicture;
	CStatic m_rushPicture;
	CStatic m_outTimePicture;

protected:
	//INT m_x;
	//INT m_y;
	//HBITMAP m_bpBackgrd; // 析构时注意释放
	//HBITMAP m_bpBackgrd2;
	

public:
	void ResetLang();

protected:
	void DoAction(INT nIDMsg);


protected:
	BOOL m_bIsCombine;	//为TRUE时表示选择合并
	BOOL m_bIsRush;//为TRUE时表示催菜
public:
	BOOL m_bShowSelectPicture; //为TRUE时显示打勾选择picture控件
	BOOL m_bItemNameCanClick;	//要在ResetShowState()函数后设置，不然ResetShowState()函数里会覆盖
	BOOL m_bSelectIn; //响应快捷键选中，TURE时更改为选中背景
	void SetCombine(BOOL bFlag);
	BOOL GetCombineFlag();
	void refreshTipPicture(INT ctrlId,BOOL bBlag,CString pngNameFlagTure,CString pngNameFlagFalse);
	void SelectItemPage(BOOL bFlag);

public:
	BOOL bShowTable;
};
