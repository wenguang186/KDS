// DlgItemOrder.cpp : 实现文件
//

#include "stdafx.h"
#include "atlimage.h"
#include <gdiplus.h>

#include "KDSManager.h"
#include "DlgItemOrder.h"
#include "common.h"
#include "KDSManagerDlg.h"
#include "datamodel.h"

// CDlgItemOrder 对话框

IMPLEMENT_DYNAMIC(CDlgItemOrder, CDialog)

CDlgItemOrder::CDlgItemOrder(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgItemOrder::IDD, pParent)
{
	CImage img;
	if(img.Load(_T(".\\res\\item_order_bgimage.png"))==S_OK)
	{
		m_x=img.GetWidth();
		m_y=img.GetHeight();
		m_bpBackgrd=img.Detach();
	}

	// 自定义按钮
	
	m_bpBtnPend[0]=Gdiplus::Image::FromFile(_T(".\\res\\btn_pend.png"));
	m_bpBtnPend[1]=Gdiplus::Image::FromFile(_T(".\\res\\btn_pushdown.png"));

	m_bpBtnDone[0]=Gdiplus::Image::FromFile(_T(".\\res\\btn_done.png"));
	m_bpBtnDone[1]=Gdiplus::Image::FromFile(_T(".\\res\\btn_pushdown.png"));

	m_bpBtnRecovery[0]=Gdiplus::Image::FromFile(_T(".\\res\\btn_done.png"));
	m_bpBtnRecovery[1]=Gdiplus::Image::FromFile(_T(".\\res\\btn_pushdown.png"));

	if(m_bpBtnPend[0] == NULL || m_bpBtnPend[0]->GetLastStatus() != Gdiplus::Ok)
	{
		AfxMessageBox(_T("Failed on loading pirctures, please check whether the pictures are damaged."));
	}
}

CDlgItemOrder::~CDlgItemOrder()
{
	::DeleteObject(m_bpBackgrd);
	delete m_bpBtnPend[0];
	delete m_bpBtnPend[1];

	delete m_bpBtnDone[0];
	delete m_bpBtnDone[1];

	delete m_bpBtnRecovery[0];
	delete m_bpBtnRecovery[1];

	if (m_itemFont.m_hObject != NULL)
		m_itemFont.DeleteObject();
}

void CDlgItemOrder::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_VAL_EMPLOYEE, m_employee);
	DDX_Control(pDX, IDC_STATIC_VAL_POS, m_pos);
	DDX_Control(pDX, IDC_STATIC_VAL_ORDERTIME, m_ordertime);
	DDX_Control(pDX, IDC_STATIC_VAL_EATTYPE, m_eattype);
	DDX_Control(pDX, IDC_STATIC_VAL_REMARK, m_remark);
	DDX_Control(pDX, IDC_STATIC_ORDER, m_title);
	DDX_Control(pDX, IDC_STATIC_VAL_CUSTNUM, m_custnum);
	DDX_Control(pDX, IDC_LIST_DETAIL, m_list);
	DDX_Control(pDX, IDC_BTN_PEND, m_btnPend);
	DDX_Control(pDX, IDC_BTN_DONE, m_btnDone);
	DDX_Control(pDX, IDC_BTN_RECOVERY, m_btnRecovery);
	DDX_Control(pDX, IDC_STATIC_VAL_HEADID, m_orderHeadId);
	DDX_Control(pDX, IDC_STATIC_VAL_CHKNUMBER, m_checkNumber);
}


BEGIN_MESSAGE_MAP(CDlgItemOrder, CDialog)
	ON_WM_SHOWWINDOW()
	ON_BN_CLICKED(IDC_BTN_PEND, &CDlgItemOrder::OnBnClickedBtnPend)
	ON_BN_CLICKED(IDC_BTN_DONE, &CDlgItemOrder::OnBnClickedBtnDone)
	ON_WM_ERASEBKGND()
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_BTN_RECOVERY, &CDlgItemOrder::OnBnClickedBtnRecovery)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_LIST_DETAIL, &CDlgItemOrder::OnNMCustomdrawListDetail)
END_MESSAGE_MAP()


