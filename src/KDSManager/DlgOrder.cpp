// DlgOrder.cpp : 实现文件
//

#include "stdafx.h"
#include "atlimage.h"
#include <gdiplus.h>

#include "KDSManager.h"
#include "DlgOrder.h"
#include "KDSManagerDlg.h"

// CDlgOrder 对话框

IMPLEMENT_DYNAMIC(CDlgOrder, CDialog)

CDlgOrder::CDlgOrder(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgOrder::IDD, pParent)
	, m_headId(0)
	, m_checkNum(_T(""))
	, m_employee(_T(""))
	, m_orderTime(_T(""))
	, m_makeTime(_T(""))
	, m_pos(_T(""))
	, m_eatType(_T(""))
	, m_remark(_T(""))
	, m_custNum(0)
	, m_checkName(_T(""))
	, m_headNum(_T(""))
{
	//InitListHead(); //不能再构造函数初始化时进行，要在显示窗体时
	strProgress[E_P_COOKING] = IDS_STR_COOKING;
	strProgress[E_P_COOKED] = IDS_STR_COOKED;
	strProgress[E_P_SENDED] = IDS_STR_SENDED;
	strProgress[E_P_COOKPEND] = IDS_STR_COOKING;
	strProgress[E_P_SENDPEND] = IDS_STR_COOKED;
	strProgress[E_P_INVALID] = IDS_STR_INVALID;

	// 背景图片
	CImage img;
	if(img.Load(_T(".\\res\\bg_image.jpg"))==S_OK)
	{
		m_x=img.GetWidth();
		m_y=img.GetHeight();
		m_bpBackgrd=img.Detach();
	}

	m_bpBtnClose[0]=Gdiplus::Image::FromFile(_T(".\\res\\btn_close.png"));
	m_bpBtnClose[1]=Gdiplus::Image::FromFile(_T(".\\res\\btn_pushdown.png"));

	if(m_bpBtnClose[0] == NULL || m_bpBtnClose[0]->GetLastStatus() != Gdiplus::Ok)
	{
		AfxMessageBox(_T("Failed on loading pirctures, please check whether the pictures are damaged."));
	}
}

CDlgOrder::~CDlgOrder()
{
	::DeleteObject(m_bpBackgrd);
	delete m_bpBtnClose[0];
	delete m_bpBtnClose[1];
}

void CDlgOrder::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_STATIC_VAL_HEADID, m_headNum);
	DDX_Text(pDX, IDC_STATIC_VAL_CHECKID, m_checkNum);
	DDX_Text(pDX, IDC_STATIC_VAL_EMPLOYEE, m_employee);
	DDX_Text(pDX, IDC_STATIC_VAL_ORDERTIME, m_orderTime);
	DDX_Text(pDX, IDC_STATIC_VAL_MAKETIME, m_makeTime);
	DDX_Text(pDX, IDC_STATIC_VAL_POS, m_pos);
	DDX_Text(pDX, IDC_STATIC_VAL_EATTYPE, m_eatType);
	DDX_Text(pDX, IDC_STATIC_VAL_REMARK, m_remark);
	DDX_Text(pDX, IDC_STATIC_VAL_CUSTNUM, m_custNum);
	DDX_Control(pDX, IDC_LIST_DETAIL, m_list);
	DDX_Control(pDX, IDOK, m_btnClose);
	DDX_Text(pDX, IDC_STATIC_VAL_CHECKNAME, m_checkName);
}


BEGIN_MESSAGE_MAP(CDlgOrder, CDialog)
	ON_BN_CLICKED(IDOK, &CDlgOrder::OnBnClickedOk)
	ON_NOTIFY(HDN_ENDTRACK, 0, &CDlgOrder::OnHdnEndtrackListDetail)
	ON_WM_ERASEBKGND()
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()


BOOL CDlgOrder::OnInitDialog()
{
	CDialog::OnInitDialog();

	//m_btnClose.SetWindowText(_T(""));
	m_btnClose.SetImages(m_bpBtnClose[0], m_bpBtnClose[1], false);

	LOGFONT lFont;
	GetFont()->GetLogFont(&lFont);
	lFont.lfHeight = 36;
	lFont.lfWidth = 0;
	m_btnClose.SetFont(&lFont);

	InitListHead();
	((CKDSManagerDlg*)AfxGetMainWnd())->LoadOrderInfo(m_headId, this, m_checkId);

	ResetLang();

	UpdateData(FALSE); // 以便空间变量正确显示	

	return TRUE;
}

