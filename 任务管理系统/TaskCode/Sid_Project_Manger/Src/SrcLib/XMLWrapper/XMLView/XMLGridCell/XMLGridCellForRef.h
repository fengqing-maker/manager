#pragma once
#include <map>
#include "GridCell.h"

/** @brief ʹ�á���-ֵ�ԡ����͵�ѡ�� */
typedef struct combooption
{
	CString optionKey;			/**< @brief ѡ��Ĺؼ��ֽ���������Ͽ� */
	CString optionValue;		/**< @brief ��ѡ��ָ���ؼ���ʱ��Ӧ��ѡ��ֵ��д��༭�� */
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
	void  SetStyle(DWORD dwStyle)           { ASSERT(0);/* ��Ӧ������style */	m_dwStyle = dwStyle; }
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
	/* ��Ҫ��CBS_DROPDOWNLIST�����Edit��Ϊֻ���ؼ��������ȿɷ�ֹ�û����룬�ֿ�ʹ��SetWindowText���ı�༭���ֵ */
	/* ͨ���԰������벻������Ӧ��ʵ��ֻ�� */
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
	/* ��Ҫ��CBS_DROPDOWNLIST�����Edit��Ϊֻ���ؼ��������ȿɷ�ֹ�û����룬�ֿ�ʹ��SetWindowText���ı�༭���ֵ */
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
	*@brief ���б��ؼ����ӵ�ָ��������
	*note	ֻ�б�����ĳһ�������ϵı༭��ؼ�����������б༭���� 
	*/
	BOOL AttachToGridCell(CGridCell* pCell);
	/** 
	*@brief ���б��ؼ��ӱ����з��� 
	*@note	�б������������޷�������б༭����
	*/
	BOOL DetachFromGridCell();

	/** @brief �ж���༭��ؼ������ı����Ƿ���Ч */
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
	CGridCell* m_pCell;		/* �༭��ؼ����������ָ�� */
};