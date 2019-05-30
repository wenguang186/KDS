// DlgLangSelect.cpp : 实现文件
//

#include "stdafx.h"
#include "KDSManager.h"
#include "DlgLangSelect.h"
#include "common.h"


// CDlgLangSelect 对话框

IMPLEMENT_DYNAMIC(CDlgLangSelect, CDialog)

CDlgLangSelect::CDlgLangSelect(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgLangSelect::IDD, pParent)
{

}

CDlgLangSelect::~CDlgLangSelect()
{
}

void CDlgLangSelect::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgLangSelect, CDialog)
	ON_WM_CTLCOLOR()
	ON_WM_ERASEBKGND()
	ON_CONTROL_RANGE(BN_CLICKED, MY_BUTTON_ID_START, MY_BUTTON_ID_START+MAX_BUTTON_NUM, &CDlgLangSelect::OnBnClickedRadio)
END_MESSAGE_MAP()


// CDlgLangSelect 消息处理程序

BOOL CDlgLangSelect::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	//	初始化语言选择框
	INT cnt = GetPrivateProfileInt(_T("LANGUAGES"), _T("COUNT"), 0, _T(".\\config.ini"));
	GetPrivateProfileString(_T("POS"), _T("LANGUAGE"), _T(""), m_strLanguage.GetBuffer(256), 256, _T(".\\config.ini"));

	CString lang;
	vector<CString> vtorLang;
	for (INT i=0; i<cnt; i++)
	{
		CString tmp;
		tmp.Format(_T("Lang%d"), i+1);
		GetPrivateProfileString(_T("LANGUAGES"), tmp, _T(""), lang.GetBuffer(256), 256, _T(".\\config.ini"));
		lang.ReleaseBuffer();
		vtorLang.push_back(lang);
	}

	RECT rect;
	((CButton*)GetDlgItem(IDC_BUTTON_LANGUAGE))->GetWindowRect(&rect);
	ScreenToClient(&rect);
	
	INT height = rect.bottom - rect.top;
	INT width = rect.right - rect.left;
	// 起始X,Y轴位置
	INT startX = rect.left;
	INT startY = rect.top;
	
	for (INT i = 0; i < cnt; i++)
	{
		rect.top = startY + height * i + 20 * i;
		rect.right = rect.left + width;
		rect.bottom = rect.top + height;

		CButton *pBtn = new CButton;
		if (!pBtn->Create(vtorLang[i], WS_CHILD|WS_VISIBLE|BS_AUTOCHECKBOX|BS_PUSHLIKE|WS_GROUP, rect, this, i + MY_BUTTON_ID_START))
		{
			POSMessageBox(IDS_STR_LOADLANGUAGE);
			break;
		}
		pBtn->SetFont(GetFont());
		m_btnVector.push_back(pBtn);
	}

	m_btnVector[0]->SetCheck(1);

	if (g_lang.m_strLang.Compare(_T("English")) == 0)
	{
		SetWindowText(_T("Language"));
	}
	else
	{
		SetWindowText(_T("语言选择"));
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

HBRUSH CDlgLangSelect::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  在此更改 DC 的任何属性

	// TODO:  如果默认的不是所需画笔，则返回另一个画笔
	return hbr;
}

BOOL CDlgLangSelect::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	return CDialog::OnEraseBkgnd(pDC);
}

void CDlgLangSelect::OnBnClickedRadio(UINT nID)
{
	// TODO: 在此添加控件通知处理程序代码
	//MessageBox(_T("hello"), _T("提示"), MB_OK);
	for (INT i = 0;i < m_btnVector.size();i++)
	{
		INT id = m_btnVector[i]->GetDlgCtrlID();
		if (id == nID)
		{
			m_btnVector[i]->SetCheck(1);
			m_btnVector[i]->GetWindowText(m_strLanguage);
		}
		else
		{
			m_btnVector[i]->SetCheck(0);
		}
	}

	OnOK();
}