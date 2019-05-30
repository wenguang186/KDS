// DlgSetting.cpp : 实现文件
//

#include "stdafx.h"
#include "atlimage.h"
#include "KDSManager.h"
#include "DlgSetting.h"
#include "KDSManagerDlg.h"
#include <algorithm>
#include "DlgCombine.h"

// CDlgSetting 对话框

IMPLEMENT_DYNAMIC(CDlgSetting, CDialog)

CDlgSetting::CDlgSetting(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgSetting::IDD, pParent)
	, m_refreshTime(0)
{
	// 背景图片
	CImage img;
	if(img.Load(_T(".\\res\\bg_image.jpg"))==S_OK)
	{
		m_x=img.GetWidth();
		m_y=img.GetHeight();
		m_bpBackgrd=img.Detach();
	}

	m_bpBtnSave[0]=Gdiplus::Image::FromFile(_T(".\\res\\btn_save.png"));
	m_bpBtnSave[1]=Gdiplus::Image::FromFile(_T(".\\res\\btn_pushdown.png"));

	m_bpBtnCancel[0]=Gdiplus::Image::FromFile(_T(".\\res\\btn_close.png"));
	m_bpBtnCancel[1]=Gdiplus::Image::FromFile(_T(".\\res\\btn_pushdown.png"));

	if(m_bpBtnSave[0] == NULL || m_bpBtnSave[0]->GetLastStatus() != Gdiplus::Ok)
	{
		AfxMessageBox(_T("Failed on loading pirctures, please check whether the pictures are damaged."));
	}
}

CDlgSetting::~CDlgSetting()
{
	::DeleteObject(m_bpBackgrd);
	//delete m_bpBtnClose[0];
	//delete m_bpBtnClose[1];

	// todo - 删除按钮
	for (UINT i=0; i<m_chkStalls.size(); i++)
	{
		delete m_chkStalls[i];
	}

	delete m_bpBtnSave[0];
	delete m_bpBtnSave[1];
	delete m_bpBtnCancel[0];
	delete m_bpBtnCancel[1];
}

void CDlgSetting::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_REFRESHTIME, m_refreshTime);
	DDV_MinMaxUInt(pDX, m_refreshTime, 1, 100);	
	DDX_Control(pDX, IDC_BTN_SAVE, m_btnSave);
	DDX_Control(pDX, IDC_BTN_CANCEL, m_btnCancel);
	DDX_Control(pDX, IDC_STATIC_STYLE, m_ctrlStyle);
	DDX_Control(pDX, IDC_STATIC_MODE, m_ctrlMode);
	DDX_Control(pDX, IDC_STATIC_REFRESHTIME, m_ctrlRefresh);
	DDX_Control(pDX, IDC_STATIC_KITCHEN, m_ctrlStalls);
	DDX_Control(pDX, IDC_STATIC_LANGUAGE, m_ctrlLang);
	DDX_Control(pDX, IDC_COMBO_LANGUAGE, m_comboLang);
	DDX_Control(pDX, IDC_BTN_LANGUAGE1, m_btnLanguage1);
	DDX_Control(pDX, IDC_BTN_LANGUAGE2, m_btnLanguage2);
	DDX_Control(pDX, IDC_BTN_LANGUAGE3, m_btnLanguage3);
	DDX_Control(pDX, IDC_BTN_LANGUAGE4, m_btnLanguage4);
	DDX_Control(pDX, IDC_STATIC_ITEMNAME, m_ctrlItemName);
	DDX_Control(pDX, IDC_STATIC_RUSHLABEL, m_ctrlFoodPriority);
	DDX_Control(pDX, IDC_STATIC_VERSION, m_ctrlVersion);
	DDX_Control(pDX, IDC_STATIC_SERVERSETLABEL, m_ctrlServerIP);
	DDX_Control(pDX, IDC_BUTTON_SERVERSET, m_btnServerSet);
	DDX_Control(pDX, IDC_STATIC_SHOWCHECKID, m_ctrlCheckId);
	DDX_Control(pDX, IDC_STATIC_SELECTPRINT, m_ctrlSelectPrint);
	DDX_Control(pDX, IDC_COMBO_PRINT, m_comboPrint);
	DDX_Control(pDX, IDC_STATIC_SECOND, m_ctrlUnitSecond);
	DDX_Control(pDX, IDC_STATIC_VERLABEL, m_ctrlVersionLabel);
}


