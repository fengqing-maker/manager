#include "stdafx.h"
#include "XMLGridCellForEnum.h"

#include "GridCell.h"
#include "GridCtrl.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif



CComboEditForEnum::CComboEditForEnum()
{
}
CComboEditForEnum::~CComboEditForEnum()
{
}

// Stoopid win95 accelerator key problem workaround - Matt Weagle.
BOOL CComboEditForEnum::PreTranslateMessage(MSG* pMsg) 
{
	// Make sure that the keystrokes continue to the appropriate handlers
	if (pMsg->message == WM_KEYDOWN || pMsg->message == WM_KEYUP)
	{
		::TranslateMessage(pMsg);
		::DispatchMessage(pMsg);
		return TRUE;
	}	

	// Catch the Alt key so we don't choke if focus is going to an owner drawn button
	if (pMsg->message == WM_SYSCHAR)
		return TRUE;

	return CEdit::PreTranslateMessage(pMsg);
}

BEGIN_MESSAGE_MAP(CComboEditForEnum, CEdit)
	//{{AFX_MSG_MAP(CComboEditForEnum)
	ON_WM_KILLFOCUS()
	ON_WM_KEYDOWN()
	ON_WM_KEYUP()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CComboEditForEnum message handlers

void CComboEditForEnum::OnKillFocus(CWnd* pNewWnd) 
{
	CEdit::OnKillFocus(pNewWnd);
	
    CInPlaceListForEnum* pOwner = (CInPlaceListForEnum*) GetOwner();  // This MUST be a CInPlaceListForEnum
    if (pOwner)
        pOwner->EndEdit();	
}

void CComboEditForEnum::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	if ((nChar == VK_PRIOR || nChar == VK_NEXT ||
		 nChar == VK_DOWN  || nChar == VK_UP   ||
		 nChar == VK_RIGHT || nChar == VK_LEFT) &&
		(GetKeyState(VK_CONTROL) < 0 && GetDlgCtrlID() == IDC_COMBOEDIT))
    {
        CWnd* pOwner = GetOwner();
        if (pOwner)
            pOwner->SendMessage(WM_KEYDOWN, nChar, nRepCnt+ (((DWORD)nFlags)<<16));
        return;
    }

	CEdit::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CComboEditForEnum::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	if (nChar == VK_ESCAPE) 
	{
        CWnd* pOwner = GetOwner();
        if (pOwner)
            pOwner->SendMessage(WM_KEYUP, nChar, nRepCnt + (((DWORD)nFlags)<<16));
        return;
    }

	if (nChar == VK_TAB || nChar == VK_RETURN || nChar == VK_ESCAPE)
    {
        CWnd* pOwner = GetOwner();
        if (pOwner)
            pOwner->SendMessage(WM_KEYUP, nChar, nRepCnt + (((DWORD)nFlags)<<16));
        return;
    }

	CEdit::OnKeyUp(nChar, nRepCnt, nFlags);
}


/////////////////////////////////////////////////////////////////////////////
// CInPlaceList

