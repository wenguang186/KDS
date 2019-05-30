#pragma once
#include "afxwin.h"
#include <vector>
#include "dbmanager.h"
#include "common.h"
#include "datamodel.h"

// CDlgCombine �Ի���

class CDlgCombine : public CDialog
{
	DECLARE_DYNAMIC(CDlgCombine)

public:
	CDlgCombine(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgCombine();

// �Ի�������
	enum { IDD = IDD_DLG_COMBINE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);

	DECLARE_MESSAGE_MAP()
public:
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnPaint();
	afx_msg void OnBnClickedBtnCombineconfirm();
	afx_msg void OnBnClickedBtnCancel();

public:
	CButton m_btnCombineConfirm;
	CButton m_btnCancel;

protected:
	vector<CDialog*> m_pages;
	vector<CBaseData*> m_pPageData;
	map<CString,INT> m_conbineData;

	vector<CDataOrderDetail*> m_pData;

public:	
	CDataOrderDetail m_data;

public:
	CDBManager m_dbmngr;

protected:
	KDS_Mode m_kitchenMode; // ����������
	CString m_strKitchenStalls;

public:
	INT m_pageRows;	//��
	INT	m_pageCols;	//��
	INT m_currentPage;
	INT m_ItemShowCount; //��Ʒ��ʾ����

protected:
	INT m_nWidth;
	INT m_nHeight;
	INT m_x;
	INT m_y;

public:
	void UpdateItems();
	void ClearData();
	void ResetPages();
	void ResetLang();
	void refreshItemDlg(WPARAM msg_wParam);
	void msgNumKeyWP(WPARAM msg_wParam);
	INT JudgeCombine(void);
};
