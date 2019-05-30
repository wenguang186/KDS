#include "stdafx.h"
#include <afxdb.h>
#include "datamodel.h"


void CDataOrderInfo::LoadData(CRecordset &rs)
{
	CString val;
	CODBCFieldInfo fieldinfo;
	for( short index = 0; index < rs.GetODBCFieldCount(); index++ )
	{
		rs.GetFieldValue(index, val);
		rs.GetODBCFieldInfo(index, fieldinfo);
		m_data[fieldinfo.m_strName] = val;
	}
}

const CString& CDataOrderInfo::GetValue(CString strKey) 
{
	return m_data[strKey];
}

void CDataOrderInfo::ClearData()
{
	m_data.clear();
}

CDataOrder::~CDataOrder()
{
	for (UINT i=0; i<m_detail.size(); i++)
	{
		delete m_detail[i];
	}
}

void CDataOrder::LoadData(CRecordset &rs)
{
	m_head.LoadData(rs);
}

INT CDataOrder::LoadDetail(CRecordset &rs)
{
	INT cnt;
	for (cnt=0; !rs.IsEOF(); cnt++, rs.MoveNext())
	{
		CDataOrderDetail* pDetail = new CDataOrderDetail;
		pDetail->LoadData(rs);
		m_detail.push_back(pDetail);
	}
	return cnt;
}

void CDataOrder::ClearData()
{
	m_head.ClearData();
	for (UINT i=0; i<m_detail.size(); i++)
	{
		delete m_detail[i];
	}
	m_detail.clear();
}