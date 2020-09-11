/*********************************************************************************************************/
/*Class Name: MultiLineListCtrl                                                                          */
/*Class Definition: It creates a CListCtrl to support multiple lines in one row. When there are many     */
/*                  lines, a [+] icon show to user be able to expand the row.                            */
/*Class Author: Carlos Andre Sanches de Souza                                                            */
/*Class Creation Date: 08/08/2007                                                                        */
/*********************************************************************************************************/

#include "stdafx.h"
#include "MultiLineListCtrl.h"

// CMultiLineListCtrl

IMPLEMENT_DYNAMIC(CMultiLineListCtrl, CMFCListCtrl)
CMultiLineListCtrl::CMultiLineListCtrl()
: m_skipTextCheck(false)
, m_lastSelRow(-1)
, m_lastTopSelRow(-1)
, m_selRow(-1)
, m_topSelRow(-1)
, m_repeatEvent(false)
, m_bShowGrid(true)
, m_bShowMinimized(true)
, m_gridColor(RGB(180,180,180))
, m_symbolColor(RGB(180,180,180))
{
	LOGBRUSH LogBrush;
	LogBrush.lbColor = m_symbolColor;
	LogBrush.lbStyle = PS_SOLID;
	m_dotPen.CreatePen( PS_COSMETIC | PS_ALTERNATE , 1, &LogBrush, 0, NULL );
}

CMultiLineListCtrl::~CMultiLineListCtrl()
{
	m_dotPen.DeleteObject();
}


BEGIN_MESSAGE_MAP(CMultiLineListCtrl, CMFCListCtrl)
	ON_NOTIFY_REFLECT(NM_CUSTOMDRAW, OnCustomDraw)
	ON_NOTIFY_REFLECT(LVN_ITEMACTIVATE, OnActivate)
	ON_NOTIFY_REFLECT(NM_DBLCLK, OnDblClick)
END_MESSAGE_MAP()

/**********************************************************************************************/
/*Function Name: SetGridColor                                                                 */
/*Function Definition: Set grid color                                                         */
/*Function Author: Carlos Andre Sanches de Souza                                              */
/*Function Creation Date: 08/08/2007                                                          */
/*Variables Name:                                                                             */
/*   COLORREF color: new color to grid                                                        */
/**********************************************************************************************/
BOOL CMultiLineListCtrl::SetGridColor(COLORREF color) 
{
	m_gridColor = color;
	Invalidate();
	return TRUE;
}

/**********************************************************************************************/
/*Function Name: SetBkColor                                                                   */
/*Function Definition: Set background color                                                   */
/*Function Author: Carlos Andre Sanches de Souza                                              */
/*Function Creation Date: 08/08/2007                                                          */
/*Variables Name:                                                                             */
/*   COLORREF color: new color to background                                                  */
/**********************************************************************************************/
BOOL CMultiLineListCtrl::SetBkColor(COLORREF cr) {
	m_gridColor = cr;
	return CMFCListCtrl::SetBkColor(cr);
}

/**********************************************************************************************/
/*Function Name: ShowGrid                                                                     */
/*Function Definition: Show/hide grid                                                         */
/*Function Author: Carlos Andre Sanches de Souza                                              */
/*Function Creation Date: 08/08/2007                                                          */
/*Variables Name:                                                                             */
/*   bool show: true = show grid                                                              */
/**********************************************************************************************/
void CMultiLineListCtrl::ShowGrid(bool show) 
{
	m_bShowGrid = show;
}

/**********************************************************************************************/
/*Function Name: ShowMinimized                                                                */
/*Function Definition: Show/hide the whole string in one line when minimized                  */
/*Function Author: Carlos Andre Sanches de Souza                                              */
/*Function Creation Date: 08/08/2007                                                          */
/*Variables Name:                                                                             */
/*   bool show: true = show the string minimized                                              */
/**********************************************************************************************/
void CMultiLineListCtrl::ShowMinimized(bool show) 
{
	m_bShowMinimized = show;
}