void CDlgOrder::ResetLang()
{
	CString caption;
	
	CStatic *pWnd = (CStatic*)GetDlgItem(IDC_STATIC_HEADID);
	g_lang.LoadString(caption, IDS_STR_HEADID);
	pWnd->SetWindowText(caption);

	pWnd = (CStatic*)GetDlgItem(IDC_STATIC_CHECKID);
	g_lang.LoadString(caption, IDS_STR_CHECKNUM);
	pWnd->SetWindowText(caption);

	pWnd = (CStatic*)GetDlgItem(IDC_STATIC_CHECKNAME);
	g_lang.LoadString(caption, IDS_STR_CHECKNAME);
	pWnd->SetWindowText(caption);

	pWnd = (CStatic*)GetDlgItem(IDC_STATIC_EMPLOYEE);
	g_lang.LoadString(caption, IDS_STR_OPENEMPLOYEE);
	pWnd->SetWindowText(caption);

	pWnd = (CStatic*)GetDlgItem(IDC_STATIC_POS);
	g_lang.LoadString(caption, IDS_STR_POS);
	pWnd->SetWindowText(caption);

	pWnd = (CStatic*)GetDlgItem(IDC_STATIC_ORDERTIME);
	g_lang.LoadString(caption, IDS_STR_ORDERTIME);
	pWnd->SetWindowText(caption);

	pWnd = (CStatic*)GetDlgItem(IDC_STATIC_MAKETIME);
	g_lang.LoadString(caption, IDS_STR_KDSTIME);
	pWnd->SetWindowText(caption);

	pWnd = (CStatic*)GetDlgItem(IDC_STATIC_EATTYPE);
	g_lang.LoadString(caption, IDS_STR_EATTYPE);
	pWnd->SetWindowText(caption);

	pWnd = (CStatic*)GetDlgItem(IDC_STATIC_CUSTNUM);
	g_lang.LoadString(caption, IDS_STR_CUSTNUM4);
	pWnd->SetWindowText(caption);


	pWnd = (CStatic*)GetDlgItem(IDC_STATIC_REMARK);
	g_lang.LoadString(caption, IDS_STR_REMARK);
	pWnd->SetWindowText(caption);

	g_lang.LoadString(caption, IDS_STR_CLOSE);
	m_btnClose.SetWindowText(caption);
}

void CDlgOrder::InitListHead()
{
	/*LVCOLUMN colQuantity, colUnit, colName, colDesc, colState;
	CString strText;

	strText.LoadString(IDS_STR_QUANTITY);	
	colQuantity.mask = LVCF_TEXT;  
	colQuantity.pszText =strText.GetBuffer(0);  
	colQuantity.cchTextMax = strText.GetLength();  
	m_list.SetColumn(0 ,&colQuantity);*/ 

	INT col_width[e_max+1];
	INT col_head[] = {IDS_STR_QUANTITY, IDS_STR_UNIT, IDS_STR_ITEM, IDS_STR_CONDIMENT, IDS_STR_PROCESS};
	CString col_key;
	for(INT i=0; i<e_max+1; i++)
	{
		col_key.Format(_T("col%d_width"), i);
		col_width[i] = GetPrivateProfileInt(_T("list_width"), col_key, 50, _T(".\\config.ini"));
	}

	CString header;
	g_lang.LoadString(header, col_head[e_qua]);
	m_list.InsertColumn(e_qua, header, LVCFMT_LEFT, col_width[e_qua]);

	g_lang.LoadString(header, col_head[e_unit]);
	m_list.InsertColumn(e_unit, header, LVCFMT_LEFT, col_width[e_unit]);

	g_lang.LoadString(header, col_head[e_name]);
	m_list.InsertColumn(e_name, header, LVCFMT_LEFT, col_width[e_name]);

	g_lang.LoadString(header, col_head[e_desc]);
	m_list.InsertColumn(e_desc, header, LVCFMT_LEFT, col_width[e_desc]);

	g_lang.LoadString(header, col_head[e_state]);
	m_list.InsertColumn(e_state, header, LVCFMT_LEFT, col_width[e_state]);	
}

// CDlgOrder 消息处理程序

void CDlgOrder::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	OnOK();
}