BEGIN_MESSAGE_MAP(CDlgSetting, CDialog)
	ON_BN_CLICKED(IDC_BTN_SAVE, &CDlgSetting::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BTN_CANCEL, &CDlgSetting::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_RADIO_SEND, &CDlgSetting::OnBnClickedRadioSend)
	ON_BN_CLICKED(IDC_RADIO_COOK, &CDlgSetting::OnBnClickedRadioCook)
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_RADIO_ORDER, &CDlgSetting::OnBnClickedRadioOrder)
	ON_BN_CLICKED(IDC_RADIO_DETIAL, &CDlgSetting::OnBnClickedRadioDetial)
	ON_WM_ERASEBKGND()
	ON_WM_CTLCOLOR()
	ON_CBN_SELCHANGE(IDC_COMBO_LANGUAGE, &CDlgSetting::OnCbnSelchangeComboLanguage)
	ON_BN_CLICKED(IDC_BTN_LANGUAGE1, &CDlgSetting::OnBnClickedBtnLanguage1)
	ON_BN_CLICKED(IDC_BTN_LANGUAGE2, &CDlgSetting::OnBnClickedBtnLanguage2)
	ON_BN_CLICKED(IDC_BTN_LANGUAGE3, &CDlgSetting::OnBnClickedBtnLanguage3)
	ON_BN_CLICKED(IDC_BTN_REFRESHTIMESUBTRACT, &CDlgSetting::OnBnClickedBtnRefreshTimeSubtract)
	ON_BN_CLICKED(IDC_BTN_REFRESHTIMEADD, &CDlgSetting::OnBnClickedBtnRefreshTimeAdd)
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_CHECK_ALLITEM, &CDlgSetting::OnBnClickedCheckAllitem)
	ON_BN_CLICKED(IDC_RADIO_ONLYNAME1, &CDlgSetting::OnBnClickedRadioOnlyname1)
	ON_BN_CLICKED(IDC_RADIO_ONLYNAME2, &CDlgSetting::OnBnClickedRadioOnlyname2)
	ON_BN_CLICKED(IDC_RADIO_NAME1ANDNAME2, &CDlgSetting::OnBnClickedRadioName1andname2)
	ON_BN_CLICKED(IDC_RADIO_RUSHOPEN, &CDlgSetting::OnBnClickedRadioRushopen)
	ON_BN_CLICKED(IDC_RADIO_RUSHCLOSE, &CDlgSetting::OnBnClickedRadioRushclose)
	ON_BN_CLICKED(IDC_BTN_LANGUAGE4, &CDlgSetting::OnBnClickedBtnLanguage4)
	ON_BN_CLICKED(IDC_BUTTON_SERVERSET, &CDlgSetting::OnBnClickedButtonServerSet)
	ON_BN_CLICKED(IDC_RADIO_CALL, &CDlgSetting::OnBnClickedRadioCall)
	ON_BN_CLICKED(IDC_RADIO_SHOWCHECKID, &CDlgSetting::OnBnClickedRadioShowcheckid)
	ON_BN_CLICKED(IDC_RADIO_HIDECHECKID, &CDlgSetting::OnBnClickedRadioHidecheckid)
	ON_CBN_SELCHANGE(IDC_COMBO_PRINT, &CDlgSetting::OnCbnSelchangeComboPrint)
END_MESSAGE_MAP()


BOOL CDlgSetting::OnInitDialog()
{
	CDialog::OnInitDialog();

	LOGFONT lFont;
	GetFont()->GetLogFont(&lFont);
	lFont.lfHeight = 36;
	lFont.lfWidth = 0;
	m_btnSave.SetFont(&lFont);
	m_btnCancel.SetFont(&lFont);
	
	m_btnSave.SetImages(m_bpBtnSave[0], m_bpBtnSave[1], false);
	m_btnCancel.SetImages(m_bpBtnCancel[0], m_bpBtnCancel[1], false);

	// 初始化厨房档口
	m_strStalls = ((CKDSManagerDlg*)AfxGetMainWnd())->m_strKitchenStalls;
	((CKDSManagerDlg*)AfxGetMainWnd())->m_dbmngr.GetKitchenType(this);

	// 初始化刷新时间
	m_refreshTime = ((CKDSManagerDlg*)AfxGetMainWnd())->GetRefreshTime();

	// 初始化单选框
	// note - 对档口有依赖	
	CButton* radioDetail = (CButton*)GetDlgItem(IDC_RADIO_DETIAL);
	CButton* radioOrder = (CButton*)GetDlgItem(IDC_RADIO_ORDER);
	m_kdsStyle = ((CKDSManagerDlg*)AfxGetMainWnd())->m_curStyle;
	switch(m_kdsStyle)
	{
	case E_STYLE_DETAIL:
		EnableKitchenStalls(TRUE);
		radioDetail->SetCheck(1);
		radioOrder->SetCheck(0);
		break;

	case E_STYLE_ORDER:
		EnableKitchenStalls(FALSE);
		radioDetail->SetCheck(0);
		radioOrder->SetCheck(1);
		break;
	}

	CButton* radioCook = (CButton*)GetDlgItem(IDC_RADIO_COOK);
	CButton* radioSend = (CButton*)GetDlgItem(IDC_RADIO_SEND);
	CButton* radioCall = (CButton*)GetDlgItem(IDC_RADIO_CALL);
	m_kitchenMode = ((CKDSManagerDlg*)AfxGetMainWnd())->GetKitchenMode();
	switch (m_kitchenMode)
	{
	case E_M_COOK:
		//EnableKitchenStalls(TRUE);
		radioCook->SetCheck(1);		
		radioSend->SetCheck(0);
		radioCall->SetCheck(0);
		EnableCtrl(TRUE);
		break;

	case E_M_SEND:
		//EnableKitchenStalls(FALSE);
		radioCook->SetCheck(0);		
		radioSend->SetCheck(1);
		radioCall->SetCheck(0);
		EnableCtrl(TRUE);
		break;
	case E_M_CNT:
		radioCook->SetCheck(0);		
		radioSend->SetCheck(0);
		radioCall->SetCheck(1);
		EnableCtrl(FALSE);
		break;
	}

	//	初始化语言选择框 
	initLang();

	//菜品名称显示模式 1-只显示菜品名1  2-只显示菜品名2  3-显示菜品名1和菜品名2
	CButton* radioName1Show = (CButton*)GetDlgItem(IDC_RADIO_ONLYNAME1);
	CButton* radioName2Show = (CButton*)GetDlgItem(IDC_RADIO_ONLYNAME2);
	CButton* radioName1AndName2Show = (CButton*)GetDlgItem(IDC_RADIO_NAME1ANDNAME2);
	m_nNameShow = (E_ItemName_ShowType)GetPrivateProfileInt(_T("ITEMDETAIL"), _T("NameShow"), 1, _T(".\\config.ini"));
	switch(m_nNameShow)
	{
	case E_ONLYNAME1:
		radioName1Show->SetCheck(1);
		radioName2Show->SetCheck(0);
		radioName1AndName2Show->SetCheck(0);
		break;
	case E_ONLYNAME2:
		radioName1Show->SetCheck(0);
		radioName2Show->SetCheck(1);
		radioName1AndName2Show->SetCheck(0);
		break;
	case E_NAME1ANDNAME2:
		radioName1Show->SetCheck(0);
		radioName2Show->SetCheck(0);
		radioName1AndName2Show->SetCheck(1);
		break;
	}

	//催菜优先开关 1-开  0-关
	CButton* radioRushOpen = (CButton*)GetDlgItem(IDC_RADIO_RUSHOPEN);
	CButton* radioRushClose = (CButton*)GetDlgItem(IDC_RADIO_RUSHCLOSE);
	m_nFoodSortType = (E_SORT_TYPE)GetPrivateProfileInt(_T("ITEMDETAIL"), _T("FoodPriority"), 0, _T(".\\config.ini"));
	switch(m_nFoodSortType)
	{
	case E_RUSH:
		radioRushOpen->SetCheck(1);
		radioRushClose->SetCheck(0);
		break;
	case E_ORDER_TIME:
		radioRushOpen->SetCheck(0);
		radioRushClose->SetCheck(1);
		break;
	}

	//叫号里分单号开关 1-开  0-关
	CButton* radioShowCheckId = (CButton*)GetDlgItem(IDC_RADIO_SHOWCHECKID);
	CButton* radioHideCheckId = (CButton*)GetDlgItem(IDC_RADIO_HIDECHECKID);
	m_bShowCheckId = (BOOL)GetPrivateProfileInt(_T("CALL"), _T("CheckIdShow"), 0, _T(".\\config.ini"));
	if (m_bShowCheckId)
	{
		radioShowCheckId->SetCheck(1);
		radioHideCheckId->SetCheck(0);
	} 
	else
	{
		radioShowCheckId->SetCheck(0);
		radioHideCheckId->SetCheck(1);
	}
	

	CString strVersion = GetAppVersion();
	//strVersion.LoadString(IDS_STR_VERSIONINFO);
	m_ctrlVersion.SetWindowText(strVersion);


	// 初始化厨房档口
	((CKDSManagerDlg*)AfxGetMainWnd())->m_dbmngr.GetPrinter(this);

	// 设置文本显示
	ResetLang();
	

	UpdateData(FALSE);
	return TRUE;
}

