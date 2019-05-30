// DlgOrderFunction.cpp : 实现文件
//

#include "stdafx.h"
#include "KDSManager.h"
#include "DlgOrderFunction.h"
#include "DlgCombine.h"
#include "KDSManagerDlg.h"


// CDlgOrderFunction 对话框

IMPLEMENT_DYNAMIC(CDlgOrderFunction, CDialog)

CDlgOrderFunction::CDlgOrderFunction(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgOrderFunction::IDD, pParent)
{

}

CDlgOrderFunction::~CDlgOrderFunction()
{
}

void CDlgOrderFunction::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BTN_COOKCOMPLETE, m_btnCookComplete);
	DDX_Control(pDX, IDC_BTN_COMBINEORDER, m_btnCombineOrder);
	DDX_Control(pDX, IDC_BTN_PENDORDER, m_btnPendOrder);
	DDX_Control(pDX, IDC_BTN_DONOTMAKE, m_btnDoNotMake);
	DDX_Control(pDX, IDC_BTN_CLOSEDLG, m_btnCloseDlg);
	DDX_Control(pDX, IDC_STATIC_CHECKNAME, m_checkName);
	DDX_Control(pDX, IDC_STATIC_TABELNAME, m_tabelName2);
	DDX_Control(pDX, IDC_STATIC_CUSTOMERNUM, m_customerNum);
	DDX_Control(pDX, IDC_STATIC_EATTYPEDETAIL, m_eatTypeDetail);
	DDX_Control(pDX, IDC_STATIC_REMARK, m_remark);
	DDX_Control(pDX, IDC_STATIC_OPENEMPLOEENAME, m_openEmploeeName);
	DDX_Control(pDX, IDC_STATIC_POSNAME, m_posName);
	DDX_Control(pDX, IDC_STATIC_ORDERTIME, m_orderTime);
	DDX_Control(pDX, IDC_STATIC_COMPLETETIME, m_completeTime);
}


BEGIN_MESSAGE_MAP(CDlgOrderFunction, CDialog)
	ON_WM_CTLCOLOR()
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_BTN_COMBINEORDER, &CDlgOrderFunction::OnBnClickedBtnCombineorder)
	ON_BN_CLICKED(IDC_BTN_COOKCOMPLETE, &CDlgOrderFunction::OnBnClickedBtnCookcomplete)
	ON_BN_CLICKED(IDC_BTN_PENDORDER, &CDlgOrderFunction::OnBnClickedBtnPendorder)
	ON_BN_CLICKED(IDC_BTN_DONOTMAKE, &CDlgOrderFunction::OnBnClickedBtnDoNotMake)
	ON_BN_CLICKED(IDC_BTN_CLOSEDLG, &CDlgOrderFunction::OnBnClickedBtnClosedlg)
END_MESSAGE_MAP()


// CDlgOrderFunction 消息处理程序

BOOL CDlgOrderFunction::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	RefreshData();
	ResetShowState();
	ShowItemDetailDlg();
	ResetLang();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

HBRUSH CDlgOrderFunction::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  在此更改 DC 的任何属性

	// TODO:  如果默认的不是所需画笔，则返回另一个画笔
	if (nCtlColor == CTLCOLOR_STATIC)
	{
		pDC->SetTextColor(RGB(0, 0, 0));
		pDC->SetBkMode(TRANSPARENT);
		return (HBRUSH)::GetStockObject(NULL_BRUSH); //返回此画刷可以使静态文本透明
	}
	return hbr;
}

BOOL CDlgOrderFunction::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CRect rect;
	GetWindowRect(&rect);

	Gdiplus::Graphics graphics(pDC->m_hDC); // Create a GDI+ graphics object
	Gdiplus::Image image(_T(".\\res\\bg_image.jpg")); 
	graphics.DrawImage(&image, 0, 0, rect.Width(), rect.Height());	// 画界面背景

	return TRUE;
	return CDialog::OnEraseBkgnd(pDC);
}

void CDlgOrderFunction::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: 在此处添加消息处理程序代码
	// 不为绘图消息调用 CDialog::OnPaint()
}