BOOL CDlgItemOrder::OnInitDialog()
{
	CDialog::OnInitDialog();
	//m_list.InsertColumn(e_qua, _T("数量"), LVCFMT_LEFT, 30);
	//m_list.InsertColumn(e_unit, _T("规格"), LVCFMT_LEFT, 30);
	//m_list.InsertColumn(e_name, _T("菜品"), LVCFMT_LEFT, 150);
	//m_list.InsertColumn(e_desc, _T("调味品"), LVCFMT_LEFT, 50);
	//m_list.AddString(_T("12345678901234567890123456789012345678901234567890"));
	//m_list.AddString(_T("一二三四五六七八九十一二三四五六七八九十"));
	RECT rect;
	m_list.GetClientRect(&rect);
	INT width_col_1 = 20;
	INT width_col_2 = 55;
	m_list.InsertColumn(0, _T("完成"), LVCFMT_LEFT, width_col_1);
	m_list.InsertColumn(1, _T("数量"), LVCFMT_LEFT, width_col_2);
	m_list.InsertColumn(2, _T("菜品"), LVCFMT_LEFT, rect.right-rect.left-width_col_1-width_col_2);
	m_list.SetExtendedStyle(LVS_EX_FULLROWSELECT);//|LVS_EX_GRIDLINES);
	
	m_btnDone.GetWindowRect(&rect);
	m_btnRecovery.MoveWindow(&rect);

	//((CKDSManagerDlg*)AfxGetMainWnd())->LoadOrderInfo(m_headId, this);
	//UpdateData(FALSE); // 以便空间变量正确显示

	//m_btnPend.SetWindowText(_T(""));
	m_btnPend.SetImages(m_bpBtnPend[0], m_bpBtnPend[1], false);
	//m_btnPend.SetInvalid();

	//m_btnDone.SetWindowText(_T(""));
	m_btnDone.SetImages(m_bpBtnDone[0], m_bpBtnDone[1], false);

	//m_btnRecovery.SetWindowText(_T(""));
	m_btnRecovery.SetImages(m_bpBtnRecovery[0], m_bpBtnRecovery[1], false);

	LOGFONT lFont;
	m_title.GetFont()->GetLogFont(&lFont);
	lFont.lfHeight = 30;
	lFont.lfWidth = 0;

	m_itemFont.CreateFontIndirect(&lFont);
	m_title.SetFont(&m_itemFont);

	lFont.lfHeight = 36;
	m_btnPend.SetFont(&lFont);
	m_btnDone.SetFont(&lFont);
	m_btnRecovery.SetFont(&lFont);

	// 设置listbox字体，使其等宽，一边空格正确显示
	//lFont.lfHeight = 20;
	//memset(&lFont, 0, sizeof(LOGFONT));
	//lFont.lfFaceName = _T("微软雅黑");
	//strcpy(lFont.lfFaceName, _T("Courier New")); 
	//m_listFont.CreateFontIndirect(&lFont);
	//m_list.SetFont(&m_listFont);


	ResetLang();
	return TRUE;
}

void CDlgItemOrder::ResetLang()
{
	CString caption;
	CStatic *pWnd = (CStatic*)GetDlgItem(IDC_STATIC_HEADID);
	g_lang.LoadString(caption, IDS_STR_HEADID);
	pWnd->SetWindowText(caption);

	pWnd = (CStatic*)GetDlgItem(IDC_STATIC_CHECK_NUMBER);
	g_lang.LoadString(caption, IDS_STR_CHECKNUM);
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

	pWnd = (CStatic*)GetDlgItem(IDC_STATIC_EATTYPE);
	g_lang.LoadString(caption, IDS_STR_EATTYPE);
	pWnd->SetWindowText(caption);

	pWnd = (CStatic*)GetDlgItem(IDC_STATIC_CUSTNUM);
	g_lang.LoadString(caption, IDS_STR_CUSTNUM);
	pWnd->SetWindowText(caption);


	pWnd = (CStatic*)GetDlgItem(IDC_STATIC_REMARK);
	g_lang.LoadString(caption, IDS_STR_REMARK);
	pWnd->SetWindowText(caption);

	g_lang.LoadString(caption, IDS_STR_PENDING);
	m_btnPend.SetWindowText(caption);

	g_lang.LoadString(caption, IDS_STR_DONE);
	m_btnDone.SetWindowText(caption);

	g_lang.LoadString(caption, IDS_STR_RECOVERY);
	m_btnRecovery.SetWindowText(caption);

	m_btnPend.SetInvalid();
	m_btnDone.SetInvalid();
	m_btnRecovery.SetInvalid();
	Invalidate();
}