void CDlgSetting::ResetLang()
{
	CString caption;

	if (g_lang.m_strLang.Compare(_T("English")) == 0)
	{
		SetWindowText(_T(""));
	}
	else
	{
		SetWindowText(_T("设置"));
	}

	g_lang.LoadString(caption, IDS_STR_SAVE);
	m_btnSave.SetWindowText(caption);
	g_lang.LoadString(caption, IDS_STR_CANCEL);
	m_btnCancel.SetWindowText(caption);
	g_lang.LoadString(caption, IDS_STR_SERVERSETTING);
	m_btnServerSet.SetWindowText(caption);
	
	//AfxMessageBox(caption);
	//m_btnSave.UpdateData();
	//m_btnSave.Invalidate();
	m_btnSave.SetInvalid();
	m_btnCancel.SetInvalid();

	g_lang.LoadString(caption, IDS_STR_LANG);
	m_ctrlLang.SetWindowText(caption); 

	g_lang.LoadString(caption, IDS_STR_VERSIONLABEL);
	m_ctrlVersionLabel.SetWindowText(caption);
	g_lang.LoadString(caption, IDS_STR_KDSSTYLE);
	m_ctrlStyle.SetWindowText(caption);
	g_lang.LoadString(caption, IDS_STR_KDSMODE);
	m_ctrlMode.SetWindowText(caption);
	g_lang.LoadString(caption, IDS_STR_SERVERIP);
	m_ctrlServerIP.SetWindowText(caption);
	g_lang.LoadString(caption, IDS_STR_REFRESHTIME);
	m_ctrlRefresh.SetWindowText(caption);
	g_lang.LoadString(caption, IDS_STR_STALL);
	m_ctrlStalls.SetWindowText(caption);
	g_lang.LoadString(caption, IDS_STR_TIMESECOND);
	m_ctrlUnitSecond.SetWindowText(caption);
	g_lang.LoadString(caption, IDS_STR_ITEMNAME);
	m_ctrlItemName.SetWindowText(caption);
	g_lang.LoadString(caption, IDS_STR_RUSHLABEL);
	m_ctrlFoodPriority.SetWindowText(caption);
	g_lang.LoadString(caption, IDS_STR_SHOWCHECKID);
	m_ctrlCheckId.SetWindowText(caption);
	g_lang.LoadString(caption, IDS_STR_PRINTLABEL);
	m_ctrlSelectPrint.SetWindowText(caption);


	CButton *pBtn = (CButton *)GetDlgItem(IDC_RADIO_DETIAL);
	g_lang.LoadString(caption, IDS_STR_BYDETAIL);
	pBtn->SetWindowText(caption);

	pBtn = (CButton *)GetDlgItem(IDC_RADIO_ORDER);
	g_lang.LoadString(caption, IDS_STR_BYORDER);
	pBtn->SetWindowText(caption);

	pBtn = (CButton *)GetDlgItem(IDC_RADIO_COOK);
	g_lang.LoadString(caption, IDS_STR_KITCHEN);
	pBtn->SetWindowText(caption);

	pBtn = (CButton *)GetDlgItem(IDC_RADIO_SEND);
	g_lang.LoadString(caption, IDS_STR_SEND);
	pBtn->SetWindowText(caption);

	pBtn = (CButton *)GetDlgItem(IDC_RADIO_CALL);
	g_lang.LoadString(caption, IDS_STR_CALL);
	pBtn->SetWindowText(caption);

	pBtn = (CButton *)GetDlgItem(IDC_RADIO_ONLYNAME1);
	g_lang.LoadString(caption, IDS_STR_ONLYNAME1);
	pBtn->SetWindowText(caption);

	pBtn = (CButton *)GetDlgItem(IDC_RADIO_ONLYNAME2);
	g_lang.LoadString(caption, IDS_STR_ONLYNAME2);
	pBtn->SetWindowText(caption);

	pBtn = (CButton *)GetDlgItem(IDC_RADIO_NAME1ANDNAME2);
	g_lang.LoadString(caption, IDS_STR_NAME1ANDNAME2);
	pBtn->SetWindowText(caption);

	pBtn = (CButton *)GetDlgItem(IDC_RADIO_RUSHOPEN);
	g_lang.LoadString(caption, IDS_STR_RUSHOPEN);
	pBtn->SetWindowText(caption);

	pBtn = (CButton *)GetDlgItem(IDC_RADIO_RUSHCLOSE);
	g_lang.LoadString(caption, IDS_STR_RUSHCLOSE);
	pBtn->SetWindowText(caption);

	pBtn = (CButton *)GetDlgItem(IDC_CHECK_ALLITEM);
	g_lang.LoadString(caption, IDS_STR_ALLPRINTCLASS);
	pBtn->SetWindowText(caption);

	pBtn = (CButton *)GetDlgItem(IDC_RADIO_SHOWCHECKID);
	g_lang.LoadString(caption, IDS_STR_YES);
	pBtn->SetWindowText(caption);

	pBtn = (CButton *)GetDlgItem(IDC_RADIO_HIDECHECKID);
	g_lang.LoadString(caption, IDS_STR_NO);
	pBtn->SetWindowText(caption);

	Invalidate();
}