/**********************************************************************************************/
/*Function Name: OnCustomDraw                                                                 */
/*Function Definition: Draw list items                                                        */
/*Function Author: Carlos Andre Sanches de Souza                                              */
/*Function Creation Date: 08/08/2007                                                          */
/**********************************************************************************************/
void CMultiLineListCtrl::OnCustomDraw(NMHDR* pNMHDR, LRESULT* pResult)
{
	*pResult = CDRF_DODEFAULT;
	LPNMLVCUSTOMDRAW  lplvcd = (LPNMLVCUSTOMDRAW)pNMHDR;

	switch(lplvcd->nmcd.dwDrawStage) 
	{
	case CDDS_PREPAINT :
		{
			*pResult = CDRF_NOTIFYITEMDRAW;
			return;
		}
	case CDDS_ITEMPREPAINT:
		{
			LVITEM lvItem;
			int nItem = static_cast<int>(lplvcd->nmcd.dwItemSpec);
			CDC* pDC = CDC::FromHandle(lplvcd->nmcd.hdc);
			
			BOOL bListHasFocus = (this->GetSafeHwnd() == ::GetFocus());
			//get the image index and selected/focused state of the item being drawn
			ZeroMemory(&lvItem,sizeof(LVITEM));
			lvItem.mask = LVIF_IMAGE|LVIF_STATE;
			lvItem.iItem = nItem;
			lvItem.stateMask = LVIS_SELECTED | LVIS_FOCUSED;
			lvItem.iImage = -1;
			this->GetItem(&lvItem);

			//get the rect that holds the item's icon
			CRect rcItem;
			this->GetItemRect(nItem,&rcItem,LVIR_ICON);
			//draw the icon
			UINT uFormat = ILD_TRANSPARENT;
			if ((lvItem.state & LVIS_SELECTED) && bListHasFocus)
			{
				uFormat |= ILD_FOCUS;
			}
			CImageList* imageList = GetImageList(LVSIL_SMALL);
			if (imageList != 0)
			{
				imageList->Draw(pDC,lvItem.iImage,rcItem.TopLeft(),uFormat);
			}

			*pResult = CDRF_NOTIFYSUBITEMDRAW;
			return;
		}
	case CDDS_SUBITEM | CDDS_PREPAINT | CDDS_ITEM: 
		{
			int nItem = lplvcd->nmcd.dwItemSpec;
			int nSubItem = lplvcd->iSubItem;
			int nNumLines = (int)GetItemData(nItem);
			int nCurrentSelection;
			bool changeSelection = false;
			POSITION posSelection;
			HDC hdc = lplvcd->nmcd.hdc;
			CRect boxRect, bounds, rect;
			CDC* pDC = CDC::FromHandle(hdc);

			// Get background box
			boxRect = lplvcd->nmcd.rc;
			GetItemRect( lplvcd->nmcd.dwItemSpec, &bounds, LVIR_BOUNDS );
			boxRect.top = bounds.top;
			boxRect.bottom = bounds.bottom;
			if( nSubItem == 0 )
			{
				CRect lrect;
				GetItemRect( lplvcd->nmcd.dwItemSpec, &lrect, LVIR_LABEL );
				boxRect.left = lrect.left;
				boxRect.right = lrect.right;
			}
			else
			{
				boxRect.right += bounds.left;
				boxRect.left  += bounds.left;
			}

			// Get selection
			posSelection = GetFirstSelectedItemPosition();
			if (posSelection) 
				nCurrentSelection = GetNextSelectedItem(posSelection);
			else
				nCurrentSelection = -1;
			if (nCurrentSelection != m_selRow) 
			{
				m_lastSelRow = m_selRow;
				m_selRow = nCurrentSelection;
				changeSelection = true;
			}

			// Fill background box
			if ( m_selRow == nItem ||
				(nNumLines>1 && nItem<GetItemCount()-1 && (int)GetItemData(nItem+1)<1 && m_selRow>nItem && m_selRow<nItem+nNumLines) ||
				(nNumLines<1 && m_selRow>=nItem+nNumLines && m_selRow<nItem+nNumLines+(int)GetItemData(nItem+nNumLines))) 
			{
				pDC->FillRect(boxRect, &CBrush(::GetSysColor(COLOR_HIGHLIGHT)));
				pDC->SetTextColor(GetSysColor(COLOR_HIGHLIGHTTEXT)) ;
			}
			else 
			{
				pDC->FillRect(boxRect, &CBrush(::GetSysColor(COLOR_WINDOW)));
				pDC->SetTextColor(GetSysColor(COLOR_WINDOWTEXT)) ;
			}
			// Get text box
			rect = boxRect;
			rect.left += nSubItem?6:2;

			// Draw symbol [+]
			if (nNumLines>1 && GetItemText(nItem, nSubItem).Find('\n')>-1 )
			{
				CPen light(PS_SOLID, 1, m_symbolColor);
				pDC->SelectObject(light);
				pDC->MoveTo(rect.left,rect.top+2);
				pDC->LineTo(rect.left+8,rect.top+2);
				pDC->LineTo(rect.left+8,rect.top+10);
				pDC->LineTo(rect.left,rect.top+10);
				pDC->LineTo(rect.left,rect.top+2);
				pDC->MoveTo(rect.left+2,rect.top+6);
				pDC->LineTo(rect.left+7,rect.top+6);
				if (nItem<GetItemCount()-1 && (int)GetItemData(nItem+1)<1) 
				{
					pDC->SelectObject(m_dotPen);
					pDC->MoveTo(rect.left+4,rect.top+10);
					pDC->LineTo(rect.left+4,rect.bottom);
				}
				else 
				{
					pDC->MoveTo(rect.left+4,rect.top+4);
					pDC->LineTo(rect.left+4,rect.top+9);
				}
				rect.left += 12;
			}

			// Draw dot lines
			if (nNumLines<1 && GetItemText(nItem+nNumLines, nSubItem).Find('\n')>-1) {
				int countLines = 1;
				CString text = GetItemText(nItem+nNumLines, nSubItem);
				for (int k=0;k<text.GetLength();k++)
					if (text.GetAt(k)=='\n')
						countLines++;
				if (countLines>-nNumLines) {
					pDC->SelectObject(m_dotPen);
					pDC->MoveTo(rect.left+4,rect.top);
					if (countLines-1==-nNumLines) {
						pDC->LineTo(rect.left+4,rect.top+6);
						pDC->LineTo(rect.left+8,rect.top+6);
					}
					else
						pDC->LineTo(rect.left+4,rect.bottom);
				}
				rect.left += 12;
			}

			// Draw text
			CString str = GetItemDisplayText(nItem,nSubItem);
			pDC->DrawText( str, rect, DT_SINGLELINE|DT_NOPREFIX|DT_LEFT|DT_VCENTER|DT_END_ELLIPSIS);


			// Draw grid
			if (m_bShowGrid) {
				CPen gridline(PS_SOLID, 1, m_gridColor);
				pDC->SelectObject(gridline);
				if (nSubItem>0) {
					pDC->MoveTo(boxRect.left, boxRect.top);
					pDC->LineTo(boxRect.left, boxRect.bottom);
				}
				if (nNumLines==1 || nItem==GetItemCount()-1 || (int)GetItemData(nItem+1)>=1) {
					pDC->MoveTo(boxRect.left, boxRect.bottom-1);
					pDC->LineTo(boxRect.right, boxRect.bottom-1);
				}
			}
			*pResult = CDRF_SKIPDEFAULT;

			// invalidate
			if (changeSelection)
			{
				int topSel = m_selRow+((m_selRow>-1 && (int)GetItemData(m_selRow)<1)?(int)GetItemData(m_selRow):0);
				//int topLast = m_lastSelRow+((m_lastSelRow>-1 && (int)GetItemData(m_lastSelRow)<1)?(int)GetItemData(m_lastSelRow):0);
				m_topSelRow = topSel;
				int topLast = m_lastTopSelRow;
				CRect clean;
				int itemCount = CMFCListCtrl::GetItemCount();
				if (topSel != topLast)
				{
					if (topSel > -1 && topSel < itemCount) 
					{
						GetItemRect(topSel, &clean, LVIR_BOUNDS);
						if ((int)GetItemData(topSel)>1 && topSel<GetItemCount()-1 && (int)GetItemData(topSel+1)<1)
						{
							GetItemRect(topSel+GetItemData(topSel)-1, &bounds, LVIR_BOUNDS);
							clean.bottom = bounds.bottom;
							InvalidateRect(clean, false);
						}	
					}
					if (topLast > -1 && topLast < itemCount) 
					{
						GetItemRect(topLast, &clean, LVIR_BOUNDS);
						if ((int)GetItemData(topLast)>1 && topLast<GetItemCount()-1 && (int)GetItemData(topLast+1)<1)
						{
							GetItemRect(topLast+GetItemData(topLast)-1, &bounds, LVIR_BOUNDS);
							clean.bottom = bounds.bottom;
							InvalidateRect(clean, false);
						}	
					}	
				}
				m_lastTopSelRow = m_topSelRow;
			}
			return;
		}
	}
}

