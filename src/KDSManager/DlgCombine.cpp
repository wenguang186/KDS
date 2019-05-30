// DlgCombine.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "KDSManager.h"
#include "DlgCombine.h"
#include "DlgItemDetailNew2.h"
#include "KDSManagerDlg.h"


// CDlgCombine �Ի���

IMPLEMENT_DYNAMIC(CDlgCombine, CDialog)

CDlgCombine::CDlgCombine(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgCombine::IDD, pParent)
{
	m_currentPage = 0;
	m_pageCols = 0;
	m_pageRows = 0;
}

CDlgCombine::~CDlgCombine()
{
	for (UINT i=0; i<m_pages.size(); i++)
	{
		delete m_pages[i];
	}
	m_pages.clear();
	m_pPageData.clear();
}

void CDlgCombine::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BTN_COMBINECONFIRM, m_btnCombineConfirm);
	DDX_Control(pDX, IDC_BTN_CANCEL, m_btnCancel);
}


BEGIN_MESSAGE_MAP(CDlgCombine, CDialog)
	ON_WM_CTLCOLOR()
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_BTN_COMBINECONFIRM, &CDlgCombine::OnBnClickedBtnCombineconfirm)
	ON_BN_CLICKED(IDC_BTN_CANCEL, &CDlgCombine::OnBnClickedBtnCancel)
END_MESSAGE_MAP()


// CDlgCombine ��Ϣ�������

HBRUSH CDlgCombine::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  �ڴ˸��� DC ���κ�����

	// TODO:  ���Ĭ�ϵĲ������軭�ʣ��򷵻���һ������
	return hbr;
}


BOOL CDlgCombine::OnEraseBkgnd(CDC* pDC)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	CRect rect;
	GetWindowRect(&rect);

	Gdiplus::Graphics graphics(pDC->m_hDC); // Create a GDI+ graphics object
	Gdiplus::Image image(_T(".\\res\\bg_image.jpg")); 
	graphics.DrawImage(&image, 0, 0, rect.Width(), rect.Height());	// �����汳��

	return TRUE;
	return CDialog::OnEraseBkgnd(pDC);
}

void CDlgCombine::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: �ڴ˴������Ϣ����������
	// ��Ϊ��ͼ��Ϣ���� CDialog::OnPaint()
}

BOOL CDlgCombine::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	m_nWidth = GetSystemMetrics(SM_CXSCREEN);   //��ȡ��Ļˮƽ�ֱ���;   SM_CXFULLSCREEN
	m_nHeight = GetSystemMetrics(SM_CYSCREEN);     //��ȡ��Ļ��ֱ�ֱ���;SM_CYFULLSCREEN

	INT nWidth = m_nWidth * 5 / 6;
	INT nHeight = m_nHeight * 5 / 6;
	INT nleft = (m_nWidth - nWidth) / 2;
	INT ntop = (m_nHeight - nHeight) / 2;
	MoveWindow(nleft,ntop,nWidth, nHeight);

	CRect rect;
	GetWindowRect(&rect);
	m_nWidth = rect.Width();
	m_nHeight = rect.Height();

	const INT rows = 2, cols = 2;
	INT btn_width = m_nWidth/10;
	INT btn_height = m_nHeight/15;
	INT btn_span = btn_width/8;
	INT left_pos = (m_nWidth-btn_width*rows)/2;
	INT top_pos= m_nHeight-btn_height-btn_height;

	m_btnCombineConfirm.MoveWindow(left_pos,top_pos,btn_width,btn_height,TRUE);
	m_btnCancel.MoveWindow(left_pos + btn_span + btn_width,top_pos,btn_width,btn_height,TRUE);

	// ���ݿ���
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

	// ���ó���ģʽ	
	m_kitchenMode = (KDS_Mode)GetPrivateProfileInt(_T("basic"), _T("KDS_MODE"), 0, _T(".\\config.ini"));
	m_dbmngr.ResetKitchenMode(m_kitchenMode);
	GetPrivateProfileString(_T("kitchen"), _T("kitchen"), _T("1,2,3"), m_strKitchenStalls.GetBuffer(256), 256, _T(".\\config.ini"));
	m_strKitchenStalls.ReleaseBuffer();
	m_dbmngr.ResetKitchenType(m_strKitchenStalls);

	ResetLang();

	ResetPages();
	UpdateItems();

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

