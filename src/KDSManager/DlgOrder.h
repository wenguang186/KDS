#pragma once

#include "afxwin.h"
#include <afxdb.h>
#include <vector>

#include "afxcmn.h"
#include "common.h"
#include "RoundButton2.h"

// CDlgOrder 对话框

class CDlgOrder : public CDialog
{
	DECLARE_DYNAMIC(CDlgOrder)

public:
	CDlgOrder(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgOrder();

// 对话框数据
	enum { IDD = IDD_DLG_ORDER };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();

public:
	long m_headId;
	CString m_headNum;
public:
	long m_checkId;
	CString m_checkNum;
public:
	CString m_employee;
public:
	CString m_orderTime;
public:
	CString m_makeTime;
public:
	CString m_pos;
public:
	CString m_eatType;
public:
	CString m_remark;
public:
	CListCtrl m_list;

public:
	BOOL LoadHead(CRecordset &rs); // 前缀const反而不行，找不到可用重载函数GetFieldValue()
	INT LoadDetail(CRecordset &rs);
public:
	long m_custNum;

protected:
	void InitListHead();
public:
	afx_msg void OnHdnEndtrackListDetail(NMHDR *pNMHDR, LRESULT *pResult);

public:	
	INT strProgress[E_P_CNT];

	enum COLINDEX{e_qua, e_unit, e_name, e_desc, e_state, e_max=e_state };
protected:
	INT m_x;
	INT m_y;
	HBITMAP m_bpBackgrd;

public:
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
public:
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
public:
	CRoundButton2 m_btnClose;
	Gdiplus::Image *m_bpBtnClose[2]; 
public:
	CString m_checkName;
	void ResetLang();
};