inline void CDlgOrderFunction::DoAction(INT nIDMsg)
{
	AfxGetMainWnd()->SendMessage(nIDMsg, (WPARAM)(&m_data), 0);
	//不使用PostMessage，因为PostMessage立即返回，在未处理完成时OnOK()会释放m_data，其他地方访问m_data出问题。
	OnOK();
}

void CDlgOrderFunction::OnBnClickedBtnCookcomplete()
{
	// TODO: 在此添加控件通知处理程序代码
	DoAction(WM_MSG_DONE);
}

void CDlgOrderFunction::OnBnClickedBtnCombineorder()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_showType == E_S_DONING)
	{
		CDlgCombine dlg;
		dlg.ClearData();
		dlg.m_data = m_data;
		if (dlg.DoModal() == IDOK)
		{
			OnOK();
		}
	} 
	else
	{
		DoAction(WM_MSG_RECOVERY);
	}
	
}

void CDlgOrderFunction::OnBnClickedBtnPendorder()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_showType == E_S_DONING)
	{
		DoAction(WM_MSG_PEND);
	} 
	else
	{
		DoAction(WM_MSG_DONOTMAKE);
	}
	
}

void CDlgOrderFunction::OnBnClickedBtnDoNotMake()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_showType == E_S_DONING)
	{
		DoAction(WM_MSG_DONOTMAKE);
	} 
	else
	{
		OnOK();
	}
}

void CDlgOrderFunction::OnBnClickedBtnClosedlg()
{
	// TODO: 在此添加控件通知处理程序代码
	OnOK();
}

void CDlgOrderFunction::SetShowType(KDS_ShowType showType)
{

	this->m_showType = showType;
}

void CDlgOrderFunction::ResetShowState()
{
	CString str;

	switch(m_showType)
	{
	case E_S_DONING:
		{
			m_btnCookComplete.ShowWindow(TRUE);
			m_btnCloseDlg.ShowWindow(TRUE);
			g_lang.LoadString(str,IDS_STR_COMBINEORDER);
			m_btnCombineOrder.SetWindowText(str);
			g_lang.LoadString(str,IDS_STR_PENDORDER);
			m_btnPendOrder.SetWindowText(str);
			g_lang.LoadString(str,IDS_STR_DONOTMAKE);
			m_btnDoNotMake.SetWindowText(str);

			CString tmp = m_data.GetValue(_T("has_same"));
			long iCombineNum = _ttoi(m_data.GetValue(_T("CombineNum")));
			if (tmp.Compare(_T("yes")) == 0 || iCombineNum > 1)
			{
				m_btnCombineOrder.EnableWindow(TRUE);
			} 
			else
			{
				m_btnCombineOrder.EnableWindow(FALSE);
			}

			if (iCombineNum > 1)
			{
				//m_btnCombineOrder.EnableWindow(TRUE);
				m_btnPendOrder.EnableWindow(FALSE);
				m_btnDoNotMake.EnableWindow(FALSE);
			}
			else
			{
				//m_btnCombineOrder.EnableWindow(FALSE);
				m_btnPendOrder.EnableWindow(TRUE);
				m_btnDoNotMake.EnableWindow(TRUE);
			}

			KDS_Mode mode = ((CKDSManagerDlg*)AfxGetMainWnd())->GetKitchenMode();
			if (mode == E_M_SEND)
			{
				m_btnCombineOrder.EnableWindow(FALSE);
				m_btnDoNotMake.EnableWindow(FALSE);
			}
		}
		break;

	case E_S_PENDED:	
		{
			m_btnCookComplete.ShowWindow(FALSE);
			m_btnCloseDlg.ShowWindow(FALSE);
			g_lang.LoadString(str,IDS_STR_CANCELPEND);
			m_btnCombineOrder.SetWindowText(str);
			g_lang.LoadString(str,IDS_STR_DONOTMAKE);
			m_btnPendOrder.SetWindowText(str);
			g_lang.LoadString(str,IDS_STR_CLOSEDLG);
			m_btnDoNotMake.SetWindowText(str);
			m_btnCombineOrder.EnableWindow(TRUE);
		}
		break;

	case E_S_DONE:

		break;

	default:
		AfxMessageBox(_T("Unknown Show Type in ResetShowState"));
	}	
}

