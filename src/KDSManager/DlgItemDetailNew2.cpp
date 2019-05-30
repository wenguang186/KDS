// DlgItemDetailNew2.cpp : 实现文件
//

#include "stdafx.h"
#include "KDSManager.h"
#include "DlgItemDetailNew2.h"
#include "DlgOrderFunction.h"
#include "KDSManagerDlg.h"
#include "KDSManagerDlg.h"


// CDlgItemDetailNew2 对话框

IMPLEMENT_DYNAMIC(CDlgItemDetailNew2, CDialog)

CDlgItemDetailNew2::CDlgItemDetailNew2(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgItemDetailNew2::IDD, pParent)
{
	m_bIsCombine = FALSE;
	m_bShowSelectPicture = FALSE;
	m_bIsRush = TRUE;
	m_bItemNameCanClick = TRUE;
	m_bSelectIn = FALSE;
	bShowTable = FALSE;
	// 图片资源，析构时要释放掉
	/*
	// 使用CImage加载有透明的PNG图片时，需要确认该图像包含Alpha通道，需要处理。
	// 使用GDI+画图，不使用CImage
	*/
	/*
	CImage img;
	if(img.Load(_T(".\\res\\item_detail_bgimage1.png"))==S_OK)
	{
		m_x=img.GetWidth();
		m_y=img.GetHeight();
		m_bpBackgrd=img.Detach();
	}

	CImage img2;
	if(img2.Load(_T(".\\res\\item_detail_bgimage2.png"))==S_OK)
	{
		m_x=img2.GetWidth();
		m_y=img2.GetHeight();
		m_bpBackgrd2=img2.Detach();
	}
	*/
}

CDlgItemDetailNew2::~CDlgItemDetailNew2()
{
	//::DeleteObject(m_bpBackgrd);
	//::DeleteObject(m_bpBackgrd2);
}

void CDlgItemDetailNew2::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_EATTYPE, m_eat_type);
	DDX_Control(pDX, IDC_STATIC_ITEMNAME1, m_itemName1);
	DDX_Control(pDX, IDC_STATIC_ITEMNAME2, m_itemName2);
	DDX_Control(pDX, IDC_STATIC_DESCRIPTION, m_description);
	DDX_Control(pDX, IDC_STATIC_QUANTITY, m_quantity);
	DDX_Control(pDX, IDC_STATIC_WAITTIME, m_waitTime);
	DDX_Control(pDX, IDC_STATIC_TABELNAME, m_tableName);
	DDX_Control(pDX, IDC_STATIC_WAITPICTURE, m_waitPicture);
	DDX_Control(pDX, IDC_STATIC_SELECTPICTURE, m_selectPicture);
	DDX_Control(pDX, IDC_STATIC_RUSHPICTRUE, m_rushPicture);
	DDX_Control(pDX, IDC_STATIC_OUTTIME, m_outTimePicture);
}


BEGIN_MESSAGE_MAP(CDlgItemDetailNew2, CDialog)
	ON_WM_CTLCOLOR()
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
	ON_STN_CLICKED(IDC_STATIC_ITEMNAME1, &CDlgItemDetailNew2::OnStnClickedStaticItemName1)
	ON_WM_NCHITTEST()
END_MESSAGE_MAP()


// CDlgItemDetailNew2 消息处理程序

