#include "stdafx.h"
#include <assert.h>
#include "dbmanager.h"

#include "DlgSetting.h"
#include "KDSManagerDlg.h"


CDBManager::CDBManager()
{
	// 显示调用，以便检测数据库是否正常连接
	//SetBootTime();
}

void CDBManager::Init()
{
	SetBootTime();
}

void CDBManager::ResetKitchenType(CString &type)
{
	//m_kitchenType = type;
	m_kitchenStalls = type;
}

void CDBManager::ResetKitchenMode(KDS_Mode mode)
{
	m_kitchenMode = mode;
}

void CDBManager::ResetSortType(E_SORT_TYPE type)
{
	m_sortType = type;
}

inline void CDBManager::SetBootTime()
{
	try
	{
		OpenDB();
		CRecordset rs( &m_db);

		CString strSQL;
		strSQL.Format(_T("SELECT NOW() as now"));
		rs.Open(CRecordset::forwardOnly,strSQL);
		if(!rs.IsEOF())
		{
			CString var;
			rs.GetFieldValue(_T("now"),var);
			m_bootTime.Format(_T("%s"),var);
		}
		rs.Close();
		//AfxMessageBox(m_bootTime);

		strSQL.Format(_T("SELECT dayend_time FROM total_statistics"));
		rs.Open(CRecordset::forwardOnly,strSQL);
		if(!rs.IsEOF())
		{
			CString var;
			rs.GetFieldValue(_T("dayend_time"),var);
			m_startTime.Format(_T("%s"),var);
		}
		rs.Close();
		//AfxMessageBox(m_startTime);

		strSQL.Format(_T("SELECT date_sub(NOW(),interval 8 hour)"));
		rs.Open(CRecordset::forwardOnly,strSQL);
		if(!rs.IsEOF())
		{
			CString var;
			rs.GetFieldValue((short)0,var);
			if(var>m_startTime)
				m_startTime.Format(_T("%s"),var);
		}
		rs.Close();
		//AfxMessageBox(m_startTime);
	}
	catch(...)
	{
	}

}

BOOL CDBManager::ConnectTest()
{
	try
	{
		return OpenDB();
	}
	catch (...)
	{		
		return FALSE;
	}

	return TRUE;
}

inline BOOL CDBManager::OpenDB()
{
	if(!m_db.IsOpen())
		return m_db.OpenEx(_T("DSN=agile_pos;"), CDatabase::noOdbcDialog);
	
	return TRUE;
}

//INT CDBManager::runSql(CString strSQL, CRecordset *pRs)
//{
//	try
//	{
//		if(!m_db.IsOpen())
//			m_db.OpenEx(_T("DSN=agile_pos;"), CDatabase::noOdbcDialog);
//
//	}
//	catch (CException* e)
//	{
//		e->ReportError();
//	}
//}

inline KDS_Process CDBManager::GetProcessState(KDS_ShowType showType)
{
	// 2017.3.9 zzl 添加不可制作E_P_INVALID
	KDS_Process kds[E_M_CNT][E_P_CNT] = {
		{E_P_COOKING, E_P_COOKPEND, E_P_COOKED, E_P_INVALID}, 
		{E_P_COOKED, E_P_SENDPEND, E_P_SENDED, E_P_INVALID}};
	
	return kds[m_kitchenMode][showType];
}

inline void CDBManager::GetStrMakeValid(CString &strMake)
{
	// 默认当所有档口菜品完成时订单完成
	strMake.Format(_T("(is_make < %d or is_make is NULL) and menu_item.print_class>0"), E_P_INVALID);
}

inline void CDBManager::GetStrMake(KDS_Process isMake, BOOL isQuery, CString &strMake)
{
	switch(isMake)
	{
		case E_P_COOKING:
			if (isQuery)
			{
				strMake.Format(_T("is_make is NULL"));
			} 
			else
			{
				strMake.Format(_T("is_make=NULL"));
			}
			
			break;

		default:
			if (isQuery && m_kitchenMode == E_M_COOK && isMake == E_P_COOKED)
			{
				// 如果是厨房模式，并且查询历史cooked
				strMake.Format(_T("is_make in(%d,%d,%d,%d)"), E_P_COOKED, E_P_SENDED, E_P_SENDPEND, E_P_INVALID);	
			}
			else
			{
				strMake.Format(_T("is_make=%d"), isMake);
			}
			break;					
	}
}

void CDBManager::SetCookState(long order_detail_id, KDS_Process isMake)
{
	CString strSQL, strMake;
	CString strSQLHistory;

	GetStrMake(isMake, FALSE, strMake);
#ifdef INCLUDE_COOKED_TIME
	if (isMake == E_P_COOKED && m_kitchenMode == E_M_COOK) // 设置is_make=1也可能是传菜挂起恢复，此时不设置cooked_time
	{
		strMake += _T(",cooked_time=now()");
	}
#endif
	strSQL.Format(_T("update order_detail set %s where order_detail_id=%ld"), strMake, order_detail_id);
	strSQLHistory.Format(_T("update history_order_detail set %s where order_detail_id=%ld"), strMake, order_detail_id);
	DebugMessageBox(strSQL);

	try
	{
		OpenDB();
		m_db.ExecuteSQL(strSQL);
		m_db.ExecuteSQL(strSQLHistory);
	}
	catch (CException* e)
	{
		e->ReportError();
	}
}

/*************************************************************************
* Remark - 更新订单烹饪状态
*
* Modify - 170224 lv 增加check_id，处理分单
**************************************************************************/
void CDBManager::SetOrderState(long order_head_id, KDS_Process isMake, BOOL updateDetail, long check_id)
{
	CString strMakeHead, strMakeDetail;
	CString strSQLHead, strSQLHistoryHead, strSQLDetail, strSQLHistoryDetail;

	GetStrMake(isMake, FALSE, strMakeHead);
	GetStrMake(isMake, FALSE, strMakeDetail);
	if (isMake == E_P_COOKED && m_kitchenMode == E_M_COOK) // 设置is_make=1也可能是传菜挂起恢复，此时不设置cooked_time
	{
		strMakeHead += _T(",kds_time=now(),kds_show=0");
#ifdef INCLUDE_COOKED_TIME
		strMakeDetail += _T(",cooked_time=now()");
#endif
	}
	else if (isMake == E_P_SENDED)
	{
		strMakeHead += _T(",kds_time=now(),kds_show=0");
	}

	strSQLHead.Format(_T("update order_head set %s where order_head_id=%ld and check_id=%ld"), 
		strMakeHead, order_head_id, check_id);
	strSQLHistoryHead.Format(_T("update history_order_head set %s where order_head_id=%ld and check_id=%ld"), 
		strMakeHead, order_head_id, check_id);
	
	if (m_kitchenMode == E_M_COOK)
	{
		strSQLDetail.Format(_T("update order_detail set %s where order_head_id=%ld and check_id=%ld and is_make is NULL"), 
			strMakeDetail, order_head_id, check_id);
		strSQLHistoryDetail.Format(_T("update history_order_detail set %s where order_head_id=%ld and check_id=%ld and is_make is NULL"), 
			strMakeDetail, order_head_id, check_id);
	}

	if (m_kitchenMode == E_M_SEND)
	{
		strSQLDetail.Format(_T("update order_detail set %s where order_head_id=%ld and check_id=%ld and is_make=1"), 
			strMakeDetail, order_head_id, check_id);
		strSQLHistoryDetail.Format(_T("update history_order_detail set %s where order_head_id=%ld and check_id=%ld and is_make=1"), 
			strMakeDetail, order_head_id, check_id);
	}
	
	
	//DebugMessageBox(strSQL);

	try
	{
		OpenDB();
		m_db.ExecuteSQL(strSQLHead);
		m_db.ExecuteSQL(strSQLHistoryHead);
		if (updateDetail)
		{
			m_db.ExecuteSQL(strSQLDetail);
			m_db.ExecuteSQL(strSQLHistoryDetail);
		}		
	}
	catch (CException* e)
	{
		e->ReportError();
	}
}

