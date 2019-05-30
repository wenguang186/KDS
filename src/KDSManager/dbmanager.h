#pragma once

#include <afxdb.h>
#include "resource.h"		// ������
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
	INT m_sortType; //����ʽ 0-�µ�ʱ�� 1-�߲�����
	
	// KDS��POS������������ϵͳ���ֱ�����������ݿ�
	// ��δ��KDSʱ��pos�¶���ʱ��is_makeĬ��NULL����δ��ʼ����
	// ��˿ͻ��ľ����ݣ������ڵ����ݸ��δ��ʼ�������ص�
	// Ϊ������ʾN��ǰ�����ݣ�������ʱ�������
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

	// ��ȡ��Ʒ��
	INT GetItemDetail(KDS_ShowType showType, INT begin, INT num, vector<CBaseData*> &buf);
	// ͨ���ص���ȡ����
	//INT GetItemOrder(KDS_ShowType showType, INT begin, INT num, vector<CBaseData*> &buf);
	INT GetItemCnt(KDS_ShowType showType, long order_head_id=0, long check_id=0);

	// ���ò�Ʒ״̬
	void SetOrderProcess(long order_head_id, long check_id, KDS_ShowType type);
	void SetDetailProcess(long order_detail_id, KDS_ShowType type, long order_head_id, long check_id);

	// ͨ���ص�����ȡ���ݿ���Ϣ
	void GetOrder(long order_head_id, CDlgOrder*pDlg, long check_id);
	void GetKitchenType(CWnd*pDlg);
	//INT GetOrderDetail(long order_head_id, CString, CDlgItemOrder*pDlg);

	INT GetItemOrderDetail(long order_head_id, CString tbl, CDataOrder* buf, long check_id=1);
	INT GetItemOrderHead(KDS_ShowType showType, INT begin, INT num, vector<CDataOrderHead*> &buf);
	INT GetItemOrderCnt(KDS_ShowType showType);

	//static INT GetEatTypeStrId(INT eatType);

	// ��ȡ��ͬ��Ʒ
	INT GetSameItemDetail(KDS_ShowType showType, INT beginId, INT num, CDataOrderDetail m_data, vector<CDataOrderDetail*> &buf);
	INT GetAllItemDetail(KDS_ShowType showType, vector<CDataOrderDetail*> &buf);

	// ����order_detail_id��ȡ��Ʒ����
	INT GetItemByDetailId(KDS_ShowType showType, CString detailId, CBaseData* buf);

	INT GetAvgCookTime();

	// ���������ֶ�
	void ResetKitchenType(CString &type);
	void ResetKitchenMode(KDS_Mode mode);
	void ResetSortType(E_SORT_TYPE type);

	// ��ȡ�Ӽ۵�ζƷ
	INT GetCondimentPriced(long order_detail_id, CString &val);
	void SetCondimentPriced(KDS_ShowType type, long order_detail_id);

	// ��ȡ�к�ģ��Ĳ�Ʒ
	INT GetItemOrderCheck(vector<CDataOrderDetail*> &buf);

	// ��ȡ�˲���Ϣ
	int GetReturnItems(vector<INT> &buf);

	// ��ȡ�߲���Ϣ
	int GetRushItems(map<INT,INT> &buf);


	// ����DetailId��ȡ����ͷ��Ϣ
	BOOL GetItemOrderHeadByDetailId(KDS_ShowType showType, CDataOrderHead* buf,long DetailId);

	// ��ȡ���д�ӡ����Ϣ
	void GetPrinter(CWnd*pDlg);

	// ��ȡ���µĲ�ƷDetail_id����������Ƿ����²�Ʒ�µ�
	INT GetLastDetailId(INT preDetailId);

	// ����order_head_id��ȡ�ײ����ƣ�������ײ͵Ļ�
	CString GetSetmealNameByOrderHeadId(CString order_head_id);

	// �ײ�֮��Ʒ�Ƿ�ȫ�����(�����ж��ײ�)
	BOOL IsSetmealComplete(long order_head_id, long check_id);
};