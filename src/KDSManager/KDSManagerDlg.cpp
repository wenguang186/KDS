// KDSManagerDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "atlimage.h"
#include <gdiplus.h>

#include "KDSManager.h"
#include "KDSManagerDlg.h"

#include "DlgSetting.h"
#include "DlgItemOrder.h"
#include "DlgItemDetail.h"
#include "DlgItemDetailNew2.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//#ifdef _DEBUG
//#pragma comment(lib, "LicenseLibD.lib")
//#else
//#pragma comment(lib, "LicenseLib.lib")
//#endif
//
//#pragma comment(lib, "libeay32.lib")

#include "SoftwareKey.h"

#define ORDERNUMBER_SHOW 11 // 叫号模块显示的订单数

// CKDSManagerDlg 对话框

CKDSManagerDlg::CKDSManagerDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CKDSManagerDlg::IDD, pParent)
	, m_avgTime(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	// 背景图片
	CImage img;
	if(img.Load(_T(".\\res\\bg_image.jpg"))==S_OK)
	{
		m_x=img.GetWidth();
		m_y=img.GetHeight();
		m_bpBackgrd=img.Detach();
	}

	//if(m_bpImageAvgTime.Load(_T(".\\res\\avgtime_bgimgage.png"))==S_OK)
	//{
	//	m_xAvgTime = m_bpImageAvgTime.GetWidth();
	//	m_yAvgTime = m_bpImageAvgTime.GetHeight();
	//	//m_bpAvgTime = m_bpImageAvgTime.Detach();		
	//}	

	// 按钮背景
	m_bpBtnReturn[0]=Gdiplus::Image::FromFile(_T(".\\res\\menu_return.png"));
	m_bpBtnReturn[1]=Gdiplus::Image::FromFile(_T(".\\res\\menu_pushdown.png"));

	m_bpBtnPendList[0]=Gdiplus::Image::FromFile(_T(".\\res\\menu_btn.png"));
	m_bpBtnPendList[1]=Gdiplus::Image::FromFile(_T(".\\res\\menu_pushdown.png"));

	m_bpBtnHistoryList[0]=Gdiplus::Image::FromFile(_T(".\\res\\menu_btn.png"));
	m_bpBtnHistoryList[1]=Gdiplus::Image::FromFile(_T(".\\res\\menu_pushdown.png"));

	m_bpBtnFirst[0]=Gdiplus::Image::FromFile(_T(".\\res\\menu_btn.png"));
	m_bpBtnFirst[1]=Gdiplus::Image::FromFile(_T(".\\res\\menu_pushdown.png"));

	m_bpBtnPre[0]=Gdiplus::Image::FromFile(_T(".\\res\\menu_btn.png"));
	m_bpBtnPre[1]=Gdiplus::Image::FromFile(_T(".\\res\\menu_pushdown.png"));

	m_bpBtnNext[0]=Gdiplus::Image::FromFile(_T(".\\res\\menu_btn.png"));
	m_bpBtnNext[1]=Gdiplus::Image::FromFile(_T(".\\res\\menu_pushdown.png"));

	m_bpBtnEnd[0]=Gdiplus::Image::FromFile(_T(".\\res\\menu_btn.png"));
	m_bpBtnEnd[1]=Gdiplus::Image::FromFile(_T(".\\res\\menu_pushdown.png"));

	m_bpBtnSetting[0]=Gdiplus::Image::FromFile(_T(".\\res\\menu_btn.png"));
	m_bpBtnSetting[1]=Gdiplus::Image::FromFile(_T(".\\res\\menu_pushdown.png"));

	m_bpBtnServerIP[0]=Gdiplus::Image::FromFile(_T(".\\res\\menu_btn.png"));
	m_bpBtnServerIP[1]=Gdiplus::Image::FromFile(_T(".\\res\\menu_pushdown.png"));

	m_bpBtnClose[0]=Gdiplus::Image::FromFile(_T(".\\res\\menu_close.png"));
	m_bpBtnClose[1]=Gdiplus::Image::FromFile(_T(".\\res\\menu_pushdown.png"));

	if(m_bpBtnPendList[0] == NULL || m_bpBtnPendList[0]->GetLastStatus() != Gdiplus::Ok)
	{
		AfxMessageBox(_T("Failed on loading pirctures, please check whether the pictures are damaged."));
	}

	//m_nWidth = 1024;
	//m_nHeight = 768;
	m_nWidth = GetSystemMetrics(SM_CXSCREEN);   //获取屏幕水平分辨率;   SM_CXFULLSCREEN
	m_nHeight = GetSystemMetrics(SM_CYSCREEN);     //获取屏幕垂直分辨率;SM_CYFULLSCREEN

	m_doneCnt = 0;
	m_currentPage = 0;
	m_preDetailId = 0;
}

CKDSManagerDlg::~CKDSManagerDlg()
{
	for (UINT i=0; i<m_pages.size(); i++)
	{
		delete m_pages[i];
	}
	m_pages.clear();
}

void CKDSManagerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_STATIC_AVGTIME, m_avgTime);
	DDX_Control(pDX, IDC_BTN_PENDING, m_btnPendList);
	DDX_Control(pDX, IDC_BTN_HISTORY, m_btnHistoryList);
	DDX_Control(pDX, IDC_BTN_FIRST, m_btnFirst);
	DDX_Control(pDX, IDC_BTN_PRE, m_btnPre);
	DDX_Control(pDX, IDC_BTN_NEXT, m_btnNext);
	DDX_Control(pDX, IDC_BTN_END, m_btnEnd);
	DDX_Control(pDX, IDC_BTN_SETTING, m_btnSetting);
	DDX_Control(pDX, IDC_BTN_CLOSE, m_btnClose);
	DDX_Control(pDX, IDC_STATIC_AVGTIME, m_ctrlAvgTime);
	DDX_Control(pDX, IDC_BTN_SERVERIP, m_btnServerIP);
	DDX_Control(pDX, IDC_BTN_COOKING, m_btnCookList);
}

BEGIN_MESSAGE_MAP(CKDSManagerDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BTN_PENDING, &CKDSManagerDlg::OnBnClickedBtnPending)
	ON_BN_CLICKED(IDC_BTN_HISTORY, &CKDSManagerDlg::OnBnClickedBtnHistory)
	ON_BN_CLICKED(IDC_BTN_FIRST, &CKDSManagerDlg::OnBnClickedBtnFirst)
	ON_BN_CLICKED(IDC_BTN_PRE, &CKDSManagerDlg::OnBnClickedBtnPre)
	ON_BN_CLICKED(IDC_BTN_NEXT, &CKDSManagerDlg::OnBnClickedBtnNext)
	ON_BN_CLICKED(IDC_BTN_END, &CKDSManagerDlg::OnBnClickedBtnEnd)
	ON_BN_CLICKED(IDC_BTN_SETTING, &CKDSManagerDlg::OnBnClickedBtnSetting)
	ON_BN_CLICKED(IDC_BTN_CLOSE, &CKDSManagerDlg::OnBnClickedBtnClose)

	ON_MESSAGE(WM_MSG_REFRESH, OnMsgRefresh)
	ON_MESSAGE(WM_MSG_DONE, OnMsgDone)
	ON_MESSAGE(WM_MSG_PEND, OnMsgPend)
	ON_MESSAGE(WM_MSG_ORDER, OnMsgOrder)
	ON_MESSAGE(WM_MSG_RECOVERY, OnMsgRecovery)
	ON_MESSAGE(WM_MSG_DONOTMAKE, OnMsgDoNotMake)
	ON_MESSAGE(WM_MSG_ADDCOMBINEITEM, OnMsgAddCombineItem)

	ON_WM_TIMER()

	ON_WM_ERASEBKGND()
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_BTN_SERVERIP, &CKDSManagerDlg::OnBnClickedBtnServerip)
	ON_BN_CLICKED(IDC_BTN_COOKING, &CKDSManagerDlg::OnBnClickedBtnCooking)
END_MESSAGE_MAP()


// CKDSManagerDlg 消息处理程序