void CDBManager::SetDetailProcess(long order_detail_id, KDS_ShowType type, long order_head_id, long check_id)
{
	SetCookState(order_detail_id, GetProcessState(type));
	if (type == E_S_DONE)
	{
		CString tmp;
		tmp.Format(_T("%d"),order_detail_id);
		tmp = GetSetmealNameByOrderHeadId(tmp);
		if (tmp.GetLength() > 0)
		{
			// 测试同一个order_head下的套餐菜品是否全部完成，完成则update order_head	
			if (IsSetmealComplete(order_head_id, check_id))
			{
				DebugMessageBox(_T("相关订单全部完成，更新订单状态"));
				//SetOrderComplete(order_head_id);
				SetOrderState(order_head_id, GetProcessState(type), TRUE, check_id);
			}
		}
		else
		{
			// 测试同一个order_head下的菜品是否全部完成，完成则update order_head	
			if (IsOrderComplete(order_head_id, check_id))
			{		
				DebugMessageBox(_T("相关订单全部完成，更新订单状态"));
				//SetOrderComplete(order_head_id);
				SetOrderState(order_head_id, GetProcessState(type), TRUE, check_id);
			}
		}
	}
}

void CDBManager::SetOrderProcess(long order_head_id, long check_id, KDS_ShowType type)
{
	SetOrderState(order_head_id, GetProcessState(type), TRUE, check_id);
}


/************************************************************************
* Remark -  烹饪 - 所有菜品烹饪完成，无挂起，无待做
*			传菜 - 所有菜品传菜完成，无烹饪挂起，无待烹饪，无传菜挂起
************************************************************************/
BOOL CDBManager::IsOrderComplete(long order_head_id, long check_id)
{
	// 默认当所有档口菜品完成时订单完成
	CString strMake;
	switch (m_kitchenMode)
	{
	case E_M_COOK:
		//strMake.Format(_T("(ISNULL(is_make) or is_make=3)"));
		GetStrMake(E_P_COOKED, TRUE, strMake);
		break;

	case E_M_SEND:
		// NULL不能用于比较运算符，NULL=2还算正常，不匹配，NULL!=2就会造成问题，也是不匹配，就变成了NULL=2的效果
		//strMake.Format(_T("(ISNULL(is_make) or is_make!=2)")); 
		GetStrMake(E_P_SENDED, TRUE, strMake);
		break;

	default:
		AfxMessageBox(_T("Unkown m_kitchenMode in IsOrderComplete."));
		break;
	}

	//return GetItemCnt(strMake, order_head_id, TRUE, ) == 0;
	CString strMakeValid;
	GetStrMakeValid(strMakeValid);
	// 只查分配档口的菜品
	strMake += _T(" and menu_item.print_class>0 and condiment_belong_item=0 ");
	strMakeValid += _T(" and condiment_belong_item=0 ");
	return (GetItemCnt(strMake, order_head_id, TRUE, check_id) 
		==  GetItemCnt(strMakeValid, order_head_id, TRUE, check_id));
}


/*************************************************************************
* Remark - order_head_id>0表示查询该订单的菜品进度情况，忽略菜品类型
*
**************************************************************************/
INT CDBManager::GetItemCnt(CString &strMake, long order_head_id, BOOL ignoreKitchenType, long check_id)
{
	CString strSQL, strSQLHistory, strHead, strKitchen;
	INT cnt = 0;
	
	// 如果果按订单查询，则不使用print_class
	// 考虑到界面可能不会区分档口，那么print_class也要改变为类似is_make的形式
	if (order_head_id > 0)
	{	
		strHead.Format(_T(" and order_head_id=%ld and check_id=%ld"), order_head_id, check_id);
	}
	
	if(!ignoreKitchenType)
	{
		strKitchen.Format(_T(" and menu_item.print_class in (%s)"), m_kitchenStalls);
	}

	// is_return_item=0 没有退掉
	// menu_item_id>0 不是折扣信息项，实际上order_detail.menu_item_id = menu_item.item_id已经暗含约束
	// order_detail与history_xx没有做区分是因为，POS与KDS独立，POS可能会在KDS不知情时，把order_detail移动到history
	// pint_class没用order_detail.pint_class，是因为未赋值，故使用menu_item.print_class
	strSQL.Format(
		_T("select count(*) as cnt\
		   from order_detail,menu_item\
		   where order_detail.menu_item_id = menu_item.item_id\
		   and %s\
		   and order_time>'%s'\
		   and is_return_item=0\
		   and is_send=1\
		   %s%s"), strMake, m_startTime, strHead, strKitchen);

	strSQLHistory.Format(
		_T("select count(*) as cnt\
		   from history_order_detail,menu_item\
		   where history_order_detail.menu_item_id = menu_item.item_id\
		   and %s\
		   and order_time>'%s'\
		   and is_return_item=0\
		   and is_send=1\
		   %s%s"), strMake, m_startTime, strHead, strKitchen);

	//DebugMessageBox(strSQL);
	//DebugMessageBox(strSQLHistory);

	try
	{
		CString tmp;
		
		OpenDB();
		CRecordset rs(&m_db);

		rs.Open(CRecordset::forwardOnly, strSQL);
		rs.GetFieldValue(_T("cnt"), tmp);
		cnt += _ttoi(tmp);
		rs.Close();

		rs.Open(CRecordset::forwardOnly, strSQLHistory);
		rs.GetFieldValue(_T("cnt"), tmp);
		cnt += _ttoi(tmp);
		rs.Close();		
	}
	catch (CException* e)
	{
		e->ReportError();
	}

	return cnt;
}



