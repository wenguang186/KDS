// DlgItemDetail.cpp : 实现文件
//

#include "stdafx.h"
#include "atlimage.h"
#include <gdiplus.h>

#include "KDSManager.h"
#include "DlgItemDetail.h"
#include "KDSManagerDlg.h"

// CDlgItemDetail 对话框

IMPLEMENT_DYNAMIC(CDlgItemDetail, CDialog)

CDlgItemDetail::CDlgItemDetail(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgItemDetail::IDD, pParent)
{
	// 图片资源，析构时要释放掉
	CImage img;
	if(img.Load(_T(".\\res\\item_detail_bgimage.png"))==S_OK)
	{
		m_x=img.GetWidth();
		m_y=img.GetHeight();
		m_bpBackgrd=img.Detach();
	}

	m_bpBtnPend[0]=Gdiplus::Image::FromFile(_T(".\\res\\btn_pend.png"));
	m_bpBtnPend[1]=Gdiplus::Image::FromFile(_T(".\\res\\btn_pushdown.png"));

	m_bpBtnDone[0]=Gdiplus::Image::FromFile(_T(".\\res\\btn_done.png"));
	m_bpBtnDone[1]=Gdiplus::Image::FromFile(_T(".\\res\\btn_pushdown.png"));

	m_bpBtnRecovery[0]=Gdiplus::Image::FromFile(_T(".\\res\\btn_done.png"));
	m_bpBtnRecovery[1]=Gdiplus::Image::FromFile(_T(".\\res\\btn_pushdown.png"));

	m_bpBtnOrder[0]=Gdiplus::Image::FromFile(_T(".\\res\\btn_order.png"));
	m_bpBtnOrder[1]=Gdiplus::Image::FromFile(_T(".\\res\\btn_pushdown.png"));

	if(m_bpBtnPend[0] == NULL || m_bpBtnPend[0]->GetLastStatus() != Gdiplus::Ok)
	{
		AfxMessageBox(_T("Failed on loading pirctures, please check whether the pictures are damaged."));
	}
}

CDlgItemDetail::~CDlgItemDetail()
{
	::DeleteObject(m_bpBackgrd);
	delete m_bpBtnPend[0];
	delete m_bpBtnPend[1];

	delete m_bpBtnDone[0];
	delete m_bpBtnDone[1];

	delete m_bpBtnRecovery[0];
	delete m_bpBtnRecovery[1];

	delete m_bpBtnOrder[0];
	delete m_bpBtnOrder[1];

	if (m_itemFont.m_hObject != NULL)
		m_itemFont.DeleteObject();
}

void CDlgItemDetail::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_VAL_ITEM, m_menu_item_name);
	DDX_Control(pDX, IDC_STATIC_VAL_NUM, m_quantity);
	DDX_Control(pDX, IDC_STATIC_VAL_UNIT, m_unit);
	DDX_Control(pDX, IDC_STATIC_VAL_DESC, m_description);
	DDX_Control(pDX, IDC_STATIC_VAL_ORDER_REMARK, m_remark);
	DDX_Control(pDX, IDC_STATIC_VAL_EATTYPE, m_eat_type);
	DDX_Control(pDX, IDC_STATIC_VAL_EMPLOYEE, m_employee);
	DDX_Control(pDX, IDC_STATIC_VAL_POS, m_pos);
	DDX_Control(pDX, IDC_STATIC_VAL_ORDER_TIME, m_order_time);
	DDX_Control(pDX, IDC_STATIC_VAL_CHKNUM, m_check_number);
	DDX_Control(pDX, IDC_STATIC_VAL_TABLE, m_table_name);
	DDX_Control(pDX, IDC_STATIC_VAL_CUSTNUM, m_customer_num);
	DDX_Control(pDX, IDC_BTN_RECOVERY, m_btnRecovery);
	DDX_Control(pDX, IDC_BTN_DONE, m_btnDone);
	DDX_Control(pDX, IDC_BTN_ORDER, m_btnOrder);
	DDX_Control(pDX, IDC_BTN_PEND, m_btnPend);
	DDX_Control(pDX, IDC_STATIC_VAL_CHECKNAME, m_checkName);
}