BOOL CKDSManagerDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码

	// 全屏显示
	
	MoveWindow(0,0,m_nWidth, m_nHeight);

	// 测试数据库
		
	// 读取配置文件
	
	//AfxMessageBox(g_lang.m_strLang);
	//CString tmp;
	//g_lang.LoadString(tmp,IDS_STR_CLOSE);
	//AfxMessageBox(tmp);
	m_curStyle = (KDS_STYLE)GetPrivateProfileInt(_T("basic"), _T("KDS_STYLE"), 0, _T(".\\config.ini"));
	m_kitchenMode = (KDS_Mode)GetPrivateProfileInt(_T("basic"), _T("KDS_MODE"), 0, _T(".\\config.ini"));
	m_timerElapse = GetPrivateProfileInt(_T("basic"),  _T("KDS_TIMER"), 5, _T(".\\config.ini"));
	GetPrivateProfileString(_T("kitchen"), _T("kitchen"), _T("1,2,3"), m_strKitchenStalls.GetBuffer(256), 256, _T(".\\config.ini"));
	m_strKitchenStalls.ReleaseBuffer();
	m_nFoodSortType = (E_SORT_TYPE)GetPrivateProfileInt(_T("ITEMDETAIL"), _T("FoodPriority"), 0, _T(".\\config.ini"));
	m_nNameShow = (E_ItemName_ShowType)GetPrivateProfileInt(_T("ITEMDETAIL"), _T("NameShow"), 1, _T(".\\config.ini"));
	m_bShowCheckId = (BOOL)GetPrivateProfileInt(_T("CALL"), _T("CheckIdShow"), 0, _T(".\\config.ini"));

	// 菜品项布局 itemDlg自定义宽度，根据屏幕定义高度
	ResetPages();	

	// 底部btn布局，第二列为stringtable中的id
	const INT rows = 8, cols = 2;
	INT btnList[rows][cols] = 
	{
		{IDC_BTN_COOKING, IDS_STR_COOKLIST},
		/*{IDC_STATIC_PAGENUM, IDS_STR_PAGENUM},*/
		{IDC_BTN_PENDING, IDS_STR_PENDLIST},
		{IDC_BTN_HISTORY, IDS_STR_HISTORYLIST},
		{IDC_BTN_FIRST, IDS_STR_FIRST},
		{IDC_BTN_PRE, IDS_STR_PRE},
		{IDC_BTN_NEXT, IDS_STR_NEXT},
		/*{IDC_BTN_END, IDS_STR_END},*/
		{IDC_BTN_SETTING, IDS_STR_SETTING},
		/*{IDC_BTN_SERVERIP, IDS_STR_SERVERIP},*/
		{IDC_BTN_CLOSE, IDS_STR_CLOSE},
		//{IDC_STATIC_AVGTIME, IDS_STR_AVGTIME},
	};

	INT btn_width = m_nWidth/(rows+2);
	INT btn_height = m_nHeight/15;
	INT btn_span = btn_width/10;
	INT left_pos = (m_nWidth-btn_width*rows-40)/2;
	INT top_pos= m_nHeight-btn_height-10;

	for(INT i=0; i<rows; i++)
	{
		CWnd* pCtrl = GetDlgItem(btnList[i][0]);
		pCtrl->MoveWindow(left_pos,top_pos,btn_width,btn_height);
		left_pos += btn_width + btn_span;

		CString str;
		//str.LoadString(btnList[i][1]);
		g_lang.LoadString(str, btnList[i][1]);
		pCtrl->SetWindowText(str);
	}

	//m_btnCooking.SetWindowText(_T(""));
	m_btnCookList.SetImages(m_bpBtnPendList[0], m_bpBtnPendList[1], false);
	m_btnCookList.SetTextColor(RGB(255, 255, 255));

	//m_btnPendList.SetWindowText(_T(""));
	m_btnPendList.SetImages(m_bpBtnPendList[0], m_bpBtnPendList[1], false);
	m_btnPendList.SetTextColor(RGB(255, 255, 255));
	//m_btnHistoryList.SetWindowText(_T(""));
	m_btnHistoryList.SetImages(m_bpBtnHistoryList[0], m_bpBtnHistoryList[1], false);
	m_btnHistoryList.SetTextColor(RGB(255, 255, 255));
	//m_btnFirst.SetWindowText(_T(""));
	m_btnFirst.SetImages(m_bpBtnFirst[0], m_bpBtnFirst[1], false);
	m_btnFirst.SetTextColor(RGB(255, 255, 255));
	//m_btnPre.SetWindowText(_T(""));
	m_btnPre.SetImages(m_bpBtnPre[0], m_bpBtnPre[1], false);
	m_btnPre.SetTextColor(RGB(255, 255, 255));
	//m_btnNext.SetWindowText(_T(""));
	m_btnNext.SetImages(m_bpBtnNext[0], m_bpBtnNext[1], false);
	m_btnNext.SetTextColor(RGB(255, 255, 255));
	//m_btnEnd.SetWindowText(_T(""));
	m_btnEnd.SetImages(m_bpBtnEnd[0], m_bpBtnEnd[1], false);
	m_btnEnd.SetTextColor(RGB(255, 255, 255));
	//m_btnSetting.SetWindowText(_T(""));
	m_btnSetting.SetImages(m_bpBtnSetting[0], m_bpBtnSetting[1], false);
	m_btnSetting.SetTextColor(RGB(255, 255, 255));

	m_btnServerIP.SetImages(m_bpBtnSetting[0], m_bpBtnSetting[1], false);
	m_btnServerIP.SetTextColor(RGB(255, 255, 255));

	//m_btnClose.SetWindowText(_T(""));
	m_btnClose.SetImages(m_bpBtnClose[0], m_bpBtnClose[1], false);
	m_btnClose.SetTextColor(RGB(255, 255, 255));

	LOGFONT lFont;
	GetFont()->GetLogFont(&lFont);
	lFont.lfHeight = 24;
	lFont.lfWidth = 0;
	m_btnCookList.SetFont(&lFont);
	m_btnPendList.SetFont(&lFont);
	m_btnHistoryList.SetFont(&lFont);
	m_btnFirst.SetFont(&lFont);
	m_btnPre.SetFont(&lFont);
	m_btnNext.SetFont(&lFont);
	m_btnEnd.SetFont(&lFont);
	m_btnSetting.SetFont(&lFont);
	m_btnClose.SetFont(&lFont);

	m_btnServerIP.SetFont(&lFont);

	//RECT rect;
	//m_ctrlAvgTime.GetWindowRect(&rect);
	//m_ctrlAvgTime.MoveWindow(rect.left-20,rect.top-30, rect.right-rect.left+50,rect.bottom-rect.top+30);
	//m_ctrlAvgTime.ModifyStyle(0, WS_CHILD|WS_VISIBLE|SS_BITMAP|SS_CENTERIMAGE);
	//m_ctrlAvgTime.SetBitmap(m_bpAvgTime);

	// 设置叫号模块字体
	/*
	BOOL CreateFont(
	int nHeight, //字体的高度,逻辑单位
	int nWidth, //字体的宽度
	int nEscapement, //字体显示的角度
	int nOrientation, //字体的角度
	int nWeight, //字体的磅数
	BYTE bItalic, //斜体字体
	BYTE bUnderline, //带下划线的字体
	BYTE cStrikeOut, //带删除线的字体
	BYTE nCharSet, //所需的字符集
	BYTE nOutPrecision, //输出的精度
	BYTE nClipPrecision, //裁减的精度
	BYTE nQuality, //逻辑字体与输出设备的实际
	//字体之间的精度
	BYTE nPitchAndFamily, //字体间距和字体集
	LPCTSTR lpszFacename //字体名称
	);
	*/

	//font.CreatePointFont(224,_T("宋体"));
	m_nDrawHight = m_nHeight*14/15;
	m_fontName.LoadString(IDS_STR_FONT);
	m_fontOrder.CreateFont(m_nDrawHight/ORDERNUMBER_SHOW,0,0,0,FW_MEDIUM,FALSE,FALSE,0,ANSI_CHARSET,OUT_DEFAULT_PRECIS,
		CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,DEFAULT_PITCH | FF_SWISS,m_fontName);
	
	m_fontTip.CreateFont(m_nDrawHight/20,0,0,0,FW_MEDIUM,FALSE,FALSE,0,ANSI_CHARSET,OUT_DEFAULT_PRECIS,
		CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,DEFAULT_PITCH | FF_SWISS,m_fontName);

	// 数据库检测
	if (!m_dbmngr.ConnectTest())
	{
		//LOG4CPLUS_INFO(log_pos,"OpenDatabase Failed");
		//ShellExecute(NULL, NULL, _T("ServerIpSetting.exe"), NULL, NULL,SW_NORMAL);
		//OnOK();
		//return TRUE;
		//ShellExecute(NULL, NULL, _T("ServerIpSetting.exe"), NULL, NULL,SW_NORMAL);
		POSMessageBox(IDS_STR_LOSTSERVER);
		return TRUE;
	}

	m_dbmngr.Init();

	// 定时刷新
	m_timeIdCall = 0;
	m_timerId = 0;
	ResetTimer();

	// 设置厨显模式	
	m_dbmngr.ResetKitchenType(m_strKitchenStalls);
	m_dbmngr.ResetKitchenMode(m_kitchenMode);
	m_dbmngr.ResetSortType(m_nFoodSortType);

	// 显示平均菜品制作时间，要执行sql，所以在m_dbmngr设置完成之后
	ResetAvgTime();

	//ResetPageState();
	ResetShowType(E_S_DONING);
	//UpdateItems();

	if (E_M_CNT == m_kitchenMode)
	{
		UpdateItems();
	}

	RefreshPendMenuText();
	RefreshHistoryMenuText();
	RefreshCookMenuText();

// 	if (E_M_CNT == m_kitchenMode)
// 	{
// 		// 叫号模式默认不打印
// 		m_printerId = 0;
// 	}
	

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CKDSManagerDlg::ResetLang()
{
	const INT rows=8, cols=2;
	INT btnText[rows][cols] =
	{
		{IDC_BTN_COOKING, IDS_STR_COOKLIST},
		{IDC_BTN_PENDING, IDS_STR_PENDLIST},
		{IDC_BTN_HISTORY, IDS_STR_HISTORYLIST},
		{IDC_BTN_FIRST, IDS_STR_FIRST},
		{IDC_BTN_PRE, IDS_STR_PRE},
		{IDC_BTN_NEXT, IDS_STR_NEXT},
		/*{IDC_BTN_END, IDS_STR_END},*/
		{IDC_BTN_SETTING, IDS_STR_SETTING},
		{IDC_BTN_CLOSE, IDS_STR_CLOSE}
	};

	CString str;
	for(INT i=0; i<rows; i++)
	{
		CRoundButton2* pCtrl = (CRoundButton2*)GetDlgItem(btnText[i][0]);		
		g_lang.LoadString(str, btnText[i][1]);
		pCtrl->SetWindowText(str);
		pCtrl->SetInvalid();
	}
}

void CKDSManagerDlg::ResetAvgTime()
{
	// 暂时屏蔽时间计算
	GetDlgItem(IDC_STATIC_AVGTIME)->ShowWindow(SW_HIDE);
	return;

	CString tmp, tmp2;
	tmp.LoadString(IDS_STR_AVGTIME);
	tmp2.Format(tmp, m_dbmngr.GetAvgCookTime());
	//m_avgTime = tmp2; // 要加上UpdateData(FALSE); 才能能正确显示
	GetDlgItem(IDC_STATIC_AVGTIME)->SetWindowText(tmp2);
}

/************************************************************************
* 函数介绍：按时刷新时间显示
* 输入参数：
* 输出参数：
* 返回值  ：
************************************************************************/
void CKDSManagerDlg::OnTimer(UINT_PTR nIDEvent)
{
	//AfxMessageBox(_T("定时器执行"));
	if (nIDEvent == m_timerId)
	{
		UpdateItems();
	}
	if (nIDEvent == m_timeIdCall)
	{
		if (E_M_CNT == m_kitchenMode)
		{
			m_timeTick++;
			if(m_timeTick > 10)
			{
				m_btnSetting.ShowWindow(FALSE);
				m_btnClose.ShowWindow(FALSE);
				KillTimer(nIDEvent);
			}
		}
	}
	CDialog::OnTimer(nIDEvent);
}

inline INT CKDSManagerDlg::GetItemCnt(KDS_ShowType type)
{
	switch(m_curStyle)
	{
	case E_STYLE_DETAIL:
		return m_dbmngr.GetItemCnt(type);

	case E_STYLE_ORDER:
		return m_dbmngr.GetItemOrderCnt(type);;

	default:
		AfxMessageBox(_T("Unknown KDS Style in GetItemCnt"));
		return 0;
	}

}

INT CKDSManagerDlg::GetItemCnt()
{
	return GetItemCnt(m_showType);	
}