INT CDBManager::GetItemCnt(KDS_ShowType showType, long order_head_id, long check_id)
{	
	CString strFilter;//, strHistoryMake;

	GetStrMake(GetProcessState(showType), TRUE, strFilter);
	/*if (E_P_COOKING == GetProcessState(showType))
	{
		strFilter += _T(" and is_send=1");
	}*/
	if (E_STYLE_DETAIL == ((CKDSManagerDlg*)AfxGetMainWnd())->GetKdsStype())
	{
		strFilter += _T(" and menu_item.item_type!=3");
	}
	return GetItemCnt(strFilter, order_head_id, check_id);
}
/***
*
* Remark - 设计sql要注意，history_order_detail.(is_make,order_time)是联合索引，要注意书写顺序
*/
INT CDBManager::GetItems(INT begin, INT num, CString &strMake, vector<CBaseData*> &buf)
{
	INT retNum = 0;
	CString strSQL;
	
	// is_return_item=0 没有退掉
	// menu_item_id>0 不是折扣信息项，实际上order_detail.menu_item_id = menu_item.item_id已经暗含约束
	// order_detail与history_xx没有做区分是因为，POS与KDS独立，POS可能会在KDS不知情时，把order_detail移动到history
	// pint_class没用order_detail.pint_class，是因为未赋值，故使用menu_item.print_class
	// 添加查询字段menu_item_id，menu_item.item_name2,TIMESTAMPDIFF(MINUTE,order_time,NOW()) AS time_diff
	strSQL.Format(
		_T("select order_detail_id,order_detail.order_head_id,order_detail.check_id,menu_item_id,menu_item_name,\
					quantity,unit,description,order_detail.order_employee_name,order_detail.pos_name,order_detail.order_time,\
					order_head.remark,order_detail.eat_type,order_head.check_number,order_head.customer_num,order_head.table_name,\
					order_head.check_name,menu_item.item_name2,TIMESTAMPDIFF(MINUTE,order_time,NOW()) AS time_diff,\
					order_detail.condiment_belong_item,\
					order_detail.is_discount\
			from order_detail,order_head,menu_item\
			where order_detail.order_head_id = order_head.order_head_id\
				and order_detail.check_id = order_head.check_id\
				and order_detail.menu_item_id = menu_item.item_id\
				and order_detail.%s\
				and order_time>'%s'\
				and menu_item.print_class in(%s)\
				and is_return_item=0\
				and is_send=1\
			union\
			select order_detail_id,\
					history_order_detail.order_head_id,\
					history_order_detail.check_id,\
					menu_item_id,\
					menu_item_name,\
					quantity,\
					unit,\
					description,\
					history_order_detail.order_employee_name,\
					history_order_detail.pos_name,\
					history_order_detail.order_time,\
					history_order_head.remark,\
					history_order_detail.eat_type,\
					history_order_head.check_number,\
					history_order_head.customer_num,\
					history_order_head.table_name,\
					history_order_head.check_name,\
					menu_item.item_name2,\
					TIMESTAMPDIFF(MINUTE,order_time,NOW()) AS time_diff,\
					history_order_detail.condiment_belong_item,\
					history_order_detail.is_discount\
			from history_order_detail,history_order_head,menu_item\
			where history_order_detail.order_head_id = history_order_head.order_head_id\
				and history_order_detail.check_id = history_order_head.check_id\
				and history_order_detail.menu_item_id = menu_item.item_id\
				and history_order_detail.%s\
				and order_time>'%s'\
				and menu_item.print_class in(%s)\
				and is_return_item=0\
				and is_send=1\
			order by order_detail_id\
			limit %d,%d"), strMake, m_startTime, m_kitchenStalls, 
		strMake, m_startTime, m_kitchenStalls, begin, num);

	//DebugMessageBox(strSQL);

	try
	{
		OpenDB();
		CRecordset rs(&m_db);
		rs.Open(CRecordset::forwardOnly, strSQL);
		while(!rs.IsEOF())
		{
			buf[retNum]->LoadData(rs);
			//buf->LoadData(&rs);
			rs.MoveNext();
			retNum++;
			
			/*char msg[255]; 
			sprintf(msg, "num %d cnt %d.", retNum, rs.GetRecordCount());
			AfxMessageBox(CString(msg));*/
		}
		rs.Close();
	}
	catch (CException* e)
	{
		//AfxMessageBox(_T("发生异常"));
		e->ReportError();
	}

	/*char msg[255]; 
	sprintf(msg, "return %d vs %d.", retNum, num);
	AfxMessageBox(CString(msg));*/

	assert(retNum <= num);
	return retNum;
}

INT CDBManager::GetAllItems(CString &strMake, vector<CDataOrderDetail*> &buf)
{
	INT retNum = 0;
	CString strSQL;
	CString strSortType;
	switch(m_sortType)
	{
	case E_ORDER_TIME:
		strSortType = _T("order_detail_id");
		break;
	case E_RUSH:
		strSortType = _T("rush DESC,order_detail_id ASC");
		break;
	default:
		strSortType = _T("order_detail_id");
		break;
	}
	
	// is_return_item=0 没有退掉
	// menu_item_id>0 不是折扣信息项，实际上order_detail.menu_item_id = menu_item.item_id已经暗含约束
	// order_detail与history_xx没有做区分是因为，POS与KDS独立，POS可能会在KDS不知情时，把order_detail移动到history
	// pint_class没用order_detail.pint_class，是因为未赋值，故使用menu_item.print_class
	strSQL.Format(
		_T("select order_detail_id,order_detail.order_head_id,order_detail.check_id,menu_item_id,menu_item_name,\
					quantity,unit,description,order_detail.order_employee_name,order_detail.pos_name,order_detail.order_time,\
					order_head.remark,order_detail.eat_type,order_head.check_number,order_head.customer_num,order_head.table_name,\
					order_head.check_name,menu_item.item_name2,TIMESTAMPDIFF(MINUTE,order_time,NOW()) AS time_diff,rush,\
					order_detail.condiment_belong_item,\
					order_detail.is_discount,\
					menu_item.item_type,\
					order_head.kds_time,\
					order_detail.is_make \
			from order_detail,order_head,menu_item\
			where order_detail.order_head_id = order_head.order_head_id\
				and order_detail.check_id = order_head.check_id\
				and order_detail.menu_item_id = menu_item.item_id\
				and order_detail.%s\
				and order_time>'%s'\
				and menu_item.print_class in(%s)\
				and is_return_item=0\
				and is_send=1\
				and menu_item.item_type!=3\
			union\
			select order_detail_id,\
					history_order_detail.order_head_id,\
					history_order_detail.check_id,\
					menu_item_id,\
					menu_item_name,\
					quantity,\
					unit,\
					description,\
					history_order_detail.order_employee_name,\
					history_order_detail.pos_name,\
					history_order_detail.order_time,\
					history_order_head.remark,\
					history_order_detail.eat_type,\
					history_order_head.check_number,\
					history_order_head.customer_num,\
					history_order_head.table_name,\
					history_order_head.check_name,\
					menu_item.item_name2,\
					TIMESTAMPDIFF(MINUTE,order_time,NOW()) AS time_diff,\
					rush,\
					history_order_detail.condiment_belong_item,\
					history_order_detail.is_discount,\
					menu_item.item_type,\
					history_order_head.kds_time,\
					history_order_detail.is_make \
			from history_order_detail,history_order_head,menu_item\
			where history_order_detail.order_head_id = history_order_head.order_head_id\
				and history_order_detail.check_id = history_order_head.check_id\
				and history_order_detail.menu_item_id = menu_item.item_id\
				and history_order_detail.%s\
				and order_time>'%s'\
				and menu_item.print_class in(%s)\
				and is_return_item=0\
				and is_send=1 \
				and menu_item.item_type!=3\
			order by %s"), strMake, m_startTime, m_kitchenStalls, 
		strMake, m_startTime, m_kitchenStalls, strSortType);

	//DebugMessageBox(strSQL);

	try
	{
		OpenDB();
		CRecordset rs(&m_db);
		rs.Open(CRecordset::forwardOnly, strSQL);
		while(!rs.IsEOF())
		{
			CDataOrderDetail* data = new CDataOrderDetail;
			//buf[retNum]->LoadData(rs);
			data->LoadData(rs);
			buf.push_back(data);
			//buf->LoadData(&rs);
			rs.MoveNext();
			retNum++;
			//delete data;
			/*char msg[255]; 
			sprintf(msg, "num %d cnt %d.", retNum, rs.GetRecordCount());
			AfxMessageBox(CString(msg));*/
		}
		rs.Close();
	}
	catch (CException* e)
	{
		//AfxMessageBox(_T("发生异常"));
		e->ReportError();
	}

	/*char msg[255]; 
	sprintf(msg, "return %d vs %d.", retNum, num);
	AfxMessageBox(CString(msg));*/

	//assert(retNum <= num);
	return retNum;
}

