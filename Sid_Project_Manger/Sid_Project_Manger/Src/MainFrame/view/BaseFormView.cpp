#include "stdafx.h"
#include "BaseFormView.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

IMPLEMENT_DYNCREATE(CBaseFormView, CFormView)

CBaseFormView::CBaseFormView( UINT id )
	: CFormView(id)
{
	 m_bFirstActive = TRUE;
}
//
CBaseFormView::CBaseFormView( )
	:CFormView( (UINT)0 )
{
     m_bFirstActive = TRUE;
}
CBaseFormView::~CBaseFormView()
{
}

void CBaseFormView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CEventView)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CBaseFormView, CFormView)
	//{{AFX_MSG_MAP(CEventView)
	//}}AFX_MSG_MAP
	ON_WM_DESTROY()
	ON_WM_CREATE()
END_MESSAGE_MAP()


// CEventView Õï¶Ï

#ifdef _DEBUG
void CBaseFormView::AssertValid() const
{
	CFormView::AssertValid();
}
#ifndef _WIN32_WCE
void CBaseFormView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG

BOOL CBaseFormView::CreateFormCtrl(UINT nID, CWnd *pParent)
{
	if (!pParent || !pParent->GetSafeHwnd())
	{
		return FALSE;
	}
	//CWnd *pCtrl = pParent->GetDlgItem(nID);
	//if (!pCtrl)
	//{
	//	return FALSE;
	//}
	CRect rcCtrl;
	//pParent->GetWindowRect(rcCtrl);
	//pParent->ScreenToClient(rcCtrl);
	//UINT style = ::GetWindowLong(/*pCtrl->*/pParent->GetSafeHwnd(), GWL_STYLE);
	//pCtrl->DestroyWindow();
	UINT style = WS_CHILD | WS_VISIBLE|WS_VSCROLL|WS_THICKFRAME;

	return Create(NULL, NULL, style,
		rcCtrl, pParent, nID, NULL);
}
void CBaseFormView::OnInitialUpdate()
{
	if ( TRUE == m_bFirstActive )
	{
		m_bFirstActive = FALSE;
		CFormView::OnInitialUpdate();
	}
}

int CBaseFormView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFormView::OnCreate(lpCreateStruct) == -1)
		return -1;
	return 0;
}
void CBaseFormView::OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView)
{
	CFormView::OnActivateView( bActivate, pActivateView, pDeactiveView );
	if ( m_bFirstActive )
	{
		OnInitialUpdate();
	}

}
void       CBaseFormView::OnSize( UINT nType, int cx, int cy,CGridCtrl &ctrObj )
{
	CFormView::OnSize(nType, cx, cy);
	if ( ctrObj.GetSafeHwnd() )
	{
		ctrObj.MoveWindow( 0, 100, cx, cy-100);
	}
}