void CKDSManagerDlg::ResetPages()
{
	INT nID;
	CRect rect;
	INT pageWidth, pageWidthSpan, pageHeightSpan, pageHeight, left;
	INT top;

	m_pageNum = m_pageRows = m_pageCols = 0;

	// 如果有数据，清空之
	for (UINT i=0; i<m_pages.size(); i++)
	{
		delete m_pages[i];
	}
	m_pages.clear();
	m_pPageData.clear();

	if (E_M_CNT == m_kitchenMode)
	{
		m_btnCookList.ShowWindow(FALSE);
		m_btnPendList.ShowWindow(FALSE);
		m_btnHistoryList.ShowWindow(FALSE);
		m_btnFirst.ShowWindow(FALSE);
		m_btnPre.ShowWindow(FALSE);
		m_btnNext.ShowWindow(FALSE);
		m_btnSetting.ShowWindow(FALSE);
		m_btnClose.ShowWindow(FALSE);
		return;
	}
	else
	{
		m_btnCookList.ShowWindow(TRUE);
		m_btnPendList.ShowWindow(TRUE);
		m_btnHistoryList.ShowWindow(TRUE);
		m_btnFirst.ShowWindow(TRUE);
		m_btnPre.ShowWindow(TRUE);
		m_btnNext.ShowWindow(TRUE);
		m_btnSetting.ShowWindow(TRUE);
		m_btnClose.ShowWindow(TRUE);
	}

	// 初始化item
	for (INT i=0;; i++)
	{
		CDialog *pPage = NULL;
		switch(m_curStyle)
		{
		case E_STYLE_DETAIL:

#ifdef USE_OLD_DETAIL_PAGE
			pPage = new CDlgItemDetail;
			m_pPageData.push_back(((CDlgItemDetail*)pPage)->GetDataPtr());
			nID = IDD_DLG_ITEM_DETAIL;
#else
			pPage = new CDlgItemDetailNew2;
			m_pPageData.push_back(((CDlgItemDetailNew2*)pPage)->GetDataPtr());
			nID = IDD_DLG_ITEM_DETAILNEW2;
#endif
			break;

		case E_STYLE_ORDER:
			pPage = new CDlgItemOrder;
			m_pPageData.push_back(((CDlgItemOrder*)pPage)->GetDataPtr());
			nID = IDD_DLG_ITEM_ORDER;
			break;

		default:
			AfxMessageBox(_T("Error creating Dialog Item."));
			OnCancel();
			return;
		}		

		if (pPage == NULL
			|| !pPage->Create(nID, this))
		{
			AfxMessageBox(_T("Error on creating item-pages."));
			return;
		}

		if (m_pageNum == 0)
		{
			CRect btnRect;
			m_btnCookList.GetWindowRect(&btnRect);
			pPage->GetWindowRect(&rect);
			pageWidth = rect.Width();
			m_pageNum = m_nWidth/pageWidth;
			m_pageRows = m_nHeight / (rect.Height() + m_nHeight/15 + 10);
			m_pageCols = m_nWidth / pageWidth;
			pageWidthSpan = (m_nWidth - m_pageNum*pageWidth) / (m_pageNum+1);
			
			if (m_pageRows == 0)
			{
				m_pageRows = 1;
				pageHeight = m_nHeight*13/15; // 订单item约占全屏13/15，只有一行, btn约占1/15
				top = pageHeightSpan = 20;
			}
			else
			{
				pageHeight = rect.Height();// 单品item约占全屏13/15, btn约占1/15
				pageHeightSpan = (m_nHeight - m_pageRows*pageHeight - btnRect.Height() - m_nHeight/15) / (m_pageRows+1);
				top = pageHeightSpan;
			}
			
		}

		left = i*(pageWidth+pageWidthSpan) + pageWidthSpan;

		INT n = m_pages.size() / m_pageNum;
		if (m_pages.size() >= m_pageNum)
		{
			pPage->GetWindowRect(&rect);
			top = rect.Height() * n + pageHeightSpan * (n+1);
			left = (i - m_pageNum * n)*(pageWidth+pageWidthSpan) + pageWidthSpan;
		}

		pPage->MoveWindow(left, top, pageWidth, pageHeight);
		//pPage->ShowWindow(SW_SHOW);
		//m_items.push_back(pPage); // style-refactor之后，可以作废了
		m_pages.push_back(pPage);
		//不知道为什么取到的是同一个地址
		//m_pPageData.push_back(((CPageInterface*)pPage)->GetDataPtr());
		//m_pPageData.push_back(((CDlgItemOrder*)pPage)->GetDataPtr());

		if (m_pages.size() >= m_pageNum * m_pageRows)
			break;
	}
}

#if 0
INT CKDSManagerDlg::LoadOrderHead(CRecordset &rs)
{
	INT cnt = 0;
	for (; !rs.IsEOF(); cnt++)
	{
		((CDlgItemOrder*)m_items[cnt])->LoadHead(rs);
		rs.MoveNext();
	}

	return cnt;
}


void CKDSManagerDlg::LoadOrderDetail(long order_head_id, CDlgItemOrder* pDlg)
{
	if (m_dbmngr.GetOrderDetail(order_head_id, _T("order_detail"), pDlg) == 0)
	{
		m_dbmngr.GetOrderDetail(order_head_id, _T("history_order_detail"), pDlg);
	}
	
}
#endif

//void CKDSManagerDlg::UpdateItems()
//{
//	switch(m_curStyle)
//	{
//	case E_DETAIL:
//		UpdateItemDetail();
//		break;
//
//	case E_ORDER:
//		UpdateItemOrder();
//		break;
//	}
//}
//
//void CKDSManagerDlg::UpdateItemOrder()
//{
//
//}

INT CKDSManagerDlg::LoadDataOrder(INT begin, INT num)
{
	vector<CDataOrderHead*> heads;
	for (INT i=0; i < num; i++)
	{
		heads.push_back(&(((CDataOrder*)m_pPageData[i])->m_head));
	}
	INT cnt = m_dbmngr.GetItemOrderHead(m_showType, begin, num, heads);

	for (INT i=0; i<cnt; i++)
	{	
		long order_head_id = _ttoi(heads[i]->m_data[_T("order_head_id")]);
		long check_id = _ttoi(heads[i]->m_data[_T("check_id")]);
		INT tbl = _ttoi(heads[i]->m_data[_T("tableType")]);
		switch(tbl)
		{
		case E_TMP_ORDER:
			m_dbmngr.GetItemOrderDetail(order_head_id, _T("order_detail"), (CDataOrder*)m_pPageData[i], check_id);
			break;

		case E_HISTORY_ORDER:
			m_dbmngr.GetItemOrderDetail(order_head_id, _T("history_order_detail"), (CDataOrder*)m_pPageData[i], check_id);
			break;

		default:
			AfxMessageBox(_T("Unknown order table type"));
		}		
	}

	return cnt;
}

void CKDSManagerDlg::UpdateItems()
{
	//DWORD dwStart = GetTickCount();//精确度大概20ms

	/*
	LARGE_INTEGER nFreq;
	LARGE_INTEGER nBeginTime;
	LARGE_INTEGER nEndTime;
	double time;
	// 作用：返回硬件支持的高精度计数器的频率。
	// 返回值：非零，硬件支持高精度计数器；零，硬件不支持，读取失败。
	QueryPerformanceFrequency(&nFreq);
	QueryPerformanceCounter(&nBeginTime); 
	*/

	INT lastOrderId;

	INT pageSize = m_pages.size();
	INT fetchNum = 0;
	
	if (E_M_CNT == m_kitchenMode)
	{
		for (INT i = 0;i < pageSize;i++)
		{
			m_pages[i]->ShowWindow(SW_HIDE);
		}
		m_pData.clear();
		m_dbmngr.GetItemOrderCheck(m_pData);
		Invalidate(TRUE);
		return;
	}

	// 以下代码导致屏幕闪烁太厉害，只有在有必要时进行隐藏
	//for (INT i=0; i<pageSize; i++)
	//{
	//	// 放到OnShowWindow()，隐藏时自动清空数据
	//	m_pages[i]->ShowWindow(SW_HIDE);
	//}

	// 向数据库组件取<待做菜品> 从n开始取m个
	INT begin = m_curPageIndex * pageSize;
	
	switch(m_curStyle)
	{
	case E_STYLE_DETAIL:
#ifdef USE_OLD_DETAIL_PAGE
		for (INT i=0; i<pageSize; i++)
		{
			((CDlgItemDetail*)m_pages[i])->ClearData();
		}
#else
		for (INT i=0; i<pageSize; i++)
		{
			((CDlgItemDetailNew2*)m_pages[i])->ClearData();
		}
#endif
		for (INT i = 0;i < m_pData.size();i++)
		{
			delete m_pData[i];
		}
		m_pData.clear();


#ifdef USE_OLD_DETAIL_PAGE
		fetchNum = m_dbmngr.GetItemDetail(m_showType, begin, pageSize, m_pPageData);
		for (INT i=0; i<fetchNum && i<pageSize; i++)
		{
			((CDlgItemDetail*)m_pages[i])->RefreshData();
			((CDlgItemDetail*)m_pages[i])->ResetShowState(m_showType);
			((CDlgItemDetail*)m_pages[i])->Invalidate();
		}
#else
		fetchNum = m_dbmngr.GetAllItemDetail(m_showType, m_pData);
		fetchNum = DeleteCombineItem();//返回移除合并菜品后的菜品数量
		
		for (INT i=0; i+begin<fetchNum && i<pageSize; i++)
		{
			((CDlgItemDetailNew2*)m_pages[i])->m_data = *((CDataOrderDetail*)m_pData[i+begin]);
			((CDlgItemDetailNew2*)m_pages[i])->RefreshData();
			((CDlgItemDetailNew2*)m_pages[i])->ResetShowState(m_showType);
			((CDlgItemDetailNew2*)m_pages[i])->Invalidate();
		}
#endif
		break;

	case E_STYLE_ORDER:
		for (INT i=0; i<pageSize; i++)
		{
			((CDlgItemOrder*)m_pages[i])->ClearData();
			//((CDlgItemOrder*)m_pages[i])->Invalidate();
		}
		//fetchNum = m_dbmngr.GetItemOrder(m_showType, begin, pageSize, m_pPageData);
		fetchNum = LoadDataOrder(begin, pageSize);

		for (INT i=0; i<fetchNum && i<pageSize; i++)
		{
			((CDlgItemOrder*)m_pages[i])->ResetShowState(m_showType);
			((CDlgItemOrder*)m_pages[i])->RefreshData();
			((CDlgItemOrder*)m_pages[i])->Invalidate();
		}
		break;
	}
	
	

	if (E_STYLE_DETAIL == m_curStyle)
	{
#ifdef USE_OLD_DETAIL_PAGE
		// 发送取值结果到itempage要求显示更新
		for (INT i=0; i< fetchNum && i < pageSize; i++)
		{
			//m_pages[i]->ShowWindow(SW_HIDE);
			m_pages[i]->ShowWindow(SW_SHOW);
			//((CDlgItem*)m_items[i])->RefreshData(); //放到OnShowWindow()
		}

		for (INT i = fetchNum; i < pageSize; i++)
		{
			m_pages[i]->ShowWindow(SW_HIDE);
		}
		INT itemCnt = GetItemCnt();
		INT totalPages = itemCnt%pageSize? itemCnt/pageSize+1:itemCnt/pageSize;
		m_beLastPage = (m_curPageIndex+1 >= totalPages);
#else
		if (begin > fetchNum)
		{
			m_beLastPage = TRUE;
			m_curPageIndex--;
			return;
		}
		else
		{
			// 发送取值结果到itempage要求显示更新
			for (INT i=0; i< fetchNum && i < pageSize; i++)
			{
				//m_pages[i]->ShowWindow(SW_HIDE);
				m_pages[i]->ShowWindow(SW_SHOW);
				//((CDlgItem*)m_items[i])->RefreshData(); //放到OnShowWindow()
			}
			for (INT i = fetchNum - begin; i < pageSize; i++)
			{
				m_pages[i]->ShowWindow(SW_HIDE);
			}
			m_beLastPage = FALSE;
		}
		
#endif
		
	}
	else
	{
		// 发送取值结果到itempage要求显示更新
		for (INT i=0; i< fetchNum && i < pageSize; i++)
		{
			//m_pages[i]->ShowWindow(SW_HIDE);
			m_pages[i]->ShowWindow(SW_SHOW);
			//((CDlgItem*)m_items[i])->RefreshData(); //放到OnShowWindow()
		}
		for (INT i = fetchNum; i < pageSize; i++)
		{
			m_pages[i]->ShowWindow(SW_HIDE);
		}

		INT itemCnt = GetItemCnt();
		INT totalPages = itemCnt%pageSize? itemCnt/pageSize+1:itemCnt/pageSize;
		m_beLastPage = (m_curPageIndex+1 >= totalPages);
	}
	

	
	// 设置页码显示
	//CString strPageNum,tmp;
	//INT itemCnt = GetItemCnt();
	//INT totalPages = itemCnt%pageSize? itemCnt/pageSize+1:itemCnt/pageSize;
	//tmp.LoadString(IDS_STR_PAGENUM);
/* 新版不再显示页码
	g_lang.LoadString(tmp, IDS_STR_PAGENUM);
	strPageNum.Format(tmp, itemCnt, m_curPageIndex+1, totalPages);
	CWnd* pPageNum = GetDlgItem(IDC_STATIC_PAGENUM);
	//pPageNum->SetWindowText(_T("")); // 文字重叠，没用
	pPageNum->SetWindowText(strPageNum);
	//pPageNum->UpdateWindow(); //没用
	pPageNum->ShowWindow(SW_HIDE);
	pPageNum->ShowWindow(SW_SHOW);
*/

	//m_beLastPage = (m_curPageIndex+1 >= totalPages);

	// 显示排队、挂起、历史菜单数量
	RefreshCookMenuText();
	RefreshPendMenuText();
	RefreshHistoryMenuText();

#ifndef USE_OLD_DETAIL_PAGE
	if (E_STYLE_DETAIL == m_curStyle)
	{
		((CDlgItemDetailNew2*)m_pages[m_currentPage])->SelectItemPage(TRUE);
		if (begin > pageSize)
		{
			//m_ItemShowCount = pageSize - (fetchNum - begin) - 1;
			m_ItemShowCount = (fetchNum - begin) > pageSize?pageSize:(fetchNum - begin);
		}
		else
		{
			m_ItemShowCount = fetchNum >= pageSize?pageSize:fetchNum;
		}
	}
#endif
	

	
	// 只有厨房设语音
	if (E_M_COOK == m_kitchenMode/* && E_S_DONING == m_showType*/)
	{
		// 新单语音提示
		CheckHasNewItem();

		// 催菜语音提示
		//CheckItemRush();
    
		// 判断是否有退菜
		//CheckItemReturn();
	} 

	
	// 计算一次更新的时间
	//DWORD dwEnd = GetTickCount();
	//DWORD dwDiff = dwEnd - dwStart;
	//CString TMP;
	//TMP.Format(_T("%d"),dwDiff);
	//DebugMessageBox(TMP);

	/*
	CString TMP;
	QueryPerformanceCounter(&nEndTime);
	time = (double)(nEndTime.QuadPart-nBeginTime.QuadPart)/(double)nFreq.QuadPart;
	TMP.Format(_T("%f"),time);
	DebugMessageBox(TMP);
	*/
	
	// 防止出现加载图片后，刷新页码出现的数字重贴的问题
	//UpdateData();
	//Invalidate(); // 会导致闪烁
}