HBRUSH CDlgItemDetailNew2::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  在此更改 DC 的任何属性
	INT id = pWnd->GetDlgCtrlID();
	if (id == IDC_STATIC_ITEMNAME1)
	{
		CString text;
		m_itemName1.GetWindowText(text);
		INT i = text.GetLength();
		
		CFont font;
		LOGFONT lFont;
		m_itemName1.GetFont()->GetLogFont(&lFont);
		if (i < 18)
		{//一行文字或两行
			lFont.lfHeight = 32;
		}
		else if (i < 36)
		{//三行文字
			lFont.lfHeight = 21;
		}
		else
		{
			lFont.lfHeight = 15;
		}
		//lFont.lfHeight = 28;
		lFont.lfWidth = 0;
		font.CreateFontIndirect(&lFont);

		pDC->SelectObject(&font);
		pDC->SetTextColor(RGB(0, 0, 0));
		pDC->SetBkMode(TRANSPARENT);
		return (HBRUSH)::GetStockObject(NULL_BRUSH);
	}

	if (id == IDC_STATIC_QUANTITY)
	{
		CString text;
		m_quantity.GetWindowText(text);
		INT i = text.GetLength();
		CFont font;
		LOGFONT lFont;
		m_quantity.GetFont()->GetLogFont(&lFont);
		
		if (i < 8)
		{
			lFont.lfHeight = 38;
		}
		else
		{

			lFont.lfHeight = 24;
		}
		
		//lFont.lfHeight = 18;
		lFont.lfWidth = 0;
		font.CreateFontIndirect(&lFont);

		pDC->SelectObject(&font);
		CString tmp = m_data.GetValue(_T("has_same"));
		INT iCombine = (INT)_ttoi(m_data.GetValue(_T("CombineNum")));

		CRect rect; 
		GetWindowRect(&rect);

		if( showType == E_S_DONING )
		{
			if (tmp.Compare(_T("yes")) == 0 || iCombine > 1)
			{
				pDC->SetTextColor(RGB(255, 255, 255));
			}
			else
			{
				pDC->SetTextColor(RGB(0, 0, 0));
			}
		}
		pDC->SetBkMode(TRANSPARENT);
		return (HBRUSH)::GetStockObject(NULL_BRUSH);
	}

	if (id == IDC_STATIC_EATTYPE)
	{
		INT eat_type = g_strEatTypeID[_ttoi(m_data.GetValue(_T("eat_type")))];
		if (eat_type == IDS_STR_EATIN)
		{
			pDC->SetTextColor(RGB(255, 255, 255));
		}
		else
		{
			pDC->SetTextColor(RGB(255, 0, 0));
		}
		pDC->SetBkMode(TRANSPARENT);
		return (HBRUSH)::GetStockObject(NULL_BRUSH);
	}

	if (id == IDC_STATIC_DESCRIPTION)
	{
		pDC->SetTextColor(RGB(255, 64, 0));
		pDC->SetBkMode(TRANSPARENT);
		return (HBRUSH)::GetStockObject(NULL_BRUSH);
	}

	if (nCtlColor == CTLCOLOR_STATIC)
	{
		pDC->SetTextColor(RGB(0, 0, 0));
		pDC->SetBkMode(TRANSPARENT);
		return (HBRUSH)::GetStockObject(NULL_BRUSH); //返回此画刷可以使静态文本透明
	}


	// TODO:  如果默认的不是所需画笔，则返回另一个画笔
	return hbr;
}