BEGIN_MESSAGE_MAP(CDlgItemDetail, CDialog)
	ON_BN_CLICKED(IDC_BTN_PEND, &CDlgItemDetail::OnBnClickedBtnPend)
	ON_BN_CLICKED(IDC_BTN_ORDER, &CDlgItemDetail::OnBnClickedBtnOrder)
	ON_BN_CLICKED(IDC_BTN_DONE, &CDlgItemDetail::OnBnClickedBtnDone)
	ON_WM_SHOWWINDOW()
	ON_WM_ERASEBKGND()
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_BTN_RECOVERY, &CDlgItemDetail::OnBnClickedBtnRecovery)
END_MESSAGE_MAP()


// CDlgItemDetail 消息处理程序

BOOL CDlgItemDetail::OnInitDialog()
{
	CDialog::OnInitDialog();

	RECT rect;
	m_btnDone.GetWindowRect(&rect);
	m_btnRecovery.MoveWindow(&rect);

	//m_btnPend.SetWindowText(_T(""));
	m_btnPend.SetImages(m_bpBtnPend[0], m_bpBtnPend[1], false);	
	//m_btnPend.SetTextColor(RGB(255, 255, 255));

	//m_btnOrder.SetWindowText(_T(""));
	m_btnOrder.SetImages(m_bpBtnOrder[0], m_bpBtnOrder[1], false);
	m_btnOrder.SetTextColor(RGB(255, 255, 255));
	
	
	//m_btnDone.SetWindowText(_T(""));
	m_btnDone.SetImages(m_bpBtnDone[0], m_bpBtnDone[1], false);
	//m_btnDone.SetTextColor(RGB(255, 255, 255));

	//m_btnRecovery.SetWindowText(_T(""));
	m_btnRecovery.SetImages(m_bpBtnRecovery[0], m_bpBtnRecovery[1], false);
	//m_btnRecovery.SetTextColor(RGB(255, 255, 255));

	LOGFONT lFont;
	m_menu_item_name.GetFont()->GetLogFont(&lFont);
	lFont.lfHeight = 30;
	lFont.lfWidth = 0;
	
	m_itemFont.CreateFontIndirect(&lFont);
	//m_itemFont.CreateFont(
	//	6,                        // nHeight
	//	0,                         // nWidth
	//	0,                         // nEscapement
	//	0,                         // nOrientation
	//	FW_NORMAL,                 // nWeight
	//	FALSE,                     // bItalic
	//	FALSE,                     // bUnderline
	//	0,                         // cStrikeOut
	//	ANSI_CHARSET,              // nCharSet
	//	OUT_DEFAULT_PRECIS,        // nOutPrecision
	//	CLIP_DEFAULT_PRECIS,       // nClipPrecision
	//	DEFAULT_QUALITY,           // nQuality
	//	DEFAULT_PITCH | FF_SWISS,  // nPitchAndFamily
	//	_T("微软雅黑"));                 
	m_menu_item_name.SetFont(&m_itemFont);
	//AfxMessageBox(lFont.lfFaceName);
	
	lFont.lfHeight = 36;
	m_btnPend.SetFont(&lFont);
	m_btnDone.SetFont(&lFont);
	m_btnOrder.SetFont(&lFont);
	m_btnRecovery.SetFont(&lFont);

	ResetLang();

	return TRUE;
}