/************************************************************************
* Summary - 刷新显示挂起列表数量
* Return  - None
* Params  - None
*************************************************************************/
void CKDSManagerDlg::RefreshPendMenuText()
{
// 	if (m_showType == E_S_PENDED)
// 	{
// 		return;
// 	}

	INT cnt = GetItemCnt(E_S_PENDED);
	CString str;
	g_lang.LoadString(str, IDS_STR_PENDLIST);
	//str.Format(tmp, cnt);
	if (0 != cnt)
	{
		str.AppendFormat(_T("(%d)"), cnt);
	}

	m_btnPendList.SetWindowText(str);
	m_btnPendList.SetInvalid();
}

/************************************************************************
* Summary - 刷新显示排队列表数量
* Return  - None
* Params  - None
*************************************************************************/
void CKDSManagerDlg::RefreshCookMenuText()
{
	INT cnt = GetItemCnt(E_S_DONING);
	CString str;
	g_lang.LoadString(str, IDS_STR_COOKLIST);
	//str.Format(tmp, cnt);
	if (0 != cnt)
	{
		str.AppendFormat(_T("(%d)"), cnt);
	}

	m_btnCookList.SetWindowText(str);
	m_btnCookList.SetInvalid();
}

/************************************************************************
* Summary - 刷新显示历史列表数量
* Return  - None
* Params  - None
*************************************************************************/
void CKDSManagerDlg::RefreshHistoryMenuText()
{
	INT cnt = GetItemCnt(E_S_DONE);
	CString str;
	g_lang.LoadString(str, IDS_STR_HISTORYLIST);
	//str.Format(tmp, cnt);
	if (0 != cnt)
	{
		str.AppendFormat(_T("(%d)"), cnt);
	}

	m_btnHistoryList.SetWindowText(str);
	m_btnHistoryList.SetInvalid();
}


inline void CKDSManagerDlg::UpdateDBProcess(CDataOrderInfo *pData, KDS_ShowType type)
{
	if (m_curStyle == E_STYLE_DETAIL)
	{
		long order_detail_id = _ttoi(pData->GetValue(_T("order_detail_id")));
		long order_head_id = _ttoi(pData->GetValue(_T("order_head_id")));
		long check_id = _ttoi(pData->GetValue(_T("check_id")));

		//如果有加菜，设置加菜完成
		if (type == E_S_DONE)
		{
			m_dbmngr.SetCondimentPriced(type,order_detail_id);
		}
		
		m_dbmngr.SetDetailProcess(order_detail_id, type, order_head_id, check_id);
		
	} 
	else
	{
		long order_head_id = _ttoi(pData->GetValue(_T("order_head_id")));
		long check_id = _ttoi(pData->GetValue(_T("check_id")));
		m_dbmngr.SetOrderProcess(order_head_id, check_id, type);
	}	
}

//响应消息函数
LRESULT CKDSManagerDlg::OnMsgAddCombineItem(WPARAM wParam, LPARAM lParam)
{
	CombineSigleMap mapTmp = *(CombineSigleMap*)wParam;
	
	if (m_combineList.size() == 0 && mapTmp.size() > 1)
	{
		// list不存在合并项，点击确定后有多个菜品合并，直接添加
		m_combineList.push_back(mapTmp);
	} 
	else
	{
		/*
		// 遍历m_combineList，对比mapTmp查找是否有第一条目相同的detail_id，
		// 如果相同，删除掉，如果新的合并菜品数量大于一个的，重新添加。
		*/
		BOOL bFind = FALSE;
		list<CombineSigleMap>::iterator iter;
		iter = m_combineList.end();
		for (iter = m_combineList.begin();iter != m_combineList.end();)
		{
			m_combineSingle.clear();
			m_combineSingle = *iter;
			if (isSubset(mapTmp, m_combineSingle, FALSE) || isSubset(m_combineSingle, mapTmp, FALSE))
			{
				m_combineList.erase(iter);
				iter = m_combineList.begin();
				continue;
			}

			map<INT,INT>::iterator iterCombineSigle;
			for (iterCombineSigle = m_combineSingle.begin();iterCombineSigle != m_combineSingle.end();++iterCombineSigle)
			{
				map<INT,INT>::iterator iterTmp1;
				for(iterTmp1 = mapTmp.begin();iterTmp1 != mapTmp.end();++iterTmp1)
				{
					if (iterTmp1->first == iterCombineSigle->first)
					{
						iter = m_combineList.erase(iter);
						if (mapTmp.size() < 2)
						{
						}
						else
						{
							m_combineList.push_back(mapTmp);
						}
						
						bFind = TRUE;
						//break;
						continue;
					}
				}// for
			}// for
			if(!bFind)
			{
				++iter;
			}
		}// for
		if (!bFind && mapTmp.size() > 1)
		{
			m_combineList.push_back(mapTmp);
		}
	}
	
	UpdateItems();
	return 1;
}

//响应消息函数
LRESULT CKDSManagerDlg::OnMsgDoNotMake(WPARAM wParam, LPARAM lParam)
{
	//m_dbmngr.SetItemRecovery(wParam, lParam);
	UpdateDBProcess((CDataOrderInfo *)wParam, E_S_INVALID);
	UpdateItems();
	return 1;
}

//响应消息函数
LRESULT CKDSManagerDlg::OnMsgRecovery(WPARAM wParam, LPARAM lParam)
{
	//m_dbmngr.SetItemRecovery(wParam, lParam);
	UpdateDBProcess((CDataOrderInfo *)wParam, E_S_DONING);
	UpdateItems();
	return 1;
}

