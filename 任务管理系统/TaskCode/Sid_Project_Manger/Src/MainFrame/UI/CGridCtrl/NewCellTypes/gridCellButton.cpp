#include "stdafx.h"  
#include "../GridCell.h"  
#include "../GridCtrl.h"  
#include "GridCellButton.h" 

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

	IMPLEMENT_DYNCREATE(CGridCellButton, CGridCell)  

CGridCellButton::CGridCellButton(void)  
{  
    m_bPushing = FALSE;  
}  
CGridCellButton::~CGridCellButton(void)  
{  
}  
BOOL CGridCellButton::Draw(CDC* pDC, int nRow, int nCol, CRect rect, BOOL bEraseBkgnd /* = TRUE */)  
{   
	  if ( GetText() == _T("") )
	  {//2015-4-17 lhz add 
		  return CGridCell::Draw( pDC, nRow,nCol,rect,bEraseBkgnd);
	  }
      m_rect = rect;  
	  pDC->SetBkMode(TRANSPARENT);  
	  rect.DeflateRect( GetMargin(), 0);  
	  CFont* pOldFont = pDC->SelectObject(GetFontObject());  
	  pDC->DrawFrameControl(rect, DFC_BUTTON, m_bPushing?DFCS_BUTTONPUSH | DFCS_PUSHED:DFCS_BUTTONPUSH);  
	  COLORREF ColorCurrent = pDC->GetTextColor();  
      pDC->SetTextColor(::GetSysColor(COLOR_BTNTEXT));  
      pDC->DrawText(GetText(), -1, rect, DT_CENTER|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS);  
	  pDC->SetTextColor( ColorCurrent);  
	  return TRUE;  
}  
void CGridCellButton::OnClick(CPoint PointCellRelative)  
{  
	    m_bPushing = !m_bPushing;  
        GetGrid()->InvalidateRect(m_rect);  
}  
