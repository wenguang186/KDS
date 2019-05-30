// DlgItem.cpp : 实现文件
//

#include "stdafx.h"
#include "KDSManager.h"
#include "DlgItem.h"
#include "KDSManagerDlg.h"

// CDlgItem 对话框

IMPLEMENT_DYNAMIC(CDlgItem, CDialog)

CDlgItem::CDlgItem(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgItem::IDD, pParent)
	, m_itemName(_T(""))
	, m_quantity(_T(""))
	, m_unit(_T(""))
	, m_desc(_T(""))
	, m_remark(_T(""))
	, m_orderType(_T(""))
	, m_employeeName(_T(""))
	, m_posName(_T(""))
	, m_orderTime(_T(""))
	, m_chkNum(0)
	, m_tblName(_T(""))
	, m_custNum(0)
{

}

CDlgItem::~CDlgItem()
{
}

void CDlgItem::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_STATIC_VAL_ITEM, m_itemName);
	DDX_Text(pDX, IDC_STATIC_VAL_NUM, m_quantity);
	DDX_Text(pDX, IDC_STATIC_VAL_UNIT, m_unit);
	DDX_Text(pDX, IDC_STATIC_VAL_DESC, m_desc);
	DDX_Text(pDX, IDC_STATIC_VAL_ORDER_REMARK, m_remark);
	DDX_Text(pDX, IDC_STATIC_VAL_EATTYPE, m_orderType);
	DDX_Text(pDX, IDC_STATIC_VAL_EMPLOYEE, m_employeeName);
	DDX_Text(pDX, IDC_STATIC_VAL_POS, m_posName);
	DDX_Text(pDX, IDC_STATIC_VAL_ORDER_TIME, m_orderTime);
	DDX_Text(pDX, IDC_STATIC_VAL_CHKNUM, m_chkNum);
	DDX_Text(pDX, IDC_STATIC_VAL_TABLE, m_tblName);
	DDX_Text(pDX, IDC_STATIC_VAL_CUSTNUM, m_custNum);
}


BEGIN_MESSAGE_MAP(CDlgItem, CDialog)
	ON_WM_SHOWWINDOW()

	ON_BN_CLICKED(IDC_BTN_DONE, &CDlgItem::OnBnClickedBtnDone)
	ON_BN_CLICKED(IDC_BTN_ORDER, &CDlgItem::OnBnClickedBtnOrder)
	ON_BN_CLICKED(IDC_BTN_PEND, &CDlgItem::OnBnClickedBtnPend)
//	ON_STN_CLICKED(IDC_STATIC_ITEM, &CDlgItem::OnStnClickedStaticItem)
END_MESSAGE_MAP()

void CDlgItem::OnBnClickedBtnDone()
{
	// TODO: 在此添加控件通知处理程序代码
	//m_data.DoComplete();
	
	/*if (order_head_id <= 0)
	{
		return;
	}*/

	/*long order_head_id = _ttoi(m_data.GetValue(_T("order_head_id")));
	long order_detail_id = _ttoi(m_data.GetValue(_T("order_detail_id")));
	AfxGetMainWnd()->PostMessage(WM_MSG_DONE, order_head_id, order_detail_id);*/
	DoAction(WM_MSG_DONE);
}

void CDlgItem::OnBnClickedBtnOrder()
{
	// TODO: 在此添加控件通知处理程序代码
	/*if (m_data.order_head_id <= 0)
	{
		return;
	}*/

	/*long order_head_id = _ttoi(m_data.GetValue(_T("order_head_id")));
	long order_detail_id = _ttoi(m_data.GetValue(_T("order_detail_id")));
	AfxGetMainWnd()->PostMessage(WM_MSG_ORDER, order_head_id, order_detail_id);*/
	DoAction(WM_MSG_ORDER);
}

void CDlgItem::OnBnClickedBtnPend()
{
	// TODO: 在此添加控件通知处理程序代码
	/*if (m_data.order_head_id <= 0)
	{
		return;
	}*/

	//long order_head_id = _ttoi(m_data.GetValue(_T("order_head_id")));
	//long order_detail_id = _ttoi(m_data.GetValue(_T("order_detail_id")));
	//AfxGetMainWnd()->PostMessage(WM_MSG_PEND, m_data.order_head_id, m_data.order_detail_id);
	DoAction(WM_MSG_PEND);
}