void CDlgItemDetail::ResetLang()
{
	CString caption;
	CStatic *pWnd = (CStatic*)GetDlgItem(IDC_STATIC_NUM);
	g_lang.LoadString(caption, IDS_STR_NUM);
	pWnd->SetWindowText(caption);

	pWnd = (CStatic*)GetDlgItem(IDC_STATIC_UNIT);
	g_lang.LoadString(caption, IDS_STR_UNIT);
	pWnd->SetWindowText(caption);

	pWnd = (CStatic*)GetDlgItem(IDC_STATIC_DESC);
	g_lang.LoadString(caption, IDS_STR_DESC);
	pWnd->SetWindowText(caption);

	pWnd = (CStatic*)GetDlgItem(IDC_STATIC_ORDER_REMARK);
	g_lang.LoadString(caption, IDS_STR_REMARK);
	pWnd->SetWindowText(caption);

	pWnd = (CStatic*)GetDlgItem(IDC_STATIC_EATTYPE);
	g_lang.LoadString(caption, IDS_STR_EATTYPE);
	pWnd->SetWindowText(caption);

	pWnd = (CStatic*)GetDlgItem(IDC_STATIC_EMPLOYEE);
	g_lang.LoadString(caption, IDS_STR_OPENEMPLOYEE);
	pWnd->SetWindowText(caption);

	pWnd = (CStatic*)GetDlgItem(IDC_STATIC_POS);
	g_lang.LoadString(caption, IDS_STR_POS);
	pWnd->SetWindowText(caption);

	pWnd = (CStatic*)GetDlgItem(IDC_STATIC_ORDER_TIME);
	g_lang.LoadString(caption, IDS_STR_ORDERTIME);
	pWnd->SetWindowText(caption);

	pWnd = (CStatic*)GetDlgItem(IDC_STATIC_CHKNUM);
	g_lang.LoadString(caption, IDS_STR_CHECKNUM);
	pWnd->SetWindowText(caption);

	pWnd = (CStatic*)GetDlgItem(IDC_STATIC_CHECKNAME);
	g_lang.LoadString(caption, IDS_STR_CHECKNAME);
	pWnd->SetWindowText(caption);

	pWnd = (CStatic*)GetDlgItem(IDC_STATIC_TABLE);
	g_lang.LoadString(caption, IDS_STR_TABLE);
	pWnd->SetWindowText(caption);

	pWnd = (CStatic*)GetDlgItem(IDC_STATIC_CUSTNUM);
	g_lang.LoadString(caption, IDS_STR_CUSTNUM);
	pWnd->SetWindowText(caption);

	g_lang.LoadString(caption, IDS_STR_PENDING);
	m_btnPend.SetWindowText(caption);

	g_lang.LoadString(caption, IDS_STR_DONE);
	m_btnDone.SetWindowText(caption);

	g_lang.LoadString(caption, IDS_STR_ORDER);
	m_btnOrder.SetWindowText(caption);

	g_lang.LoadString(caption, IDS_STR_RECOVERY);
	m_btnRecovery.SetWindowText(caption);

	m_btnPend.SetInvalid();
	m_btnDone.SetInvalid();
	m_btnRecovery.SetInvalid();
	m_btnOrder.SetInvalid();
	Invalidate();
}

void CDlgItemDetail::OnBnClickedBtnPend()
{
	// TODO: 在此添加控件通知处理程序代码
	DoAction(WM_MSG_PEND);
}

void CDlgItemDetail::OnBnClickedBtnOrder()
{
	// TODO: 在此添加控件通知处理程序代码
	DoAction(WM_MSG_ORDER);
}

void CDlgItemDetail::OnBnClickedBtnDone()
{
	// TODO: 在此添加控件通知处理程序代码
	DoAction(WM_MSG_DONE);		
}

void CDlgItemDetail::OnBnClickedBtnRecovery()
{
	// TODO: 在此添加控件通知处理程序代码
	DoAction(WM_MSG_RECOVERY);
}

void CDlgItemDetail::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialog::OnShowWindow(bShow, nStatus);

	// TODO: 在此处添加消息处理程序代码
	//if (bShow)
	//{		
	//	RefreshData();
	//	ResetShowState(((CKDSManagerDlg*)AfxGetMainWnd())->m_showType);
	//	//UpdateData(FALSE);
	//	//Invalidate(FALSE);
	//}
	//else
	//{
	//	ClearData();
	//}
}

inline void CDlgItemDetail::DoAction(INT nIDMsg)
{
	AfxGetMainWnd()->PostMessage(nIDMsg, (WPARAM)(&m_data), 0);
}