// CDlgItemOrder 消息处理程序

void CDlgItemOrder::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialog::OnShowWindow(bShow, nStatus);

	// TODO: 在此处添加消息处理程序代码
	//if (bShow)
	//{
	//	// 加载detail
	//	//((CKDSManagerDlg*)AfxGetMainWnd())->LoadOrderDetail(m_headId, this);
	//	RefreshData();
	//	ResetShowState(((CKDSManagerDlg*)AfxGetMainWnd())->m_showType);
	//}
	//else
	//{
	//	// 清空数据
	//	ClearData();
	//}
}

void CDlgItemOrder::ClearData()
{
	m_headId = 0;
	m_list.DeleteAllItems();
	//m_list.ResetContent();
	m_data.ClearData();
	//Invalidate();
}

void CDlgItemOrder::RefreshData()
{
	CString val;
	
	//val.Format(_T("%s.%s.%s"), m_data.m_head.m_data[_T("table_name")], 
	//	m_data.m_head.m_data[_T("order_head_id")], m_data.m_head.m_data[_T("check_name")]);
	if (m_data.m_head.m_data[_T("check_name")] == _T(""))
	{
		val.Format(_T("%s.%s/%s"), m_data.m_head.m_data[_T("table_name")], 
			m_data.m_head.m_data[_T("check_number")], m_data.m_head.m_data[_T("check_id")]);
	}
	else
	{
		val.Format(_T("%s.%s"), m_data.m_head.m_data[_T("table_name")], 
			m_data.m_head.m_data[_T("check_name")]);
	}
	m_title.SetWindowText(val);
	//m_title.UpdateData();
	//m_title.Invalidate();
	//RECT rect;
	//m_title.GetClientRect(&rect);
	//m_title.InvalidateRect(&rect);
	//m_title.ShowWindow(SW_HIDE);
	//m_title.ShowWindow(SW_SHOW);

	m_checkNumber.SetWindowText(m_data.m_head.m_data[_T("order_head_id")]);

	val.Format(_T("%s/%s"), m_data.m_head.m_data[_T("check_number")],
		m_data.m_head.m_data[_T("check_id")]);
	m_orderHeadId.SetWindowText(val);

	m_ordertime.SetWindowText(DATETIME_CUTTER(m_data.m_head.m_data[_T("order_start_time")]));

	//m_kdstime.SetWindowText(m_data.m_head.m_data[_T("kds_time")]);

	m_employee.SetWindowText(m_data.m_head.m_data[_T("open_employee_name")]);

	//val.LoadString(g_strEatTypeID[_ttoi(m_data.m_head.m_data[_T("eat_type")])]);
	g_lang.LoadString(val, g_strEatTypeID[_ttoi(m_data.m_head.m_data[_T("eat_type")])]);
	m_eattype.SetWindowText(val);

	m_custnum.SetWindowText(m_data.m_head.m_data[_T("customer_num")]);

	m_pos.SetWindowText(m_data.m_head.m_data[_T("pos_name")]);

	m_remark.SetWindowText(m_data.m_head.m_data[_T("remark")]);

	

	for (UINT i=0; i<m_data.m_detail.size(); i++)
	{
		CDataOrderDetail *pDetail = m_data.m_detail[i];
		COLORREF itemBgColor = RGB(0, 0, 0);

		//CString val(_T("*                     "));
		//val.Insert(1, pDetail->m_data[_T("quantity")]);
		//val.Insert(11, pDetail->m_data[_T("menu_item_name")]);

		CString tmp;
		INT row = m_list.GetItemCount();

		INT is_make = _ttoi(pDetail->m_data[_T("is_make")]);
		if (is_make == 1 || is_make == 2)
		{
			m_list.InsertItem(row, _T("√"));
		}
		else
		{
			m_list.InsertItem(row, _T(""));
		}
		

		tmp.Format(_T("x%s %s"), pDetail->m_data[_T("quantity")], pDetail->m_data[_T("unit")]);
		m_list.SetItemText(row, 1, tmp);
		//m_list.InsertItem(row, tmp);

		tmp.Empty();
		if (_ttoi(pDetail->m_data[_T("condiment_belong_item")]) != 0)
		{
			// 套餐子项
			tmp.Append(_T("#"));
		}

		INT eatType =g_strEatTypeID[_ttoi(pDetail->m_data[_T("eat_type")])];
		if (eatType == IDS_STR_TAKEOUT)
		{
			//tmp.LoadString(eatType);
			g_lang.LoadString(tmp, eatType);
			tmp.Append(_T("_"));
			itemBgColor = RGB( 255, 0, 0);
		}			

		if (pDetail->GetValue(_T("is_discount")) != _T(""))
		{
			CString tmp_2;
			g_lang.LoadString(tmp_2, IDS_STR_FREE);
			tmp.AppendFormat(_T("%s(%s)"), pDetail->m_data[_T("menu_item_name")], tmp_2);
		}
		else
		{
		tmp.Append(pDetail->m_data[_T("menu_item_name")]);
		}
		
		m_list.SetItemText(row, 2, tmp);
		m_list.SetItemData(row, itemBgColor);

		if (!pDetail->m_data[_T("description")].IsEmpty())
		{
			// 每2个调味描述换一行
			CString desc;
			INT i=0;
			tmp.Empty();
			while (AfxExtractSubString(desc, pDetail->m_data[_T("description")], i++, ' '))
			{
				tmp.AppendFormat(_T("  %s"), desc);
				if (i%2 == 0)
				{
					m_list.InsertItem(++row, _T(""));
					m_list.SetItemText(row, 1, tmp);
					m_list.SetItemData(row, itemBgColor);

					tmp.Empty();
				}
			}

			if (!tmp.IsEmpty())
			{
				m_list.InsertItem(++row, _T(""));
				m_list.SetItemText(row, 1, tmp);
				m_list.SetItemData(row, itemBgColor);
			}						
		}

		//val.AppendFormat(_T("*%-#9d"), _ttoi(pDetail->m_data[_T("quantity")]));
		//val.Format(_T("*%s"), pDetail->m_data[_T("quantity")]);
#if 0		
		if (_ttoi(pDetail->m_data[_T("condiment_belong_item")]) != 0)
		{
			//配菜，不显示数量
			val.Format(_T("\t%s"), pDetail->m_data[_T("menu_item_name")]);
		}
		else
		{
			//val.Format(_T("*%-#6s%s"), pDetail->m_data[_T("quantity")], pDetail->m_data[_T("menu_item_name")]);
			//val.Insert(1, pDetail->m_data[_T("quantity")]);
			//val.Insert(9, pDetail->m_data[_T("menu_item_name")]);
			val.Format(_T("*%s\t"), pDetail->m_data[_T("quantity")]);
			/*if (val.GetLength() < 10)
			{
				INT length = 10-val.GetLength();
				CString tmp(_T('.'), length);
				val.Append(tmp);
			}*/
			val.Append(pDetail->m_data[_T("menu_item_name")]);
		}
		
		if(!val.IsEmpty())
			m_list.AddString(val);

		val.Empty();
		if(!pDetail->m_data[_T("unit")].IsEmpty()
			|| !pDetail->m_data[_T("description")].IsEmpty())
		{
			val.Format(_T("%s %s"), pDetail->m_data[_T("unit")], pDetail->m_data[_T("description")]);
			for (INT i=0; i<val.GetLength(); )
			{
				CString tmp;
				//val.Insert(0, _T("\t"));
				tmp.Format(_T("\t%s"), val.Mid(i, 10));
				i += 10;
				m_list.AddString(tmp);
			}
			
		}
#endif
		//m_list.InsertItem(i, pDetail->m_data[_T("quantity")]);

		//m_list.SetItemText(i, e_unit, pDetail->m_data[_T("unit")]);

		//m_list.SetItemText(i, e_name, pDetail->m_data[_T("menu_item_name")]);

		//m_list.SetItemText(i, e_desc, pDetail->m_data[_T("description")]);
	}
	//Invalidate();
}
#if 0
void CDlgItemOrder::__LoadHead(CRecordset &rs)
{
	CString val, tmp, tmp2;

	rs.GetFieldValue(_T("table_name"), tmp);
	////val = tmp ; // 好像这样初始化不行，+/+=不起作用了
	////val.Format(_T("%s"), tmp);
	//rs.GetFieldValue(_T("order_head_id"), tmp2);
	////val += _T(".") + tmp; //好似=/+/+=只能使用一次？
	//m_headId = _ttoi(tmp2);
	////val.Format(_T("%s.%s"), tmp, tmp2);
	rs.GetFieldValue(_T("check_name"), tmp);
	if (!tmp.IsEmpty())
	{
		val += _T(".") + tmp;
	}
	m_title.SetWindowText(val);

	rs.GetFieldValue(_T("order_head_id"), val);
	m_orderHeadId.SetWindowText(val);
	m_headId = _ttoi(val);

	rs.GetFieldValue(_T("order_start_time"), val);
	m_ordertime.SetWindowText(val);

	//rs.GetFieldValue(_T("kds_time"), val);
	//m_kdstime.SetWindowText(val);

	//rs.GetFieldValue(_T("kds_show"), str);
	
	rs.GetFieldValue(_T("open_employee_name"), val);
	m_employee.SetWindowText(val);
	
	rs.GetFieldValue(_T("eat_type"), tmp);
	val.LoadString(g_strEatTypeID[_ttoi(tmp)]);
	m_eattype.SetWindowText(val);

	rs.GetFieldValue(_T("customer_num"), val);
	m_custnum.SetWindowText(val);

	rs.GetFieldValue(_T("pos_name"), val);
	m_pos.SetWindowText(val);	
}