INT CDBManager::GetAllItemDetail(KDS_ShowType showType, vector<CDataOrderDetail*> &buf)
{
	INT fetchNum = 0;
	CString strMake;
	GetStrMake(GetProcessState(showType), TRUE, strMake);
	return GetAllItems(strMake, buf);
}

INT CDBManager::GetItemDetail(KDS_ShowType showType, INT begin, INT num, vector<CBaseData*> &buf)
{
	INT fetchNum = 0;
	CString strMake;
	GetStrMake(GetProcessState(showType), TRUE, strMake);
	return GetItems(begin, num, strMake, buf);
}


BOOL CDBManager::GetOrderHead(long order_head_id, CString tbl, CDlgOrder*pDlg, long check_id)
{	
	BOOL rst = FALSE;

	CString strSQL;
	strSQL.Format(_T("select * from %s where order_head_id=%ld and check_id=%ld"), tbl, order_head_id, check_id);
	//AfxMessageBox(strSQL);

	try
	{
		OpenDB();
		CRecordset rs(&m_db);
		rs.Open(CRecordset::forwardOnly, strSQL);
		rst = pDlg->LoadHead(rs);
		rs.Close();
	}
	catch (CException* e)
	{
		//AfxMessageBox(_T("发生异常"));
		e->ReportError();
	}

	return rst;
}

INT CDBManager::GetOrderDetail(long order_head_id, CString tbl, CDlgOrder*pDlg, long check_id)
{	
	INT cnt = 0; 
	CString strSQL;
	// 170223 lv 加入退菜条件过滤，退菜也不显示
	// 170223 lv 加入check_id分单号
	//strSQL.Format(_T("select * from %s where order_head_id=%ld and menu_item_id>0 and is_return_item=0 and check_id=%ld"), 
	//				tbl, order_head_id, check_id);
	// 170227 lv 退菜照常显示，退菜在订单详情中显示为“退菜”
	strSQL.Format(_T("select * from %s where order_head_id=%ld and menu_item_id>0 and check_id=%ld"), 
					tbl, order_head_id, check_id);
	//AfxMessageBox(strSQL);

	try
	{
		OpenDB();
		CRecordset rs(&m_db);
		rs.Open(CRecordset::forwardOnly, strSQL);
		cnt = pDlg->LoadDetail(rs);
		rs.Close();
	}
	catch (CException* e)
	{
		//AfxMessageBox(_T("发生异常"));
		e->ReportError();
	}

	return cnt;
}

void CDBManager::GetOrder(long order_head_id, CDlgOrder*pDlg, long check_id)
{
	if (GetOrderHead(order_head_id, _T("order_head"), pDlg, check_id))
	{
		GetOrderDetail(order_head_id, _T("order_detail"), pDlg, check_id);
	} 
	else
	{
		GetOrderHead(order_head_id, _T("history_order_head"), pDlg, check_id);
		GetOrderDetail(order_head_id, _T("history_order_detail"), pDlg, check_id);
	}
}
/************************************************************************
*  Remark - 获取订单风格下的订单信息                                                                   
************************************************************************/
//INT CDBManager::GetItemOrder(KDS_ShowType showType, INT begin, INT num, vector<CDataOrder*> &buf)
//{
//
//}

INT CDBManager::GetItemOrderDetail(long order_head_id, CString tbl, CDataOrder* buf, long check_id)
{
	INT cnt = 0;
	CString strSQL;
	// 170223 lv - 修复分单显示的BUG，增加查询条件check_id
	strSQL.Format(_T("select * from %s where order_head_id=%ld and menu_item_id>0 and is_return_item=0 and check_id=%ld"), tbl, order_head_id, check_id);
	//AfxMessageBox(strSQL);

	try
	{
		OpenDB();
		CRecordset rs(&m_db);
		rs.Open(CRecordset::forwardOnly, strSQL);
		buf->LoadDetail(rs);
		rs.Close();
	}
	catch (CException* e)
	{		
		e->ReportError();
	}
	return cnt;
}

// 170225 lv - 这里面没有加分单ID的处理，运行起来还是对的
INT CDBManager::GetItemOrderCnt(KDS_ShowType showType)
{
	INT cnt = 0;
	CString strSQL, strMake;

	if (GetProcessState(showType) == E_P_COOKING)
	{
		// 好像是pos把order_head中的is_make设置为0了，张工发的数据库中NULL/0都有
		strMake.Format(_T("(is_make is NULL or is_make=0)"));
	}
	else
	{
		GetStrMake(GetProcessState(showType), TRUE, strMake);
	}

	// 不必细致的查看detail了，只要包含已送厨、未退菜的菜品就行了，其余看订单状态
	CString strDetailMake(_T("1=1"));

	strSQL.Format(
		_T("select sum(cnt) as cnt from\
			(select count(*) as cnt\
			FROM history_order_head\
			WHERE %s\
				and order_start_time>='%s'\
				and order_head_id IN\
					(SELECT DISTINCT order_head_id\
					FROM history_order_detail\
					WHERE is_return_item=0\
						AND %s\
						AND order_time>'%s'\
						AND is_send=1\
						AND menu_item_id>0)\
			union all\
			SELECT count(*) as cnt\
			FROM order_head\
			WHERE %s\
				and order_start_time>='%s'\
				and order_head_id IN\
					(SELECT DISTINCT order_head_id\
					FROM order_detail\
					WHERE is_return_item=0\
						AND %s\
						and order_time>'%s'\
						AND is_send=1\
						and menu_item_id>0)) tmp"),  strMake, m_startTime, strDetailMake, m_startTime,
			strMake, m_startTime, strDetailMake, m_startTime);
	try
	{
		CString tmp;
		
		OpenDB();
		CRecordset rs(&m_db);

		rs.Open(CRecordset::forwardOnly, strSQL);
		rs.GetFieldValue(_T("cnt"), tmp);
		cnt = _ttoi(tmp);
		rs.Close();
		
	}
	catch (CException* e)
	{
		e->ReportError();
	}

	return cnt;
}

