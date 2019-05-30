#pragma once

#include <afxdb.h>
#include <vector>
#include <map>

using namespace std;

class CBaseData
{
public:
	virtual void LoadData(CRecordset &rs) {};
	virtual void ClearData() {};
};

class CDataOrderInfo:public CBaseData
{
public:
	map<CString, CString> m_data;

public:
	void LoadData(CRecordset &rs);
	void ClearData();
	const CString& GetValue(CString); 
};

class CDataOrderDetail:public CDataOrderInfo
{

};


class CDataOrderHead:public CDataOrderInfo
{

};

class CDataOrder:public CBaseData
{
public:
	~CDataOrder(); // �ͷ�vectorָ��

public:
	CDataOrderHead m_head;
	std::vector<CDataOrderDetail*> m_detail; // ���Ϸ��䣬����ʱ�ǵ��ͷ�

public:
	void LoadData(CRecordset &rs);
	void ClearData();
	INT LoadDetail(CRecordset &rs);
};