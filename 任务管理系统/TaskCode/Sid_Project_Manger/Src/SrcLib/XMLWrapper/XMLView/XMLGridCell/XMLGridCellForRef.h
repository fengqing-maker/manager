#pragma once
#include <map>
#include "GridCell.h"

/** @brief 使用“键-值对”类型的选项 */
typedef struct combooption
{
	CString optionKey;			/**< @brief 选项的关键字将被加入组合框 */
	CString optionValue;		/**< @brief 当选择指定关键字时对应的选项值被写入编辑框 */
}RefOption;
typedef std::map<CString,RefOption>		RefOptionMap;
typedef RefOptionMap::iterator			RefOptionMapItor;
typedef RefOptionMap::const_iterator	RefOptionMapConstItor;

class CXMLGridCellForRef : public CGridCell
{
	friend class CGridCtrl;
	DECLARE_DYNCREATE(CXMLGridCellForRef)

public:
	CXMLGridCellForRef();
	~CXMLGridCellForRef();

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
	void  SetOptions(const RefOptionMap& optionMap);
	void  SetStyle(DWORD dwStyle)           { ASSERT(0);/* 不应该设置style */	m_dwStyle = dwStyle; }
	DWORD GetStyle()                        { return m_dwStyle;    }

protected:
	virtual BOOL Draw(CDC* pDC, int nRow, int nCol, CRect rect, BOOL bEraseBkgnd = TRUE);

	RefOptionMap m_optionMap;
	DWORD        m_dwStyle;
};



/////////////////////////////////////////////////////////////////////////////

#define IDC_COMBOEDIT 1001

class CComboEditForRef : public CEdit
{
	// Construction
public:
	CComboEditForRef();

	// Attributes
public:

	// Operations

public:

	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CComboEditForRef)
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL

	// Implementation
public:
	virtual ~CComboEditForRef();

	// Generated message map functions
protected:
	//{{AFX_MSG(CComboEditForRef)
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()

private:
	/* 若要求CBS_DROPDOWNLIST风格则将Edit置为只读控件，这样既可防止用户输入，又可使用SetWindowText来改变编辑框的值 */
	/* 通过对按键输入不进行响应来实现只读 */
};

/////////////////////////////////////////////////////////////////////////////
// CInPlaceList window

class CInPlaceListForRef : public CComboBox
{
	friend class CComboEditForRef;

	// Construction
public:
	CInPlaceListForRef(CWnd* pParent,         // parent
		CRect& rect,           // dimensions & location
		DWORD dwStyle,         // window/combobox style
		UINT nID,              // control ID
		int nRow, int nColumn, // row and column
		COLORREF crFore, COLORREF crBack,  // Foreground, background colour
		RefOptionMap& optionMap,// options in list
		CString sInitText,     // initial selection
		UINT nFirstChar);      // first character to pass to control

	// Attributes
public:
	CComboEditForRef m_edit;  // subclassed edit control
	/* 若要求CBS_DROPDOWNLIST风格则将Edit置为只读控件，这样既可防止用户输入，又可使用SetWindowText来改变编辑框的值 */
	BOOL			 m_bReadOnly;	
	RefOptionMap	 m_optionMap;

protected:
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

	// Implementation
public:
	virtual ~CInPlaceListForRef();
	void EndEdit();
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
	afx_msg void OnSelendOK();

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