INT CDlgItemOrder::__LoadDetail(CRecordset &rs)
{	
	INT cnt = 0;
	CString val;
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

		rs.MoveNext();
		cnt++;
	}
	return cnt;
}
#endif
CBaseData* CDlgItemOrder::GetDataPtr()
{
	return (CBaseData*)(&m_data);
}

void CDlgItemOrder::OnBnClickedBtnPend()
{
	// TODO: 在此添加控件通知处理程序代码
	//long order_head_id = _ttoi(m_data.m_head.m_data[_T("order_head_id")]);
	//long check_id = _ttoi(m_data.m_head.m_data[_T("check_id")]);
	//AfxGetMainWnd()->PostMessage(WM_MSG_PEND, &(m_data.m_head), 0);
	DoAction(WM_MSG_PEND);
}

void CDlgItemOrder::OnBnClickedBtnDone()
{
	// TODO: 在此添加控件通知处理程序代码
	//long order_head_id = _ttoi(m_data.m_head.m_data[_T("order_head_id")]);
	//long check_id = _ttoi(m_data.m_head.m_data[_T("check_id")]);
	//AfxGetMainWnd()->PostMessage(WM_MSG_DONE, &(m_data.m_head), 0);
	DoAction(WM_MSG_DONE);
}

void CDlgItemOrder::OnBnClickedBtnRecovery()
{
	// TODO: 在此添加控件通知处理程序代码
	//long order_head_id = _ttoi(m_data.m_head.m_data[_T("order_head_id")]);
	//long check_id = _ttoi(m_data.m_head.m_data[_T("check_id")]);
	//AfxGetMainWnd()->PostMessage(WM_MSG_RECOVERY, &(m_data.m_head), 0);
	DoAction(WM_MSG_RECOVERY);
}

