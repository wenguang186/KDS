#pragma once
#include "afxwin.h"

#include "datamodel.h"
#include "common.h"
#include "RoundButton2.h"

// CDlgItemDetail 对话框

class CDlgItemDetail : public CDialog
{
	DECLARE_DYNAMIC(CDlgItemDetail)

public:
	CDlgItemDetail(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgItemDetail();

// 对话框数据
	enum { IDD = IDD_DLG_ITEM_DETAIL };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()
public:
	CStatic m_menu_item_name;
public:
	CStatic m_quantity;
public:
	CStatic m_unit;
public:
	CStatic m_description;
public:
	CStatic m_remark;
public:
	CStatic m_eat_type;
public:
	CStatic m_employee;
public:
	CStatic m_pos;
public:
	CStatic m_order_time;
public:
	CStatic m_check_number;
public:
	CStatic m_table_name;
public:
	CStatic m_customer_num;
public:
	afx_msg void OnBnClickedBtnPend();
public:
	afx_msg void OnBnClickedBtnOrder();
public:
	afx_msg void OnBnClickedBtnDone();
public:
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);

protected:
	//INT nIDMsgDone;
	INT m_x;
	INT m_y;
	HBITMAP m_bpBackgrd; // 析构时注意释放

public:	
	CDataOrderDetail m_data;

protected:
	void DoAction(INT nIDMsg);
	

public:	
	void ResetShowState(KDS_ShowType showType);
	void ClearData();
	void RefreshData();
	CBaseData* GetDataPtr();

public:
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
public:
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
public:
	afx_msg void OnBnClickedBtnRecovery();
public:
	CRoundButton2 m_btnRecovery;
public:
	CRoundButton2 m_btnDone;
public:
	CRoundButton2 m_btnOrder;
public:
	CRoundButton2 m_btnPend;
public:
	// 析构时注意释放
	Gdiplus::Image *m_bpBtnPend[2]; //想改成static而不得，出现error LNK2001.Image又不能使用默认构造函数
	Gdiplus::Image *m_bpBtnDone[2];
	Gdiplus::Image *m_bpBtnRecovery[2];
	Gdiplus::Image *m_bpBtnOrder[2];
	CFont m_itemFont; //放在局部变量中不起作用
public:
	CStatic m_checkName;
	void ResetLang();
};