void CDlgCombine::OnBnClickedBtnCombineconfirm()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	map<INT,INT> mapTmp;
	mapTmp.insert(make_pair(_ttoi(m_data.GetValue(_T("order_detail_id"))),_ttoi(m_data.GetValue(_T("quantity")))));
	BOOL bCombineFlag;
	INT pageSize = m_pages.size();
	for (INT i=0; i<pageSize; i++)
	{
		bCombineFlag = ((CDlgItemDetailNew2*)m_pages[i])->GetCombineFlag();
		if (bCombineFlag)
		{
			INT strId = _ttoi(((CDataOrderDetail*)m_pPageData[i])->GetValue(_T("order_detail_id")));
			INT num = _ttoi(((CDataOrderDetail*)m_pPageData[i])->GetValue(_T("quantity")));
			mapTmp.insert(make_pair(strId,num));
		}
	}
	AfxGetMainWnd()->SendMessage(WM_MSG_ADDCOMBINEITEM, (WPARAM)(&mapTmp), 0);
	OnOK();
}

void CDlgCombine::OnBnClickedBtnCancel()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	OnOK();
}

void CDlgCombine::ResetPages()
{
	INT nID = IDD_DLG_ITEM_DETAILNEW2;
	CRect rect,parentRect;
	UINT pageNum = 0;
	INT pageWidth, pageWidthSpan, pageHeightSpan, pageHeight, left;
	INT top = 10;

	// ��������ݣ����֮
	for (UINT i=0; i<m_pages.size(); i++)
	{
		delete m_pages[i];
	}
	m_pages.clear();
	m_pPageData.clear();

	// ��ʼ��item
	for (INT i=0;; i++)
	{
		CDialog *pPage = NULL;

		pPage = new CDlgItemDetailNew2;
		m_pPageData.push_back(((CDlgItemDetailNew2*)pPage)->GetDataPtr());
		
		
		if (pPage == NULL
			|| !pPage->Create(nID, this))
		{
			AfxMessageBox(_T("Error on creating item-pages."));
			return;
		}

		if (pageNum == 0)
		{
			CRect btnRect;
			m_btnCombineConfirm.GetWindowRect(&btnRect);
			pPage->GetWindowRect(&rect);
			pageWidth = rect.Width();
			pageHeight = rect.Height();//m_nHeight*13/15; // itemԼռȫ��13/15, btnԼռ1/15
			pageNum = m_nWidth/pageWidth;
			m_pageRows = m_nHeight / (rect.Height() + m_nHeight/15 + 10);
			m_pageCols = m_nWidth / pageWidth;
			pageWidthSpan = (m_nWidth - pageNum*pageWidth) / (pageNum+1);
			pageHeightSpan = (m_nHeight - m_pageRows*pageHeight - btnRect.Height() - m_nHeight/15) / (m_pageRows+1);
			top = pageHeightSpan;
		}

		left = i*(pageWidth+pageWidthSpan) + pageWidthSpan;

		INT n = m_pages.size() / pageNum;
		if (m_pages.size() >= pageNum)
		{
			pPage->GetWindowRect(&rect);
			top = rect.Height() * n + pageHeightSpan * (n+1);
			left = (i - pageNum * n)*(pageWidth+pageWidthSpan) + pageWidthSpan;
		}

		pPage->MoveWindow(left, top, pageWidth, pageHeight);
		//pPage->ShowWindow(SW_SHOW);
		m_pages.push_back(pPage);

		if (m_pages.size() >= pageNum * m_pageRows)
			break;
	}
}

void CDlgCombine::ClearData()
{
	m_data.ClearData();
	//UpdateData(FALSE);
}

