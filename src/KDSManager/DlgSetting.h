#pragma once
//#include "afxwin.h"
#include <vector>
#include<map>
#include "dbmanager.h"
#include "common.h"
#include "RoundButton2.h"
#include "DlgLangSelect.h"
#include "afxwin.h"

#define MY_CTRL_ID_START 2000
// CDlgSetting 对话框

class CDlgSetting : public CDialog
{
	DECLARE_DYNAMIC(CDlgSetting)

public:
	CDlgSetting(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgSetting();

// 对话框数据
	enum { IDD = IDD_DLG_SETTING };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
public:
	INT m_refreshTime;
	CStatic m_ctrlUnitSecond;
	afx_msg void OnBnClickedBtnRefreshTimeSubtract();
	afx_msg void OnBnClickedBtnRefreshTimeAdd();

public:
	afx_msg void OnBnClickedOk();
public:
	afx_msg void OnBnClickedCancel();

protected:
	void EnableKitchenStalls(BOOL enable);

public:
	void SetKitchenType(CRecordset &rs);
	
public:
	KDS_Mode m_kitchenMode;
	KDS_STYLE m_kdsStyle;
	
public:
	afx_msg void OnBnClickedRadioSend();
public:
	afx_msg void OnBnClickedRadioCook();

public:
	KDS_Mode GetKitchenMode();
	std::vector<INT>& GetKitchenType();

public:
	afx_msg void OnCbnSelchangeComboKitchen();
public:
	std::vector<CButton*> m_chkStalls;
	std::vector<INT> m_curSelStalls;
	CString m_strStalls;
	afx_msg void OnBnClickedCheckAllitem();

public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
public:
	afx_msg void OnBnClickedRadioOrder();
public:
	afx_msg void OnBnClickedRadioDetial();
public:
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
public:
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
public:
	afx_msg void OnPaint();

protected:
	INT m_x;
	INT m_y;
	HBITMAP m_bpBackgrd;
	Gdiplus::Image *m_bpBtnSave[2]; //想改成static而不得，出现error LNK2001.Image又不能使用默认构造函数
	Gdiplus::Image *m_bpBtnCancel[2];

public:
	CRoundButton2 m_btnSave;
public:
	CRoundButton2 m_btnCancel;
	CStatic m_ctrlStyle;
	CStatic m_ctrlMode;
	CStatic m_ctrlRefresh;
	CStatic m_ctrlStalls;

public:
	void ResetLang();
	CStatic m_ctrlLang;
	afx_msg void OnCbnSelchangeComboLanguage();
	CComboBox m_comboLang;
	CString m_lang;
	CString m_oldLang;
	afx_msg void OnBnClickedBtnLanguage1();
	afx_msg void OnBnClickedBtnLanguage2();
	afx_msg void OnBnClickedBtnLanguage3();

public:
	void initLang(void);
	CButton m_btnLanguage1;
	CButton m_btnLanguage2;
	CButton m_btnLanguage3;
	CButton m_btnLanguage4;

public:
	E_ItemName_ShowType m_nNameShow;
	afx_msg void OnBnClickedRadioOnlyname1();
	afx_msg void OnBnClickedRadioOnlyname2();
	afx_msg void OnBnClickedRadioName1andname2();

public:
	E_SORT_TYPE m_nFoodSortType;
	afx_msg void OnBnClickedRadioRushopen();
	afx_msg void OnBnClickedRadioRushclose();

	afx_msg void OnBnClickedBtnLanguage4();
	CStatic m_ctrlItemName;
	CStatic m_ctrlFoodPriority;

public:
	CStatic m_ctrlVersion;
	CString GetAppVersion();

public:
	afx_msg void OnBnClickedButtonServerSet();
	CStatic m_ctrlServerIP;
	CButton m_btnServerSet;
	afx_msg void OnBnClickedRadioCall();
	void EnableCtrl(BOOL enable);

public:
	BOOL m_bShowCheckId;
	CStatic m_ctrlCheckId;
	afx_msg void OnBnClickedRadioShowcheckid();
	afx_msg void OnBnClickedRadioHidecheckid();

public:
	CStatic m_ctrlSelectPrint;
	CComboBox m_comboPrint;
	INT m_printerId;
	void SetPrinterCombo(CRecordset &rs);
	afx_msg void OnCbnSelchangeComboPrint();
	CStatic m_ctrlVersionLabel;
};
