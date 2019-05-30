#include "stdafx.h"

#include "ListCtrlEx.h"


IMPLEMENT_DYNAMIC(CListCtrlEx, CListCtrl)

ON_NOTIFY_REFLECT ( NM_CUSTOMDRAW, OnCustomdraw )

void CListCtrlEx::OnCustomdraw ( NMHDR* pNMHDR, LRESULT* pResult )
{

	NMLVCUSTOMDRAW* pLVCD = reinterpret_cast<NMLVCUSTOMDRAW*>( pNMHDR );

	*pResult = 0;

	USES_CONVERSION;
	// Request item-specific notifications if this is the
	// beginning of the paint cycle.

	if ( CDDS_PREPAINT == pLVCD->nmcd.dwDrawStage )
	{
		*pResult = CDRF_NOTIFYITEMDRAW;
	}
	else if ( CDDS_ITEMPREPAINT == pLVCD->nmcd.dwDrawStage )
	{
		// This is the beginning of an item's paint cycle.
		LVITEM   rItem;
		int      nItem = static_cast<int>( pLVCD->nmcd.dwItemSpec );
		CDC*     pDC   = CDC::FromHandle ( pLVCD->nmcd.hdc );
		COLORREF crBkgnd;
		BOOL     bListHasFocus;
		CRect    rcItem;
		CRect    rcText, rcText1;
		CString  sText;
		UINT     uFormat;
		CImageList* pImage = CListCtrl::GetImageList(LVSIL_SMALL);
		bListHasFocus = (GetSafeHwnd() == ::GetFocus() );

		// Get the image index and selected/focused state of the
		// item being drawn.
		ZeroMemory ( &rItem, sizeof(LVITEM) );
		rItem.mask  = LVIF_IMAGE | LVIF_STATE | LVIF_TEXT;
		rItem.iItem = nItem;
		rItem.stateMask = LVIS_SELECTED | LVIS_FOCUSED;
		GetItem ( &rItem );
		// Get the rect that bounds the text label.
		GetItemRect ( nItem, rcItem, LVIR_BOUNDS );

		// Draw the background of the list item.  Colors are selected 
		// according to the item's state.

		if ( rItem.state & LVIS_SELECTED )
		{
			crBkgnd = RGB(198,227,231);

		}
		else
		{
			crBkgnd = RGB(255,255,255);
		}
		// Draw the background & prep the DC for the text drawing.  Note
		// that the entire item RECT is filled in, so this emulates the full-
		// row selection style of normal lists.
		pDC->FillSolidRect ( rcItem, crBkgnd );
		pDC->SetBkMode ( TRANSPARENT );

		if (pImage != NULL)
		{
			// Get the rect that holds the item's icon.
			GetItemRect ( nItem, &rcItem, LVIR_ICON );

			// Draw the icon.
			uFormat = ILD_TRANSPARENT;
			pImage->Draw ( pDC, rItem.iImage, rcItem.TopLeft(), uFormat );
		}

		// Draw the background & prep the DC for the text drawing.  Note
		// that the entire item RECT is filled in, so this emulates the full-
		// row selection style of normal lists.

		GetItemRect ( nItem, rcItem, LVIR_LABEL );

		pDC->FillSolidRect ( rcItem, crBkgnd );
		pDC->SetBkMode ( TRANSPARENT );


		// Tweak the rect a bit for nicer-looking text alignment.
		rcText1 = rcText = rcItem;
		// rcText.left += 3;
		rcText.top += 3;
		rcText.bottom -= rcText.Height()*2/5;

		rcText1.top = rcText.bottom;

		// Draw the text.
		CALLLOGPTR pItem = (CALLLOGPTR)GetItemData(nItem);

		if (pItem != NULL)
		{ 
			if (_tcslen((WCHAR*)pItem->strName) != 0)
			{
				sText = (WCHAR*)pItem->strName;
				if (_tcscmp((WCHAR*)pItem->strContact, 
					(WCHAR*)pItem->strName) != 0)
				{
					sText += _T("(") + CString((WCHAR*)pItem->strContact) + _T(")");
				}
			}
			else 
				sText = (WCHAR*)pItem->strContact;

			// pDC->DrawText ( sText, rcText, DT_VCENTER | DT_SINGLELINE );

			CFont vertFont,tipFont;
			CFont *pOldFont;

			//×ÖÌå
			vertFont.CreateFont(15, 0, 0, 0, FW_NORMAL,
				0, 0, 0, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS,
				CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
				DEFAULT_PITCH, _T("MS Sans Serif"));
			tipFont.CreateFont(10, 0, 0, 0, FW_NORMAL,
				0, 0, 0, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS,
				CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
				DEFAULT_PITCH, _T("MS Sans Serif"));

			pOldFont = pDC->SelectObject(&vertFont);
			pDC->DrawText (sText, rcText, DT_VCENTER | DT_SINGLELINE);

			CTime m_Time((time_t)(pItem->dwBeginTime));
			CString txtBelow = _T("");
			txtBelow.Format(_T("%d-%02d-%02d %02d:%02d"),m_Time.GetYear(), m_Time.GetMonth(),
				m_Time.GetDay(),m_Time.GetHour(), m_Time.GetMinute());
			if (pItem->dwLasting != 0)
			{
				CString callTime;
				int day, hour, minute, second, tmp;
				day = pItem->dwLasting / (24*60*60);
				tmp = pItem->dwLasting % (24*60*60);
				hour = tmp / (60 * 60);
				tmp = tmp % (60 * 60);
				minute = tmp / 60;
				second = tmp % 60;
				if (day != 0)
					callTime.Format(_T("%d, %d:%02d:%02d"), day, hour, minute, second);
				else if (hour != 0)
					callTime.Format(_T("%d:%02d:%02d"), hour, minute, second);
				else
					callTime.Format(_T("%02d:%02d"),minute, second);
				CSyslogDlg *pParent = NULL;
				pParent = (CSyslogDlg*)this->GetParent();

				txtBelow = txtBelow + _T("  ") + pParent->m_Duration + _T("  ") + callTime;
			}
			pDC->SelectObject(&tipFont);
			COLORREF  oldTxtCor = pDC->GetTextColor();
			pDC->SetTextColor(RGB(128,128,128));    
			pDC->DrawText ( txtBelow, rcText1, DT_VCENTER | DT_SINGLELINE);
			pDC->SelectObject(pOldFont);   
			pDC->SetTextColor(oldTxtCor);    
			tipFont.DeleteObject();
			vertFont.DeleteObject();
		}

		*pResult = CDRF_SKIPDEFAULT;    // We've painted everything.
	}
}