void CDlgOrderFunction::RefreshData()
{
	CString tmp,tmpFormat;

	g_lang.LoadString(tmp, g_strEatTypeID[_ttoi(m_data.GetValue(_T("eat_type")))]);
	m_eatTypeDetail.SetWindowText(tmp);

	//tmp = m_data.GetValue(_T("check_name"));
	//if (tmp.GetLength() == 0)
	//{
		CString tmp1 = m_data.GetValue(_T("check_number"));
		CString tmp2 = m_data.GetValue(_T("check_id"));
		if (tmp2.GetLength() == 0)
		{
			tmp = tmp1;
		} 
		else
		{
			tmp.Format(_T("%s/%s"),tmp1,tmp2);
		}
	//}
	m_checkName.SetWindowText(tmp);

	m_tabelName2.SetWindowText(m_data.GetValue(_T("table_name")));

	m_customerNum.SetWindowText(m_data.GetValue(_T("customer_num")));

	//m_remark.SetWindowText(m_data.GetValue(_T("remark")));//remark暂时不使用
	m_remark.SetWindowText(m_data.GetValue(_T("check_name")));

	m_openEmploeeName.SetWindowText(m_data.GetValue(_T("order_employee_name")));

	m_posName.SetWindowText(m_data.GetValue(_T("pos_name")));

	m_orderTime.SetWindowText(m_data.GetValue(_T("order_time")));

	m_completeTime.SetWindowText(m_data.GetValue(_T("kds_time")));

	//UpdateData(FALSE);
	//Invalidate(TRUE);
}

void CDlgOrderFunction::ClearData()
{
	m_data.ClearData();
	//UpdateData(FALSE);
}


void CDlgOrderFunction::ResetLang()
{
	CString caption;

	if (g_lang.m_strLang.Compare(_T("English")) == 0)
	{
		SetWindowText(_T(""));
	}
	else
	{
		SetWindowText(_T("菜品功能"));
	}

	g_lang.LoadString(caption,IDS_STR_COOKCOMPLETE);
	m_btnCookComplete.SetWindowText(caption);
	g_lang.LoadString(caption,IDS_STR_COMBINEORDER);
	m_btnCombineOrder.SetWindowText(caption);
	g_lang.LoadString(caption,IDS_STR_PENDORDER);
	m_btnPendOrder.SetWindowText(caption);
	g_lang.LoadString(caption,IDS_STR_DONOTMAKE);
	m_btnDoNotMake.SetWindowText(caption);
	g_lang.LoadString(caption,IDS_STR_CLOSEDLG);
	m_btnCloseDlg.SetWindowText(caption);

	CStatic *pWnd = (CStatic*)GetDlgItem(IDC_STATIC_CHECKNAMELABEL);
	g_lang.LoadString(caption, IDS_STR_CHECKNAME);
	pWnd->SetWindowText(caption);

	pWnd = (CStatic*)GetDlgItem(IDC_STATIC_TABELNAMELABEL);
	g_lang.LoadString(caption, IDS_STR_TABLE);
	pWnd->SetWindowText(caption);

	pWnd = (CStatic*)GetDlgItem(IDC_STATIC_COMPLETETIMELABEL);
	g_lang.LoadString(caption, IDS_STR_CUSTNUM4);
	pWnd->SetWindowText(caption);

	pWnd = (CStatic*)GetDlgItem(IDC_STATIC_EATTYPELABEL);
	g_lang.LoadString(caption, IDS_STR_EATTYPE);
	pWnd->SetWindowText(caption);

	pWnd = (CStatic*)GetDlgItem(IDC_STATIC_REMARKLABEL);
	g_lang.LoadString(caption, IDS_STR_REMARK);
	pWnd->SetWindowText(caption);

	pWnd = (CStatic*)GetDlgItem(IDC_STATIC_OPENEMPLOYEENAMELABEL);
	g_lang.LoadString(caption, IDS_STR_OPENEMPLOYEE);
	pWnd->SetWindowText(caption);

	pWnd = (CStatic*)GetDlgItem(IDC_STATIC_POSNAMELABEL);
	g_lang.LoadString(caption, IDS_STR_POS);
	pWnd->SetWindowText(caption);

	pWnd = (CStatic*)GetDlgItem(IDC_STATIC_CUSTOMERNUMLABEL);
	g_lang.LoadString(caption, IDS_STR_CUSTNUM);
	pWnd->SetWindowText(caption);

	pWnd = (CStatic*)GetDlgItem(IDC_STATIC_EATTYPELABEL);
	g_lang.LoadString(caption, IDS_STR_EATTYPE);
	pWnd->SetWindowText(caption);

	pWnd = (CStatic*)GetDlgItem(IDC_STATIC_ORDERTIMELABEL);
	g_lang.LoadString(caption, IDS_STR_ORDERTIME);
	pWnd->SetWindowText(caption);

	pWnd = (CStatic*)GetDlgItem(IDC_STATIC_COMPLETETIMELABEL);
	g_lang.LoadString(caption, IDS_STR_KDSTIME);
	pWnd->SetWindowText(caption);

	ResetShowState();
}