inline void CDlgSetting::EnableKitchenStalls(BOOL enable)
{
	//m_chkSelAll.EnableWindow(enable);
	for (UINT i=0; i<m_chkStalls.size(); i++)
	{
		m_chkStalls[i]->EnableWindow(enable);
	}
}

void CDlgSetting::SetKitchenType(CRecordset &rs)
{
	std::vector<INT> curStalls;// = ((CKDSManagerDlg*)AfxGetMainWnd())->m_curKitchenStalls;
	
	CString val;
	INT i=0;
	while (AfxExtractSubString(val, m_strStalls, i++, ','))
	{
		curStalls.push_back(_ttoi(val));
	}	

	//std::map<INT, CString> stalls = ((CKDSManagerDlg*)AfxGetMainWnd())->m_kitchenStalls;

	RECT rect;// = {85, 105, 160, 120};
	// 调试中出现抽疯，当时怎么调位置都不对，加了图片后，调整文字大小位置时，可以正常工作了
	((CStatic*)GetDlgItem(IDC_STATIC_KITCHEN))->GetWindowRect(&rect);
	ScreenToClient(&rect);
	//ClientToScreen(&rect);
	//m_chkSelAll.GetWindowRect(&rect);
	INT height = rect.bottom - rect.top;
	INT width = rect.right - rect.left;
	// 起始Y轴位置
	INT startY = rect.top + height;
	//rect.top = rect.bottom;
	//rect.bottom = rect.top + height;
	//rect.left = rect.right + 10;
	//rect.right = rect.left + width;
	
	// 起始X轴位置
	((CEdit*)GetDlgItem(IDC_EDIT_REFRESHTIME))->GetWindowRect(&rect);
	ScreenToClient(&rect);

	INT startX = rect.left;

	// 起始位置
	rect.right = rect.left + width;
	rect.top = startY;
	rect.bottom = rect.top + height;

	//for (std::map<INT, CString> ::const_iterator i=stalls.begin(); i!=stalls.end(); i++)
	for (INT cnt=1; !rs.IsEOF(); cnt++)
	{
		CString strId, strName, val;
		rs.GetFieldValue(_T("print_class_id"), strId);
		rs.GetFieldValue(_T("print_class_name"), strName);
		//val.Format(_T("<%s>%s"), strId, strName);

		CButton *pBtn = new CButton;
		if (!pBtn->Create(strName, WS_CHILD|WS_VISIBLE|BS_AUTOCHECKBOX, rect, this, _ttoi(strId)+MY_CTRL_ID_START))
		{
			POSMessageBox(IDS_STR_INITKITCHENTYPE);
		}
		pBtn->SetFont(GetFont()); //设置为父窗口的字体
		m_chkStalls.push_back(pBtn);

		//CString msg;
		//msg.Format(_T("CButton id %d"), pBtn->GetDlgCtrlID());
		//DebugMessageBox(msg);

		std::vector<int>::iterator result = find( curStalls.begin( ), curStalls.end( ), _ttoi(strId));
		if (result != curStalls.end()) 		
		{
			pBtn->SetCheck(1);
		}

		if (cnt%5 == 0)
		{
			rect.left = startX;
			rect.top = startY + (height+10)*(cnt/5);
		}else
		{
			rect.left += width + 5;
		}		
		rect.right = width + rect.left;
		rect.bottom = rect.top + height;

		rs.MoveNext();
	}

	BOOL bAllchkStalls = TRUE;
	for (INT i = 0;i < m_chkStalls.size();i++)
	{
		if (m_chkStalls[i]->GetCheck() == 0)
		{
			bAllchkStalls = FALSE;
			break;
		}
	}
	CButton* checkAllItem = (CButton*)GetDlgItem(IDC_CHECK_ALLITEM);
	
	if (bAllchkStalls)
	{
		checkAllItem->SetCheck(1);
	}
	else
	{
		checkAllItem->SetCheck(0);
	}
}