// 170225 lv - 这里面没有加分单ID的处理，运行起来还是对的
INT CDBManager::GetItemOrderHead(KDS_ShowType showType, INT begin, INT num, vector<CDataOrderHead*> &buf)
{
	INT fetchCnt = 0;
	CString strMake;
	//GetStrMake(GetProcessState(showType), TRUE, strMake);
	if (GetProcessState(showType) == E_P_COOKING)
	{
		// 好像是pos把order_head中的is_make设置为0了，张工发的数据库中NULL/0都有
		strMake.Format(_T("(is_make is NULL or is_make=0)"));
	}
	else
	{
		//strMake.Format(_T("is_make=%d"), GetProcessState(showType));
		GetStrMake(GetProcessState(showType), TRUE, strMake);
	}

	// 订单风格下，忽视厨房档口的配置，待烹饪订单中必须有is_send=1的菜品
	// 同时注意order_detail聚集索引(is_make,order_time的顺序)
	CString strSQL;

	// 不必细致的查看detail了，只要包含已送厨、未退菜的菜品就行了，其余看订单状态
	CString strDetailMake(_T("1=1"));

	strSQL.Format(
		_T("SELECT %d as tableType, history_order_head.*\
		   FROM history_order_head\
		   WHERE %s\
				and order_start_time>='%s'\
				and order_head_id IN\
					(SELECT DISTINCT order_head_id\
					FROM history_order_detail\
					WHERE is_return_item=0\
						AND %s\
						AND order_time>'%s'\
						AND is_send=1\
						AND menu_item_id>0)\
			union all\
			SELECT %d as tableType, order_head.*\
			FROM order_head\
			WHERE %s\
				and order_start_time>='%s'\
				and order_head_id IN\
					(SELECT DISTINCT order_head_id\
					FROM order_detail\
					WHERE is_return_item=0\
						AND %s\
						and order_time>'%s'\
						AND is_send=1\
						and menu_item_id>0)\
			order by order_start_time ,kds_show DESC\
			LIMIT %d,%d"), E_HISTORY_ORDER, strMake, m_startTime, strDetailMake, m_startTime,
			E_TMP_ORDER, strMake, m_startTime, strDetailMake, m_startTime, begin, num);
	
	//DebugMessageBox(strSQL);

	try
	{
		OpenDB();
		CRecordset rs(&m_db);
		rs.Open(CRecordset::forwardOnly, strSQL);
		for (fetchCnt=0; !rs.IsEOF(); fetchCnt++, rs.MoveNext())
		{
			buf[fetchCnt]->LoadData(rs);			
		}
		rs.Close();
	}
	catch (CException* e)
	{
		e->ReportError();
	}

	return fetchCnt;
}

void CDBManager::GetKitchenType(CWnd*pDlg)
{
	CString strSQL;

	strSQL.Format(_T("select print_class_id,print_class_name from print_class order by print_class_id"));

	try
	{
		OpenDB();
		CRecordset rs(&m_db);

		rs.Open(CRecordset::forwardOnly, strSQL);
		((CDlgSetting*)pDlg)->SetKitchenType(rs);
		rs.Close();
	}
	catch (CException* e)
	{
		e->ReportError();
	}	
}

//这个函数的sql有问题，这个平均时间是为了厨师绩效的，但是怎么计算要重新设计
INT CDBManager::GetAvgCookTime()
{
	CString strSQL;
	INT avgTime = 0;
	
	strSQL.Format(
		_T("select avg(timespan)as avgTime\
		   from (SELECT TIMESTAMPDIFF(second, order_time, cooked_time) as timespan\
				from order_detail,menu_item\
				where order_detail.menu_item_id = menu_item.item_id\
					and cooked_time is not null\
					and order_time>'%s'\
					and menu_item.print_class in(%s)\
				union all\
				select TIMESTAMPDIFF(second, order_time, cooked_time) as timespan\
				from history_order_detail,menu_item\
				where history_order_detail.menu_item_id = menu_item.item_id\
					and cooked_time is not null\
					and order_time>'%s'\
					and menu_item.print_class in(%s))\
				as tmpTbl"), m_startTime, m_kitchenStalls, m_startTime, m_kitchenStalls);
	//DebugMessageBox(strSQL);
	try
	{
		//CDBVariant tmp;	
		CString tmp;

		OpenDB();
		CRecordset rs(&m_db);

		rs.Open(CRecordset::forwardOnly, strSQL);
		rs.GetFieldValue(_T("avgTime"), tmp);
		
		/*if (tmp.m_dwType != DBVT_NULL)
		{
			avgTime = _ttoi(tmp.m_pstringW);
		}*/
		if(!tmp.IsEmpty())
		{
			//avgTime = _ttoi(_T("333.333"));
			avgTime = _ttoi(tmp);
		}

		rs.Close();
	}
	catch (CException* e)
	{
		e->ReportError();
	}

	return avgTime;
}