CInPlaceListForEnum::CInPlaceListForEnum(CWnd* pParent, CRect& rect, DWORD dwStyle, UINT nID,
                           int nRow, int nColumn, 
                           COLORREF crFore, COLORREF crBack,
						   CStringArray& Items, CString sInitText,
						   UINT nFirstChar)
{
    m_crForeClr = crFore;
    m_crBackClr = crBack;

	m_nNumLines = 4;
	m_sInitText = sInitText;
 	m_nRow		= nRow;
 	m_nCol      = nColumn;
 	m_nLastChar = 0; 
	m_bExitOnArrows = FALSE; //(nFirstChar != VK_LBUTTON);	// If mouse click brought us here,

	// Create the combobox
 	DWORD dwComboStyle = WS_BORDER|WS_CHILD|WS_VISIBLE|WS_VSCROLL|
 					     CBS_AUTOHSCROLL | dwStyle;
	int nHeight = rect.Height();
	rect.bottom = rect.bottom + m_nNumLines*nHeight + ::GetSystemMetrics(SM_CYHSCROLL);
	if (!Create(dwComboStyle, rect, pParent, nID)) return;

	// Add the strings
	for (int i = 0; i < Items.GetSize(); i++) 
		AddString(Items[i]);

	SetFont(pParent->GetFont());
	SetItemHeight(-1, nHeight);

    int nMaxLength = GetCorrectDropWidth();
    /*
    if (nMaxLength > rect.Width())
	    rect.right = rect.left + nMaxLength;
	// Resize the edit window and the drop down window
	MoveWindow(rect);
    */

	SetDroppedWidth(nMaxLength);

	SetHorizontalExtent(0); // no horz scrolling

	// Set the initial text to m_sInitText
    if (::IsWindow(m_hWnd) && SelectString(-1, m_sInitText) == CB_ERR) 
		SetWindowText(m_sInitText);		// No text selected, so restore what was there before

    ShowDropDown();

    // Subclass the combobox edit control if style includes CBS_DROPDOWN
    if ((dwStyle & CBS_DROPDOWNLIST) != CBS_DROPDOWNLIST)
    {
        m_edit.SubclassDlgItem(IDC_COMBOEDIT, this);
 	    SetFocus();
        switch (nFirstChar)
        {
            case VK_LBUTTON: 
            case VK_RETURN:   m_edit.SetSel((int)_tcslen(m_sInitText), -1); return;
            case VK_BACK:     m_edit.SetSel((int)_tcslen(m_sInitText), -1); break;
            case VK_DOWN: 
            case VK_UP:   
            case VK_RIGHT:
            case VK_LEFT:  
            case VK_NEXT:  
            case VK_PRIOR: 
            case VK_HOME:  
            case VK_END:      m_edit.SetSel(0,-1); return;
            default:          m_edit.SetSel(0,-1);
        }
        SendMessage(WM_CHAR, nFirstChar);
    }
    else
 	    SetFocus();
}

CInPlaceListForEnum::~CInPlaceListForEnum()
{
}

void CInPlaceListForEnum::EndEdit()
{
	if (!IsGridCellValid())
	{
		this->DestroyWindow();	/* 关联表项无效时销毁列表框 */
		return;
	}

    CString str;
    if (::IsWindow(m_hWnd))
        GetWindowText(str);
 
    // Send Notification to parent
    GV_DISPINFO dispinfo;

    dispinfo.hdr.hwndFrom = GetSafeHwnd();
    dispinfo.hdr.idFrom   = GetDlgCtrlID();
    dispinfo.hdr.code     = GVN_ENDLABELEDIT;
 
    dispinfo.item.mask    = LVIF_TEXT|LVIF_PARAM;
    dispinfo.item.row     = m_nRow;
    dispinfo.item.col     = m_nCol;
    dispinfo.item.strText = str;
    dispinfo.item.lParam  = (LPARAM) m_nLastChar; 
 
    CWnd* pOwner = GetOwner();
    if (IsWindow(pOwner->GetSafeHwnd()))
        pOwner->SendMessage(WM_NOTIFY, GetDlgCtrlID(), (LPARAM)&dispinfo );
 
    // Close this window (PostNcDestroy will delete this)
    if (::IsWindow(m_hWnd))
        PostMessage(WM_CLOSE, 0, 0);
}

