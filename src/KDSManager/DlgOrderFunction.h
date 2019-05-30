#pragma once
#include "afxwin.h"
#include "datamodel.h"
#include "common.h"
#include "DlgItemDetailNew2.h"


// CDlgOrderFunction 对话框

class CDlgOrderFunction : public CDialog
{
	DECLARE_DYNAMIC(CDlgOrderFunction)

public:
	CDlgOrderFunction(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgOrderFunction();

// 对话框数据
	enum { IDD = IDD_DLG_ORDERFUNCTION };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);

	DECLARE_MESSAGE_MAP()
public:
	CButton m_btnCookComplete;
	CButton m_btnCombineOrder;
	CButton m_btnPendOrder;
	CButton m_btnDoNotMake;
	CButton m_btnCloseDlg;

public:
	CStatic m_checkName;
	CStatic m_tabelName2;
	CStatic m_customerNum;
	CStatic m_eatTypeDetail;
	CStatic m_remark;
	CStatic m_openEmploeeName;
	CStatic m_posName;
	CStatic m_orderTime;
	CStatic m_completeTime;

protected:
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnPaint();

public:
	afx_msg void OnBnClickedBtnCombineorder();
	afx_msg void OnBnClickedBtnCookcomplete();
	afx_msg void OnBnClickedBtnPendorder();
	afx_msg void OnBnClickedBtnDoNotMake();
	afx_msg void OnBnClickedBtnClosedlg();

public:
	CDlgItemDetailNew2 dlgItemDetailNew2;

protected:
	void DoAction(INT nIDMsg);

public:	
	CDataOrderDetail m_data;
	KDS_ShowType m_showType;

public:
	void ResetShowState();
	void ClearData();
	void RefreshData();
	void SetShowType(KDS_ShowType showType);
	void ResetLang();
	
	void ShowItemDetailDlg(void);
};