INT CDBManager::GetSameItemDetail(KDS_ShowType showType, INT beginId, INT num, CDataOrderDetail m_data, vector<CDataOrderDetail*> &buf)
{
	INT retNum = 0;
	CString strSQL;

	CString strMake = _T("is_make is NULL");

	CString menu_item_id = m_data.GetValue(_T("menu_item_id"));
	CString unit = m_data.GetValue(_T("unit"));
	CString eat_type = m_data.GetValue(_T("eat_type"));
	CString description = m_data.GetValue(_T("description"));

	if (unit.GetLength() == 0)
	{
		unit.Format(_T("(unit is NULL or unit='')"));
	}
	else
	{
		unit.Format(_T("unit='%s'"),unit);
	}

	if (description.GetLength() == 0)
	{
		description.Format(_T("(description is NULL or description='')"));
	}
	else
	{
		description.Format(_T("description='%s'"),description);
	}
	
	// is_return_item=0 没有退掉
	// menu_item_id>0 不是折扣信息项，实际上order_detail.menu_item_id = menu_item.item_id已经暗含约束
	// order_detail与history_xx没有做区分是因为，POS与KDS独立，POS可能会在KDS不知情时，把order_detail移动到history
	// pint_class没用order_detail.pint_class，是因为未赋值，故使用menu_item.print_class
	strSQL.Format(
		_T("select order_detail_id,order_detail.order_head_id,order_detail.check_id,menu_item_name,\
					quantity,unit,description,order_detail.order_employee_name,order_detail.pos_name,order_detail.order_time,\
					order_head.remark,order_detail.eat_type,order_head.check_number,order_head.customer_num,order_head.table_name,\
					order_head.check_name,menu_item.item_name2,TIMESTAMPDIFF(MINUTE,order_time,NOW()) AS time_diff,rush,\
					order_detail.condiment_belong_item,\
					order_detail.is_discount,\
					menu_item.item_type \
			from order_detail,order_head,menu_item\
			where order_detail.order_head_id = order_head.order_head_id\
				and order_detail.check_id = order_head.check_id\
				and order_detail.menu_item_id = menu_item.item_id\
				and order_detail.%s\
				and order_time>'%s'\
				and menu_item.print_class in(%s)\
				and is_return_item=0\
				and is_send=1\
				and order_detail.order_detail_id>=%d\
				and menu_item_id=%s\
				and %s\
				and order_detail.eat_type=%s\
				and %s\
			union\
			select order_detail_id,\
					history_order_detail.order_head_id,\
					history_order_detail.check_id,\
					menu_item_name,\
					quantity,\
					unit,\
					description,\
					history_order_detail.order_employee_name,\
					history_order_detail.pos_name,\
					history_order_detail.order_time,\
					history_order_head.remark,\
					history_order_detail.eat_type,\
					history_order_head.check_number,\
					history_order_head.customer_num,\
					history_order_head.table_name,\
					history_order_head.check_name,\
					menu_item.item_name2,\
					TIMESTAMPDIFF(MINUTE,order_time,NOW()) AS time_diff,\
					rush,\
					history_order_detail.condiment_belong_item,\
					history_order_detail.is_discount,\
					menu_item.item_type \
			from history_order_detail,history_order_head,menu_item\
			where history_order_detail.order_head_id = history_order_head.order_head_id\
				and history_order_detail.check_id = history_order_head.check_id\
				and history_order_detail.menu_item_id = menu_item.item_id\
				and history_order_detail.%s\
				and order_time>'%s'\
				and menu_item.print_class in(%s)\
				and is_return_item=0\
				and is_send=1\
				and history_order_detail.order_detail_id>=%d\
				and menu_item_id=%s\
				and %s\
				and history_order_detail.eat_type=%s\
				and %s\
			order by order_detail_id limit 0,%d"), strMake, m_startTime, m_kitchenStalls, beginId, menu_item_id, unit, eat_type, 
			description,strMake, m_startTime, m_kitchenStalls, beginId, menu_item_id, unit, eat_type, description, num);

	//DebugMessageBox(strSQL);

	try
	{
		OpenDB();
		CRecordset rs(&m_db);
		rs.Open(CRecordset::forwardOnly, strSQL);
		while(!rs.IsEOF())
		{
			CDataOrderDetail* data = new CDataOrderDetail;
			//buf[retNum]->LoadData(rs);
			data->LoadData(rs);
			buf.push_back(data);
			rs.MoveNext();
			retNum++;
			
			/*char msg[255]; 
			sprintf(msg, "num %d cnt %d.", retNum, rs.GetRecordCount());
			AfxMessageBox(CString(msg));*/
		}
		rs.Close();
	}
	catch (CException* e)
	{
		//AfxMessageBox(_T("发生异常"));
		e->ReportError();
	}

	/*char msg[255]; 
	sprintf(msg, "return %d vs %d.", retNum, num);
	AfxMessageBox(CString(msg));*/

	assert(retNum <= num);
	return retNum;
}


INT CDBManager::GetItemByDetailId(KDS_ShowType showType, CString detailId, CBaseData* buf)
{
	INT retNum = 0;
	CString strSQL;

	CString strMake = _T("is_make is NULL");
	
	// is_return_item=0 没有退掉
	// menu_item_id>0 不是折扣信息项，实际上order_detail.menu_item_id = menu_item.item_id已经暗含约束
	// order_detail与history_xx没有做区分是因为，POS与KDS独立，POS可能会在KDS不知情时，把order_detail移动到history
	// pint_class没用order_detail.pint_class，是因为未赋值，故使用menu_item.print_class
	strSQL.Format(
		_T("select order_detail_id,\
					order_detail.order_head_id,\
					order_detail.check_id,\
					menu_item_name,\
					quantity,\
					unit,\
					description,\
					order_detail.order_employee_name,\
					order_detail.pos_name,\
					order_detail.order_time,\
					order_detail.eat_type,\
					menu_item.item_name2,\
					TIMESTAMPDIFF(MINUTE,order_time,NOW()) AS time_diff,\
					rush,\
					order_detail.condiment_belong_item,\
					order_detail.is_discount,\
					order_head.*\
			from order_detail,order_head,menu_item\
			where order_detail.order_head_id = order_head.order_head_id\
				and order_detail.check_id = order_head.check_id\
				and order_detail.menu_item_id = menu_item.item_id\
				and order_detail.%s\
				and order_time>'%s'\
				and menu_item.print_class in(%s)\
				and is_return_item=0\
				and is_send=1\
				and order_detail_id='%s'\
			union\
			select order_detail_id,\
					history_order_detail.order_head_id,\
					history_order_detail.check_id,\
					menu_item_name,\
					quantity,\
					unit,\
					description,\
					history_order_detail.order_employee_name,\
					history_order_detail.pos_name,\
					history_order_detail.order_time,\
					history_order_detail.eat_type,\
					menu_item.item_name2,\
					TIMESTAMPDIFF(MINUTE,order_time,NOW()) AS time_diff,\
					rush,\
					history_order_detail.condiment_belong_item,\
					history_order_detail.is_discount,\
					history_order_head.*\
			from history_order_detail,history_order_head,menu_item\
			where history_order_detail.order_head_id = history_order_head.order_head_id\
				and history_order_detail.check_id = history_order_head.check_id\
				and history_order_detail.menu_item_id = menu_item.item_id\
				and history_order_detail.%s\
				and order_time>'%s'\
				and menu_item.print_class in(%s)\
				and is_return_item=0\
				and is_send=1\
				and order_detail_id='%s'\
			order by order_detail_id"), strMake, m_startTime, m_kitchenStalls, detailId, strMake, m_startTime, m_kitchenStalls, detailId);

	//DebugMessageBox(strSQL);

	try
	{
		OpenDB();
		CRecordset rs(&m_db);
		rs.Open(CRecordset::forwardOnly, strSQL);
		while(!rs.IsEOF())
		{
			buf->LoadData(rs);
			//buf->LoadData(&rs);
			rs.MoveNext();
			retNum++;
			
			/*char msg[255]; 
			sprintf(msg, "num %d cnt %d.", retNum, rs.GetRecordCount());
			AfxMessageBox(CString(msg));*/
		}
		rs.Close();
	}
	catch (CException* e)
	{
		//AfxMessageBox(_T("发生异常"));
		e->ReportError();
	}

	/*char msg[255]; 
	sprintf(msg, "return %d vs %d.", retNum, num);
	AfxMessageBox(CString(msg));*/

	return retNum;
}

INT CDBManager::GetCondimentPriced(long order_detail_id, CString &val)
{
	CString strSQL;
	INT fetchCnt = 0;

	strSQL.Format(
		_T("select menu_item_name\
		   from order_detail\
		   where condiment_belong_item=%ld\
		   and order_time>'%s'\
		   union all\
		   select menu_item_name\
		   from history_order_detail\
		   where condiment_belong_item=%ld\
		   and order_time>'%s'"),
		   order_detail_id, m_startTime, order_detail_id, m_startTime);

	//DebugMessageBox(strSQL);
	try
	{
		//CDBVariant tmp;	
		CString tmp;

		OpenDB();
		CRecordset rs(&m_db);

		rs.Open(CRecordset::forwardOnly, strSQL);		

		for (fetchCnt=0; !rs.IsEOF(); fetchCnt++, rs.MoveNext())
		{
			rs.GetFieldValue(_T("menu_item_name"), tmp);
			val.AppendFormat(_T("%s "), tmp);
		}
		rs.Close();
	}
	catch (CException* e)
	{
		e->ReportError();
	}

	return fetchCnt;
}