//响应消息函数
LRESULT CKDSManagerDlg::OnMsgDone(WPARAM wParam, LPARAM lParam)
{
	m_doneCnt ++;

	if (m_curStyle == E_STYLE_ORDER)
	{
		UpdateDBProcess((CDataOrderInfo *)wParam, E_S_DONE);
		UpdateItems();
		ResetAvgTime();
		return 1;
	}

	// 单品模式
	CDataOrderDetail data = *(CDataOrderDetail*)wParam;
	CString strSrc = data.GetValue(_T("CombineItem"));

	if (strSrc.GetLength() == 0)
	{
		long detail_id = _ttoi(data.GetValue(_T("order_detail_id")));

		UpdateDBProcess((CDataOrderInfo *)wParam, E_S_DONE);
		UpdateItems();
		ResetAvgTime();
		return 1;
	}

	CStringArray strResult;
	CString strGap = _T(",");
	int nPos = strSrc.Find(strGap);

	CString strLeft = _T("");
	while(0 <= nPos)
	{
		strLeft = strSrc.Left(nPos);
		if (!strLeft.IsEmpty())
			strResult.Add(strLeft);

		strSrc = strSrc.Right(strSrc.GetLength() - nPos - 1);
		nPos = strSrc.Find(strGap);
	}

	if (!strSrc.IsEmpty()) {
		strResult.Add(strSrc);
	}

	CDataOrderDetail buf;
	int nSize = strResult.GetSize();
	for (int i = 0; i < nSize; i++)
	{
		buf.ClearData();
		m_dbmngr.GetItemByDetailId(E_S_DONING,strResult.GetAt(i),&buf);
		UpdateDBProcess(&buf, E_S_DONE);
	}

	//m_dbmngr.SetItemComplete(wParam, lParam);
	//UpdateDBProcess((CDataOrderInfo *)wParam, E_S_DONE);
	UpdateItems();
	ResetAvgTime();

	list<CombineSigleMap>::iterator iter;
	BOOL bFind = FALSE;
	INT iItemIdTmp;
	for (iter = m_combineList.begin();iter != m_combineList.end();++iter)
	{
		m_combineSingle.clear();
		m_combineSingle = *iter;
		map<INT,INT>::iterator iterTmp;
		for (iterTmp = m_combineSingle.begin();iterTmp != m_combineSingle.end();++iterTmp)
		{
			iItemIdTmp = (INT)_ttoi(strResult.GetAt(0));
			if (iItemIdTmp == iterTmp->first)
			{
				iter = m_combineList.erase(iter);
				bFind = TRUE;
				break;
			}
			if (bFind)
			{
				break;
			}
		}
		if (bFind)
		{
			break;
		}
	}

	return 1;
}

void CKDSManagerDlg::LoadOrderInfo(long order_head_id, CDlgOrder* pDlg, long check_id)
{
	m_dbmngr.GetOrder(order_head_id, pDlg, check_id);
}


//响应消息函数
LRESULT CKDSManagerDlg::OnMsgPend(WPARAM wParam, LPARAM lParam)
{
	//m_dbmngr.SetItemPended(wParam, lParam, m_curStyle);
	UpdateDBProcess((CDataOrderInfo *)wParam, E_S_PENDED);
	UpdateItems();
	return 1;
}

//响应消息函数
LRESULT CKDSManagerDlg::OnMsgOrder(WPARAM wParam, LPARAM lParam)
{
	CDlgOrder dlg;
	//dlg.m_headId = wParam;
	//dlg.m_checkId = lParam;
	CDataOrderInfo *pData = (CDataOrderInfo *)wParam;
	dlg.m_headId = _ttoi(pData->GetValue(_T("order_head_id")));
	dlg.m_checkId = _ttoi(pData->GetValue(_T("check_id")));
	dlg.DoModal();
	return 1;
}

//响应消息函数
LRESULT CKDSManagerDlg::OnMsgRefresh(WPARAM wParam, LPARAM lParam)
{
	UpdateItems();
	return 1;
}

INT CKDSManagerDlg::GetRefreshTime()
{
	return m_timerElapse;
}

void CKDSManagerDlg::ResetTimer()
{
	if (m_timerId != 0)
	{
		KillTimer(m_timerId);
		m_timerId = 0;
	}

	m_timerId = SetTimer(1, m_timerElapse*1000, NULL);
	if(m_timerId == 0)
	{
		AfxMessageBox(_T("The timer didn't take effect, does not automatically refresh"));
	}	
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CKDSManagerDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		/*CRect rect; 
		m_ctrlAvgTime.GetWindowRect(&rect); 
		m_bpImageAvgTime.Draw(GetDC()->GetSafeHdc(),rect.left,rect.top,rect.Width(), rect.Height());*/

		
		CDialog::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标显示。
HCURSOR CKDSManagerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CKDSManagerDlg::ResetShowType(KDS_ShowType newType)
{
	if (E_M_CNT == m_kitchenMode)
	{
		return;
	}
	CRoundButton2* pBtnCooking = (CRoundButton2*)GetDlgItem(IDC_BTN_COOKING);
	CRoundButton2* pBtnPend = (CRoundButton2*)GetDlgItem(IDC_BTN_PENDING);
	CRoundButton2* pBtnHistory = (CRoundButton2*)GetDlgItem(IDC_BTN_HISTORY);
	CString str;

	INT nStrGoback = IDS_STR_GOBACK_COOK;
	if (m_kitchenMode == E_M_SEND)
	{
		nStrGoback = IDS_STR_GOBACK_SEND;
	}

	INT nHistoryStr = IDS_STR_HISTORYLIST;
	INT nPendStr = IDS_STR_PENDLIST;

	switch(newType)
	{
	case E_S_DONING:
		nPendStr = IDS_STR_PENDLIST;	
		nHistoryStr = IDS_STR_HISTORYLIST;

		m_btnCookList.SetImages(m_bpBtnReturn[0], m_bpBtnReturn[1], false);
		m_btnPendList.SetImages(m_bpBtnPendList[0], m_bpBtnPendList[1], false);
		m_btnHistoryList.SetImages(m_bpBtnHistoryList[0], m_bpBtnHistoryList[1], false);
		break;

	case E_S_PENDED:
		nPendStr = nStrGoback;	
		nHistoryStr = IDS_STR_HISTORYLIST;

		m_btnCookList.SetImages(m_bpBtnPendList[0], m_bpBtnPendList[1], false);
		m_btnPendList.SetImages(m_bpBtnReturn[0], m_bpBtnReturn[1], false);
		m_btnHistoryList.SetImages(m_bpBtnHistoryList[0], m_bpBtnHistoryList[1], false);
		break;

	case E_S_DONE:
		nPendStr = IDS_STR_PENDLIST;	
		nHistoryStr = nStrGoback;

		m_btnCookList.SetImages(m_bpBtnPendList[0], m_bpBtnPendList[1], false);
		m_btnPendList.SetImages(m_bpBtnPendList[0], m_bpBtnPendList[1], false);
		m_btnHistoryList.SetImages(m_bpBtnReturn[0], m_bpBtnReturn[1], false);
		break;

	default:
		AfxMessageBox(_T("UNknown show type in ResetShowType"));
	}

	g_lang.LoadString(str, nHistoryStr);
//	pBtnHistory->SetWindowText(str);
	g_lang.LoadString(str, nPendStr);
//	pBtnPend->SetWindowText(str);

	pBtnCooking->SetInvalid();
	pBtnHistory->SetInvalid();
	pBtnPend->SetInvalid();

	m_showType = newType;
	m_curPageIndex = 0;

	if (E_STYLE_DETAIL == m_curStyle)
	{
		((CDlgItemDetailNew2*)m_pages[m_currentPage])->SelectItemPage(FALSE);
		m_currentPage = 0;
	}

	UpdateItems();	
}

void CKDSManagerDlg::OnBnClickedBtnPending()
{
	// TODO: 在此添加控件通知处理程序代码
	//CWnd* pCtrl = GetDlgItem(IDC_BTN_PENDING);
	//CString str;

	//switch(m_showType)
	//{
	//case E_S_PENDED: // 当前显示挂起菜单，点击返回，显示cooking菜单
	//	str.LoadString(IDS_STR_PENDING);
	//	m_showType = E_S_DONING;
	//	m_curPageIndex = 0;
	//	break;

	//default: // 点击挂起，显示pending菜单
	//	str.LoadString(IDS_STR_GOBACK);
	//	m_showType = E_S_PENDED;
	//	m_curPageIndex = 0;
	//	break;
	//}

	//pCtrl->SetWindowText(str);
	//UpdateItems();
//	switch(m_showType)
//	{
//	case E_S_PENDED:
		//TransferState(m_showType, E_S_DONING);
//		ResetShowType(E_S_DONING);
//		break;

//	default:
		//TransferState(m_showType, E_S_PENDED);
		ResetShowType(E_S_PENDED);
//		break;
//	}
}

void CKDSManagerDlg::OnBnClickedBtnHistory()
{
	// TODO: 在此添加控件通知处理程序代码
	
	//CWnd* pCtrl = GetDlgItem(IDC_BTN_HISTORY);
	//CString str;

	//switch(m_showType)
	//{
	//case E_S_DONE: // 当前显示历史菜单，点击‘历史（返回）’，显示待做菜单，且历史菜单按钮显示为‘历史菜单’
	//	str.LoadString(IDS_STR_HISTORYLIST);
	//	m_showType = E_S_DONING;
	//	m_curPageIndex = 0;
	//	break;

	//default: // 当前未显示‘历史菜单’，点击显示历史菜单菜单，按钮文本‘返回待做’
	//	str.LoadString(IDS_STR_GOBACK);
	//	m_showType = E_S_DONE;
	//	m_curPageIndex = 0;
	//	break;
	//}

	//pCtrl->SetWindowText(str);
	//UpdateItems();	

//	switch(m_showType)
//	{
//	case E_S_DONE:
		//TransferState(m_showType, E_S_DONING);
//		ResetShowType(E_S_DONING);
//		break;

//	default:
		//TransferState(m_showType, E_S_DONE);
		ResetShowType(E_S_DONE);
//		break;
//	}
}

void CKDSManagerDlg::OnBnClickedBtnFirst()
{
	// TODO: 在此添加控件通知处理程序代码
	m_curPageIndex = 0;
	if (E_STYLE_DETAIL == m_curStyle)
	{
		((CDlgItemDetailNew2*)m_pages[m_currentPage])->SelectItemPage(FALSE);
		m_currentPage = 0;
	}
	UpdateItems();
}

void CKDSManagerDlg::OnBnClickedBtnPre()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_curPageIndex > 0)
	{
		m_curPageIndex--;
		if (E_STYLE_DETAIL == m_curStyle)
		{
			((CDlgItemDetailNew2*)m_pages[m_currentPage])->SelectItemPage(FALSE);
			m_currentPage = 0;
		}
		UpdateItems();
	}
}

void CKDSManagerDlg::OnBnClickedBtnNext()
{
	// TODO: 在此添加控件通知处理程序代码
	if(!m_beLastPage)
	{
		m_curPageIndex++;
		if (E_STYLE_DETAIL == m_curStyle)
		{
			((CDlgItemDetailNew2*)m_pages[m_currentPage])->SelectItemPage(FALSE);
			m_currentPage = 0;
		}
		UpdateItems();
	}	
}

void CKDSManagerDlg::OnBnClickedBtnEnd()
{
	// TODO: 在此添加控件通知处理程序代码
	INT itemCnt = GetItemCnt();
	INT pageSize = m_pages.size();

	if (0 == itemCnt)
	{
		m_curPageIndex = 0;
	}
	else
	{
		m_curPageIndex = itemCnt%pageSize? itemCnt/pageSize:itemCnt/pageSize-1;
	}	
	if (E_STYLE_DETAIL == m_curStyle)
	{
		((CDlgItemDetailNew2*)m_pages[m_currentPage])->SelectItemPage(FALSE);
		m_currentPage = 0;
	}
	UpdateItems();
}