/**********************************************************************************************/
/*Function Name: OnActivate                                                                   */
/*Function Definition: Expand or contract the row when clicked twice                          */
/*Function Author: Carlos Andre Sanches de Souza                                              */
/*Function Creation Date: 08/08/2007                                                          */
/**********************************************************************************************/
void CMultiLineListCtrl::OnActivate(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	*pResult = 0;

	if (m_repeatEvent)
		return;

	if (GetItemData((int)pNMLV->iItem)==1)
		return;

	m_repeatEvent = true;
	OpenCloseRow((int)pNMLV->iItem);
}

/**********************************************************************************************/
/*Function Name: OnDblClick                                                                   */
/*Function Definition: Avoid to repeat OnActivate event                                       */
/*Function Author: Carlos Andre Sanches de Souza                                              */
/*Function Creation Date: 08/08/2007                                                          */
/**********************************************************************************************/
void CMultiLineListCtrl::OnDblClick(NMHDR *pNMHDR, LRESULT *pResult)
{
	m_repeatEvent = false;
	*pResult = 0;
}

/**********************************************************************************************/
/*Function Name: InsertItem / SetItemText                                                     */
/*Function Definition: Overwrite functions to calculate number of lines                       */
/*Function Author: Carlos Andre Sanches de Souza                                              */
/*Function Creation Date: 08/08/2007                                                          */
/**********************************************************************************************/
int CMultiLineListCtrl::InsertItem(const LVITEM* pItem) {
	int result = CMFCListCtrl::InsertItem(pItem);
	if (result == -1)
		return -1;
	CalcNumberOfLines(pItem->iItem,pItem->pszText);
	return result;
}
int CMultiLineListCtrl::InsertItem(int nItem,LPCTSTR lpszItem) 
{
	int result = CMFCListCtrl::InsertItem(nItem,lpszItem,-1);	/* 使用-1作为默认图标序号，即无图标 */
	if (result == -1)
		return -1;
	CalcNumberOfLines(nItem,lpszItem);
	return result;
}
int CMultiLineListCtrl::InsertItem(int nItem,LPCTSTR lpszItem,int nImage) {
	int result = CMFCListCtrl::InsertItem(nItem,lpszItem,nImage);
	if (result == -1)
		return -1;
	CalcNumberOfLines(nItem,lpszItem);
	return result;
}