BOOL CDlgOrder::LoadHead(CRecordset &rs)
{
	if (rs.IsEOF())
		return FALSE;

	CDBVariant var;
	CString tmp;

	//rs.GetFieldValue(_T("order_head_id"), var);
	//m_headId = var.m_lVal;
	rs.GetFieldValue(_T("order_head_id"), tmp);
	rs.GetFieldValue(_T("check_id"), var);
	m_checkId = var.m_lVal;
	m_headNum.Format(_T("%05s/%ld"),tmp, m_checkId);

	rs.GetFieldValue(_T("check_number"), tmp);
	rs.GetFieldValue(_T("check_id"), var);
	m_checkId = var.m_lVal;
	m_checkNum.Format(_T("%s/%ld"),tmp, m_checkId);

	rs.GetFieldValue(_T("customer_num"), var);
	m_custNum = var.m_lVal;

	rs.GetFieldValue(_T("eat_type"), var);	
	g_lang.LoadString(m_eatType, g_strEatTypeID[var.m_lVal]);
	//m_eatType.LoadString(tmp);

	rs.GetFieldValue(_T("open_employee_name"), m_employee);

	
	rs.GetFieldValue(_T("order_start_time"), tmp);
	m_orderTime = DATETIME_CUTTER(tmp);
	rs.GetFieldValue(_T("kds_time"),tmp);
	m_makeTime = DATETIME_CUTTER(tmp);
	rs.GetFieldValue(_T("pos_name"), m_pos);
	rs.GetFieldValue(_T("remark"), m_remark);

	rs.GetFieldValue(_T("check_name"), m_checkName);
	

	return TRUE;
}

INT CDlgOrder::LoadDetail(CRecordset &rs)
{	
	INT cnt = 0;
	CString val;
	CDBVariant varValue;

	for(INT i=0; !rs.IsEOF(); i++)
	{
		rs.GetFieldValue(_T("quantity"), val);
		m_list.InsertItem(i, val);

		rs.GetFieldValue(_T("unit"), val);
		m_list.SetItemText(i, e_unit, val);

		rs.GetFieldValue(_T("menu_item_name"), val);
		m_list.SetItemText(i, e_name, val);

		rs.GetFieldValue(_T("description"), val);
		m_list.SetItemText(i, e_desc, val);

		
		rs.GetFieldValue(_T("is_return_item"), varValue);
		//val.Format(_T("%ld"), varValue.m_lVal);
		//AfxMessageBox(val);

		if (varValue.m_lVal == 1)
		{
			g_lang.LoadString(val, IDS_STR_RETURN);
			m_list.SetItemText(i, e_state, val);
		}
		else
		{
		rs.GetFieldValue(_T("is_make"), val);
		
		INT is_make = 0;
		if (!val.IsEmpty())
		{
			is_make = _ttoi(val);
		}
		//val.LoadString(strProgress[is_make]);
		g_lang.LoadString(val, strProgress[is_make]);
		m_list.SetItemText(i, e_state, val);
		}		
		
		rs.MoveNext();
		cnt++;
	}

	return cnt;
}

void CDlgOrder::OnHdnEndtrackListDetail(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMHEADER phdr = reinterpret_cast<LPNMHEADER>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;

	CString col_key, col_width;
	//AfxMessageBox(_T("OnHdnEndtrackListDetail")); // 为什么是两次？
	CHeaderCtrl* pHeaderCtrl = m_list.GetHeaderCtrl();

	if (pHeaderCtrl != NULL)	
		for (INT i=0; i<pHeaderCtrl->GetItemCount(); i++)
		{
			INT width = m_list.GetColumnWidth(i);

			//CString msg;
			//msg.Format(_T("预留列宽调整%d"), width);
			//AfxMessageBox(msg);

			col_key.Format(_T("col%d_width"), i);
			col_width.Format(_T("%d"), width);
			WritePrivateProfileString(_T("list_width"), col_key, col_width,_T(".\\config.ini")); 
		}
		
}

BOOL CDlgOrder::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (m_bpBackgrd==NULL)
	{
		return FALSE;
	}
	CRect rect; 
	GetClientRect(&rect); 

	CDC MemDc;
	MemDc.CreateCompatibleDC(pDC);
	HGDIOBJ hOldBmp = MemDc.SelectObject(m_bpBackgrd);
	pDC->SetStretchBltMode(HALFTONE);
	//pDC->StretchBlt(0,0,CreatButton::m_nFullWidth,CreatButton::m_nFullHeight,&MemDc,0,0,m_x,m_y,SRCCOPY);
	pDC->StretchBlt(0,0,rect.Width(), rect.Height(),&MemDc,0,0,m_x,m_y,SRCCOPY);
	MemDc.SelectObject(hOldBmp);
	return TRUE;

	//return CDialog::OnEraseBkgnd(pDC);
}

HBRUSH CDlgOrder::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  在此更改 DC 的任何属性
	if(nCtlColor==CTLCOLOR_STATIC)
	{
		pDC->SetTextColor(RGB(0, 0, 0));
		pDC->SetBkMode(TRANSPARENT);
		return (HBRUSH)::GetStockObject(NULL_BRUSH); //返回此画刷可以使静态文本透明
	}

	// TODO:  如果默认的不是所需画笔，则返回另一个画笔
	return hbr;
}