void CKDSManagerDlg::OnBnClickedBtnSetting()
{
	// TODO: 在此添加控件通知处理程序代码
	CDlgSetting dlg;

	// 进入设置界面之前停止定时器，以防发生其他问题，可以在设置完成后重启
	KillTimer(m_timerId);

	if (IDOK == dlg.DoModal())
	{		
		BOOL bRefresh = FALSE;
	
		// 界面风格
		if (m_curStyle != dlg.m_kdsStyle)
		{
			bRefresh = TRUE;
			m_curStyle = dlg.m_kdsStyle;

			CString val;
			val.Format(_T("%d"), m_curStyle);			
			WritePrivateProfileString(_T("basic"), _T("KDS_STYLE"), val, _T(".\\config.ini"));
			//ResetPages(); //如果设置后重启，就不要这行
		}

		// 显示模式
		if(m_kitchenMode != dlg.GetKitchenMode())
		{
			CString val;
			bRefresh = TRUE;
			m_kitchenMode = (KDS_Mode)dlg.GetKitchenMode();
			val.Format(_T("%d"), m_kitchenMode);			
			WritePrivateProfileString(_T("basic"), _T("KDS_MODE"), val, _T(".\\config.ini"));
			m_dbmngr.ResetKitchenMode(m_kitchenMode);
			//ResetPages();//如果设置后重启，就不要这行
		}

		// 厨房档口
		//if (!VectorCmp(m_curKitchenStalls, dlg.m_curSelStalls))
		if (m_strKitchenStalls != dlg.m_strStalls)
		{			
			bRefresh = TRUE;
			m_strKitchenStalls = dlg.m_strStalls;					
			WritePrivateProfileString(_T("kitchen"), _T("kitchen"), m_strKitchenStalls, _T(".\\config.ini"));
			m_dbmngr.ResetKitchenType(m_strKitchenStalls);
		}

		// 刷新时间
		if (dlg.m_refreshTime != m_timerElapse)
		{
			CString val;
			bRefresh = TRUE;
			m_timerElapse = dlg.m_refreshTime;
			val.Format(_T("%d"), m_timerElapse);
			WritePrivateProfileString(_T("basic"), _T("KDS_TIMER"), val, _T(".\\config.ini"));	
			ResetTimer();
		}

		// 语言选择
		CString lang;
		GetPrivateProfileString(_T("POS"), _T("LANGUAGE"), _T(""), lang.GetBuffer(256), 256, _T(".\\config.ini"));
		lang.ReleaseBuffer();
		if (dlg.m_lang != lang)
		{
			//AfxMessageBox(_T("language changed."));

			//g_lang.Initialize(dlg.m_lang);
			bRefresh = TRUE;
			WritePrivateProfileString(_T("POS"), _T("LANGUAGE"), dlg.m_lang, _T(".\\config.ini"));
			ResetLang();
			for (INT i=0; i<m_pages.size(); i++)
			{
				switch(m_curStyle)
				{
				case E_STYLE_DETAIL:
					((CDlgItemDetailNew2*)m_pages[i])->ResetLang();
					break;
				case E_STYLE_ORDER:
					((CDlgItemOrder*)m_pages[i])->ResetLang();
					break;
				}
			}
		}

		//菜品名称显示
		E_ItemName_ShowType nNameShow = (E_ItemName_ShowType)GetPrivateProfileInt(_T("ITEMDETAIL"), _T("NameShow"), 1, _T(".\\config.ini"));
		if (dlg.m_nNameShow != nNameShow)
		{
			CString val;
			//bRefresh = TRUE;
			val.Format(_T("%d"), dlg.m_nNameShow);
			WritePrivateProfileString(_T("ITEMDETAIL"), _T("NameShow"), val, _T(".\\config.ini"));
			m_nNameShow = dlg.m_nNameShow;
		}

		//催菜优先
		E_SORT_TYPE FoodSortType = (E_SORT_TYPE)GetPrivateProfileInt(_T("ITEMDETAIL"), _T("FoodPriority"), 0, _T(".\\config.ini"));
		if (dlg.m_nFoodSortType != FoodSortType)
		{
			CString val;
			//bRefresh = TRUE;
			val.Format(_T("%d"), dlg.m_nFoodSortType);
			WritePrivateProfileString(_T("ITEMDETAIL"), _T("FoodPriority"), val, _T(".\\config.ini"));
			m_nFoodSortType = dlg.m_nFoodSortType;
			m_dbmngr.ResetSortType(m_nFoodSortType);
		}

		//叫号里分单号的显示
		BOOL bCheclIdShow = (BOOL)GetPrivateProfileInt(_T("CALL"), _T("CheckIdShow"), 0, _T(".\\config.ini"));
		if (dlg.m_bShowCheckId != bCheclIdShow)
		{
			CString val;
			//bRefresh = TRUE;
			val.Format(_T("%d"), dlg.m_bShowCheckId);
			WritePrivateProfileString(_T("CALL"), _T("CheckIdShow"), val, _T(".\\config.ini"));
			m_bShowCheckId = dlg.m_bShowCheckId;
		}

		//打印机选择
		INT printerId = (KDS_STYLE)GetPrivateProfileInt(_T("basic"), _T("printer"), 1, _T(".\\config.ini"));
		if (dlg.m_printerId != printerId)
		{
			CString val;
			bRefresh = TRUE;
			val.Format(_T("%d"), dlg.m_printerId);
			WritePrivateProfileString(_T("basic"), _T("printer"), val, _T(".\\config.ini"));
		}

		if (bRefresh)
		{
			// 提示重启程序
			POSMessageBox(IDS_STR_CONFIGTIP);
			OnBnClickedBtnClose();
			return;

			//ResetPageState();
			ResetShowType(E_S_DONING);
			UpdateItems();
			SetFocus();
			Invalidate(TRUE);
		}
	}

	// 如果设置后不重启，重新打开定时器
	UpdateItems();
	SetFocus();
	Invalidate(TRUE);
	m_timerId = 0;
	ResetTimer();
}

void CKDSManagerDlg::OnBnClickedBtnClose()
{
	// TODO: 在此添加控件通知处理程序代码
	OnOK();
}

KDS_Mode CKDSManagerDlg::GetKitchenMode()
{
	return m_kitchenMode;
}

BOOL CKDSManagerDlg::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (m_bpBackgrd==NULL)
	{
		return FALSE;
	}

	if (E_M_CNT == m_kitchenMode)
	{
		DrawCallInfo(pDC);
	} 
	else
	{
		CRect rect; 
		GetClientRect(&rect); 
		CDC MemDc;
		MemDc.CreateCompatibleDC(pDC);
		HGDIOBJ hOldBmp = MemDc.SelectObject(m_bpBackgrd);
		pDC->SetStretchBltMode(HALFTONE);
		//pDC->StretchBlt(0,0,CreatButton::m_nFullWidth,CreatButton::m_nFullHeight,&MemDc,0,0,m_x,m_y,SRCCOPY);
		pDC->StretchBlt(0,0,rect.Width(), rect.Height(),&MemDc,0,0,m_x,m_y,SRCCOPY);
		MemDc.SelectObject(hOldBmp);
	}
	

	//m_ctrlAvgTime.SetBitmap(m_bpAvgTime);

	/*CDC MemDc2;
	CRect rect2; 
	HGDIOBJ hOldBmp2;
	m_ctrlAvgTime.GetWindowRect(&rect2); 

	MemDc2.CreateCompatibleDC(pDC);
	hOldBmp2 = MemDc2.SelectObject(m_bpAvgTime);
	pDC->SetStretchBltMode(HALFTONE);
	pDC->StretchBlt(rect2.left,rect2.top,rect2.Width(), rect2.Height(),&MemDc2,0,0,m_xAvgTime,m_yAvgTime,SRCCOPY);
	MemDc2.SelectObject(hOldBmp2);*/

	//CRect rect; 
	//m_ctrlAvgTime.GetWindowRect(&rect); 
	//m_bpImageAvgTime.Draw(pDC->GetSafeHdc(),rect.left,rect.top,rect.Width(), rect.Height());

	return TRUE;

	//return CDialog::OnEraseBkgnd(pDC);
}

HBRUSH CKDSManagerDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	//m_ctrlAvgTime.SetBitmap(m_bpAvgTime);
	// TODO:  在此更改 DC 的任何属性
	//if(nCtlColor==CTLCOLOR_STATIC)

	/*if(nCtlColor==CTLCOLOR_BTN)
	{
		pDC->SetTextColor(RGB(0, 0, 0));
	}*/

	if (pWnd->GetDlgCtrlID() == IDC_STATIC_PAGENUM)
	{
		pDC->SetTextColor(RGB(0, 0, 0));
		pDC->SetBkMode(TRANSPARENT);
		return (HBRUSH)::GetStockObject(NULL_BRUSH); //返回此画刷可以使静态文本透明
	}

	if (pWnd->GetDlgCtrlID() == IDC_STATIC_AVGTIME)
	{
		/*CRect rect; 
		m_ctrlAvgTime.GetWindowRect(&rect); 
		m_bpImageAvgTime.Draw(pDC->GetSafeHdc(),rect.left,rect.top,rect.Width(), rect.Height());*/

		pDC->SetBkColor(RGB(183, 201, 227)); //设置的是文字背景色
		pDC->SetDCBrushColor(RGB(183, 201, 227));//设置的是无文字部分的背景色
		return (HBRUSH)::GetStockObject(DC_BRUSH);

		//CDC MemDc2;
		//CRect rect2; 
		//HGDIOBJ hOldBmp2;
		//m_ctrlAvgTime.GetWindowRect(&rect2); 

		//MemDc2.CreateCompatibleDC(pDC);
		//hOldBmp2 = MemDc2.SelectObject(m_bpAvgTime);
		//pDC->SetStretchBltMode(HALFTONE);
		//pDC->StretchBlt(rect2.left,rect2.top,rect2.Width(), rect2.Height(),&MemDc2,0,0,m_xAvgTime,m_yAvgTime,SRCCOPY);
		//MemDc2.SelectObject(hOldBmp2);
		//return (HBRUSH)::GetStockObject(DC_BRUSH); //返回此画刷可以使静态文本透明
	}

	// TODO:  如果默认的不是所需画笔，则返回另一个画笔
	return hbr;
}

void CKDSManagerDlg::OnBnClickedBtnServerip()
{
	// TODO: 在此添加控件通知处理程序代码
	ShellExecute(NULL, NULL, _T("ServerIpSetting.exe"), NULL, NULL,SW_NORMAL);
	//OnOK();
}

void CKDSManagerDlg::OnBnClickedBtnCooking()
{
	// TODO: 在此添加控件通知处理程序代码
	ResetShowType(E_S_DONING);

}