void CDBManager::SetCondimentPriced(KDS_ShowType type, long order_detail_id)
{
	CString strSQL;
	CString strSQLHistory;

	KDS_Process is_make = GetProcessState(type);
	CString strMake;
	strMake.Format(_T("is_make=%d"),is_make);

	strSQL.Format(_T("update order_detail set %s where condiment_belong_item=%ld"), strMake, order_detail_id);
	strSQLHistory.Format(_T("update history_order_detail set %s where condiment_belong_item=%ld"), strMake, order_detail_id);
	DebugMessageBox(strSQL);

	try
	{
		OpenDB();
		m_db.ExecuteSQL(strSQL);
		m_db.ExecuteSQL(strSQLHistory);
	}
	catch (CException* e)
	{
		e->ReportError();
	}
}

INT CDBManager::GetItemOrderCheck(vector<CDataOrderDetail*> &buf)
{
	INT retNum = 0;
	CString strMake;
	strMake.Format(_T("(is_make is NULL or is_make in(%d,%d,%d,%d))"), E_P_COOKING, E_P_COOKED, E_P_COOKPEND, E_P_SENDPEND);

	CString strDetailMake;
	strDetailMake.Format(_T("(is_make is NULL or is_make in(%d,%d,%d,%d))"), E_P_COOKING, E_P_COOKED, E_P_COOKPEND, E_P_SENDPEND);

	CString strSQL;

	strSQL.Format(
		_T("SELECT %d as tableType, history_order_head.*\
		   FROM history_order_head\
		   WHERE %s\
		   and order_start_time>='%s'\
		   and order_head_id IN\
		   (SELECT DISTINCT order_head_id\
		   FROM history_order_detail\
		   WHERE is_return_item=0\
		   AND %s\
		   AND order_time>'%s'\
		   AND is_send=1\
		   AND menu_item_id>0)\
		   union all\
		   SELECT %d as tableType, order_head.*\
		   FROM order_head\
		   WHERE %s\
		   and order_start_time>='%s'\
		   and order_head_id IN\
		   (SELECT DISTINCT order_head_id\
		   FROM order_detail\
		   WHERE is_return_item=0\
		   AND %s\
		   and order_time>'%s'\
		   AND is_send=1\
		   and menu_item_id>0)\
		   order by order_start_time ,kds_show DESC"), E_HISTORY_ORDER, strMake, m_startTime, strDetailMake, m_startTime,
		   E_TMP_ORDER, strMake, m_startTime, strDetailMake, m_startTime);

	//DebugMessageBox(strSQL);

	try
	{
		OpenDB();
		CRecordset rs(&m_db);
		rs.Open(CRecordset::forwardOnly, strSQL);
		while(!rs.IsEOF())
		{
			CDataOrderDetail* data = new CDataOrderDetail;
			//buf[retNum]->LoadData(rs);
			data->LoadData(rs);
			buf.push_back(data);
			//buf->LoadData(&rs);
			rs.MoveNext();
			retNum++;
			//delete data;
			/*char msg[255]; 
			sprintf(msg, "num %d cnt %d.", retNum, rs.GetRecordCount());
			AfxMessageBox(CString(msg));*/
		}
		rs.Close();
	}
	catch (CException* e)
	{
		//AfxMessageBox(_T("发生异常"));
		e->ReportError();
	}

	/*char msg[255]; 
	sprintf(msg, "return %d vs %d.", retNum, num);
	AfxMessageBox(CString(msg));*/

	//assert(retNum <= num);
	return retNum;
}



int CDBManager::GetReturnItems(vector<INT> &buf)
{
	INT retNum = 0;
	CString strSQL;
	strSQL.Format(
		_T("select order_detail_id \
			from order_detail,order_head,menu_item\
			where order_detail.order_head_id = order_head.order_head_id\
				and order_detail.check_id = order_head.check_id\
				and order_detail.menu_item_id = menu_item.item_id\
				and order_time>'%s'\
				and menu_item.print_class in(%s)\
				and is_return_item>0\
				and is_send=1\
			"), m_startTime, m_kitchenStalls);

	//DebugMessageBox(strSQL);

	try
	{
		OpenDB();
		CRecordset rs(&m_db);
		rs.Open(CRecordset::forwardOnly, strSQL);
		while(!rs.IsEOF())
		{
			
			CString val;
			rs.GetFieldValue((short)0, val);
			buf.push_back((INT)_ttoi(val));
			rs.MoveNext();
			retNum++;
			//delete data;
			/*char msg[255]; 
			sprintf(msg, "num %d cnt %d.", retNum, rs.GetRecordCount());
			AfxMessageBox(CString(msg));*/
		}
		rs.Close();
	}
	catch (CException* e)
	{
		//AfxMessageBox(_T("发生异常"));
		e->ReportError();
	}

	/*char msg[255]; 
	sprintf(msg, "return %d vs %d.", retNum, num);
	AfxMessageBox(CString(msg));*/

	//assert(retNum <= num);
	return retNum;
	return 0;
}

int CDBManager::GetRushItems(map<INT,INT> &buf)
{
	INT retNum = 0;
	CString strSQL;
	strSQL.Format(
		_T("select order_detail_id,rush \
			from order_detail,order_head,menu_item\
			where order_detail.order_head_id = order_head.order_head_id\
				and order_detail.check_id = order_head.check_id\
				and order_detail.menu_item_id = menu_item.item_id\
				and order_time>'%s'\
				and menu_item.print_class in(%s)\
				and is_return_item=0\
				and is_send=1\
				and rush>0\
			"), m_startTime, m_kitchenStalls);

	//DebugMessageBox(strSQL);

	try
	{
		OpenDB();
		CRecordset rs(&m_db);
		rs.Open(CRecordset::forwardOnly, strSQL);
		while(!rs.IsEOF())
		{
			
			CString val1,val2;
			rs.GetFieldValue((short)0, val1);
			rs.GetFieldValue((short)1, val2);
			buf.insert(make_pair((INT)_ttoi(val1),(INT)_ttoi(val2)));
			rs.MoveNext();
			retNum++;
			//delete data;
			/*char msg[255]; 
			sprintf(msg, "num %d cnt %d.", retNum, rs.GetRecordCount());
			AfxMessageBox(CString(msg));*/
		}
		rs.Close();
	}
	catch (CException* e)
	{
		//AfxMessageBox(_T("发生异常"));
		e->ReportError();
	}

	/*char msg[255]; 
	sprintf(msg, "return %d vs %d.", retNum, num);
	AfxMessageBox(CString(msg));*/

	//assert(retNum <= num);
	return retNum;
	return 0;
}

