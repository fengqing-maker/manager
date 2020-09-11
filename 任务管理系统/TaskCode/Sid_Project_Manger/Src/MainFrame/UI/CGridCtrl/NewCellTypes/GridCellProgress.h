// GridCellProgress.h: interface for the CGridCellProgress class.
// Written by Iain Clarke  (iain@imcsoft.co.uk / imcclarke@yahoo.com)
//
//////////////////////////////////////////////////////////////////////

#pragma once

#include "../GridCell.h"


class CGridCellProgress : public CGridCell  
{
    DECLARE_DYNCREATE(CGridCellProgress)

public:
	CGridCellProgress ();

    virtual BOOL Edit(int nRow, int nCol, CRect rect, CPoint point, UINT nID, UINT nChar)
	{
		return FALSE; // We don't want to edit this! It's for information after all...
	}

    virtual BOOL Draw(CDC* pDC, int nRow, int nCol, CRect rect, BOOL bEraseBkgnd = TRUE);


	virtual	BOOL	EnablePercentage (BOOL bEnable);
	virtual	BOOL	EnablePathCompaction (BOOL bEnable);

	virtual	void	SetRange(int nLower, int nUpper);
	virtual	void	SetLower(int nLower);
	virtual	void	SetUpper(int nUpper);
	virtual	void	SetPos	(int nPos);
	virtual void	SetColor(COLORREF clr);

	virtual	void	GetRange(int &nLower, int &nUpper) {  nLower = m_nLower; nUpper = m_nUpper; }
	virtual	int		GetLower()	{ return m_nLower; }
	virtual	int		GetUpper()	{ return m_nUpper; }
	virtual	int		GetPos	()	{ return m_nPos; }
	virtual COLORREF GetColor()	{ return m_clrBar; }

protected:
	BOOL	m_bPercentage, m_bCompactPath;
	int		m_nPos, m_nLower, m_nUpper;
	COLORREF m_clrBar;
};