// CDlgSetting 消息处理程序
void CDlgSetting::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	INT iAllItemCheck = ((CButton*)GetDlgItem(IDC_CHECK_ALLITEM))->GetCheck();
	for (UINT i=0; i<m_chkStalls.size(); i++)
	{
		if (iAllItemCheck)
		{
			m_curSelStalls.push_back(m_chkStalls[i]->GetDlgCtrlID()-MY_CTRL_ID_START);
		} 
		else
		{
			if (m_chkStalls[i]->GetCheck())
			{
				m_curSelStalls.push_back(m_chkStalls[i]->GetDlgCtrlID()-MY_CTRL_ID_START);
			}
		}
		
	}

	// 无论何时都不应为空
	//if (m_kdsStyle == E_STYLE_DETAIL && m_curSelStalls.size() == 0)
	
	CButton* radioCall = (CButton*)GetDlgItem(IDC_RADIO_CALL);
	INT iCheck = radioCall->GetCheck();
	if (iCheck != 1)
	{
		// 如果选择叫号，直接返回
		if (m_curSelStalls.size() == 0)
		{
			POSMessageBox(IDS_STR_NOKITCHENTYPE);
			return;
		}

		m_strStalls.Format(_T("%d"), m_curSelStalls[0]);
		for (UINT i=1; i<m_curSelStalls.size(); i++)
		{
			CString tmp;
			tmp.Format(_T("%d"), m_curSelStalls[i]);
			m_strStalls += _T(",") + tmp;
		}
	}
	
	OnOK();
}

void CDlgSetting::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_oldLang.Compare(m_lang) != 0)
	{
		g_lang.Initialize(m_oldLang);
		ResetLang();
	}
	OnCancel();
}

void CDlgSetting::OnBnClickedRadioSend()
{
	// TODO: 在此添加控件通知处理程序代码
	
	//DEnableKitchenStalls(FALSE);
	EnableCtrl(TRUE);
	m_kitchenMode = E_M_SEND;
}

void CDlgSetting::OnBnClickedRadioCook()
{
	// TODO: 在此添加控件通知处理程序代码
	//EnableKitchenStalls(TRUE);
	EnableCtrl(TRUE);
	m_kitchenMode = E_M_COOK;	
}

void CDlgSetting::OnBnClickedRadioCall()
{
	// TODO: 在此添加控件通知处理程序代码
	EnableCtrl(FALSE);
	m_kitchenMode = E_M_CNT;
}

std::vector<INT>& CDlgSetting::GetKitchenType()
{	
	return m_curSelStalls;
}

KDS_Mode CDlgSetting::GetKitchenMode()
{	
	return m_kitchenMode;
}


void CDlgSetting::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码
	// 本来想借此布局button，但是一下代码执行完之前，dlg就跳出来了，导致代码无效
#if 0 
	if (((CStatic*)GetDlgItem(IDC_STATIC_KITCHEN))->m_hWnd)
	{
		AfxMessageBox(_T("重置checkbutton"));
	
		RECT rect;
		((CStatic*)GetDlgItem(IDC_STATIC_KITCHEN))->GetWindowRect(&rect);
		//ScreenToClient(&rect);
		//ClientToScreen(&rect);
		//m_chkSelAll.GetWindowRect(&rect);
		INT height = rect.bottom - rect.top;
		INT width = rect.right - rect.left;
		//rect.top = rect.bottom;
		//rect.bottom = rect.top + height;
		rect.left = rect.right + 10;
		rect.right = rect.left + width;

		//for (std::map<INT, CString> ::const_iterator i=stalls.begin(); i!=stalls.end(); i++)
		for (INT i=0; i<m_chkStalls.size(); i++)
		{
			m_chkStalls[i]->MoveWindow(&rect);
			rect.left += width;
			rect.right = width + rect.left;
		}
	}
	else
	{
		AfxMessageBox(_T("未重置checkbutton"));
	}

	AfxMessageBox(_T("afterif"));
#endif
}

void CDlgSetting::OnBnClickedRadioOrder()
{
	// TODO: 在此添加控件通知处理程序代码
	// 默认选中所有档口
	CButton* checkAllItem = (CButton*)GetDlgItem(IDC_CHECK_ALLITEM);
	checkAllItem->SetCheck(1);
	checkAllItem->EnableWindow(FALSE);
	for (UINT i=0; i<m_chkStalls.size(); i++)
	{
		m_chkStalls[i]->SetCheck(1);
	}
	EnableKitchenStalls(FALSE);
	m_kdsStyle = E_STYLE_ORDER;
}