BOOL CDlgOrderFunction::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
	if(pMsg->message==WM_KEYDOWN)  //判断是否有按键按下
	{
		switch(pMsg->wParam)
		{
		case VK_DOWN:     //表示是方向键中的向下的键

			break;
		case VK_UP:      //表示是方向键中的向上的键

			break;
		case VK_LEFT:      //表示是方向键中的向左的键
			
			break;
		case VK_RIGHT:      //表示是方向键中的向右的键
			
			break;
		case VK_NUMPAD0:      //表示是数字键中的0键

			break;
		case VK_NUMPAD1:      //表示是数字键中的1键

			break;
		case VK_NUMPAD2:      //表示是数字键中的2键

			break;
		case VK_NUMPAD3:      //表示是数字键中的3键

			break;
		case VK_NUMPAD4:      //表示是数字键中的4键

			break;
		case VK_NUMPAD5:      //表示是数字键中的5键

			break;
		case VK_NUMPAD6:      //表示是数字键中的6键

			break;
		case VK_NUMPAD7:      //表示是数字键中的7键

			break;
		case VK_NUMPAD8:      //表示是数字键中的8键

			break;
		case VK_NUMPAD9:      //表示是数字键中的9键

			break;
		case VK_RETURN:      //表示是功能键中的F1键
			if (m_showType == E_S_DONING)
			{
				OnBnClickedBtnCookcomplete();
			} 
			break;
		case VK_F1:      //表示是功能键中的F2键
			OnBnClickedBtnCombineorder();
			break;
		case VK_F2:      //表示是功能键中的F3键
			if (m_showType == E_S_DONING)
			{
				DoAction(WM_MSG_PEND);
			} 
			else
			{
				DoAction(WM_MSG_RECOVERY);
			}
			break;
		case VK_F3:      //表示是功能键中的F4键
			OnBnClickedBtnDoNotMake();
			break;
		case VK_F4:      //表示是功能键中的F5键
			OnBnClickedBtnClosedlg();
			break;
		case VK_F5:      //表示是功能键中的F6键
			
			break;
		case VK_F6:      //表示是回车键

			break;
		case VK_ESCAPE:      //表示是功能键中的ESC键

			break;
		default:
			break;
		}
	}

	return CDialog::PreTranslateMessage(pMsg);
}

void CDlgOrderFunction::ShowItemDetailDlg(void)
{
	dlgItemDetailNew2.Create(IDD_DLG_ITEM_DETAILNEW2,this);
	dlgItemDetailNew2.m_data = m_data;
	dlgItemDetailNew2.RefreshData();
	dlgItemDetailNew2.ResetShowState(E_S_DONING);
	dlgItemDetailNew2.m_bItemNameCanClick = FALSE;
	CRect rect;
	((CStatic*)GetDlgItem(IDC_STATIC_POSITION))->GetWindowRect(&rect);
	ScreenToClient(&rect);
	CRect dlgRect;
	dlgItemDetailNew2.GetWindowRect(&dlgRect);
	dlgItemDetailNew2.MoveWindow(rect.left,rect.top,dlgRect.Width(),dlgRect.Height());
	dlgItemDetailNew2.ShowWindow(TRUE);
}