BOOL CDlgItemDetailNew2::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	/*
	if (m_bpBackgrd==NULL)
	{
		return FALSE;
	}
	CRect rect; 
	GetWindowRect(&rect); 

	CDC MemDc;
	MemDc.CreateCompatibleDC(pDC);
	HGDIOBJ hOldBmp;

	CString tmp = m_data.GetValue(_T("has_same"));
	INT iCombine = (INT)_ttoi(m_data.GetValue(_T("CombineNum")));
	if (tmp.Compare(_T("yes")) == 0 || iCombine > 1)
	{
		hOldBmp = MemDc.SelectObject(m_bpBackgrd2);
	} 
	else
	{
		hOldBmp = MemDc.SelectObject(m_bpBackgrd);
	}

	//pDC->FillSolidRect(0,0,rect.Width(),rect.Height(),RGB(255,255,255));

	pDC->SetStretchBltMode(HALFTONE);
	//pDC->StretchBlt(0,0,CreatButton::m_nFullWidth,CreatButton::m_nFullHeight,&MemDc,0,0,m_x,m_y,SRCCOPY);
	// (0,0)作为upper-left corner.使用rect只能显示到第一个item
	// +7是为了把item-dlg的边框黑条变小一点，暂时没什么别的办法
	switch(showType)
	{
	case E_S_DONING:
		pDC->StretchBlt(0, 0,rect.Width(), rect.Height(),&MemDc,0,0,m_x,m_y,SRCCOPY);
		MemDc.SelectObject(hOldBmp);
		break;

	case E_S_PENDED:	
	case E_S_DONE:
		pDC->StretchBlt(0, 0,rect.Width(), rect.Height(),&MemDc,0,0,m_x,m_y,SRCCOPY);
		MemDc.SelectObject(hOldBmp);
		break;

	default:
		AfxMessageBox(_T("Unknown Show Type in ResetShowState"));
	}	
	//pDC->StretchBlt(0, 0,rect.Width(), rect.Height(),&MemDc,0,0,m_x,m_y,SRCCOPY);
	MemDc.SelectObject(hOldBmp);
	*/
	Gdiplus::Graphics graphics(pDC->m_hDC); // Create a GDI+ graphics object
	
	CString tmp = m_data.GetValue(_T("has_same"));
	INT iCombine = (INT)_ttoi(m_data.GetValue(_T("CombineNum")));

	CRect rect; 
	GetWindowRect(&rect);

	if( showType == E_S_DONING )
	{
		if (tmp.Compare(_T("yes")) == 0 || iCombine > 1)
		{
			if (m_bSelectIn)
			{
				Gdiplus::Image image(_T(".\\res\\item_detail_bgimage2Select.png"));
				graphics.DrawImage(&image, 0, 0, rect.Width(), rect.Height());
			} 
			else
			{
				Gdiplus::Image image(_T(".\\res\\item_detail_bgimage2.png"));
				graphics.DrawImage(&image, 0, 0, rect.Width(), rect.Height());
			}
		} 
		else
		{
			if (m_bSelectIn)
			{
				Gdiplus::Image image(_T(".\\res\\item_detail_bgimage1Select.png"));
				graphics.DrawImage(&image, 0, 0, rect.Width(), rect.Height());
			} 
			else
			{
				Gdiplus::Image image(_T(".\\res\\item_detail_bgimage1.png"));
				graphics.DrawImage(&image, 0, 0, rect.Width(), rect.Height());
			}
		}
	}
	else if ( showType == E_S_PENDED || showType == E_S_DONE)
	{
		if (m_bSelectIn)
		{
			Gdiplus::Image image(_T(".\\res\\item_detail_bgimage1Select.png"));
			graphics.DrawImage(&image, 0, 0, rect.Width(), rect.Height());
		} 
		else
		{
			Gdiplus::Image image(_T(".\\res\\item_detail_bgimage1.png"));
			graphics.DrawImage(&image, 0, 0, rect.Width(), rect.Height());
		}
	}
	else
	{
		AfxMessageBox(_T("Unknown Show Type in ResetShowState"));
	}

	return TRUE;
	return CDialog::OnEraseBkgnd(pDC);
}

void CDlgItemDetailNew2::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: 在此处添加消息处理程序代码
	refreshTipPicture(IDC_STATIC_WAITPICTURE,TRUE,_T(".\\res\\wait.png"),_T(".\\res\\wait.png"));
	refreshTipPicture(IDC_STATIC_SELECTPICTURE,m_bIsCombine,_T(".\\res\\select.png"),_T(".\\res\\donotselect.png"));
	refreshTipPicture(IDC_STATIC_RUSHPICTRUE,m_bIsRush,_T(".\\res\\rush.png"),_T(".\\res\\rush.png"));
	refreshTipPicture(IDC_STATIC_OUTTIME,TRUE,_T(".\\res\\outtime.png"),_T(".\\res\\outtime.png"));
}

