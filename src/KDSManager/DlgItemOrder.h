#pragma once
#include "afxwin.h"
#include <afxdb.h>
#include "afxcmn.h"
#include "datamodel.h"
#include "common.h"
#include "RoundButton2.h"
//#include "ListBoxEx.h"

// CDlgItemOrder 对话框

#define LIST_MAX_CHAR_NUM 27
//#define LIST_QUANTITY_NUM 4

class CDlgItemOrder : public CDialog
{
	DECLARE_DYNAMIC(CDlgItemOrder)

public:
	CDlgItemOrder(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgItemOrder();

// 对话框数据
	enum { IDD = IDD_DLG_ITEM_ORDER };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);

public:
	afx_msg void OnBnClickedBtnPend();
public:
	afx_msg void OnBnClickedBtnDone();
public:
	afx_msg void OnBnClickedBtnRecovery();




public:
	CStatic m_employee;
	CStatic m_pos;
	CStatic m_ordertime;
	//CStatic m_kdstime;
	CStatic m_eattype;
	CStatic m_remark;
	CStatic m_title;
	CStatic m_custnum;
	CStatic m_orderHeadId;
	CStatic m_checkNumber;
	//CListBoxEx m_list;
	//CListBox m_list;
public:
	CListCtrl m_list;

protected:
	long m_headId;
	//INT nIDMsgDone; //增加了“恢复按钮”，所以不用分辨msgid了
	enum COLINDEX{e_qua, e_unit, e_name, e_desc, e_max=e_desc };

public:
	CDataOrder m_data;




	// 界面美化
public:
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
public:
	CRoundButton2 m_btnPend;
	CRoundButton2 m_btnDone;
	CRoundButton2 m_btnRecovery;
public:
	INT m_x;
	INT m_y;
	HBITMAP m_bpBackgrd;
	Gdiplus::Image *m_bpBtnPend[2]; //想改成static而不得，出现error LNK2001.Image又不能使用默认构造函数
	Gdiplus::Image *m_bpBtnDone[2];
	Gdiplus::Image *m_bpBtnRecovery[2];
	CFont m_itemFont; //放在局部变量中不起作用
	//CFont m_listFont;

public:
	CBaseData* GetDataPtr();
	//void __LoadHead(CRecordset &);
	//INT __LoadDetail(CRecordset &rs);
	void RefreshData();
	void ResetShowState(KDS_ShowType showType);
	void ClearData();
	void ResetLang();

	void DoAction(INT nIDMsg);

public:
	afx_msg void OnNMCustomdrawListDetail(NMHDR *pNMHDR, LRESULT *pResult);
};
