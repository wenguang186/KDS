#pragma once

#include "afxwin.h"

//#include "dbmanager.h"
#include "datamodel.h"
#include "common.h"

// CDlgItem �Ի���

class CDlgItem : public CDialog
{
	DECLARE_DYNAMIC(CDlgItem)

public:
	CDlgItem(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgItem();

// �Ի�������
	enum { IDD = IDD_DLG_ITEM };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnBnClickedBtnDone();
public:
	afx_msg void OnBnClickedBtnOrder();
public:
	afx_msg void OnBnClickedBtnPend();
public:
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);

public:
	CString m_unit;
public:
	CString m_itemName;
public:
	CString m_desc;
public:
	CString m_remark;
public:
	CString m_orderType;
public:
	CString m_employeeName;
public:
	CString m_posName;
public:
	CString m_orderTime;
public:
	long m_chkNum;
public:
	CString m_tblName;
public:
	long m_custNum;
public:
	CString m_quantity; // float���ͣ�ֱ����ʾ��ʽ�����ʣ���ת��

// �Զ���

protected:
	void Transfer2UI();

public:

	//COrderItem m_data;
	CDataOrderDetail m_data;

protected:
	void DoAction(INT nIDMsg);


public:
	void RefreshData();
	void ClearData();
	void ResetShowState(KDS_ShowType showType);
	CBaseData* GetDataPtr();
};
