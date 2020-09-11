// MultiLineListBox.cpp : implementation file  
//  

#include "stdafx.h"  
#include "CMultiLineListBox.h"  

// CMultiLineListBox  


CMultiLineListBox::CMultiLineListBox()  
{  
}  

CMultiLineListBox::~CMultiLineListBox()  
{   
}  

// CMultiLineListBox message handlers  

void CMultiLineListBox::MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct)  
{  
	// TODO: Add your code to determine the size of specified item  
	int nItem = lpMeasureItemStruct->itemID;   
	CPaintDC dc(this);   
	CString sLabel;   
	CRect rcLabel;   

	GetText( nItem, sLabel );   
	GetItemRect(nItem, rcLabel);   

	int itemHeight = dc.DrawText( sLabel, -1, rcLabel, DT_WORDBREAK | DT_CALCRECT );   
	lpMeasureItemStruct->itemHeight = itemHeight;     

}  

void CMultiLineListBox::DrawItem(LPDRAWITEMSTRUCT lpDIS)  
{  
	CDC* pDC = CDC::FromHandle(lpDIS->hDC);   

	COLORREF rColor = (COLORREF)RGB(255,255,255);//lpDIS->itemData; // RGB in item data   

	CString sLabel;   
	if (lpDIS->itemID == -1)
		return;
	GetText(lpDIS->itemID, sLabel);   

	if ((lpDIS->itemState & ODS_SELECTED) &&   
		(lpDIS->itemAction & (ODA_SELECT | ODA_DRAWENTIRE)))   
	{   

		CBrush colorBrush(rColor);   
		CRect colorRect = lpDIS->rcItem;   

		CBrush labelBrush(::GetSysColor(COLOR_HIGHLIGHT));   
		CRect labelRect = lpDIS->rcItem;   
		pDC->FillRect(&labelRect,&labelBrush);   

		COLORREF colorTextSave;   
		COLORREF colorBkSave;   

		colorTextSave = pDC->SetTextColor(::GetSysColor(COLOR_HIGHLIGHTTEXT));   
		colorBkSave = pDC->SetBkColor(::GetSysColor(COLOR_HIGHLIGHT));   
		pDC->DrawText( sLabel, -1, &lpDIS->rcItem, DT_WORDBREAK );   

		pDC->SetTextColor(colorTextSave);   
		pDC->SetBkColor(colorBkSave);   

		return;   
	}   

	if (lpDIS->itemAction & ODA_DRAWENTIRE)   
	{   
		CBrush brush(rColor);   
		CRect rect = lpDIS->rcItem;   
		pDC->SetBkColor(rColor);   
		pDC->FillRect(&rect,&brush);   
		pDC->DrawText( sLabel, -1, &lpDIS->rcItem, DT_WORDBREAK );   

		return;   
	}   

	if (!(lpDIS->itemState & ODS_SELECTED) &&   
		(lpDIS->itemAction & ODA_SELECT))   
	{   
		CRect rect = lpDIS->rcItem;   
		CBrush brush(rColor);   
		pDC->SetBkColor(rColor);   
		pDC->FillRect(&rect,&brush);   
		pDC->DrawText( sLabel, -1, &lpDIS->rcItem, DT_WORDBREAK );   

		return;   
	}   

}  



