// KDSManagerDlg.h : 头文件
//

#pragma once

#include <stdio.h>
#include <vector>
#include <map>
#include <list>
//#include "DlgItem.h"
#include "dbmanager.h"
#include "common.h"
#include "datamodel.h"
#include "afxwin.h"
#include "RoundButton2.h"
#include "atlimage.h"

//#include "printer.h"

using namespace std;

//#define USE_OLD_DETAIL_PAGE 1

// CKDSManagerDlg 对话框
class CKDSManagerDlg : public CDialog
{
// 构造
public:
	CKDSManagerDlg(CWnd* pParent = NULL);	// 标准构造函数
	virtual ~CKDSManagerDlg();

// 对话框数据
	enum { IDD = IDD_KDSMANAGER_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual void WinHelp(DWORD dwData, UINT nCmd = HELP_CONTEXT);


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
	afx_msg LRESULT OnMsgRefresh(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnMsgDone(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnMsgPend(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnMsgOrder(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnMsgRecovery(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnMsgDoNotMake(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnMsgAddCombineItem(WPARAM wParam, LPARAM lParam);
	
public:
	afx_msg void OnBnClickedBtnPending();
	afx_msg void OnBnClickedBtnHistory();
	afx_msg void OnBnClickedBtnFirst();
	afx_msg void OnBnClickedBtnPre();
	afx_msg void OnBnClickedBtnNext();
	afx_msg void OnBnClickedBtnEnd();
	afx_msg void OnBnClickedBtnSetting();
	afx_msg void OnBnClickedBtnClose();
	afx_msg void OnBnClickedBtnServerip();
	afx_msg void OnBnClickedBtnCooking();
	afx_msg void OnTimer(UINT_PTR nIDEvent);

// 自定义
protected:
	//vector<CDialog*> m_items;
	//COrderItem** m_pItems; // 指向m_items.m_data，对话框初始化是malloc分配空间
	//CBaseData** m_pPageData; // 指向item.m_data，刷新时装载进去数据
	vector<CDialog*> m_pages;
	vector<CBaseData*> m_pPageData;

	INT	m_curPageIndex;
	BOOL m_beLastPage;
	UINT m_pageNum;
	INT m_pageRows;	//行
	INT	m_pageCols;	//列
	INT m_currentPage;
	
	INT m_timerId;
	INT m_timerElapse;

	INT m_timeIdCall;
	INT m_timeTick;

protected:
	void UpdateItems();
	void UpdateItemDetail();
	void UpdateItemOrder();
	INT GetItemCnt();
	INT GetItemCnt(KDS_ShowType type);

	// 用于取代TransferState
	void ResetShowType(KDS_ShowType newType);
	//void TransferState(KDS_ShowType oldType, KDS_ShowType newType);
	void RefreshPendMenuText();
	void RefreshCookMenuText();
	void RefreshHistoryMenuText();

	void ResetAvgTime();
	void ResetTimer();
	void ResetKitchenMode();
	// void ResetPageState(); // 被ResetShowType取代
	void ResetCurKitchenStalls();
	void ResetPages();
	void ResetLang();

	void UpdateDBProcess(CDataOrderInfo *pData, KDS_ShowType type);

public:
	CDBManager m_dbmngr;

	CString m_avgTime;

	KDS_Mode m_kitchenMode; // 厨房、传菜

	//INT m_kitchenType;

	//std::vector<INT> m_curKitchenStalls;
	CString m_strKitchenStalls;
	//std::map<INT, CString> m_kitchenStalls;

	KDS_ShowType m_showType;
	KDS_STYLE m_curStyle; // 单品、订单
	INT m_ItemShowCount; //菜品显示数量

public:
	KDS_Mode GetKitchenMode();
	INT GetRefreshTime();
	//INT LoadOrderHead(CRecordset &rs);
	void LoadOrderInfo(long order_head_id, CDlgOrder* pDlg, long check_id);
	//void LoadOrderDetail(long order_head_id, CDlgItemOrder* pDlg);

	INT LoadDataOrder(INT begin, INT num);

protected:
	INT m_nWidth;
	INT m_nHeight;
	INT m_x;
	INT m_y;
	HBITMAP m_bpBackgrd;
	INT m_xAvgTime;
	INT m_yAvgTime;
	HBITMAP m_bpAvgTime;
	CImage m_bpImageAvgTime;

public:
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
public:
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
public:
	CRoundButton2 m_btnPendList;
public:
	CRoundButton2 m_btnHistoryList;
public:
	CRoundButton2 m_btnFirst;
public:
	CRoundButton2 m_btnPre;
public:
	CRoundButton2 m_btnNext;
public:
	CRoundButton2 m_btnEnd;
public:
	CRoundButton2 m_btnSetting;
public:
	CRoundButton2 m_btnClose;
	CRoundButton2 m_btnServerIP;
public:
	CRoundButton2 m_btnCookList;

protected:
	Gdiplus::Image *m_bpBtnPendList[2]; 
	Gdiplus::Image *m_bpBtnHistoryList[2]; 
	Gdiplus::Image *m_bpBtnFirst[2]; 
	Gdiplus::Image *m_bpBtnPre[2]; 
	Gdiplus::Image *m_bpBtnNext[2]; 
	Gdiplus::Image *m_bpBtnEnd[2]; 
	Gdiplus::Image *m_bpBtnSetting[2]; 
	Gdiplus::Image *m_bpBtnClose[2]; 
	Gdiplus::Image *m_bpBtnReturn[2]; 
	Gdiplus::Image *m_bpBtnServerIP[2];
public:
	CStatic m_ctrlAvgTime;
public:
	INT m_doneCnt;

public:
	BOOL registerTest();

public:
	vector<CDataOrderDetail*> m_pData;	//保存从数据库读取的所有菜品记录
	typedef map<INT,INT> CombineSigleMap; //保存合并的菜品单项
	CombineSigleMap m_combineSingle;
	list<CombineSigleMap> m_combineList;	//合并的菜品
	CString GetCombineList(void);

public:
	INT DeleteCombineItem();

public:
	void refreshItemDlg(WPARAM msg_wParam);
	void msgNumKeyWP(WPARAM msg_wParam);
	void ShowItemDlgClickIn(BOOL bFlag);
	void ClickFunctionKeys(WPARAM msg_wParam);

public:
	E_ItemName_ShowType m_nNameShow;
	E_ItemName_ShowType GetNameShowType();

public:
	E_SORT_TYPE m_nFoodSortType;

public:
	void GetCondimentPriced(long order_detail_id, CString &val);
	void DrawCallInfo(CDC* pDC);
	CFont m_fontOrder;
	CFont m_fontTip;
	CString m_fontName;
	INT m_nDrawHight;
	BOOL m_bShowCheckId;


public:
	BOOL PlayVoice(CString str = _T("NewItem.wav"), BOOL bFileName = TRUE);

public:
	INT m_preDetailId;
	BOOL CheckHasNewItem(void);

public:
	vector<INT> m_preReturnItem;
	vector<INT> m_LastReturnItem;
	BOOL CheckItemReturn(void);
	BOOL isSubset(vector<INT> v1, vector<INT> v2);

	map<INT,INT> m_preRushItem;
	map<INT,INT> m_lastRushItem;
	BOOL CheckItemRush(void);
	BOOL isSubset(map<INT,INT> m1, map<INT,INT> m2, BOOL bCheckSecond = TRUE);

public:
	BOOL GetOrderHeadByDetailId(long detail_id, CDataOrder* buf);
	CString GetSetmealNameByOrderHeadId(CString id);
	KDS_STYLE GetKdsStype(void);
};