void CDlgItem::DoAction(INT nIDMsg)
{
	long order_head_id = _ttoi(m_data.GetValue(_T("order_head_id")));
	long order_detail_id = _ttoi(m_data.GetValue(_T("order_detail_id")));
	AfxGetMainWnd()->PostMessage(nIDMsg, order_head_id, order_detail_id);
}

#if 0
void CDlgItem::Transfer2UI()
{
	m_itemName = m_data.menu_item_name;
	m_unit = m_data.unit;
	m_desc = m_data.description;
	m_remark = m_data.remark;
	m_orderType = m_data.order_type_name;
	m_employeeName = m_data.order_employee_name;
	m_posName = m_data.pos_name;
	m_orderTime = m_data.order_time;
	m_tblName = m_data.table_name;

	m_quantity = m_data.quantity;
	//	m_quantity.Format(_T("%g"), m_data.quantity);
	m_chkNum = m_data.check_number;
	m_custNum = m_data.customer_num;
}
#endif

void CDlgItem::RefreshData()
{
	
	//Transfer2UI();
	m_itemName.Format(_T("%s"), m_data.GetValue(_T("menu_item_name")));
	m_unit.Format(_T("%s"), m_data.GetValue(_T("unit")));
	m_desc.Format(_T("%s"), m_data.GetValue(_T("description")));

	m_remark.Format(_T("%s"), m_data.GetValue(_T("remark")));

	m_orderType.LoadString(g_strEatTypeID[_ttoi(m_data.GetValue(_T("eat_type")))]);

	m_employeeName.Format(_T("%s"), m_data.GetValue(_T("order_employee_name")));

	m_posName.Format(_T("%s"), m_data.GetValue(_T("pos_name")));
	m_orderTime.Format(_T("%s"), m_data.GetValue(_T("order_time")));
	m_tblName.Format(_T("%s"), m_data.GetValue(_T("table_name")));

	m_quantity.Format(_T("%s"), m_data.GetValue(_T("quantity")));
	
	m_chkNum.Format(_T("%s"), m_data.GetValue(_T("check_number")));
	m_custNum.Format(_T("%s"), m_data.GetValue(_T("customer_num")));	

	//UpdateData(FALSE);
	//Invalidate(TRUE);
}

// 数据恢复初值
void CDlgItem::ClearData()
{
	m_data.ClearData();
	//Transfer2UI();

	//UpdateData(FALSE);
}

void CDlgItem::ResetShowState(KDS_ShowType showType)
{
	CWnd* pBtnPend = GetDlgItem(IDC_BTN_PEND);
	CWnd* pBtnDone = GetDlgItem(IDC_BTN_DONE);
	CString str;	

	switch(showType)
	{
	case E_S_DONING:
		pBtnPend->ShowWindow(SW_SHOW);
		pBtnDone->ShowWindow(SW_SHOW);
		
		str.LoadString(IDS_STR_DONE);
		pBtnDone->SetWindowText(str);
		break;

	case E_S_PENDED:		
		str.LoadString(IDS_STR_RECOVERY);
		pBtnDone->SetWindowText(str);

		pBtnDone->ShowWindow(SW_SHOW);
		pBtnPend->ShowWindow(SW_HIDE);
		break;

	case E_S_DONE:
		pBtnPend->ShowWindow(SW_HIDE);
		pBtnDone->ShowWindow(SW_HIDE);
		break;

	default:
		AfxMessageBox(_T("Unknown Show Type in ResetShowState"));
	}	
}

// CDlgItemOrder 消息处理程序

void CDlgItem::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialog::OnShowWindow(bShow, nStatus);

	// TODO: 在此处添加消息处理程序代码
	if (bShow)
	{		
		RefreshData();
		ResetShowState(((CKDSManagerDlg*)AfxGetMainWnd())->m_showType);		
	}
	else
	{
		ClearData();
	}
}

CBaseData* CDlgItem::GetDataPtr()
{
	return (CBaseData*)(&m_data);
}