inline void CDlgItemOrder::DoAction(INT nIDMsg)
{
	// by lv on 170320 - LPARAM的传参是为了调用打印接口方便，打印接口需要整单信息
	AfxGetMainWnd()->PostMessage(nIDMsg, (WPARAM)(&(m_data.m_head)), (LPARAM)(&m_data));
}

void CDlgItemOrder::ResetShowState(KDS_ShowType showType)
{
	//CWnd* pBtnPend = GetDlgItem(IDC_BTN_PEND);
	//CWnd* pBtnDone = GetDlgItem(IDC_BTN_DONE);
	//CString str;

	switch(showType)
	{
	case E_S_DONING:
		m_btnPend.ShowWindow(SW_SHOW);
		m_btnDone.ShowWindow(SW_SHOW);
		m_btnRecovery.ShowWindow(SW_HIDE);

		//str.LoadString(IDS_STR_DONE);
		//pBtnDone->SetWindowText(str);
		//nIDMsgDone = WM_MSG_DONE;
		break;

	case E_S_PENDED:
		m_btnPend.ShowWindow(SW_HIDE);
		m_btnDone.ShowWindow(SW_HIDE);
		m_btnRecovery.ShowWindow(SW_SHOW);

		//str.LoadString(IDS_STR_RECOVERY);
		//pBtnDone->SetWindowText(str);
		//nIDMsgDone = WM_MSG_RECOVERY;
		break;

	case E_S_DONE:
		m_btnPend.ShowWindow(SW_HIDE);
		m_btnDone.ShowWindow(SW_HIDE);
		m_btnRecovery.ShowWindow(SW_HIDE);
		break;

	default:
		AfxMessageBox(_T("Unknown Show Type in ResetShowState"));
	}	
}