void CDlgItemDetail::ResetShowState(KDS_ShowType showType)
{
	CWnd* pBtnPend = GetDlgItem(IDC_BTN_PEND);
	CWnd* pBtnDone = GetDlgItem(IDC_BTN_DONE);
	CString str;

	switch(showType)
	{
	case E_S_DONING:
		m_btnPend.ShowWindow(SW_SHOW);
		m_btnDone.ShowWindow(SW_SHOW);
		m_btnRecovery.ShowWindow(SW_HIDE);

		//str.LoadString(IDS_STR_DONE);
		//m_btnDone.SetWindowText(str);
		//nIDMsgDone = WM_MSG_DONE;
		break;

	case E_S_PENDED:		
		//str.LoadString(IDS_STR_RECOVERY);
		//m_btnDone.SetWindowText(str);
		//nIDMsgDone = WM_MSG_RECOVERY;

		m_btnPend.ShowWindow(SW_HIDE);
		m_btnDone.ShowWindow(SW_HIDE);
		m_btnRecovery.ShowWindow(SW_SHOW);
		//pBtnDone->ShowWindow(SW_SHOW);
		//pBtnPend->ShowWindow(SW_HIDE);
		break;

	case E_S_DONE:
		m_btnPend.ShowWindow(SW_HIDE);
		m_btnDone.ShowWindow(SW_HIDE);
		m_btnRecovery.ShowWindow(SW_HIDE);
		//pBtnPend->ShowWindow(SW_HIDE);
		//pBtnDone->ShowWindow(SW_HIDE);
		break;

	default:
		AfxMessageBox(_T("Unknown Show Type in ResetShowState"));
	}	
}

void CDlgItemDetail::RefreshData()
{
	CString tmp;
	//Transfer2UI();
	//m_itemName.Format(_T("%s"), m_data.GetValue(_T("menu_item_name")));
	//AfxMessageBox(m_data.GetValue(_T("condiment_belong_item")));
	tmp.Format(_T("%s"), m_data.GetValue(_T("menu_item_name")));
	if (_ttoi(m_data.GetValue(_T("condiment_belong_item"))) < 0)
	{
		CString setmeal;
		g_lang.LoadString(setmeal, IDS_STR_SETMEAL);
		tmp.AppendFormat(_T("(%s)"), setmeal);
		//tmp.Format(_T("%s(%s)"), m_data.GetValue(_T("menu_item_name")), setmeal);		
	}
	//else if (_ttoi(m_data.GetValue(_T("condiment_belong_item"))) > 0)
	//{
	//	// 带价格的调味品
	//	CString tmp_2;
	//	g_lang.LoadString(tmp_2, IDS_STR_SETMEAL);
	//	tmp.AppendFormat(_T("(%s)"), tmp_2);
	//} 
	
	//AfxMessageBox(m_data.GetValue(_T("is_discount")));
	// 170228 lv is_discount是bit类型，不知道怎么转换，所以通过判断是否为空来看是否赠送
	if (m_data.GetValue(_T("is_discount")) != _T(""))
	{
		CString tmp_2;
		g_lang.LoadString(tmp_2, IDS_STR_FREE);
		tmp.AppendFormat(_T("(%s)"), tmp_2);
	}

	m_menu_item_name.SetWindowText(tmp);
	

	//m_unit.Format(_T("%s"), m_data.GetValue(_T("unit")));
	m_unit.SetWindowText(m_data.GetValue(_T("unit")));

	// 170228 lv 添加带价格的调味品
	//m_desc.Format(_T("%s"), m_data.GetValue(_T("description")));
	tmp.Format(_T("%s"), m_data.GetValue(_T("description")));
	((CKDSManagerDlg*)AfxGetMainWnd())->GetCondimentPriced(_ttoi(m_data.GetValue(_T("order_detail_id"))),tmp);
	m_description.SetWindowText(tmp);

	//m_remark.Format(_T("%s"), m_data.GetValue(_T("remark")));
	m_remark.SetWindowText(m_data.GetValue(_T("remark")));

	//m_orderType.LoadString(g_strEatTypeID[_ttoi(m_data.GetValue(_T("eat_type")))]);
	//tmp.LoadString(g_strEatTypeID[_ttoi(m_data.GetValue(_T("eat_type")))]);
	g_lang.LoadString(tmp, g_strEatTypeID[_ttoi(m_data.GetValue(_T("eat_type")))]);
	m_eat_type.SetWindowText(tmp);

	//m_employeeName.Format(_T("%s"), m_data.GetValue(_T("order_employee_name")));
	m_employee.SetWindowText(m_data.GetValue(_T("order_employee_name")));

	//m_posName.Format(_T("%s"), m_data.GetValue(_T("pos_name")));
	m_pos.SetWindowText(m_data.GetValue(_T("pos_name")));

	//m_orderTime.Format(_T("%s"), m_data.GetValue(_T("order_time")));
	m_order_time.SetWindowText(DATETIME_CUTTER(m_data.GetValue(_T("order_time"))));

	//m_tblName.Format(_T("%s"), m_data.GetValue(_T("table_name")));
	m_table_name.SetWindowText(m_data.GetValue(_T("table_name")));

	//m_quantity.Format(_T("%s"), m_data.GetValue(_T("quantity")));
	m_quantity.SetWindowText(m_data.GetValue(_T("quantity")));

	//m_chkNum.Format(_T("%s"), m_data.GetValue(_T("check_number")));
	tmp.Format(_T("%s/%s"), m_data.GetValue(_T("check_number")),m_data.GetValue(_T("check_id")));
	m_check_number.SetWindowText(tmp);

	//m_custNum.Format(_T("%s"), m_data.GetValue(_T("customer_num")));	
	m_customer_num.SetWindowText(m_data.GetValue(_T("customer_num")));

	m_checkName.SetWindowText(m_data.GetValue(_T("check_name")));
	//UpdateData(FALSE);
	//Invalidate(TRUE);
}

