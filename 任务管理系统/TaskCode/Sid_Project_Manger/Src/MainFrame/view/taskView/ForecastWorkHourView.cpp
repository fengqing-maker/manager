#include "stdafx.h"
#include "ForecastWorkHourView.h"
#include "../../MainFrm.h"
#include "../../messageDef.h"
#include "Src/action/factory/actionServerFactory.h"
#include "Src/MainFrame/Dlg/DlgUserChose.h"
#include "Src/Tools/UtilityTool/UtilityTool.h"

#ifdef _DEBUG
#undef THIS_FILE
	static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


// CEventView
IMPLEMENT_DYNCREATE(CForecastWorkHourView, CBaseFormView)

CForecastWorkHourView::CForecastWorkHourView()
	: CBaseFormView(CForecastWorkHourView::IDD)
	
{
	m_showBeginDate = COleDateTime::GetCurrentTime();
}

CForecastWorkHourView::~CForecastWorkHourView()
{
}

void CForecastWorkHourView::DoDataExchange(CDataExchange* pDX)
{
	CBaseFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CEventView)
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_COMBO_FIND_OBJ, m_ctlFindObj);
	DDX_DateTimeCtrl(pDX, IDC_DATE_BEGINE, m_showBeginDate);
}

BEGIN_MESSAGE_MAP(CForecastWorkHourView, CBaseFormView)
	//{{AFX_MSG_MAP(CEventView)
	//}}AFX_MSG_MAP
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_WM_CREATE()
	
	ON_NOTIFY(DTN_DATETIMECHANGE, IDC_DATE_BEGINE, &CForecastWorkHourView::OnDtnDatetimechangeDateBegine)
	ON_CBN_SELCHANGE(IDC_COMBO_FIND_OBJ, &CForecastWorkHourView::OnSelchangeComboFindObj)
END_MESSAGE_MAP()


// CEventView 诊断

#ifdef _DEBUG
void CForecastWorkHourView::AssertValid() const
{
	CBaseFormView::AssertValid();
}
#ifndef _WIN32_WCE
void CForecastWorkHourView::Dump(CDumpContext& dc) const
{
	CBaseFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CEventView 消息处理程序
void CForecastWorkHourView::OnInitialUpdate() 
{
	BOOL isChange = m_bFirstActive;
	CBaseFormView::OnInitialUpdate();
	if ( isChange )
	{
		InitCtrl();
	}
	UpdateData( FALSE );
	GetDlgItem(IDC_DATE_BEGINE)->GetFocus();
}

int CForecastWorkHourView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	return CreateCtrl();
}
int CForecastWorkHourView::CreateCtrl()
{
	CRect rect;
	this->GetWindowRect( rect );
	ClientToScreen( rect );

	CRect gridRect( 0, 60, rect.Width(), rect.Height() );
	if (!m_taskGrid.Create(gridRect,this,ID_GRID_SCHEDULE ))
	{//带滚动条
		TRACE("远方服务器信息Table创建失败.\n");
		return -1;
	}
	CActionServerFactory* pActionFactory = ((CMainFrame *)AfxGetMainWnd())->GetSeverFactory();
	ASSERT( NULL != pActionFactory );
	m_pActionFactory = pActionFactory;
	m_loginUser = m_pActionFactory->GetLoginServer()->GetLoginUser();
	m_gridManger.reset( new CForecastWorkHour(pActionFactory));
	if ( m_gridManger.get() )
	{
		m_gridManger->RegisterAsGridView( &m_taskGrid );
		m_gridManger->CreateGridView();
	}
	return 0;
}


void CForecastWorkHourView::OnSize(UINT nType, int cx, int cy)
{
	CBaseFormView::OnSize(nType, cx, cy, m_taskGrid );
}

void CForecastWorkHourView::InitCtrl()
{
	CString roleName = m_pActionFactory->GetLoginServer()->GetLoginRole().GetRoleName();
	m_pActionFactory->GetDepartSever()->GetDepartByMangerId( m_loginUser.GetId(), roleName, m_lstDepart );
	int departCnt = m_lstDepart.size();
	if ( m_lstDepart.size() > 0 )
	{
		//m_ctlFindObj.InsertString( 0, _T("所管理部门员工") );
		//2016-1-31 add 
		int i = 0;
		for ( ListDepartItor itor = m_lstDepart.begin(); itor != m_lstDepart.end(); itor++ )
		{
			m_ctlFindObj.InsertString( i, itor->GetName() );
			i++;
		}
	}else
	{
		m_ctlFindObj.InsertString( 0, _T("登陆用户") );
	}
	BOOL hasPowerReadAll = m_pActionFactory->GetLoginServer()->FindLoginPower( LookUpAllPrj );
	if ( m_lstDepart.size() > 5 )
	{
		m_pActionFactory->GetProjectSever()->GetProjectByUser( m_loginUser, hasPowerReadAll, m_lstProject );
	}
	int index = departCnt > 0 ? departCnt:1;
	for ( ListProject::iterator itor = m_lstProject.begin(); itor != m_lstProject.end(); itor++ )
	{
		m_ctlFindObj.InsertString( index++, itor->GetShortName() );
	}
	int selPos = 0;
	if ( departCnt > 2 )
	{
		selPos = 2;
	}
	m_ctlFindObj.SetCurSel( selPos );
	m_gridManger->SetSelDepartUser( m_lstDepart, selPos );

}

void CForecastWorkHourView::OnDtnDatetimechangeDateBegine(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMDATETIMECHANGE pDTChange = reinterpret_cast<LPNMDATETIMECHANGE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	UpdateData( TRUE );
	m_gridManger->SetBeginWorkDate( m_showBeginDate );

	*pResult = 0;
}


void CForecastWorkHourView::OnSelchangeComboFindObj()
{
	int selId = m_ctlFindObj.GetCurSel();
	if ( selId >= 0 )
	{
		int departCnt = m_lstDepart.size();
		if ( departCnt > 0 && selId > departCnt )
		{
			selId -= departCnt;
			ListProject::iterator itor = m_lstProject.begin();
			advance( itor, selId );
			m_gridManger->SetSelProject( itor->GetId() );
		}else
		{
			m_gridManger->SetSelDepartUser( m_lstDepart, selId );
		}
	}
}
