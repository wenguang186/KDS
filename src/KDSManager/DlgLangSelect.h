#pragma once
#include <vector>
using namespace std;

// CDlgLangSelect 对话框

#define MY_BUTTON_ID_START 4000
#define MAX_BUTTON_NUM 100

class CDlgLangSelect : public CDialog
{
	DECLARE_DYNAMIC(CDlgLangSelect)

public:
	CDlgLangSelect(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgLangSelect();

// 对话框数据
	enum { IDD = IDD_DLG_LANGSELECT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
public:
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnBnClickedRadio(UINT nID);

public:
	vector<CButton*> m_btnVector;

public:
	CString m_strLanguage;
};