void CDlgSetting::OnBnClickedRadioDetial()
{
	// TODO: 在此添加控件通知处理程序代码
	CButton* checkAllItem = (CButton*)GetDlgItem(IDC_CHECK_ALLITEM);
	checkAllItem->EnableWindow(TRUE);
	EnableKitchenStalls(TRUE);
	m_kdsStyle = E_STYLE_DETAIL;
}

BOOL CDlgSetting::OnEraseBkgnd(CDC* pDC)
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

	return CDialog::OnEraseBkgnd(pDC);
}

HBRUSH CDlgSetting::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  在此更改 DC 的任何属性
	
	// 20160823 - winXP下，使用如下方式会导致厨房档口显示为黑色框 
	//            目前使用按ID设置透明的方式。
	//if(nCtlColor==CTLCOLOR_STATIC)
	//{
	//	pDC->SetTextColor(RGB(0, 0, 0));
	//	pDC->SetBkMode(TRANSPARENT);
	//	return (HBRUSH)::GetStockObject(NULL_BRUSH); //返回此画刷可以使静态文本透明
	//}
	int ctlId=pWnd->GetDlgCtrlID();
	switch (ctlId)
	{
	case IDC_STATIC_LANGUAGE:
	case IDC_STATIC_STYLE:
	case IDC_STATIC_MODE:
	case IDC_STATIC_REFRESHTIME:
	case IDC_STATIC_KITCHEN:
	case IDC_STATIC_VERLABEL:
	case IDC_STATIC_VERSION:
	case IDC_STATIC_ITEMNAME:
	case IDC_STATIC_RUSHLABEL:
	case IDC_STATIC_SECOND:
	case IDC_STATIC_SERVERSETLABEL:
	case IDC_STATIC_SHOWCHECKID:
	case IDC_STATIC_SELECTPRINT:
		pDC->SetTextColor(RGB(0, 0, 0));
		pDC->SetBkMode(TRANSPARENT);
		return (HBRUSH)::GetStockObject(NULL_BRUSH);

	default:
		break;
	}

	// 不生效
	//if(nCtlColor==CTLCOLOR_BTN)
	////if(pWnd->GetDlgCtrlID()==IDC_RADIO_DETIAL)  
	//{
	//	pDC->SetTextColor(RGB(255, 0, 0));
	//	pDC->SetBkMode(TRANSPARENT);
	//	return (HBRUSH)::GetStockObject(NULL_BRUSH); //返回此画刷可以使静态文本透明
	//}
	 
	INT id = pWnd->GetDlgCtrlID();
	if (id == IDC_RADIO_DETIAL || id == IDC_RADIO_ORDER ||id == IDC_RADIO_COOK ||
		id == IDC_RADIO_SEND || id == IDC_RADIO_ONLYNAME1 || id == IDC_RADIO_ONLYNAME2 ||
		id == IDC_RADIO_NAME1ANDNAME2 || id == IDC_RADIO_RUSHOPEN ||id == IDC_RADIO_RUSHCLOSE ||
		id == IDC_CHECK_ALLITEM || id == IDC_RADIO_CALL || id == IDC_RADIO_SHOWCHECKID ||
		id == IDC_RADIO_HIDECHECKID)
	{
		pDC->SetBkMode(TRANSPARENT);
		CRect rc;
		pWnd->GetWindowRect(&rc);
		ScreenToClient(&rc);
		CDC* dc = GetDC();
		pDC->BitBlt(0,0,rc.Width(),rc.Height(),dc,rc.left,rc.top,SRCCOPY);	//把父窗口背景图片先画到按钮上
		ReleaseDC(dc);
		hbr = (HBRUSH) ::GetStockObject(NULL_BRUSH);
	}

	for (int i =0;i < m_chkStalls.size();i++)
	{
		if (m_chkStalls[i]->GetDlgCtrlID() == id)
		{
			pDC->SetBkMode(TRANSPARENT);
			CRect rc;
			pWnd->GetWindowRect(&rc);
			ScreenToClient(&rc);
			CDC* dc = GetDC();
			pDC->BitBlt(0,0,rc.Width(),rc.Height(),dc,rc.left,rc.top,SRCCOPY);	//把父窗口背景图片先画到按钮上
			ReleaseDC(dc);
			hbr = (HBRUSH) ::GetStockObject(NULL_BRUSH);
			break;
		}
	}

	// TODO:  如果默认的不是所需画笔，则返回另一个画笔
	return hbr;
}

void CDlgSetting::OnCbnSelchangeComboLanguage()
{
	// TODO: 在此添加控件通知处理程序代码
	//CString tmp;
	//m_comboLang.GetWindowText(tmp);
	m_comboLang.GetLBText(m_comboLang.GetCurSel(), m_lang);
	//AfxMessageBox(tmp);
	g_lang.Initialize(m_lang);
	ResetLang();
	
}

void CDlgSetting::OnBnClickedBtnLanguage1()
{
	// TODO: 在此添加控件通知处理程序代码
	m_btnLanguage1.GetWindowText(m_lang);
	g_lang.Initialize(m_lang);
	ResetLang();
}

void CDlgSetting::OnBnClickedBtnLanguage2()
{
	// TODO: 在此添加控件通知处理程序代码
	m_btnLanguage2.GetWindowText(m_lang);
	g_lang.Initialize(m_lang);
	ResetLang();
}

void CDlgSetting::OnBnClickedBtnLanguage3()
{
	// TODO: 在此添加控件通知处理程序代码
	m_btnLanguage3.GetWindowText(m_lang);
	g_lang.Initialize(m_lang);
	ResetLang();
}

