// KDSManager.cpp : ����Ӧ�ó��������Ϊ��
//

#include "stdafx.h"
#include "KDSManager.h"
#include "KDSManagerDlg.h"

#include <gdiplus.h>

using namespace Gdiplus;




#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CKDSManagerApp

BEGIN_MESSAGE_MAP(CKDSManagerApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CKDSManagerApp ����

CKDSManagerApp::CKDSManagerApp()
{
	// TODO: �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}


// Ψһ��һ�� CKDSManagerApp ����

CKDSManagerApp theApp;


// CKDSManagerApp ��ʼ��

BOOL CKDSManagerApp::InitInstance()
{
	// ���һ�������� Windows XP �ϵ�Ӧ�ó����嵥ָ��Ҫ
	// ʹ�� ComCtl32.dll �汾 6 ����߰汾�����ÿ��ӻ���ʽ��
	//����Ҫ InitCommonControlsEx()�����򣬽��޷��������ڡ�
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// ��������Ϊ��������Ҫ��Ӧ�ó�����ʹ�õ�
	// �����ؼ��ࡣ
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();

	AfxEnableControlContainer();

	// ��׼��ʼ��
	// ���δʹ����Щ���ܲ�ϣ����С
	// ���տ�ִ���ļ��Ĵ�С����Ӧ�Ƴ�����
	// ����Ҫ���ض���ʼ������
	// �������ڴ洢���õ�ע�����
	// TODO: Ӧ�ʵ��޸ĸ��ַ�����
	// �����޸�Ϊ��˾����֯��
	SetRegistryKey(_T("Ӧ�ó��������ɵı���Ӧ�ó���"));

	Gdiplus::GdiplusStartupInput gdiplusStartupInput;
	ULONG_PTR gdiplusToken;
	Gdiplus::GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

	// ���Գ�ʼ��
	g_lang.Initialize();

	// ʵ��Ψһ��
	// �Ƴ�ʵ��Ψһ������Ϊ���Ժͽп���ͬʱ����
	/*
#ifndef _DEBUG
	HANDLE hMutex = CreateMutex(NULL, TRUE, _T("COOLROID_KDS_CLIENT"));
	if ( GetLastError() == ERROR_ALREADY_EXISTS)
	{
		CString str;
		CloseHandle(hMutex);
		hMutex = NULL;
		g_lang.LoadString(IDS_ALREADYRUN,str);
		AfxMessageBox(str, MB_SYSTEMMODAL);
		return FALSE;
	}
#endif
	*/


	CKDSManagerDlg dlg;
	m_pMainWnd = &dlg;// ����Ҫ����ע����֮��

	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: �ڴ˴����ô����ʱ�á�ȷ�������ر�
		//  �Ի���Ĵ���
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: �ڴ˷��ô����ʱ�á�ȡ�������ر�
		//  �Ի���Ĵ���
	}

	GdiplusShutdown(gdiplusToken);


	// ���ڶԻ����ѹرգ����Խ����� FALSE �Ա��˳�Ӧ�ó���
	//  ����������Ӧ�ó������Ϣ�á�
	return FALSE;
}