void CDlgItemDetail::ClearData()
{
	m_data.ClearData();
	//UpdateData(FALSE);
}

CBaseData* CDlgItemDetail::GetDataPtr()
{
	return (CBaseData*)(&m_data);
}


BOOL CDlgItemDetail::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (m_bpBackgrd==NULL)
	{
		return FALSE;
	}
	CRect rect; 
	GetWindowRect(&rect); 

	CDC MemDc;
	MemDc.CreateCompatibleDC(pDC);
	HGDIOBJ hOldBmp = MemDc.SelectObject(m_bpBackgrd);
	
	//pDC->FillSolidRect(0,0,rect.Width(),rect.Height(),RGB(255,255,255));

	pDC->SetStretchBltMode(HALFTONE);
	//pDC->StretchBlt(0,0,CreatButton::m_nFullWidth,CreatButton::m_nFullHeight,&MemDc,0,0,m_x,m_y,SRCCOPY);
	// (0,0)作为upper-left corner.使用rect只能显示到第一个item
	// +7是为了把item-dlg的边框黑条变小一点，暂时没什么别的办法
	pDC->StretchBlt(0, 0,rect.Width()+7, rect.Height()+7,&MemDc,0,0,m_x,m_y,SRCCOPY);
	MemDc.SelectObject(hOldBmp);
	return TRUE;
	//return CDialog::OnEraseBkgnd(pDC);
}

HBRUSH CDlgItemDetail::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  在此更改 DC 的任何属性
	

	if(nCtlColor==CTLCOLOR_STATIC)
	{
		if (pWnd->GetDlgCtrlID() == IDC_STATIC_VAL_ITEM)
		{
			pDC->SetTextColor(RGB(255, 255, 255));
		}
		//pDC->SetTextColor(RGB(0, 0, 0));
		pDC->SetBkMode(TRANSPARENT);
		return (HBRUSH)::GetStockObject(NULL_BRUSH); //返回此画刷可以使静态文本透明
	}

	// TODO:  如果默认的不是所需画笔，则返回另一个画笔
	return hbr;
}