BOOL CMultiLineListCtrl::DeleteItem(_In_ int nItem)
{
	SetItemData(nItem,0);
	return CMFCListCtrl::DeleteItem(nItem);
}
BOOL CMultiLineListCtrl::DeleteAllItems()
{
	int itemCount = CMFCListCtrl::GetItemCount();
	for (int i = 0; i < itemCount ; i++)
	{
		CMFCListCtrl::SetItemData(i,0);
	}
	return CMFCListCtrl::DeleteAllItems();
}

BOOL CMultiLineListCtrl::SetItemText(int nItem,int nSubItem,LPCTSTR lpszText)
{
	CalcNumberOfLines(nItem,lpszText,false);
	return CMFCListCtrl::SetItemText(nItem,nSubItem,lpszText);
};

int CMultiLineListCtrl::GetItemImageIndex(int itemIndex)
{
	LVITEM lvItem;
	ZeroMemory(&lvItem,sizeof(LVITEM));
	lvItem.mask = LVIF_IMAGE;
	lvItem.iItem = itemIndex;
	lvItem.iImage = -1;
	CMFCListCtrl::GetItem(&lvItem);
	return lvItem.iImage;
}
void CMultiLineListCtrl::SetItemImageIndex(int itemIndex,int imageIndex)
{
	LVITEM lvItem;
	ZeroMemory(&lvItem,sizeof(LVITEM));
	lvItem.mask = LVIF_IMAGE;
	lvItem.iItem = itemIndex;
	lvItem.iImage = imageIndex;
	CMFCListCtrl::SetItem(&lvItem);
}