void CDlgSetting::OnBnClickedBtnLanguage4()
{
	// TODO: 在此添加控件通知处理程序代码
	INT cnt = GetPrivateProfileInt(_T("LANGUAGES"), _T("COUNT"), 0, _T(".\\config.ini"));
	if (cnt > 4)
	{
		CDlgLangSelect m_dlgLangSelect;
		m_dlgLangSelect.DoModal();
		m_lang = m_dlgLangSelect.m_strLanguage;
	} 
	else
	{
		m_btnLanguage3.GetWindowText(m_lang);
	}
	g_lang.Initialize(m_lang);
	ResetLang();
}


void CDlgSetting::OnBnClickedBtnRefreshTimeSubtract()
{
	// TODO: 在此添加控件通知处理程序代码
	--m_refreshTime;
	if (m_refreshTime < 0)
	{
		m_refreshTime = 0;
	}
	UpdateData(FALSE);
}

void CDlgSetting::OnBnClickedBtnRefreshTimeAdd()
{
	// TODO: 在此添加控件通知处理程序代码
	++m_refreshTime;
	UpdateData(FALSE);
}

void CDlgSetting::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: 在此处添加消息处理程序代码
	// 不为绘图消息调用 CDialog::OnPaint()
}

void CDlgSetting::OnBnClickedCheckAllitem()
{
	// TODO: 在此添加控件通知处理程序代码
	INT iAllItemCheck = ((CButton*)GetDlgItem(IDC_CHECK_ALLITEM))->GetCheck();
	if (iAllItemCheck)
	{
		for (UINT i=0; i<m_chkStalls.size(); i++)
		{
			m_chkStalls[i]->SetCheck(1);
		}
	}
}

void CDlgSetting::initLang(void)
{
	vector<CString> vtorLang;
	INT cnt = GetPrivateProfileInt(_T("LANGUAGES"), _T("COUNT"), 0, _T(".\\config.ini"));
	CString lang;
	for (INT i=0; i<cnt; i++)
	{
		CString tmp;
		tmp.Format(_T("Lang%d"), i+1);

		GetPrivateProfileString(_T("LANGUAGES"), tmp, _T(""), lang.GetBuffer(256), 256, _T(".\\config.ini"));
		lang.ReleaseBuffer();
		vtorLang.push_back(lang);
		//m_comboLang.AddString(lang);
	}

	GetPrivateProfileString(_T("POS"), _T("LANGUAGE"), _T(""), m_lang.GetBuffer(256), 256, _T(".\\config.ini"));
	m_lang.ReleaseBuffer();
	m_oldLang = m_lang;
	//m_comboLang.SelectString(-1, m_lang);

	INT nLangCount = vtorLang.size();
	switch(nLangCount)
	{
	case 0:
		m_btnLanguage1.ShowWindow(TRUE);
		m_btnLanguage2.ShowWindow(FALSE);
		m_btnLanguage3.ShowWindow(FALSE);
		m_btnLanguage4.ShowWindow(FALSE);
		break;
	case 1:
		m_btnLanguage1.SetWindowText(vtorLang[0]);
		m_btnLanguage1.ShowWindow(TRUE);
		m_btnLanguage2.ShowWindow(FALSE);
		m_btnLanguage3.ShowWindow(FALSE);
		m_btnLanguage4.ShowWindow(FALSE);
		break;
	case 2:
		m_btnLanguage1.SetWindowText(vtorLang[0]);
		m_btnLanguage2.SetWindowText(vtorLang[1]);
		m_btnLanguage1.ShowWindow(TRUE);
		m_btnLanguage2.ShowWindow(TRUE);
		m_btnLanguage3.ShowWindow(FALSE);
		m_btnLanguage4.ShowWindow(FALSE);
		break;
	case 3:
		m_btnLanguage1.SetWindowText(vtorLang[0]);
		m_btnLanguage2.SetWindowText(vtorLang[1]);
		m_btnLanguage3.SetWindowText(vtorLang[2]);
		m_btnLanguage1.ShowWindow(TRUE);
		m_btnLanguage2.ShowWindow(TRUE);
		m_btnLanguage3.ShowWindow(TRUE);
		m_btnLanguage4.ShowWindow(FALSE);
		break;
	case 4:
		m_btnLanguage1.SetWindowText(vtorLang[0]);
		m_btnLanguage2.SetWindowText(vtorLang[1]);
		m_btnLanguage3.SetWindowText(vtorLang[2]);
		m_btnLanguage4.SetWindowText(vtorLang[3]);
		m_btnLanguage1.ShowWindow(TRUE);
		m_btnLanguage2.ShowWindow(TRUE);
		m_btnLanguage3.ShowWindow(TRUE);
		m_btnLanguage4.ShowWindow(TRUE);
		break;
	default:
		m_btnLanguage1.SetWindowText(vtorLang[0]);
		m_btnLanguage2.SetWindowText(vtorLang[1]);
		m_btnLanguage3.SetWindowText(vtorLang[2]);
		m_btnLanguage4.SetWindowText(_T("..."));
		m_btnLanguage1.ShowWindow(TRUE);
		m_btnLanguage2.ShowWindow(TRUE);
		m_btnLanguage3.ShowWindow(TRUE);
		m_btnLanguage4.ShowWindow(TRUE);
		break;
	}
}

void CDlgSetting::OnBnClickedRadioOnlyname1()
{
	// TODO: 在此添加控件通知处理程序代码
	m_nNameShow = E_ONLYNAME1;
}