BOOL CInPlaceListForEnum::AttachToGridCell(CGridCell* pCell)
{
	m_pCell = pCell;
	return TRUE;
}
BOOL CInPlaceListForEnum::DetachFromGridCell()
{
	m_pCell = 0;
	return TRUE;
}
BOOL CInPlaceListForEnum::IsGridCellValid()
{
	if (m_pCell != 0)
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

int CInPlaceListForEnum::GetCorrectDropWidth()
{
    const int nMaxWidth = 200;  // don't let the box be bigger than this

    // Reset the dropped width
    int nNumEntries = GetCount();
    int nWidth = 0;
    CString str;

    CClientDC dc(this);
    int nSave = dc.SaveDC();
    dc.SelectObject(GetFont());

    int nScrollWidth = ::GetSystemMetrics(SM_CXVSCROLL);
    for (int i = 0; i < nNumEntries; i++)
    {
        GetLBText(i, str);
        int nLength = dc.GetTextExtent(str).cx + nScrollWidth;
        nWidth = max(nWidth, nLength);
    }
    
    // Add margin space to the calculations
    nWidth += dc.GetTextExtent(_T("0")).cx;

    dc.RestoreDC(nSave);

    nWidth = min(nWidth, nMaxWidth);

    return nWidth;
    //SetDroppedWidth(nWidth);
}

/*
// Fix by Ray (raybie@Exabyte.COM)
void CInPlaceList::OnSelendOK() 
{
    int iIndex = GetCurSel(); 
    if( iIndex != CB_ERR) 
    { 
        CString strLbText; 
        GetLBText( iIndex, strLbText); 
 
        if (!((GetStyle() & CBS_DROPDOWNLIST) == CBS_DROPDOWNLIST)) 
           m_edit.SetWindowText( strLbText); 
    } 
 
    GetParent()->SetFocus(); 	
}
*/

void CInPlaceListForEnum::PostNcDestroy() 
{
	CComboBox::PostNcDestroy();

	delete this;
}

BEGIN_MESSAGE_MAP(CInPlaceListForEnum, CComboBox)
	//{{AFX_MSG_MAP(CInPlaceListForEnum)
	ON_WM_KILLFOCUS()
	ON_WM_KEYDOWN()
	ON_WM_KEYUP()
	ON_CONTROL_REFLECT(CBN_DROPDOWN, OnDropdown)
	ON_WM_GETDLGCODE()
	ON_WM_CTLCOLOR_REFLECT()
	//}}AFX_MSG_MAP
	//ON_CONTROL_REFLECT(CBN_SELENDOK, OnSelendOK)
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CInPlaceList message handlers

UINT CInPlaceListForEnum::OnGetDlgCode() 
{
    return DLGC_WANTALLKEYS;
}

void CInPlaceListForEnum::OnDropdown() 
{
    SetDroppedWidth(GetCorrectDropWidth());
}

void CInPlaceListForEnum::OnKillFocus(CWnd* pNewWnd) 
{
	CComboBox::OnKillFocus(pNewWnd);

	if (GetSafeHwnd() == pNewWnd->GetSafeHwnd())
        return;

    // Only end editing on change of focus if we're using the CBS_DROPDOWNLIST style
    if ((GetStyle() & CBS_DROPDOWNLIST) == CBS_DROPDOWNLIST)
        EndEdit();
}

// If an arrow key (or associated) is pressed, then exit if
//  a) The Ctrl key was down, or
//  b) m_bExitOnArrows == TRUE
void CInPlaceListForEnum::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	if ((nChar == VK_PRIOR || nChar == VK_NEXT ||
		 nChar == VK_DOWN  || nChar == VK_UP   ||
		 nChar == VK_RIGHT || nChar == VK_LEFT) &&
		(m_bExitOnArrows || GetKeyState(VK_CONTROL) < 0))
	{
		m_nLastChar = nChar;
		GetParent()->SetFocus();
		return;
	}

	CComboBox::OnKeyDown(nChar, nRepCnt, nFlags);
}

// Need to keep a lookout for Tabs, Esc and Returns.
void CInPlaceListForEnum::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	if (nChar == VK_ESCAPE) 
		SetWindowText(m_sInitText);	// restore previous text

	if (nChar == VK_TAB || nChar == VK_RETURN || nChar == VK_ESCAPE)
	{
		m_nLastChar = nChar;
		GetParent()->SetFocus();	// This will destroy this window
		return;
	}

	CComboBox::OnKeyUp(nChar, nRepCnt, nFlags);
}

HBRUSH CInPlaceListForEnum::CtlColor(CDC* /*pDC*/, UINT /*nCtlColor*/) 
{
    /*
    static CBrush brush(m_crBackClr);
    pDC->SetTextColor(m_crForeClr);
    pDC->SetBkMode(TRANSPARENT);
    return (HBRUSH) brush.GetSafeHandle();
    */
	
	// TODO: Return a non-NULL brush if the parent's handler should not be called
	return NULL;
}

/////////////////////////////////////////////////////////////////////////////
// CGridCellCombo 
/////////////////////////////////////////////////////////////////////////////


IMPLEMENT_DYNCREATE(CXMLGridCellForEnum, CGridCell)

