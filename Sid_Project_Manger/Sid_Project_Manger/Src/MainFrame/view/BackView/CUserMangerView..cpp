#include "stdafx.h"
#include "CUserMangerView.h"
#include "../../MainFrm.h"
#include "Src/action/factory/actionServerFactory.h"
#include "Src/MainFrame/Dlg/BackMangerDlg/DlgNewUser.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


// CEventView


IMPLEMENT_DYNCREATE(CUserMangerView, CBaseFormView)

CUserMangerView::CUserMangerView()
	: CBaseFormView(CUserMangerView::IDD)
{
	 
}

CUserMangerView::~CUserMangerView()
{
}

void CUserMangerView::DoDataExchange(CDataExchange* pDX)
{
	CBaseFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CEventView)	
	 DDX_Control(pDX, IDC_COMBO_FIND, m_findTextBox);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CUserMangerView, CBaseFormView)
	//{{AFX_MSG_MAP(CEventView)
	//}}AFX_MSG_MAP
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_WM_CREATE()
	//ON_BN_CLICKED(IDC_BTN_FRESH, &CUserMangerView::OnBnClickedBtnFresh)

	ON_NOTIFY( GVN_ENDLABELEDIT, ID_GRID_USER_MANGER, OnGridEndEidt )
	ON_NOTIFY( NM_RCLICK, ID_GRID_USER_MANGER, OnGridRClick )

	ON_COMMAND( IDM_USER_CLEARN_PWD, OnClearnUserPwd)
	ON_COMMAND( IDM_DELTE_OBJ, OnDeleteUser ) 

	ON_CBN_SELCHANGE(IDC_COMBO_FIND, OnCbnSelchangeComboFind)
	//ON_NOTIFY(GVN_SELCHANGED,	ID_GRID_PUBLISH,	OnGridSelChanged)
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_BTN_NEW_USER, OnBnClickedBtnNewUser)
END_MESSAGE_MAP()


// CEventView 诊断

#ifdef _DEBUG
void CUserMangerView::AssertValid() const
{
	CBaseFormView::AssertValid();
}
#ifndef _WIN32_WCE
void CUserMangerView::Dump(CDumpContext& dc) const
{
	CBaseFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CEventView 消息处理程序
void CUserMangerView::OnInitialUpdate() 
{
	BOOL isChange = m_bFirstActive;
	CBaseFormView::OnInitialUpdate();
	if ( isChange )
	{
		OnInitDepart();
		m_taskGrid.GetFocus();
	}
}

int CUserMangerView::CreateCtrl()
{
	CRect rect;
	this->GetWindowRect( rect );
	ClientToScreen( rect );
	CRect gridRect( 0, 80, rect.Width(), rect.Height() );

	if (!m_taskGrid.Create(rect,this,ID_GRID_USER_MANGER ))
	{//带滚动条
		TRACE("远方服务器信息Table创建失败.\n");
		return -1;
	}
	CActionServerFactory* pActionFactory = ((CMainFrame *)AfxGetMainWnd())->GetSeverFactory();
	ASSERT( NULL != pActionFactory );

	m_gridManger.reset( new CUserGridManger( pActionFactory ));
	if ( m_gridManger.get() )
	{
		m_gridManger->RegisterAsGridView( &m_taskGrid );
		m_gridManger->CreateGridView();
	}
	return 0;
}

int CUserMangerView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	return CreateCtrl();
}
void CUserMangerView::OnInitDepart()
{
	if ( m_gridManger.get() )
	{
		const ListDepart &depart = m_gridManger->GetAllDepart();
		int cnt = 0;
		m_findTextBox.InsertString( cnt++, _T("所有部门") );
		for ( ListDepartConstItor itor = depart.begin(); itor != depart.end(); itor++ )
		{
			m_findTextBox.InsertString( cnt++, itor->GetName() );
		}
		m_findTextBox.SetCurSel( 0 );
	}
	
}
void CUserMangerView::OnSize(UINT nType, int cx, int cy)
{
	CBaseFormView::OnSize(nType, cx, cy, m_taskGrid);
}

////网格文本框编辑结束
void CUserMangerView::OnGridEndEidt( NMHDR* pNMHDR,LRESULT* pResult )
{
	if ( m_gridManger.get() )
	{
		m_gridManger->OnGridEndEidt( pNMHDR, pResult );
	}
}
void CUserMangerView::OnGridRClick( NMHDR* pNMHDR,LRESULT* pResult )
{
	if ( m_gridManger.get() )
	{
		m_gridManger->OnGridRclick( pNMHDR, pResult );
	}
}


void CUserMangerView::OnDeleteUser( void )
{
	if ( m_gridManger.get() )
	{
		m_gridManger->OnDeleteUser();
	}
}

void CUserMangerView::OnClearnUserPwd( void )
{
	if ( m_gridManger.get() )
	{
		m_gridManger->OnClearnPwd();
	}
}

void CUserMangerView::OnBnClickedBtnNewUser()
{
	CDlgNewUser dlg;
	if ( IDOK == dlg.DoModal()  )
	{
		m_gridManger->UpdateDb();
	}
	
}
void CUserMangerView::OnCbnSelchangeComboFind()
{
	if ( m_gridManger.get() )
	{
		int curSel = m_findTextBox.GetCurSel();
		const ListDepart& lst = m_gridManger->GetAllDepart();
		if ( curSel > 0 && lst.size() > 0 )
		{
		   ListDepartConstItor itor = lst.begin();
		   advance( itor, curSel - 1);
		   if ( itor != lst.end() )
		   {
			   curSel = itor->GetId();
		   }
		}
		m_gridManger->UpdateGridByDepart( curSel );
	}
}


