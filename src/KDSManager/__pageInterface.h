#pragma once

#include "datamodel.h"

class CPageInterface
{
public:
	virtual CBaseData *GetDataPtr() = 0;
	virtual void LoadHead(CRecordset &){};
	virtual INT LoadDetail(CRecordset &rs) = 0;
};