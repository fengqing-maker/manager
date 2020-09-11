#pragma once

#include "GridCell.h"


class CXMLGridCellForEnum : public CGridCell
{
	friend class CGridCtrl;
	DECLARE_DYNCREATE(CXMLGridCellForEnum)

public:
	CXMLGridCellForEnum();
	virtual ~CXMLGridCellForEnum();

	// editing cells
public:
	virtual BOOL  Edit(int nRow, int nCol, CRect rect, CPoint point, UINT nID, UINT nChar);
	virtual CWnd* GetEditWnd() const;
	virtual void  EndEdit();

	// Operations
public:
	virtual CSize GetCellExtent(CDC* pDC);

	// CGridCellCombo specific calls
public:
	void  SetOptions(const CStringArray& ar);
	void  SetStyle(DWORD dwStyle)           { m_dwStyle = dwStyle; }
	DWORD GetStyle()                        { return m_dwStyle;    }

protected:
	virtual BOOL Draw(CDC* pDC, int nRow, int nCol, CRect rect, BOOL bEraseBkgnd = TRUE);

	CStringArray m_Strings;
	DWORD        m_dwStyle;
};




#define IDC_COMBOEDIT 1001

class CComboEditForEnum : public CEdit
{
	// Construction
public:
	CComboEditForEnum();

	// Attributes
public:

	// Operations
public:

	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CComboEdit)
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL

	// Implementation
public:
	virtual ~CComboEditForEnum();

	// Generated message map functions
protected:
	//{{AFX_MSG(CComboEdit)
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

class CInPlaceListForEnum : public CComboBox
{
	friend class CComboEditForEnum;

	// Construction
public:
	CInPlaceListForEnum(CWnd* pParent,         // parent
		CRect& rect,           // dimensions & location
		DWORD dwStyle,         // window/combobox style
		UINT nID,              // control ID
		int nRow, int nColumn, // row and column
		COLORREF crFore, COLORREF crBack,  // Foreground, background colour
		CStringArray& Items,   // Items in list
		CString sInitText,     // initial selection
		UINT nFirstChar);      // first character to pass to control

	// Attributes
public:
	CComboEditForEnum m_edit;  // subclassed edit control

	// Operations
public:

	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CInPlaceListForEnum)
protected:
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL


public:
	virtual ~CInPlaceListForEnum();
	virtual void	EndEdit();

	/** 
	*@brief 将列表框控件附加到指定表项上
	*note	只有被附加某一个表项上的编辑框控件才能向表项中编辑数据 
	*/
	BOOL AttachToGridCell(CGridCell* pCell);
	/** 
	*@brief 将列表框控件从表项中分离 
	*@note	列表框与表项分离后无法向表项中编辑数据
	*/
	BOOL DetachFromGridCell();

	/** @brief 判断与编辑框控件关联的表项是否还有效 */
	BOOL IsGridCellValid();

protected:
	int GetCorrectDropWidth();

	// Generated message map functions
protected:
	//{{AFX_MSG(CInPlaceList)
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnDropdown();
	afx_msg UINT OnGetDlgCode();
	afx_msg HBRUSH CtlColor(CDC* pDC, UINT nCtlColor);
	//}}AFX_MSG
	//afx_msg void OnSelendOK();

	DECLARE_MESSAGE_MAP()

private:
	int		 m_nNumLines;
	CString  m_sInitText;
	int		 m_nRow;
	int		 m_nCol;
	UINT     m_nLastChar; 
	BOOL	 m_bExitOnArrows; 
	COLORREF m_crForeClr, m_crBackClr;
	CGridCell* m_pCell;		/* 编辑框控件关联表项的指针 */
};


