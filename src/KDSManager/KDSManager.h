// KDSManager.h : PROJECT_NAME 应用程序的主头文件
//

#pragma once

#ifndef __AFXWIN_H__
	#error "在包含此文件之前包含“stdafx.h”以生成 PCH 文件"
#endif

#include "resource.h"		// 主符号

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
// 有关此类的实现，请参阅 KDSManager.cpp
//

class CKDSManagerApp : public CWinApp
{
public:
	CKDSManagerApp();

// 重写
	public:
	virtual BOOL InitInstance();

// 实现

	DECLARE_MESSAGE_MAP()

};

extern CKDSManagerApp theApp;