void CMultiLineListCtrl::ExpandAll()
{
	for (int nRow = 0; nRow == nRow; nRow++)
	{
		if ((int)GetItemData(nRow) > 1		/* 可扩展 */
			&& (nRow == GetItemCount()-1 || (int)GetItemData(nRow+1)>=1))	/* 未展开 */
		{
			OpenCloseRow(nRow);
		}

		if (nRow == GetItemCount() - 1)
		{
			break;	/* 所有行遍历完成 */
		}
	}
}
void CMultiLineListCtrl::CollapseAll()
{
	for (int nRow = 0; nRow == nRow; nRow++)
	{
		if ((int)GetItemData(nRow) > 1		/* 可扩展 */
			&& !(nRow == GetItemCount()-1 || (int)GetItemData(nRow+1)>=1))	/* 已展开 */
		{
			OpenCloseRow(nRow);
		}

		if (nRow == GetItemCount() - 1)
		{
			break;	/* 所有行遍历完成 */
		}
	}
}
int CMultiLineListCtrl::GetMinColumnStringWidth(int nColumn)
{
	int itemCount = CMFCListCtrl::GetItemCount();
	if (itemCount == 0)
	{
		return -1;
	}
	int minWidth = INT_MAX;		
	int averageWidth = 7;
	for (int i = 0; i < itemCount; i++)
	{
		CString text = GetItemDisplayText(i,nColumn);
		int len = text.GetLength();
		int width = len * averageWidth;
		minWidth = min(minWidth,width);
	}
	return minWidth;
}
int CMultiLineListCtrl::GetMaxColumnStringWidth(int nColumn)
{
	int itemCount = CMFCListCtrl::GetItemCount();
	if (itemCount == 0)
	{
		return -1;
	}
	int maxWidth = INT_MIN;
	int averageWidth = 7;
	for (int i = 0; i < itemCount; i++)
	{
		CString text = GetItemDisplayText(i,nColumn);
		int len = text.GetLength();
		int width = len * averageWidth;
		maxWidth = max(maxWidth,width);
	}
	return maxWidth;
}
/**********************************************************************************************/
/*Function Name: CalcNumberOfLines                                                            */
/*Function Definition: Count number of lines ('\n')                                           */
/*Function Author: Carlos Andre Sanches de Souza                                              */
/*Function Creation Date: 08/08/2007                                                          */
/**********************************************************************************************/
void CMultiLineListCtrl::CalcNumberOfLines(int nItem, LPCTSTR lpszText, bool insert) {
	if (m_skipTextCheck) 
		return;
    int numLines = 1;
	for (unsigned int i=0; i< strlen( (char *)lpszText); i++)
		if (lpszText[i]==_T('\n') ) 
			numLines++;
	if (insert || (int)GetItemData(nItem)<numLines)
		SetItemData(nItem, numLines); 
}

CString CMultiLineListCtrl::GetItemDisplayText(int nItem,int nSubItem)
{
	CString text = CMFCListCtrl::GetItemText(nItem,nSubItem);
	if (GetItemData(nItem) > 1)	/* 表项可以扩展 */
	{
		if (m_bShowMinimized)
		{	
			/* 最小化显示 */
			if (text.Find(_T("\n")) > -1)
			{
				text = text.Left(text.Find(_T("\n")));
			}
			if (text.Find(_T("\r")) > -1)
			{
				text = text.Left(text.Find(_T("\r")));
			}
		}
		else
		{
			/* 非最小化显示 */
			text.Replace(_T("\r\n"),_T(" | "));
			text.Replace(_T("\n"),_T(" | "));
		}
	}
	return text;
}
/**********************************************************************************************/
/*Function Name: OpenCloseRow                                                                 */
/*Function Definition: Expand or contract a row                                               */
/*Function Author: Carlos Andre Sanches de Souza                                              */
/*Function Creation Date: 08/08/2007                                                          */
/**********************************************************************************************/
void CMultiLineListCtrl::OpenCloseRow(int row) {
	CString s, str;
	int i, j, inserted = 0;
	m_skipTextCheck = true;
	if ((int)GetItemData(row)>1 &&					/* 当前行可扩展 */
		(row==GetItemCount()-1 || (int)GetItemData(row+1)>=1)) /* 当前行是最后一行或下一行也不是扩展行 */
	{
		// open
		for (i=0; i<GetHeaderCtrl().GetItemCount(); i++) 
		{
			str = GetItemText(row, i);
			for (j=1; j<(int)GetItemData(row); j++) 
			{
				if (str.Find('\n')==-1)
					break;
				s = str = str.Right(str.GetLength() - str.Find('\n') - 1); 
				if (str.Find('\n')>-1)
					s = str.Left(str.Find('\n')); 
				if (s.Find('\r')>-1)
					s = s.Left(s.Find('\r'));
				if (j>inserted) 
				{
					InsertItem(row+j,_T(""));
					inserted = j;
				}
				SetItemText(row+j,i,s);
				SetItemData(row+j,-j);
			}
		}
	} 
	else 
	{
		// close
		if ((int)GetItemData(row)>1)	/* 当前行可扩展 */
	        for (j=1; j<(int)GetItemData(row); j++)
			{
				DeleteItem(row+1);
			}
	}
	m_skipTextCheck = false;
	Invalidate();
}