BOOL CDlgItemDetailNew2::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CDlgItemDetailNew2::ResetShowState(KDS_ShowType showType)
{
	CString str;
	this->showType = showType;
	KDS_Mode mode;
	switch(showType)
	{
	case E_S_DONING:
		mode = ((CKDSManagerDlg*)AfxGetMainWnd())->GetKitchenMode();
		if (mode == E_M_SEND || bShowTable)
		{
			m_tableName.ShowWindow(TRUE);
		}
		else
		{

			m_tableName.ShowWindow(FALSE);
		}
		m_bItemNameCanClick = TRUE;
		break;

	case E_S_PENDED:		
		m_tableName.ShowWindow(TRUE);
		m_bItemNameCanClick = TRUE;
		break;

	case E_S_DONE:
		m_tableName.ShowWindow(TRUE);
		m_bItemNameCanClick = FALSE;
		break;

	default:
		AfxMessageBox(_T("Unknown Show Type in ResetShowState"));
	}	
	if (m_bShowSelectPicture)
	{
		m_selectPicture.ShowWindow(TRUE);
		refreshTipPicture(IDC_STATIC_SELECTPICTURE,m_bIsCombine,_T(".\\res\\select.png"),_T(".\\res\\donotselect.png"));
	}
	else
	{
		m_selectPicture.ShowWindow(FALSE);
	}

	if ((INT)_ttoi(m_data.GetValue(_T("rush"))) > 0)
	{
		m_bIsRush = TRUE;
		m_rushPicture.ShowWindow(TRUE);
		refreshTipPicture(IDC_STATIC_RUSHPICTRUE,m_bIsRush,_T(".\\res\\rush.png"),_T(".\\res\\rush.png"));
	}
	else
	{
		m_bIsRush = FALSE;
		m_rushPicture.ShowWindow(FALSE);
	}

	if ((INT)_ttoi(m_data.GetValue(_T("is_make"))) == 5)
	{
		m_outTimePicture.ShowWindow(TRUE);
		refreshTipPicture(IDC_STATIC_OUTTIME,TRUE,_T(".\\res\\outtime.png"),_T(".\\res\\outtime.png"));
	}
	else
	{
		m_outTimePicture.ShowWindow(FALSE);
	}
}