INT CKDSManagerDlg::DeleteCombineItem()
{
	if (m_showType == E_S_PENDED || m_showType == E_S_DONE || E_M_SEND == m_kitchenMode)
	{
		return m_pData.size();
	}

	//移除合并项并计算合并数
	INT pDataSize = m_pData.size();
	for (INT i = 0;i < pDataSize;i++)
	{
		list<CombineSigleMap>::iterator iter;
		for (iter = m_combineList.begin();iter != m_combineList.end();++iter)
		{
			m_combineSingle.clear();
			m_combineSingle = *iter;
			INT iItemIdTmp = (INT)_ttoi(m_pData[i]->GetValue(_T("order_detail_id")));
			map<INT,INT>::iterator iterTmp = m_combineSingle.find(iItemIdTmp);//查此Detail_id是否存在合并
			if (iterTmp == m_combineSingle.end())
			{
				//不存在合并，加入map
				//INT iQuantity = (INT)_ttoi(m_pData[i]->GetValue(_T("quantity")));
				//CombineSigleMap mapTmp;
				//mapTmp.insert(make_pair(m_pData[i]->GetValue(_T("order_detail_id")),iQuantity));
				//m_combineList.push_back(mapTmp);
			}
			else if (iterTmp != m_combineSingle.end() && iterTmp != m_combineSingle.begin())
			{
				//存在合并且不是第一个条目，移除
				CDataOrderDetail* dataTmp = m_pData[i];
				m_pData.erase(m_pData.begin() + i);
				delete dataTmp;
				pDataSize = m_pData.size();//重新计算m_pData大小
				--i; //因为移除了一项，数组往前移，所有要把i减一指向往前移的那一个
			}
			else if (iterTmp != m_combineSingle.end() && iterTmp == m_combineSingle.begin())
			{
				//存在合并且是第一个条目，计算合并数
				CString strCombineNum;
				CString strCombineNumTmp;
				CString strCombineItem;
				INT iCombineNum = 0;
				map<INT,INT>::iterator iterTmp1 = m_combineSingle.begin();
				for(;iterTmp1 != m_combineSingle.end();iterTmp1++)
				{
					iCombineNum += iterTmp1->second;
					strCombineNumTmp.Format(_T("%s,%d"),strCombineItem,iterTmp1->first);
					strCombineItem = strCombineNumTmp;
				}
				strCombineNum.Format(_T("%d"),iCombineNum);
				m_pData[i]->m_data[_T("CombineNum")] = strCombineNum;
				m_pData[i]->m_data[_T("CombineItem")] = strCombineItem;
			}
		}// for
	}// for

	//循环查看是否有可以合并的菜品(相同条件：菜品id，菜品规格，食用方式，整单备注，做法)
	
	for (INT i = 0;i < m_pData.size();i++)
	{
		for (INT j = i + 1;j < m_pData.size();j++)
		{
			if (m_pData[i]->GetValue(_T("menu_item_id")).Compare(m_pData[j]->GetValue(_T("menu_item_id"))) == 0 &&
				m_pData[i]->GetValue(_T("unit")).Compare(m_pData[j]->GetValue(_T("unit"))) == 0 &&
				m_pData[i]->GetValue(_T("eat_type")).Compare(m_pData[j]->GetValue(_T("eat_type"))) == 0 &&
				m_pData[i]->GetValue(_T("description")).Compare(m_pData[j]->GetValue(_T("description"))) == 0)
			{
				CString strCondimentPriced1,strCondimentPriced2;
				GetCondimentPriced(_ttoi(m_pData[i]->GetValue(_T("order_detail_id"))),strCondimentPriced1);
				GetCondimentPriced(_ttoi(m_pData[j]->GetValue(_T("order_detail_id"))),strCondimentPriced2);
				if (strCondimentPriced1.Compare(strCondimentPriced2) == 0)
				{
					CString strItemType = m_pData[i]->GetValue(_T("item_type"));
					if (strItemType.Compare(_T("4")) == 0)
					{//自定义菜品
						if (m_pData[i]->GetValue(_T("menu_item_name")).Compare(m_pData[j]->GetValue(_T("menu_item_name"))) == 0)
						{
							//菜品条件相同，可以合并
							m_pData[i]->m_data[_T("has_same")] = _T("yes");
							break;
						}
					} 
					else
					{
						//菜品条件相同，可以合并
						m_pData[i]->m_data[_T("has_same")] = _T("yes");
						break;
					}
				}
			}// if
		}// for
	}// for

	return m_pData.size();
}
BOOL CKDSManagerDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类

	if(pMsg->message == WM_KEYDOWN)  //判断是否有按键按下
	{
		switch(pMsg->wParam)
		{
		case VK_F1:      //表示是功能键中的F1键
		case VK_F2:      //表示是功能键中的F2键
		case VK_F3:      //表示是功能键中的F3键
		case VK_F4:      //表示是功能键中的F4键
		case VK_F5:      //表示是功能键中的F5键
		case VK_F6:      //表示是功能键中的F6键
			ClickFunctionKeys(pMsg->wParam);
			break;
		case VK_RETURN:      //表示是回车键
#ifndef USE_OLD_DETAIL_PAGE
			if (E_STYLE_DETAIL == m_curStyle && m_ItemShowCount > 0)
			{
				((CDlgItemDetailNew2*)m_pages[m_currentPage])->PostMessage(WM_COMMAND, MAKEWPARAM(IDC_STATIC_ITEMNAME1, BN_CLICKED), NULL);  
			}
#endif
			return TRUE;
			break;
		case VK_ESCAPE:      //表示是功能键中的ESC键
			if (E_M_CNT == m_kitchenMode)
			{
				m_btnSetting.ShowWindow(TRUE);
				m_btnClose.ShowWindow(TRUE);
				m_timeTick = 0;
				m_timeIdCall = SetTimer(2,1000,NULL);
			}
			
			return TRUE;
			break;
		case VK_UP:
		case VK_DOWN:
		case VK_LEFT:
		case VK_RIGHT:
		case VK_NUMPAD0:
		case VK_NUMPAD1:
		case VK_NUMPAD2:
		case VK_NUMPAD3:
		case VK_NUMPAD4:
		case VK_NUMPAD5:
		case VK_NUMPAD6:
		case VK_NUMPAD7:
		case VK_NUMPAD8:
		case VK_NUMPAD9:
		case 48:
		case 49:
		case 50:
		case 51:
		case 52:
		case 53:
		case 54:
		case 55:
		case 56:
		case 57:
#ifndef USE_OLD_DETAIL_PAGE
			if (E_STYLE_DETAIL == m_curStyle && E_M_CNT != m_kitchenMode)
			{
				ShowItemDlgClickIn(FALSE);
				refreshItemDlg(pMsg->wParam);
				ShowItemDlgClickIn(TRUE);
				return TRUE;
			}
#endif
			break;
		default:
			break;
		}
	}
	
	return CDialog::PreTranslateMessage(pMsg);
}

void CKDSManagerDlg::WinHelp(DWORD dwData, UINT nCmd)
{
	// TODO: 在此添加专用代码和/或调用基类
	return;//不调用帮助文件
	CDialog::WinHelp(dwData, nCmd);
}

void CKDSManagerDlg::refreshItemDlg(WPARAM msg_wParam)
{
	switch(msg_wParam)
	{
	case VK_DOWN:     //表示是方向键中的向下的键
		if (m_currentPage + m_pageCols < m_ItemShowCount)
		{
			m_currentPage += m_pageCols;
		}
		break;
	case VK_UP:      //表示是方向键中的向上的键
		if (m_currentPage - m_pageCols >= 0)
		{
			m_currentPage -= m_pageCols;
		}
		break;
	case VK_LEFT:      //表示是方向键中的向左的键
		if (m_currentPage - 1 >= 0)
		{
			--m_currentPage;
		}
		break;
	case VK_RIGHT:      //表示是方向键中的向右的键
		if (m_currentPage + 1 < m_ItemShowCount)
		{
			++m_currentPage;
		}
		break;
	default:
		msgNumKeyWP(msg_wParam);
		break;
	}
}

void CKDSManagerDlg::msgNumKeyWP(WPARAM msg_wParam)
{
	char ch;
	int iNum;
	if(msg_wParam <= 105 && msg_wParam >= 96) //VK_NUMPAD0~VK_NUMPAD9
	{
		UINT iAscii = MapVirtualKey(msg_wParam,2);//0-MAPVK_VK_TO_VSC,1-MAPVK_VSC_TO_VK,2-MAPVK_VK_TO_CHAR,3-MAPVK_VSC_TO_VK_EX
		ch = char(iAscii);
		iNum = ch - '0';
	}
	if (msg_wParam <= 57 && msg_wParam >= 48)
	{
		ch = (char)msg_wParam;
		iNum = ch - '0';
	}

	if (iNum >= 1 && iNum <= 9 && iNum <= m_ItemShowCount)
	{
		m_currentPage = iNum - 1;
	}
}
void CKDSManagerDlg::ShowItemDlgClickIn(BOOL bFlag)
{
	if (bFlag)
	{
		((CDlgItemDetailNew2*)m_pages[m_currentPage ])->SelectItemPage(TRUE);
	} 
	else
	{
		((CDlgItemDetailNew2*)m_pages[m_currentPage])->SelectItemPage(FALSE);
	}
}

E_ItemName_ShowType CKDSManagerDlg::GetNameShowType()
{
	return m_nNameShow;
}

