#pragma once

class CListCtrlEx : public CListCtrl
{
	DECLARE_DYNAMIC(CListCtrlEx)
public:
	afx_msg void OnCustomdraw(NMHDR* pNMHDR, LRESULT* pResult);
}