void CDlgItemDetailNew2::RefreshData()
{
	CString tmp,tmpFormat,strTmp;

	tmp = m_data.GetValue(_T("menu_item_name"));
	strTmp = m_data.GetValue(_T("item_name2"));
	if (_ttoi(m_data.GetValue(_T("condiment_belong_item"))) < 0)
	{
		CString setmeal;
		g_lang.LoadString(setmeal, IDS_STR_SETMEAL);
		// 不知道为什么这里 AppendFormat 没有用
		//tmp.AppendFormat(_T("(%s)"), setmeal);
		CString tmp_2 = ((CKDSManagerDlg*)AfxGetMainWnd())->GetSetmealNameByOrderHeadId(m_data.GetValue(_T("order_detail_id")));
		CString tmp_3;
		tmp_3.Format(_T("%s\n(%s%s)"),tmp, tmp_2, setmeal);
		tmp = tmp_3;
	}

	// 170228 lv is_discount是bit类型，不知道怎么转换，所以通过判断是否为空来看是否赠送
	if (m_data.GetValue(_T("is_discount")) != _T(""))
	{
		CString tmp_2;
		g_lang.LoadString(tmp_2, IDS_STR_FREE);
		// 不知道为什么这里 AppendFormat 没有用
		//tmp.AppendFormat(_T("(%s)"), tmp_2);
		CString tmp_3;
		tmp_3.Format(_T("%s(%s)"),tmp,tmp_2);
		tmp = tmp_3;
	}

	E_ItemName_ShowType nNameShowType = ((CKDSManagerDlg*)AfxGetMainWnd())->GetNameShowType();
	switch(nNameShowType)
	{
	case E_ONLYNAME1:
		m_itemName1.SetWindowText(tmp);
		m_itemName2.SetWindowText(_T("")/*m_data.GetValue(_T("item_name2"))*/);
		break;
	case E_ONLYNAME2:
		m_itemName1.SetWindowText(_T("")/*m_data.GetValue(_T("menu_item_name"))*/);
		m_itemName2.SetWindowText(strTmp);
		break;
	case E_NAME1ANDNAME2:
		m_itemName1.SetWindowText(tmp);
		m_itemName2.SetWindowText(strTmp);
		break;
	}
	
	tmp.Empty();
	strTmp.Empty();

	((CKDSManagerDlg*)AfxGetMainWnd())->GetCondimentPriced(_ttoi(m_data.GetValue(_T("order_detail_id"))),strTmp);
	tmp.Format(_T("%s %s %s"), strTmp, m_data.GetValue(_T("description")), m_data.GetValue(_T("remark")));
	m_description.SetWindowText(tmp);

	g_lang.LoadString(tmp, g_strEatTypeID[_ttoi(m_data.GetValue(_T("eat_type")))]);
	m_eat_type.SetWindowText(tmp);

	g_lang.LoadString(tmpFormat, IDS_STR_TABLENAME);
	//tmpFormat.LoadString(IDS_STR_TABLENAME);
	tmp.Format(tmpFormat, m_data.GetValue(_T("table_name")),m_data.GetValue(_T("order_head_id")));
	m_tableName.SetWindowText(tmp);

	//tmpFormat.LoadString(IDS_STR_QUANTITY_UNIT);
	g_lang.LoadString(tmpFormat, IDS_STR_QUANTITY_UNIT);
	INT iCombine = (INT)_ttoi(m_data.GetValue(_T("CombineNum")));
	if (iCombine > 1)
	{
		tmp.Format(tmpFormat, m_data.GetValue(_T("CombineNum")),m_data.GetValue(_T("unit")));
	}
	else
	{
		tmp.Format(tmpFormat, m_data.GetValue(_T("quantity")),m_data.GetValue(_T("unit")));
	}
	//tmp.Format(tmpFormat, m_data.GetValue(_T("quantity")),m_data.GetValue(_T("unit")));
	m_quantity.SetWindowText(tmp);

	//tmpFormat.LoadString(IDS_STR_TIMEDIFF);
	g_lang.LoadString(tmpFormat, IDS_STR_TIMEDIFF);
	strTmp = m_data.GetValue(_T("kds_time"));
	tmp = m_data.GetValue(_T("order_time"));
	KDS_Mode mode = ((CKDSManagerDlg*)AfxGetMainWnd())->GetKitchenMode();
	if (E_M_SEND == mode || strTmp.GetLength() == 0)
	{
		tmp.Format(tmpFormat,m_data.GetValue(_T("time_diff")));
	}
	else
	{
		COleVariant vtime1(strTmp);   
		vtime1.ChangeType(VT_DATE);   
		COleDateTime oletime1 = vtime1;

		COleVariant vtime2(tmp);   
		vtime2.ChangeType(VT_DATE);   
		COleDateTime oletime2 = vtime2;

		COleDateTimeSpan dTimeSpan = oletime1 - oletime2; 
		int nMinSpan = dTimeSpan.GetTotalSeconds() / 60;
		strTmp.Format(_T("%d"),nMinSpan);

		tmp.Format(tmpFormat,strTmp);
	}
	m_waitTime.SetWindowText(tmp);

	//UpdateData(FALSE);
	//Invalidate(TRUE);
}

void CDlgItemDetailNew2::ClearData()
{
	m_data.ClearData();
	//UpdateData(FALSE);
}

CBaseData* CDlgItemDetailNew2::GetDataPtr()
{
	return (CBaseData*)(&m_data);
}

void CDlgItemDetailNew2::OnStnClickedStaticItemName1()
{
	// TODO: 在此添加控件通知处理程序代码
	if (!m_bItemNameCanClick)
	{
		return;
	}

	if (m_bShowSelectPicture)
	{
		if (m_bShowSelectPicture)
		{
			if (m_bIsCombine)
			{
				m_bIsCombine = FALSE;
			} 
			else
			{
				m_bIsCombine = TRUE;
			}
			Invalidate(TRUE);
			Sleep(100);
		}
		return;
	}

	CDlgOrderFunction dlg;
	dlg.SetShowType(showType);
	dlg.ClearData();
	dlg.m_data = m_data;
	dlg.DoModal();
}

void CDlgItemDetailNew2::ResetLang()
{
	Invalidate();
}

void CDlgItemDetailNew2::SetCombine(BOOL bFlag)
{
	m_bIsCombine = bFlag;
}

BOOL CDlgItemDetailNew2::GetCombineFlag()
{
	return m_bIsCombine;
}

LRESULT CDlgItemDetailNew2::OnNcHitTest(CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (::GetKeyState(VK_LBUTTON)&0x80000000)
	{
		OnStnClickedStaticItemName1();
	}
	return CDialog::OnNcHitTest(point);
}

