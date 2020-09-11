// MulitLineListBox.cpp : implementation file
//

#include "stdafx.h"
#include "MultiLineListBox.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMulitLineListBox

CMultiLineListBox::CMultiLineListBox()
{
}

CMultiLineListBox::~CMultiLineListBox()
{
	
}


BEGIN_MESSAGE_MAP(CMultiLineListBox, CListBox)
	//{{AFX_MSG_MAP(CMultiLineListBox)
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMulitLineListBox message handlers
void CMultiLineListBox::AppendString(const CString &str, COLORREF fgColor, COLORREF bgColor)
{
	LISTBOX_COLOR* pInfo = new LISTBOX_COLOR;
	pInfo->strText = str;
	pInfo->fgColor = fgColor; 
	pInfo->bgColor = bgColor;
	m_dataItem.Add( pInfo );
	int index = AddString(pInfo->strText);
	SetItemDataPtr( index, pInfo);
}
void CMultiLineListBox::AppendString( const CString &str  )
{
	LISTBOX_COLOR* pInfo = new LISTBOX_COLOR;

	pInfo->strText = str;
	pInfo->fgColor = RGB(0, 0, 0); 
	//pInfo->bgColor = RGB(255, 255, 255);
	int index = AddString(pInfo->strText);
	//pInfo->bgColor = index % 2 == 1 ? RGB(181,227,255): RGB(255, 255, 255);
	m_dataItem.Add( pInfo );
	SetItemDataPtr(index, pInfo);
}
void CMultiLineListBox::MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct) 
{
	// TODO: Add your code to determine the size of specified item
	ASSERT(lpMeasureItemStruct->CtlType == ODT_LISTBOX);
	
	CString strText(_T(""));
	GetText(lpMeasureItemStruct->itemID, strText);
	//ASSERT(TRUE != strText.IsEmpty()); 
	if ( strText.IsEmpty() )
	{
		int lineCnt = m_dataItem.GetCount();
		if ( lineCnt > 0 && lpMeasureItemStruct->itemID < (UINT)lineCnt)
		{
			LISTBOX_COLOR* pListBox  = (LISTBOX_COLOR*)m_dataItem.GetAt( lpMeasureItemStruct->itemID )  ;
			if ( pListBox )
			{
				strText = pListBox->strText;
			}
		}

	}

	CRect rect;
	GetItemRect(lpMeasureItemStruct->itemID, &rect);

	CDC* pDC = GetDC(); 
	lpMeasureItemStruct->itemHeight = pDC->DrawText(strText, -1, rect, DT_WORDBREAK | DT_CALCRECT); 
	ReleaseDC(pDC);
}

void CMultiLineListBox::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct) 
{
	// TODO: Add your code to draw the specified item
	ASSERT(lpDrawItemStruct->CtlType == ODT_LISTBOX);

	//LISTBOX_COLOR* pListBox = (LISTBOX_COLOR*)GetItemDataPtr(lpDrawItemStruct->itemID);
	//ASSERT(NULL != pListBox);
	int lineCnt = m_dataItem.GetCount();
	if ( !(lineCnt > 0 && lpDrawItemStruct->itemID < (UINT)lineCnt) )
	{
		return;
	}
	LISTBOX_COLOR* pListBox  = NULL;
	if ( NULL == (pListBox  = (LISTBOX_COLOR*)m_dataItem.GetAt( lpDrawItemStruct->itemID ))  )
	{
		return;
	}

	CDC dc;
	dc.Attach(lpDrawItemStruct->hDC);
	
	// Save these value to restore them when done drawing.
	COLORREF crOldTextColor = dc.GetTextColor();
	COLORREF crOldBkColor = dc.GetBkColor();
	
	// If this item is selected, set the background color 
	// and the text color to appropriate values. Also, erase
	// rect by filling it with the background color.
	if ((lpDrawItemStruct->itemAction | ODA_SELECT) &&
		(lpDrawItemStruct->itemState & ODS_SELECTED))
	{
		dc.SetTextColor(::GetSysColor(COLOR_HIGHLIGHTTEXT));
		dc.SetBkColor(::GetSysColor(COLOR_HIGHLIGHT)/*pListBox->fgColor*/);
		dc.FillSolidRect(&lpDrawItemStruct->rcItem, ::GetSysColor(COLOR_HIGHLIGHT)/*pListBox->fgColor*/);
	}
	else
	{
		dc.SetTextColor(pListBox->fgColor);
		dc.SetBkColor(pListBox->bgColor);
		dc.FillSolidRect(&lpDrawItemStruct->rcItem, pListBox->bgColor);
	}
	
	lpDrawItemStruct->rcItem.left += 5;
	// Draw the text.
	
	dc.DrawText(pListBox->strText, pListBox->strText.GetLength(), &lpDrawItemStruct->rcItem, DT_WORDBREAK);
	
	// Reset the background color and the text color back to their
	// original values.
	dc.SetTextColor(crOldTextColor);
	dc.SetBkColor(crOldBkColor);
	
	dc.Detach();	
}


void CMultiLineListBox::OnDestroy() 
{
	RealseData();
	CListBox::OnDestroy();
}
void CMultiLineListBox::RealseData()
{
	int nPtrCount = 	m_dataItem.GetCount();

	for(int i=0; i< nPtrCount; i++)
	{
	   LISTBOX_COLOR *pData = (LISTBOX_COLOR *)m_dataItem.GetAt( i );
	   if ( pData )
	   {
		   delete pData;
	   }
	}
	m_dataItem.RemoveAll();
}
void CMultiLineListBox::RemoveAllString()
{
	int count = GetCount();
    RealseData();
	for(int i=0; i<count; i++)
	{
		SetItemDataPtr(i, NULL );
	}
	int i = count - 1;
	while(i >= 0)
	{
		int ret = DeleteString(i);
		if (LB_ERR == ret)
		{
			ret++;
		}
		i = GetCount() - 1; 
	}
}