BOOL CDlgItemOrder::OnEraseBkgnd(CDC* pDC)
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
	pDC->SetStretchBltMode(HALFTONE);
	//pDC->StretchBlt(0,0,CreatButton::m_nFullWidth,CreatButton::m_nFullHeight,&MemDc,0,0,m_x,m_y,SRCCOPY);
	// (0,0)作为upper-left corner.使用rect只能显示到第一个item
	// +7是为了把item-dlg的边框黑条变小一点，暂时没什么别的办法
	pDC->StretchBlt(0, 0,rect.Width()+7, rect.Height()+7,&MemDc,0,0,m_x,m_y,SRCCOPY);
	MemDc.SelectObject(hOldBmp);
	return TRUE;
	//return CDialog::OnEraseBkgnd(pDC);
}

HBRUSH CDlgItemOrder::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  在此更改 DC 的任何属性
	if(nCtlColor==CTLCOLOR_STATIC)
	{
		if (pWnd->GetDlgCtrlID() == IDC_STATIC_ORDER)
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
void CDlgItemOrder::OnNMCustomdrawListDetail(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	LPNMLVCUSTOMDRAW  lplvcd = (LPNMLVCUSTOMDRAW)pNMHDR; 
	*pResult  =  CDRF_DODEFAULT;

	switch (lplvcd->nmcd.dwDrawStage) 
	{ 
	case CDDS_PREPAINT : 
		{ 
			*pResult = CDRF_NOTIFYITEMDRAW; 
			return; 
		} 
	case CDDS_ITEMPREPAINT: 
		{ 
			*pResult = CDRF_NOTIFYSUBITEMDRAW; 
			return; 
		} 
	case CDDS_ITEMPREPAINT | CDDS_SUBITEM:
		{ 

			int    nItem = static_cast<int>( lplvcd->nmcd.dwItemSpec );
			COLORREF clrNewTextColor, clrNewBkColor;
			clrNewTextColor=RGB(0,0,0);
			/*if( nItem%2 ==0 )
			{
				clrNewBkColor = RGB( 255, 255, 0);
			}
			else
			{
				clrNewBkColor = RGB( 0, 255, 0 );				
			}*/

			//lplvcd->clrText = clrNewTextColor;
			//lplvcd->clrTextBk = m_list.GetItemData(nItem);
			lplvcd->clrText = m_list.GetItemData(nItem);

			*pResult = CDRF_DODEFAULT;  
			return; 
		} 
	} 
	//*pResult = 0;
}