void CDlgItemDetailNew2::refreshTipPicture(INT ctrlId,BOOL bBlag,CString pngNameFlagTure,CString pngNameFlagFalse)
{
	CClientDC *pDC = new CClientDC(GetDlgItem(ctrlId));
	Gdiplus::Graphics graphics(pDC->m_hDC); // Create a GDI+ graphics object
	CRect rect;
	GetDlgItem(ctrlId)->GetWindowRect(&rect);
	if (bBlag)
	{
		Gdiplus::Image image(pngNameFlagTure); // Construct an image
		
		graphics.DrawImage(&image, 0, 0, rect.Width(), rect.Height());
		if (ctrlId == IDC_STATIC_RUSHPICTRUE || ctrlId == IDC_STATIC_WAITPICTURE || ctrlId == IDC_STATIC_OUTTIME)
		{
			CString tmp;

			// 字体
			//CString tmpFont;
			//tmpFont.LoadString(IDS_STR_FONT);
			//Gdiplus::FontFamily fontfamily(tmpFont);
			//Gdiplus::Font font(&fontfamily,18,Gdiplus::FontStyleRegular,Gdiplus::UnitPixel);  
			
			LOGFONT lFont;
			GetFont()->GetLogFont(&lFont);
			HDC hHDC = ::GetDC(NULL);  
			lFont.lfHeight = rect.Height() * 2 / 3;
			lFont.lfWidth = 0;
			Gdiplus::Font font( hHDC, &lFont );

			// 字体颜色
			Gdiplus::SolidBrush brush(Gdiplus::Color(255,255,255,255));  

			// 字体输出位置
			//Gdiplus::PointF  pointf(4,4); 

			// 对齐方式
			Gdiplus::StringFormat stringFormat;
			stringFormat.SetAlignment(Gdiplus::StringAlignmentCenter);
			stringFormat.SetLineAlignment(Gdiplus::StringAlignmentCenter);

			// 字体输出矩形
			Gdiplus::RectF rectf = Gdiplus::RectF(0,0,rect.Width(),rect.Height());

			if (ctrlId == IDC_STATIC_RUSHPICTRUE)
			{
				g_lang.LoadString(tmp,IDS_STR_RUSH);
			} 
			else if (ctrlId == IDC_STATIC_WAITPICTURE)
			{
				if (E_S_DONE == showType)
				{
					g_lang.LoadString(tmp,IDS_STR_COOKTIME);
				}
				else
				{
					g_lang.LoadString(tmp,IDS_STR_WAIT);
				}
				
			}
			else if (ctrlId == IDC_STATIC_OUTTIME)
			{
				if (E_S_DONE == showType)
				{
					CString is_make = m_data.GetValue(_T("is_make"));
					if (is_make.Compare(_T("5")) == 0)
					{
						g_lang.LoadString(tmp,IDS_STR_INVALID);
					}
				}
			}

			//graphics.DrawString(tmp,-1,&font,pointf,&brush);
			graphics.DrawString( tmp, -1, &font, rectf, &stringFormat, &brush);

			::ReleaseDC( NULL, hHDC );
		}
	}
	else
	{
		Gdiplus::Image image(pngNameFlagFalse); // Construct an image
		graphics.DrawImage(&image, 0, 0, rect.Width(), rect.Height());
	}
	delete pDC;
}

void CDlgItemDetailNew2::SelectItemPage(BOOL bFlag)
{
	m_bSelectIn = bFlag;
	Invalidate(TRUE);
}


BOOL CDlgItemDetailNew2::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
	if (pMsg->wParam == VK_F1)
	{
		if (m_bShowSelectPicture)
		{
			if (m_bIsCombine)
			{
				m_bIsCombine = FALSE;
			} 
			else
			{
				m_bIsCombine = TRUE;
			}
			Invalidate(TRUE);
			//不知道为什么如果不加一点延时，选中标志的显示就会有问题
			//AfxMessageBox(_T("AA"));
			Sleep(100);
			return TRUE;
		}
	}
	return CDialog::PreTranslateMessage(pMsg);
}