BOOL CDBManager::GetItemOrderHeadByDetailId(KDS_ShowType showType, CDataOrderHead* buf, long DetailId)
{
	INT fetchCnt = 0;
	CString strMake;
	//GetStrMake(GetProcessState(showType), TRUE, strMake);
	if (GetProcessState(showType) == E_P_COOKING)
	{
		// 好像是pos把order_head中的is_make设置为0了，张工发的数据库中NULL/0都有
		strMake.Format(_T("(is_make is NULL or is_make=0)"));
	}
	else
	{
		//strMake.Format(_T("is_make=%d"), GetProcessState(showType));
		GetStrMake(GetProcessState(showType), TRUE, strMake);
	}

	// 订单风格下，忽视厨房档口的配置，待烹饪订单中必须有is_send=1的菜品
	// 同时注意order_detail聚集索引(is_make,order_time的顺序)
	CString strSQL;

	// 不必细致的查看detail了，只要包含已送厨、未退菜的菜品就行了，其余看订单状态
	CString strDetailMake(_T("1=1"));

	strSQL.Format(
		_T("SELECT %d as tableType, history_order_head.*\
		   FROM history_order_head\
		   WHERE %s\
		   and order_start_time>='%s'\
		   and order_head_id IN\
		   (SELECT DISTINCT order_head_id\
		   FROM history_order_detail\
		   WHERE is_return_item=0\
		   AND %s\
		   AND order_time>'%s'\
		   AND is_send=1\
		   AND menu_item_id>0\
		   and order_detail_id=%d)\
		   union all\
		   SELECT %d as tableType, order_head.*\
		   FROM order_head\
		   WHERE %s\
		   and order_start_time>='%s'\
		   and order_head_id IN\
		   (SELECT DISTINCT order_head_id\
		   FROM order_detail\
		   WHERE is_return_item=0\
		   AND %s\
		   and order_time>'%s'\
		   AND is_send=1\
		   and menu_item_id>0\
		   and order_detail_id=%d)\
		   order by order_start_time ,kds_show DESC"),
		   E_HISTORY_ORDER, strMake, m_startTime, strDetailMake, m_startTime, DetailId,
		   E_TMP_ORDER, strMake, m_startTime, strDetailMake, m_startTime, DetailId);

	//DebugMessageBox(strSQL);

	try
	{
		OpenDB();
		CRecordset rs(&m_db);
		rs.Open(CRecordset::forwardOnly, strSQL);
		buf->LoadData(rs);			
		rs.Close();
	}
	catch (CException* e)
	{
		e->ReportError();
	}

	return 0;
}


void CDBManager::GetPrinter(CWnd*pDlg)
{
	CString strSQL;

	strSQL.Format(_T("select * from print_devices"));

	try
	{
		OpenDB();
		CRecordset rs(&m_db);

		rs.Open(CRecordset::forwardOnly, strSQL);
		((CDlgSetting*)pDlg)->SetPrinterCombo(rs);
		rs.Close();
	}
	catch (CException* e)
	{
		e->ReportError();
	}	
}
INT CDBManager::GetLastDetailId(INT preDetailId)
{
	INT lastDetailId;
	CString strSQL;
	strSQL.Format(
		_T("SELECT order_detail_id FROM order_detail,menu_item\
		   WHERE menu_item_id=item_id\
		   and is_return_item=0\
		   AND is_make is NULL\
		   and order_time>'%s'\
		   AND is_send=1\
		   and menu_item_id>0\
		   and order_detail_id>%d\
		   and menu_item.print_class>0\
		   union all\
		   SELECT order_detail_id FROM history_order_detail,menu_item\
		   WHERE menu_item_id=item_id\
		   and is_return_item=0\
		   AND is_make is NULL\
		   and order_time>'%s'\
		   AND is_send=1\
		   and menu_item_id>0\
		   and order_detail_id>%d\
		   and menu_item.print_class>0\
		   order by order_detail_id DESC"), m_startTime, preDetailId, m_startTime, preDetailId);

	//DebugMessageBox(strSQL);

	try
	{
		OpenDB();
		CRecordset rs(&m_db);
		rs.Open(CRecordset::forwardOnly, strSQL);
		CString strLastDetailId;
		if (rs.IsEOF())
		{
			lastDetailId =  preDetailId;
		} 
		else
		{
			rs.GetFieldValue((short)0,strLastDetailId);
			lastDetailId = (INT)_ttoi(strLastDetailId);
		}
		rs.Close();
		return lastDetailId;
	}
	catch (CException* e)
	{
		e->ReportError();
	}
	return preDetailId;
}

CString CDBManager::GetSetmealNameByOrderHeadId(CString order_head_id)
{
	CString strSQL;
	strSQL.Format(_T("select menu_item_name from order_detail,menu_item \
		where condiment_belong_item=0\
		and menu_item_id=item_id\
		and menu_item.item_type=3\
		and order_head_id in (select order_head_id from order_detail \
		where order_detail_id=%s)\
		union \
		select menu_item_name from history_order_detail,menu_item \
		where condiment_belong_item=0\
		and menu_item_id=item_id\
		and menu_item.item_type=3\
		and order_head_id in (select order_head_id from history_order_detail \
		where order_detail_id=%s)"),order_head_id, order_head_id);
	try
	{
		CString tmp;
		OpenDB();
		CRecordset rs(&m_db);
		rs.Open(CRecordset::forwardOnly, strSQL);
		if (rs.IsEOF())
		{
			return _T("");
		} 
		else
		{
			rs.GetFieldValue((short)0,tmp);
		}
		rs.Close();
		return tmp;
	}
	catch (CException* e)
	{
		e->ReportError();
	}
	return _T("");
}

BOOL CDBManager::IsSetmealComplete(long order_head_id, long check_id)
{
	// 默认当所有档口菜品完成时订单完成
	CString strMake;
	switch (m_kitchenMode)
	{
	case E_M_COOK:
		//strMake.Format(_T("(ISNULL(is_make) or is_make=3)"));
		GetStrMake(E_P_COOKED, TRUE, strMake);
		break;

	case E_M_SEND:
		// NULL不能用于比较运算符，NULL=2还算正常，不匹配，NULL!=2就会造成问题，也是不匹配，就变成了NULL=2的效果
		//strMake.Format(_T("(ISNULL(is_make) or is_make!=2)")); 
		GetStrMake(E_P_SENDED, TRUE, strMake);
		break;

	default:
		AfxMessageBox(_T("Unkown m_kitchenMode in IsOrderComplete."));
		break;
	}

	//return GetItemCnt(strMake, order_head_id, TRUE, ) == 0;
	CString strMakeValid;
	GetStrMakeValid(strMakeValid);
	// 只查分配档口的菜品
	strMake += _T(" and menu_item.print_class>0 and condiment_belong_item<0");
	strMakeValid += _T(" and condiment_belong_item<0");
	return (GetItemCnt(strMake, order_head_id, TRUE, check_id) 
		==  GetItemCnt(strMakeValid, order_head_id, TRUE, check_id));
}