void CDlgSetting::OnBnClickedRadioOnlyname2()
{
	// TODO: 在此添加控件通知处理程序代码
	m_nNameShow = E_ONLYNAME2;
}

void CDlgSetting::OnBnClickedRadioName1andname2()
{
	// TODO: 在此添加控件通知处理程序代码
	m_nNameShow = E_NAME1ANDNAME2;
}

void CDlgSetting::OnBnClickedRadioRushopen()
{
	// TODO: 在此添加控件通知处理程序代码
	m_nFoodSortType = E_RUSH;
}

void CDlgSetting::OnBnClickedRadioRushclose()
{
	// TODO: 在此添加控件通知处理程序代码
	m_nFoodSortType = E_ORDER_TIME;
}

void CDlgSetting::OnBnClickedButtonServerSet()
{
	// TODO: 在此添加控件通知处理程序代码
	ShellExecute(NULL, NULL, _T("ServerIpSetting.exe"), NULL, NULL,SW_NORMAL);
}



void CDlgSetting::EnableCtrl(BOOL enable)
{
	CButton* radioDetail = (CButton*)GetDlgItem(IDC_RADIO_DETIAL);
	CButton* radioOrder = (CButton*)GetDlgItem(IDC_RADIO_ORDER);
	radioDetail->EnableWindow(enable);
	radioOrder->EnableWindow(enable);
	
	CButton* radioName1Show = (CButton*)GetDlgItem(IDC_RADIO_ONLYNAME1);
	CButton* radioName2Show = (CButton*)GetDlgItem(IDC_RADIO_ONLYNAME2);
	CButton* radioName1AndName2Show = (CButton*)GetDlgItem(IDC_RADIO_NAME1ANDNAME2);
	radioName1Show->EnableWindow(enable);
	radioName2Show->EnableWindow(enable);
	radioName1AndName2Show->EnableWindow(enable);

	CButton* radioRushOpen = (CButton*)GetDlgItem(IDC_RADIO_RUSHOPEN);
	CButton* radioRushClose = (CButton*)GetDlgItem(IDC_RADIO_RUSHCLOSE);
	radioRushOpen->EnableWindow(enable);
	radioRushClose->EnableWindow(enable);

	CButton* checkAllItem = (CButton*)GetDlgItem(IDC_CHECK_ALLITEM);
	checkAllItem->EnableWindow(enable);
	EnableKitchenStalls(enable);

	CButton* radioShowCheckId = (CButton*)GetDlgItem(IDC_RADIO_SHOWCHECKID);
	CButton* radioHideCheckId = (CButton*)GetDlgItem(IDC_RADIO_HIDECHECKID);
	radioShowCheckId->EnableWindow(!enable);
	radioHideCheckId->EnableWindow(!enable);
	//m_ctrlCheckId.ShowWindow(!enable);
}

CString CDlgSetting::GetAppVersion()   
{   
	// 获取资源文件VS_VERSION_INFO里的FILEVERSION的值，也就是版本号
	TCHAR szAppPath[MAX_PATH] = {0};
	::GetModuleFileName(NULL, szAppPath, MAX_PATH);   

	VS_FIXEDFILEINFO *VInfo;   
	UINT i;   
	char *buf;   
	int ma,mj,r,b;   
	CString sReturn = _T("");   
	i = GetFileVersionInfoSize(szAppPath, 0);   
	buf=   new   char[i];   

	if (GetFileVersionInfo(szAppPath, 0, i, buf) != 0)   
	{ 
		if (VerQueryValue(buf,_T("\\"),(LPVOID*)&VInfo,&i))   
		{   
			ma = VInfo->dwFileVersionMS >> 16;   
			mj = VInfo->dwFileVersionMS & 0x00ff;   
			r = VInfo->dwFileVersionLS >> 16;   
			b = VInfo->dwFileVersionLS & 0x00ff;   
			sReturn.Format(_T("%02d.%02d.%02d.%02d"), ma,mj,r,b);    
		}   
	}    
	delete []buf;    
	return sReturn;   
}
void CDlgSetting::OnBnClickedRadioShowcheckid()
{
	// TODO: 在此添加控件通知处理程序代码
	m_bShowCheckId = TRUE;
}

void CDlgSetting::OnBnClickedRadioHidecheckid()
{
	// TODO: 在此添加控件通知处理程序代码
	m_bShowCheckId = FALSE;
}


void CDlgSetting::SetPrinterCombo(CRecordset &rs)
{
	m_comboPrint.ResetContent();
	CString strTmp;
	g_lang.LoadString(strTmp,IDS_STR_NOTPRINT);
	m_comboPrint.AddString(strTmp);
	m_comboPrint.SetItemData(0,0);

	CString strId, strName;
	long iId = 0;
	INT rsCount = rs.GetRecordCount();
	for (INT i = 0; !rs.IsEOF(); i++)
	{
		rs.GetFieldValue(_T("print_device_id"), strId);
		rs.GetFieldValue(_T("print_device_name"), strName);
		m_comboPrint.AddString(strName);
		iId = _ttoi(strId);
		m_comboPrint.SetItemData(i + 1,iId);
		if (iId == m_printerId)
		{
			m_comboPrint.SetWindowText(strName);
		}
		rs.MoveNext();
	}
	if (0 == m_printerId)
	{
		m_comboPrint.SetWindowText(strTmp);
	}
}
void CDlgSetting::OnCbnSelchangeComboPrint()
{
	// TODO: 在此添加控件通知处理程序代码
	m_printerId = (INT)m_comboPrint.GetItemData(m_comboPrint.GetCurSel());
}