CXMLGridCellForEnum::CXMLGridCellForEnum() : CGridCell()
{
    SetStyle(CBS_DROPDOWN);  // CBS_DROPDOWN, CBS_DROPDOWNLIST, CBS_SIMPLE, CBS_SORT
}
CXMLGridCellForEnum::~CXMLGridCellForEnum()
{
	/* 当表项被销毁时要将列表控件与表项分离，确保列表控件不会再对表项产生编辑动作 */
	if (m_pEditWnd)
		((CInPlaceListForEnum*)m_pEditWnd)->DetachFromGridCell();
}

// Create a control to do the editing
BOOL CXMLGridCellForEnum::Edit(int nRow, int nCol, CRect rect, CPoint /* point */, UINT nID, UINT nChar)
{
    m_bEditing = TRUE;
    
    // CInPlaceList auto-deletes itself
    m_pEditWnd = new CInPlaceListForEnum(GetGrid(), rect, GetStyle(), nID, nRow, nCol, 
                                  GetTextClr(), GetBackClr(), m_Strings, GetText(), nChar);
	if (m_pEditWnd)
		((CInPlaceListForEnum*)m_pEditWnd)->AttachToGridCell(this);
    return TRUE;
}

CWnd* CXMLGridCellForEnum::GetEditWnd() const
{
	if (m_pEditWnd 
		&& /* 当未使用CBS_DROPDOWNLIST风格时才返回编辑窗口 */
		(m_pEditWnd->GetStyle() & CBS_DROPDOWNLIST) != CBS_DROPDOWNLIST )	
		return &(((CInPlaceListForEnum*)m_pEditWnd)->m_edit);

	return NULL;
}


CSize CXMLGridCellForEnum::GetCellExtent(CDC* pDC)
{    
    CSize sizeScroll (GetSystemMetrics(SM_CXVSCROLL), GetSystemMetrics(SM_CYHSCROLL));    
    CSize sizeCell (CGridCell::GetCellExtent(pDC));    
    sizeCell.cx += sizeScroll.cx;    
    sizeCell.cy = max(sizeCell.cy,sizeScroll.cy);    
    return sizeCell;
}

// Cancel the editing.
void CXMLGridCellForEnum::EndEdit()
{
    if (m_pEditWnd)
        ((CInPlaceListForEnum*)m_pEditWnd)->EndEdit();
}

// Override draw so that when the cell is selected, a drop arrow is shown in the RHS.
BOOL CXMLGridCellForEnum::Draw(CDC* pDC, int nRow, int nCol, CRect rect,  BOOL bEraseBkgnd /*=TRUE*/)
{
#ifdef _WIN32_WCE
    return CGridCell::Draw(pDC, nRow, nCol, rect,  bEraseBkgnd);
#else
    // Cell selected?
    //if ( !IsFixed() && IsFocused())
    if (GetGrid()->IsCellEditable(nRow, nCol) && !IsEditing())
    {
        // Get the size of the scroll box
        CSize sizeScroll(GetSystemMetrics(SM_CXVSCROLL), GetSystemMetrics(SM_CYHSCROLL));

        // enough room to draw?
        if (sizeScroll.cy < rect.Width() && sizeScroll.cy < rect.Height())
        {
            // Draw control at RHS of cell
            CRect ScrollRect = rect;
            ScrollRect.left   = rect.right - sizeScroll.cx;
            ScrollRect.bottom = rect.top + sizeScroll.cy;

            // Do the draw 
            pDC->DrawFrameControl(ScrollRect, DFC_SCROLL, DFCS_SCROLLDOWN);

            // Adjust the remaining space in the cell
            rect.right = ScrollRect.left;
        }
    }

    CString strTempText = GetText();
    if (IsEditing())
        SetText(_T(""));

    // drop through and complete the cell drawing using the base class' method
    BOOL bResult = CGridCell::Draw(pDC, nRow, nCol, rect,  bEraseBkgnd);

    if (IsEditing())
        SetText(strTempText);

	return bResult;
#endif
}

// For setting the strings that will be displayed in the drop list
void CXMLGridCellForEnum::SetOptions(const CStringArray& ar)
{ 
    m_Strings.RemoveAll();
    for (int i = 0; i < ar.GetSize(); i++)
        m_Strings.Add(ar[i]);
}