void CKDSManagerDlg::GetCondimentPriced(long order_detail_id, CString &val)
{
	m_dbmngr.GetCondimentPriced(order_detail_id, val);
}
void CKDSManagerDlg::DrawCallInfo(CDC* pDC)
{
	//CPaintDC dc(this); // 用于绘制的设备上下文
	CRect rect;
	GetWindowRect(&rect);

	CRect rectDrawCook,rectDrawSend;
	// 准备中订单显示区域
	rectDrawCook.top = rect.top;
	rectDrawCook.left = rect.left + 50;
	rectDrawCook.right = rect.right / 2;
	rectDrawCook.bottom = m_nDrawHight - rect.top;

	// 请取餐订单显示区域
	rectDrawSend.top = rect.top;
	rectDrawSend.left = rect.right / 2 + 50;
	rectDrawSend.right = rect.right;
	rectDrawSend.bottom = m_nDrawHight - rect.top;

	// 画背景
	CBrush brush1;
	brush1.CreateSolidBrush(RGB(0,0,0));
	pDC->FillRect(&rect,&brush1);

	// 字体颜色
	COLORREF colorText = RGB(255,255,255);
	
	// 画准备中
	CString strTip;
	CBrush brush2;
	brush2.CreateSolidBrush(RGB(0,0,255));
	CRect rectPrepare = CRect(rect.left, rect.top, rect.right/2, m_nDrawHight/ORDERNUMBER_SHOW);
	pDC->FillRect(&rectPrepare,&brush2);
	g_lang.LoadString(strTip, IDS_STR_PREPARE);
	pDC->SetTextColor(colorText);
	pDC->SetBkMode(TRANSPARENT);
	pDC->SelectObject(&m_fontOrder);
	pDC->DrawText(strTip, rectPrepare, DT_LEFT);

	// 画请取餐
	CBrush brush3;
	brush3.CreateSolidBrush(RGB(0,255,0));
	CRect rectFetch = CRect(m_nWidth/2, rect.top, rect.right, m_nDrawHight/ORDERNUMBER_SHOW);
	pDC->FillRect(&rectFetch,&brush3);
	g_lang.LoadString(strTip, IDS_STR_FETCH);
	pDC->DrawText(strTip,rectFetch,DT_LEFT);

	// 画取餐提示
	CString strCallTip;
	g_lang.LoadString(strCallTip, IDS_STR_CALLTIP);
	// 取餐提示显示区域
	CRect rectCallTip = CRect(rect.left, rect.top + m_nHeight*14/15, rect.right, rect.bottom);
	pDC->SetTextColor(colorText);
	pDC->SetBkMode(TRANSPARENT);
	pDC->SelectObject(&m_fontTip);
	pDC->DrawText(strCallTip,rectCallTip,DT_LEFT);

	// 画订单号
	CString strCook = _T("");
	CString strSend = _T("");
	CString strCookTmp = _T("");
	CString strSendTmp = _T("");
	KDS_Process strMake = E_P_CNT;
	CString strCheckNumber = _T("");
	CString strCheckId = _T("");
	CString strCheckName = _T("");

	INT pDataSize = m_pData.size();
	for (INT i = 0;i < pDataSize;i++)
	{
		strMake = (KDS_Process)_ttoi(m_pData[i]->GetValue(_T("is_make")));
		strCheckNumber = m_pData[i]->GetValue(_T("check_number"));
		strCheckId = m_pData[i]->GetValue(_T("check_id"));
		strCheckName = m_pData[i]->GetValue(_T("check_name"));
		switch(strMake)
		{
		case E_P_COOKING:
		case E_P_COOKPEND:
			{
				if (strCheckName.GetLength() != 0)
				{
					strCookTmp.Format(_T("%s\n%s"),strCook, strCheckName);
				}
				else
				{
					if (m_bShowCheckId)
					{
						strCookTmp.Format(_T("%s\n%s / %s"),strCook, strCheckNumber, strCheckId);
					} 
					else
					{
						strCookTmp.Format(_T("%s\n%s"),strCook, strCheckNumber);
					}
				}
				strCook = strCookTmp;
			}
			break;
		case E_P_COOKED:
		case E_P_SENDPEND:
			{
				if (strCheckName.GetLength() != 0)
				{
					strSendTmp.Format(_T("%s\n%s"),strSend, strCheckName);
				}
				else
				{
					if (m_bShowCheckId)
					{
						strSendTmp.Format(_T("%s\n%s / %s"),strSend, strCheckNumber, strCheckId);
					} 
					else
					{
						strSendTmp.Format(_T("%s\n%s"),strSend, strCheckNumber);
					}
					
				}
				strSend = strSendTmp;
			}
			break;
		}
	}
	pDC->SelectObject(&m_fontOrder);
	pDC->DrawText(strCook,rectDrawCook,DT_LEFT);
	pDC->DrawText(strSend,rectDrawSend,DT_LEFT);
}

void CKDSManagerDlg::ClickFunctionKeys(WPARAM msg_wParam)
{
	if (E_M_CNT == m_kitchenMode)
	{
		return;
	}
	switch(msg_wParam)
	{
	case VK_F1:
		OnBnClickedBtnCooking();
		break;
	case VK_F2:
		OnBnClickedBtnPending();
		break;
	case VK_F3:
		OnBnClickedBtnHistory();
		break;
	case VK_F4:
		OnBnClickedBtnFirst();
		break;
	case VK_F5:
		OnBnClickedBtnPre();
		break;
	case VK_F6:
		OnBnClickedBtnNext();
		break;
	default:
		break;
	}
}


BOOL CKDSManagerDlg::PlayVoice(CString str,BOOL bFileName /* = TURE*/)
{

	if (bFileName)
	{
		CString tmp;

		tmp.Format(_T("open .\\media\\Notify.wav alias notify"));
		mciSendString(_T("close notify"), NULL, 0, NULL);
		mciSendString(tmp.AllocSysString(), NULL, 0, NULL);
		mciSendString(_T("play notify"), NULL, 0, NULL);

		Sleep(100);

		if (g_lang.m_strLang.Compare(_T("English")) == 0)
		{
			str = _T("NewItenEn.wav");
		}
		tmp.Format(_T("open .\\media\\%s alias voice"),str);
		mciSendString(_T("close voice"), NULL, 0, NULL);
		mciSendString(tmp.AllocSysString(), NULL, 0, NULL);
		mciSendString(_T("play voice"), NULL, 0, NULL);
	}
	else
	{
		CString tmp,tmp1;
		for(INT i = 0;i < str.GetLength();++i)
		{
			tmp = str.GetAt(i);
			tmp1.Format(_T("open .\\media\\%s alias voiceTip"),tmp);
			mciSendString(_T("close voiceTip"), NULL, 0, NULL);
			mciSendString(tmp1.AllocSysString(), NULL, 0, NULL);
			mciSendString(_T("play voiceTip"), NULL, 0, NULL);
		}
	}
	return 0;
}

BOOL CKDSManagerDlg::CheckItemRush(void)
{
	m_lastRushItem.clear();
	m_dbmngr.GetRushItems(m_lastRushItem);
	if (m_lastRushItem.size() == 0 && m_preRushItem.size() != 0)
	{
		PlayVoice();
		m_preReturnItem.clear();
		return TRUE;
	}
	else if (m_LastReturnItem.size() != 0)
	{
		BOOL bRushItem = isSubset(m_preRushItem,m_lastRushItem);
		if (!bRushItem)
		{
			PlayVoice();
			m_preRushItem.clear();
			m_preRushItem.insert(m_lastRushItem.begin(), m_lastRushItem.end());
			return TRUE;
		}
	}
	return FALSE;
}

BOOL CKDSManagerDlg::CheckItemReturn(void)
{
	m_LastReturnItem.clear();
	m_dbmngr.GetReturnItems(m_LastReturnItem);
	if (m_LastReturnItem.size() == 0 && m_preReturnItem.size() != 0)
	{
		PlayVoice();
		m_preReturnItem.clear();
		return TRUE;
	}
	else if (m_LastReturnItem.size() != 0)
	{
		BOOL bReturnItem = isSubset(m_preReturnItem,m_LastReturnItem);
		if (!bReturnItem)
		{
			PlayVoice();
			m_preReturnItem.clear();
			m_preReturnItem.insert(m_preReturnItem.begin(), m_LastReturnItem.begin(), m_LastReturnItem.end());
			return TRUE;
		}
	}
	return FALSE;
}

// 判断vector v1是否包含vector v2
BOOL CKDSManagerDlg::isSubset(vector<INT> v1, vector<INT> v2)
{  
	int i = 0,j = 0;  
	int m = v1.size();  
	int n = v2.size();  
	if(m < n){  
		return FALSE;  
	}  
	sort(v1.begin(),v1.end());  
	sort(v2.begin(),v2.end());  
	while(i < n && j < m){  
		if(v1[j] < v2[i]){  
			j++;  
		}  
		else if(v1[j] == v2[i]){  
			j++;  
			i++;  
		}  
		else if(v1[j] > v2[i]){  
			return FALSE;  
		}  
	}  
	if(i < n){  
		return FALSE;  
	}  
	else{  
		return TRUE;  
	}  
}  

// 判断m1是否包含m2
BOOL CKDSManagerDlg::isSubset(map<INT,INT> m1, map<INT,INT> m2, BOOL bCheckSecond)
{  

	int iSamCount = 0;
	int m = m1.size();  
	int n = m2.size();  
	if(m < n){  
		return FALSE;  
	}  

	map<INT,INT>::iterator iter1 = m1.begin();
	map<INT,INT>::iterator iter2 = m2.begin();

	while(iter1 != m1.end() && iter2 != m2.end()){  
		if(iter1->first < iter2->first){  
			++iter1;  
		}  
		else if(iter1->first == iter2->first){
			if (bCheckSecond)
			{
				if (iter1->second == iter2->second)
				{
					++iter1; 
					++iter2; 
					++iSamCount;
				}
			}
			else
			{
				++iter1; 
				++iter2;
				++iSamCount;
			}
		}  
		else if(iter1->first > iter2->first){
			return FALSE;  
		}  
	}  
	if(iSamCount == n){  
		return TRUE;  
	}  
	else{  
		return FALSE;  
	}  
}  
BOOL CKDSManagerDlg::GetOrderHeadByDetailId(long detailId, CDataOrder* buf)
{
	m_dbmngr.GetItemOrderHeadByDetailId(m_showType,&(buf->m_head),detailId);

	long order_head_id = _ttoi(buf->m_head.m_data[_T("order_head_id")]);
	long check_id = _ttoi(buf->m_head.m_data[_T("check_id")]);
	INT tbl = _ttoi(buf->m_head.m_data[_T("tableType")]);

	switch(tbl)
	{
	case E_TMP_ORDER:
		m_dbmngr.GetItemOrderDetail(order_head_id, _T("order_detail"), buf, check_id);
		break;

	case E_HISTORY_ORDER:
		m_dbmngr.GetItemOrderDetail(order_head_id, _T("history_order_detail"), buf, check_id);
		break;

	default:
		AfxMessageBox(_T("Unknown order table type"));
	}		
	return 0;
}

BOOL CKDSManagerDlg::CheckHasNewItem(void)
{
	INT lastDtailId = m_preDetailId;
	lastDtailId = m_dbmngr.GetLastDetailId(m_preDetailId);
	if (m_preDetailId != lastDtailId)
	{
		PlayVoice();
		m_preDetailId = lastDtailId;
	}
	
	return 0;
}


CString CKDSManagerDlg::GetCombineList(void)
{
	CString strCombineList;
	CString tmp;
	list<CombineSigleMap>::iterator iter;
	for (iter = m_combineList.begin();iter != m_combineList.end();++iter)
	{
		m_combineSingle.clear();
		m_combineSingle = *iter;
		map<INT,INT>::iterator iterCombineSigle;
		for (iterCombineSigle = m_combineSingle.begin();iterCombineSigle != m_combineSingle.end();++iterCombineSigle)
		{
			tmp.Format(_T("%s,%d"),strCombineList,iterCombineSigle->first);
			strCombineList = tmp;
		}// for
	}// for

	return strCombineList;
}
CString CKDSManagerDlg::GetSetmealNameByOrderHeadId(CString id)
{
	return m_dbmngr.GetSetmealNameByOrderHeadId(id);
}

KDS_STYLE CKDSManagerDlg::GetKdsStype(void)
{
	return m_curStyle;
}
