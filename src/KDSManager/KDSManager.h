// KDSManager.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������

#define WM_MSG_REFRESH WM_USER+1

// 
#define WM_MSG_DONE WM_USER+10
#define WM_MSG_PEND WM_USER+11
#define WM_MSG_ORDER WM_USER+12
#define WM_MSG_RECOVERY WM_USER+13
#define WM_MSG_DONOTMAKE WM_USER+14
#define WM_MSG_ADDCOMBINEITEM WM_USER+15

#define WM_MSG_CHANGE_REFRESH_TIME WM_USER+20
#define WM_MSG_CHANGE_KITCHEN WM_USER+21

// CKDSManagerApp:
// �йش����ʵ�֣������ KDSManager.cpp
//

class CKDSManagerApp : public CWinApp
{
public:
	CKDSManagerApp();

// ��д
	public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()

};

extern CKDSManagerApp theApp;