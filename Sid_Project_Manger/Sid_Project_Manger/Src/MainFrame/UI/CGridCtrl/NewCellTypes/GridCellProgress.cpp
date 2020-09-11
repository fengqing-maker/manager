// GridCellProgress.cpp: implementation of the GridCellProgress class.
// Written by Iain Clarke  (iain@imcsoft.co.uk / imcclarke@yahoo.com)
//

#include "stdafx.h"
#include "../GridCell.h"
#include "GridCellProgress.h"

IMPLEMENT_DYNCREATE(CGridCellProgress, CGridCell)

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

// Bring in shlwapi.dll, needed for PathCompactPath
#include "Shlwapi.h"
#pragma comment(lib,"Shlwapi.lib")

double	Interpolate ( const double Begin, const double End, const double Which, const double OutOf)
{
	if (OutOf == 0.0)
		return	Begin;
	return	((End - Begin) * Which / OutOf) + Begin;
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CGridCellProgress::CGridCellProgress ()
{
	m_nLower = 0;
	m_nPos = 50;
	m_nUpper = 100;

	m_clrBar = CLR_DEFAULT;

	m_bPercentage = m_bCompactPath = TRUE;
}


///////////////////////////////////////////////////////////////////////////////
// Simple accessor functions

BOOL	CGridCellProgress::EnablePercentage (BOOL bEnable)
{
	BOOL bOld = m_bPercentage;
	m_bPercentage = bEnable;
	return bOld;
}
BOOL	CGridCellProgress::EnablePathCompaction (BOOL bEnable)
{
	BOOL bOld = m_bCompactPath;
	m_bCompactPath = bEnable;
	return bOld;
}
void	CGridCellProgress::SetRange(int nLower, int nUpper)
{
	SetLower (nLower);
	SetUpper (nUpper);
}
void	CGridCellProgress::SetLower(int nLower)
{
	m_nLower = nLower;
}
void	CGridCellProgress::SetUpper(int nUpper)
{
	m_nUpper = nUpper;
}
void	CGridCellProgress::SetPos	(int nPos)
{
	m_nPos = nPos;
}
void	CGridCellProgress::SetColor(COLORREF clr)
{
	m_clrBar = clr;
}


// Draw function. Vast majority of this code was ripped off directly from CGridCellDefault::Draw

BOOL CGridCellProgress::Draw(CDC* pDC, int nRow, int nCol, CRect rect, BOOL bEraseBkgnd)
{
    CGridCtrl* pGrid = GetGrid();
    ASSERT(pGrid);

    if (!pGrid || !pDC)
        return FALSE;
    // Get the default cell implementation for this kind of cell. We use it if this cell
    // has anything marked as "default"
	CGridDefaultCell *pDefaultCell = (CGridDefaultCell*) GetDefaultCell();
	if (!pDefaultCell)
		return FALSE;

	double dFraction = 0;
	CRect	rcBar (rect);
	if (m_nLower != m_nUpper)
	{
		dFraction = double (m_nPos - m_nLower) / double (m_nUpper - m_nLower);
	}

	// Are we meant to be "lit?"
	BOOL bBacklit = IsFocused() || IsDropHighlighted();

	COLORREF	cBar;
	COLORREF	cText;
	COLORREF	cBack;

	if (bBacklit)
	{
		cBack	= ::GetSysColor(COLOR_HIGHLIGHT);
		cText	= ::GetSysColor(COLOR_HIGHLIGHTTEXT);
		cBar	= (m_clrBar == CLR_DEFAULT) ? GetSysColor (COLOR_HIGHLIGHT) : m_clrBar;
	} else {
		cText	= GetTextClr ();
		cBack	= GetBackClr ();
		if (cText == CLR_DEFAULT)	cText = pDefaultCell->GetTextClr ();
		if (cBack == CLR_DEFAULT)	cBack = pDefaultCell->GetBackClr ();
		cBar	= (m_clrBar == CLR_DEFAULT) ? GetSysColor (COLOR_HIGHLIGHT) : m_clrBar;
	}


	int nSaveDC = pDC->SaveDC ();

	CFont *pFont = GetFontObject();
	ASSERT(pFont);
	if (pFont)
		pDC->SelectObject(pFont);

	pDC->FillSolidRect (&rect, cBack);
	rect.DeflateRect (1,1);

	pDC->SetBkMode (TRANSPARENT);
	pDC->SetTextColor (cText);

	CRect rcTmp (rect);
	CString s;

	if (m_bPercentage)
	{
		s.Format (_T("%i%%"), int(100.0 * dFraction + 0.5));
		pDC->DrawText (s, rcTmp, DT_CALCRECT | DT_RIGHT | DT_NOPREFIX);
		rcTmp.left = rect.right - rcTmp.Width ();
		rcTmp.right = rect.right;
		pDC->DrawText (s, rcTmp, DT_NOPREFIX);

		rcTmp.right = rcTmp.left - 8;
		rcTmp.left = rect.left;
	}

	s = GetText ();
	if (m_bCompactPath)
	{
		TCHAR szText [MAX_PATH];
		lstrcpy (szText, s);
		if (PathCompactPath (pDC->GetSafeHdc (), szText, rcTmp.Width ()))
			s = szText;
	}

	pDC->DrawText (s, rcTmp, GetFormat() | DT_NOPREFIX);

	rcTmp = rect;
	dFraction *= rcTmp.Width ();
	rcTmp.right = rcTmp.left + int (dFraction);

	CBrush br (cBar);
	pDC->SelectObject (&br);

	// Draw the progress bar
	pDC->BitBlt (rcTmp.left, rcTmp.top, rcTmp.Width (), rcTmp.Height (), NULL, 0,0, 0x00F50225); // Ternary Raster Operations = PDno

	pDC->RestoreDC (nSaveDC);

	return TRUE;
}