void CDlgCombine::UpdateItems()
{
	INT pageSize = m_pages.size();
	INT fetchNum = 0;
	

	// �����ݿ����ȡ<������Ʒ> ��n��ʼȡm��
	CString strBeginId = m_data.GetValue(_T("order_detail_id"));
	INT beginId = (INT)_ttoi(strBeginId);

	for (INT i=0; i<pageSize; i++)
	{
		((CDlgItemDetailNew2*)m_pages[i])->ClearData();
	}
	fetchNum = m_dbmngr.GetSameItemDetail(E_S_DONING, beginId, pageSize, m_data, m_pData);

	fetchNum = JudgeCombine();

	for (INT i=0; i<fetchNum && i<pageSize; i++)
	{
		((CDlgItemDetailNew2*)m_pages[i])->m_bShowSelectPicture = TRUE;
		((CDlgItemDetailNew2*)m_pages[i])->bShowTable = TRUE;
		((CDlgItemDetailNew2*)m_pages[i])->m_data = *((CDataOrderDetail*)m_pData[i]);
		((CDlgItemDetailNew2*)m_pages[i])->ResetShowState(E_S_DONING);
		((CDlgItemDetailNew2*)m_pages[i])->RefreshData();
		((CDlgItemDetailNew2*)m_pages[i])->Invalidate();
		if (m_data.m_data[_T("CombineItem")].Find(((CDataOrderDetail*)m_pPageData[i])->m_data[_T("order_detail_id")]) >= 0)
		{
			((CDlgItemDetailNew2*)m_pages[i])->SetCombine(TRUE);
		}
		if (i == 0)
		{//��һ����Ʒ����ѡ��ϲ�
			((CDlgItemDetailNew2*)m_pages[i])->SetCombine(TRUE);
		}
	}
	
	for (INT i=0; i<fetchNum && i<pageSize; i++)
	{
		m_pages[i]->ShowWindow(SW_SHOW);
	}

	for (INT i=fetchNum; i<pageSize; i++)
	{
		m_pages[i]->ShowWindow(SW_HIDE);
	}

	((CDlgItemDetailNew2*)m_pages[m_currentPage])->SelectItemPage(TRUE);
	m_ItemShowCount = fetchNum >= pageSize?pageSize:fetchNum;
}


BOOL CDlgCombine::PreTranslateMessage(MSG* pMsg)
{
	// TODO: �ڴ����ר�ô����/����û���
	if(pMsg->message==WM_KEYDOWN)  //�ж��Ƿ��а�������
	{
		switch(pMsg->wParam)
		{
		case VK_RETURN:      //��ʾ�ǻس���
			OnBnClickedBtnCombineconfirm();
			pMsg->wParam = VK_ESCAPE; // �����س���ʱ���ϲ�����رգ��س�����Ϣ�����������棬���µ������ܽ���,��������Ѽ�����Ϣ�ĳ�VK_ESCAPE��
			return TRUE;
			break;
		case VK_F4:      //��ʾ�ǹ��ܼ��е�F4��
			OnBnClickedBtnCancel();
			break;
		case VK_F1:      //��ʾ�ǹ��ܼ��е�F1��
			((CDlgItemDetailNew2*)m_pages[m_currentPage])->PostMessage(WM_COMMAND, VK_F1, NULL);
			return TRUE;
			break;
		case VK_ESCAPE:      //��ʾ�ǹ��ܼ��е�ESC��
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
			refreshItemDlg(pMsg->wParam);
			return TRUE;
		default:
			break;
		}
	}

	return CDialog::PreTranslateMessage(pMsg);
}

void CDlgCombine::refreshItemDlg(WPARAM msg_wParam)
{
	((CDlgItemDetailNew2*)m_pages[m_currentPage])->SelectItemPage(FALSE);

	switch(msg_wParam)
	{
	case VK_DOWN:     //��ʾ�Ƿ�����е����µļ�
		if (m_currentPage + m_pageCols < m_ItemShowCount)
		{
			m_currentPage += m_pageCols;
		}
		break;
	case VK_UP:      //��ʾ�Ƿ�����е����ϵļ�
		if (m_currentPage - m_pageCols >= 0)
		{
			m_currentPage -= m_pageCols;
		}
		break;
	case VK_LEFT:      //��ʾ�Ƿ�����е�����ļ�
		if (m_currentPage  - 1 >= 0)
		{
			--m_currentPage;
		}
		break;
	case VK_RIGHT:      //��ʾ�Ƿ�����е����ҵļ�
		if (m_currentPage + 1 < m_ItemShowCount)
		{
			++m_currentPage;
		}
		break;
	default:
		msgNumKeyWP(msg_wParam);
		break;
	}
	((CDlgItemDetailNew2*)m_pages[m_currentPage])->SelectItemPage(TRUE);
}

