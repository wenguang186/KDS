#pragma once

#include <afxdb.h>
#include "resource.h"		// 主符号
#include "DlgOrder.h"
#include "common.h"
//#include "DlgItemOrder.h"
#include "datamodel.h"

//#include "printer.h"

class CDBManager
{
protected:
	CDatabase m_db;
	//INT m_kitchenType;
	KDS_Mode m_kitchenMode;
	CString m_kitchenStalls;
	INT m_sortType; //排序方式 0-下单时间 1-催菜优先
	
	// KDS与POS是两个独立的系统，分别独立访问数据库
	// 在未上KDS时，pos下订单时，is_make默认NULL，即未开始制作
	// 因此客户的旧数据，与现在的数据概念（未开始制作）重叠
	// 为避免显示N年前的数据，故增加时间段限制
	CString m_startTime; 
	CString m_bootTime;

public:
	
	void Init();
	BOOL ConnectTest();

protected:
	BOOL OpenDB();
	void SetBootTime();
	INT GetItems(INT begin, INT num, CString &strMake, vector<CBaseData*> &buf);
	INT GetAllItems(CString &strMake, vector<CDataOrderDetail*> &buf);
	void SetCookState(long order_detail_id, KDS_Process isMake);

	BOOL GetOrderHead(long order_head_id, CString tbl, CDlgOrder*pDlg, long check_id=1);
	INT GetOrderDetail(long order_head_id, CString tbl, CDlgOrder*pDlg, long check_id=1);


	//void SetOrderComplete(long order_head_id);
	void SetOrderState(long order_head_id, KDS_Process isMake, BOOL updateDetail=TRUE, long check_id=1);

	KDS_Process GetProcessState(KDS_ShowType);
	void GetStrMake(KDS_Process isMake, BOOL isQuery, CString &strMake);
	INT GetItemCnt(CString &strFilter, long order_head_id=0, BOOL ignoreKitchenType=FALSE, long check_id=0);
	BOOL IsOrderComplete(long order_head_id, long check_id);

	//INT GetUnmakeItems(INT begin, INT num, COrderItem** buf);
	//INT GetPendedItems(INT begin, INT num, COrderItem** buf);
	//INT GetHistoryItems(INT begin, INT num, COrderItem** buf);

	void GetStrMakeValid(CString &strMake);

public:
	CDBManager();

	// 读取菜品单
	INT GetItemDetail(KDS_ShowType showType, INT begin, INT num, vector<CBaseData*> &buf);
	// 通过回调读取订单
	//INT GetItemOrder(KDS_ShowType showType, INT begin, INT num, vector<CBaseData*> &buf);
	INT GetItemCnt(KDS_ShowType showType, long order_head_id=0, long check_id=0);

	// 设置菜品状态
	void SetOrderProcess(long order_head_id, long check_id, KDS_ShowType type);
	void SetDetailProcess(long order_detail_id, KDS_ShowType type, long order_head_id, long check_id);

	// 通过回调，获取数据库信息
	void GetOrder(long order_head_id, CDlgOrder*pDlg, long check_id);
	void GetKitchenType(CWnd*pDlg);
	//INT GetOrderDetail(long order_head_id, CString, CDlgItemOrder*pDlg);

	INT GetItemOrderDetail(long order_head_id, CString tbl, CDataOrder* buf, long check_id=1);
	INT GetItemOrderHead(KDS_ShowType showType, INT begin, INT num, vector<CDataOrderHead*> &buf);
	INT GetItemOrderCnt(KDS_ShowType showType);

	//static INT GetEatTypeStrId(INT eatType);

	// 读取相同菜品
	INT GetSameItemDetail(KDS_ShowType showType, INT beginId, INT num, CDataOrderDetail m_data, vector<CDataOrderDetail*> &buf);
	INT GetAllItemDetail(KDS_ShowType showType, vector<CDataOrderDetail*> &buf);

	// 根据order_detail_id获取菜品数据
	INT GetItemByDetailId(KDS_ShowType showType, CString detailId, CBaseData* buf);

	INT GetAvgCookTime();

	// 重置配置字段
	void ResetKitchenType(CString &type);
	void ResetKitchenMode(KDS_Mode mode);
	void ResetSortType(E_SORT_TYPE type);

	// 读取加价调味品
	INT GetCondimentPriced(long order_detail_id, CString &val);
	void SetCondimentPriced(KDS_ShowType type, long order_detail_id);

	// 获取叫号模块的菜品
	INT GetItemOrderCheck(vector<CDataOrderDetail*> &buf);

	// 获取退菜信息
	int GetReturnItems(vector<INT> &buf);

	// 获取催菜信息
	int GetRushItems(map<INT,INT> &buf);


	// 根据DetailId获取订单头信息
	BOOL GetItemOrderHeadByDetailId(KDS_ShowType showType, CDataOrderHead* buf,long DetailId);

	// 获取所有打印机信息
	void GetPrinter(CWnd*pDlg);

	// 获取最新的菜品Detail_id，用来检查是否有新菜品下单
	INT GetLastDetailId(INT preDetailId);

	// 根据order_head_id获取套餐名称，如果是套餐的话
	CString GetSetmealNameByOrderHeadId(CString order_head_id);

	// 套餐之菜品是否全部完成(用于判断套餐)
	BOOL IsSetmealComplete(long order_head_id, long check_id);
};