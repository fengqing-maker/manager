#if !defined(AFX_INPLACERICHEDIT_H__5C10C899_0269_4E69_89AC_1FA1011E8BFB__INCLUDED_)
#define AFX_INPLACERICHEDIT_H__5C10C899_0269_4E69_89AC_1FA1011E8BFB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// InPlaceRichEdit.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CInPlaceRichEdit window

class CInPlaceRichEdit : public CRichEditCtrl
{
// Construction
public:
    CInPlaceRichEdit(CWnd* pParent, CRect& rect, DWORD dwStyle, UINT nID,
                 int nRow, int nColumn, CString sInitText, UINT nFirstChar);

// Attributes
public:
 
// Operations
public:
     void EndEdit();
 
// Overrides
     // ClassWizard generated virtual function overrides
     //{{AFX_VIRTUAL(CInPlaceEdit)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL
 
// Implementation
public:
     virtual ~CInPlaceRichEdit();
 
// Generated message map functions
protected:
    //{{AFX_MSG(CInPlaceEdit)
    afx_msg void OnKillFocus(CWnd* pNewWnd);
    afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
    afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg UINT OnGetDlgCode();
	//}}AFX_MSG
    DECLARE_MESSAGE_MAP()

private:
    int     m_nRow;
    int     m_nColumn;
    CString m_sInitText;
    UINT    m_nLastChar;
    BOOL    m_bExitOnArrows;
    CRect   m_Rect;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_INPLACERICHEDIT_H__5C10C899_0269_4E69_89AC_1FA1011E8BFB__INCLUDED_)