void CDlgCombine::msgNumKeyWP(WPARAM msg_wParam)
{
	char ch;
	int iNum;
	if(msg_wParam <= 105 && msg_wParam >= 96) //VK_NUMPAD0~VK_NUMPAD9
	{
		UINT iAscii = MapVirtualKey(msg_wParam,2);
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

void CDlgCombine::ResetLang()
{
	CString caption;

	if (g_lang.m_strLang.Compare(_T("English")) == 0)
	{
		SetWindowText(_T("Select Combine"));
	}
	else
	{
		SetWindowText(_T("�ϲ�ѡ��"));
	}

	g_lang.LoadString(caption,IDS_STR_COOKCOMPLETE);
	m_btnCombineConfirm.SetWindowText(caption);
	g_lang.LoadString(caption,IDS_STR_CLOSEDLG);
	m_btnCancel.SetWindowText(caption);
}

INT CDlgCombine::JudgeCombine(void)
{
	CString strCombineList = ((CKDSManagerDlg*)AfxGetMainWnd())->GetCombineList();
	//ѭ���鿴�Ƿ��п��Ժϲ��Ĳ�Ʒ(��ͬ��������Ʒid����Ʒ���ʳ�÷�ʽ��������ע������)
	INT dataSize = m_pData.size();
	if (dataSize == 0)
	{
		return 0;
	}

	CString strCondimentPriced1,strCondimentPriced2;
	CString detailId1 = ((CDataOrderDetail*)m_pData[0])->GetValue(_T("order_detail_id"));
	((CKDSManagerDlg*)AfxGetMainWnd())->GetCondimentPriced(_ttoi(detailId1),strCondimentPriced1);
	CString tmp;
	tmp.Format(_T(",%s"),detailId1);
	BOOL firstDetailIdConbine = FALSE;
	if (strCombineList.Find(tmp,0) >= 0)
	{
		firstDetailIdConbine = TRUE;
	}

	for (INT i = 0;i < dataSize - 1;i++)
	{
		((CKDSManagerDlg*)AfxGetMainWnd())->GetCondimentPriced(_ttoi(((CDataOrderDetail*)m_pData[i + 1])->GetValue(_T("order_detail_id"))),strCondimentPriced2);
		if (strCondimentPriced1.Compare(strCondimentPriced2) == 0)
		{
			CString strItemType = m_pData[i]->GetValue(_T("item_type"));
			if (strItemType.Compare(_T("4")) == 0)
			{//�Զ����Ʒ
				if (m_pData[i]->GetValue(_T("menu_item_name")).Compare(m_pData[i + 1]->GetValue(_T("menu_item_name"))) != 0)
				{
					//��Ʒ��������ͬ���Ƴ�
					m_pData.erase(m_pData.begin() + i + 1);
					dataSize = m_pData.size();
				}
				else
				{
					tmp = ((CDataOrderDetail*)m_pData[i + 1])->GetValue(_T("order_detail_id"));
					CString tmp_1;
					tmp_1.Format(_T(",%s"),tmp);
					if (!firstDetailIdConbine)
					{
						if (strCombineList.Find(tmp_1,0) > 0)
						{
							//�Ѿ��ںϲ���Ʒ�У��Ƴ�
							m_pData.erase(m_pData.begin() + i + 1);
							dataSize = m_pData.size();
						}
					}
				}
			} 
			else
			{
				CString tmp = ((CDataOrderDetail*)m_pData[i + 1])->GetValue(_T("order_detail_id"));
				CString tmp_1;
				tmp_1.Format(_T(",%s"),tmp);
				if (!firstDetailIdConbine)
				{
					if (strCombineList.Find(tmp_1,0) > 0)
					{
						//�Ѿ��ںϲ���Ʒ�У��Ƴ�
						m_pData.erase(m_pData.begin() + i + 1);
						dataSize = m_pData.size();
					}
				}
			}
		}
		else
		{
			//��Ʒ��������ͬ���Ƴ�
			m_pData.erase(m_pData.begin() + i + 1);
			dataSize = m_pData.size();
		}
	}
	return m_pData.size();
}
