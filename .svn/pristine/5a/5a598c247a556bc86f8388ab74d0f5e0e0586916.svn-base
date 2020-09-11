#include "stdafx.h"
#include "CDepartTaskView.h"
#include "../../MainFrm.h"
#include "Src/action/factory/actionServerFactory.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


// CEventView


IMPLEMENT_DYNCREATE(CDepartTaskView, CBaseFormView)

CDepartTaskView::CDepartTaskView()
	: CBaseFormView(CDepartTaskView::IDD)
	, m_diffPerCondition(-100)
	, m_editSerarchYear(0)
{
	m_pActionFactory = NULL;
}

CDepartTaskView::~CDepartTaskView()
{
}

void CDepartTaskView::DoDataExchange(CDataExchange* pDX)
{
	CBaseFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CEventView)
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_COMBO_FIND, m_ctrlDepart);
	DDX_Control(pDX, IDC_COMBO_TASK_STATUS, m_taskStatusCtrl);
	DDX_Control(pDX, IDC_COMBO_FIND_OWNER, m_ctrlOwnern);
	DDX_Text(pDX, IDC_EDIT_DIFF_PER, m_diffPerCondition);
	DDX_Text(pDX, IDC_EDIT_SEARCH_YEAR, m_editSerarchYear);
	DDX_Control(pDX, IDC_COMBO_QUARTER, m_comboQuarter);
}

BEGIN_MESSAGE_MAP(CDepartTaskView, CBaseFormView)
	//{{AFX_MSG_MAP(CEventView)
	//}}AFX_MSG_MAP
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_WM_CREATE()

	ON_CBN_SELCHANGE(IDC_COMBO_FIND, &CDepartTaskView::OnCbnSelchangeComboFind)
	//ON_BN_CLICKED(IDC_BTN_FRESH, &CProjectView::OnBnClickedBtnFresh)
	ON_NOTIFY( NM_CLICK, ID_GRID_PROJECT,  OnGridClick )
	ON_NOTIFY( NM_RCLICK, ID_GRID_PROJECT,  OnRclickGrid )
	ON_NOTIFY(GVN_SELCHANGED,	ID_GRID_PROJECT,OnGridSelChange)
	ON_CBN_SELCHANGE(IDC_COMBO_TASK_STATUS, &CDepartTaskView::OnCbnSelchangeComboTaskStatus)
	ON_BN_CLICKED(IDC_BTN_IMPORT, &CDepartTaskView::OnBnClickedBtnImport)
	ON_BN_CLICKED(IDC_BTN_EXPORT, &CDepartTaskView::OnBnClickedBtnExport)
	ON_BN_CLICKED(IDC_BUTTON_SEARCH, &CDepartTaskView::OnBnClickedButtonSearch)

	ON_COMMAND( IDM_TASK_DEPART_REVIEW, OnDepartReview )
	ON_COMMAND( IDM_TASK_MOIDY_SCORE, OnMoidyScore )
	ON_BN_CLICKED(IDC_BTN_EXPORT_TABLE, &CDepartTaskView::OnBnClickedBtnExportTable)
END_MESSAGE_MAP()


// CEventView 诊断

#ifdef _DEBUG
void CDepartTaskView::AssertValid() const
{
	CBaseFormView::AssertValid();
}
#ifndef _WIN32_WCE
void CDepartTaskView::Dump(CDumpContext& dc) const
{
	CBaseFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CEventView 消息处理程序
void CDepartTaskView::OnInitialUpdate() 
{
	BOOL isChange = m_bFirstActive;
	CBaseFormView::OnInitialUpdate();
	if ( isChange )
	{
		OnInitCtrl();
	}
	m_taskGrid.GetFocus();
}

void CDepartTaskView::OnInitOwnerList()
{	
	if ( m_pActionFactory )
	{
		int selPos = m_ctrlDepart.GetCurSel();
		const ListDepart & listDepart = m_gridManger->GetOwnerDepartment();
		if ( listDepart.size() > 0 )
		{
			if ( selPos > 0  )
			{
				ListDepartConstItor itor = listDepart.begin(); 
				advance( itor, selPos - 1 );
				if ( itor != listDepart.end() )
				{
					m_lstUser.clear();
					m_pActionFactory->GetDepartSever()->GetDepartUser( itor->GetId(), m_lstUser, FALSE );
				}
			}else
			{
				m_lstUser.clear();
				for (  ListDepartConstItor itor = listDepart.begin(); itor != listDepart.end(); itor++ )
				{
					m_pActionFactory->GetDepartSever()->GetDepartUser( itor->GetId(), m_lstUser, FALSE );
				}
			}
			m_ctrlOwnern.ResetContent();
			m_ctrlOwnern.InsertString( 0, _T("所有部门员工") );
			int count = 1;
			for ( ListUserItor itorUser = m_lstUser.begin(); itorUser != m_lstUser.end(); itorUser++ )
			{
				m_ctrlOwnern.InsertString( count++ , itorUser->GetUserName_R() );
			}
			m_ctrlOwnern.SetCurSel( 0 );
		}
	}
}
int  CDepartTaskView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	return CreateCtrl();
}
void CDepartTaskView::OnInitCtrl()
{
	if ( m_gridManger.get() )
	{
		//初始化所有部门索引
		const ListDepart & listDepart = m_gridManger->GetOwnerDepartment();
		m_ctrlDepart.InsertString(0, _T("所有部门") );
		int cnt = 1;
		for ( ListDepart::const_iterator itor = listDepart.begin(); itor != listDepart.end(); itor++ )
		{
			m_ctrlDepart.InsertString(cnt++, itor->GetName() );
		}
		if ( listDepart.size() > 0 )
		{
			m_ctrlDepart.SetCurSel( 1 );
		}
		///初始化所有状态
		const CTaskStaus obj = m_pActionFactory->GetTaskSever()->GetTaskStausMap();
		const TaskIdStringMap maps = obj.GetTaskStringMap();
		m_taskStatusCtrl.InsertString( 0, _T("所有状态") );
		m_taskStatusCtrl.SetCurSel( 0 );
		cnt = 1;
		for( TaskIdStringMap::const_iterator itor = maps.begin(); itor != maps.end(); itor++ )
		{
			m_taskStatusCtrl.InsertString( cnt, itor->second );
			if ( itor->first == ongoing )
			{
				m_taskStatusCtrl.SetCurSel( cnt );
			}
			cnt++;
		}
		//初始化负责人列表
		OnInitOwnerList();
        //
		m_diffPerCondition = -100;
		//初始化季度信息
		m_editSerarchYear = COleDateTime::GetCurrentTime().GetYear();
		m_comboQuarter.InsertString( 0, _T("无") );
		m_comboQuarter.InsertString( 1, _T("第一季度") );
		m_comboQuarter.InsertString( 2, _T("第二季度") );
		m_comboQuarter.InsertString( 3, _T("第三季度") );
		m_comboQuarter.InsertString( 4, _T("第四季度") );
		int selQuarter = (COleDateTime::GetCurrentTime().GetMonth()-1)/3 + 1;
		m_comboQuarter.SetCurSel( selQuarter );
		//
		UpdateData( FALSE );
	}
}
int CDepartTaskView::CreateCtrl()
{
	CRect rect;
	this->GetWindowRect( rect );
	ClientToScreen( rect );
	CRect gridRect( 0, 80, rect.Width(), rect.Height()-20 );

	if (!m_taskGrid.Create(rect,this,ID_GRID_PROJECT ))
	{//带滚动条
		TRACE("远方服务器信息Table创建失败.\n");
		return -1;
	}
	m_pActionFactory = ((CMainFrame *)AfxGetMainWnd())->GetSeverFactory();
	ASSERT( NULL != m_pActionFactory );
	m_gridManger.reset( new CDepartTaskManger( m_pActionFactory ));
	if ( m_gridManger.get() )
	{
		m_gridManger->RegisterAsGridView( &m_taskGrid );
		m_gridManger->CreateGridView();

		::SendMessage( GetParent()->GetParent()->GetSafeHwnd(), 
			PRJ_VIEW_MSG_CHANGE_TASKLIST, (WPARAM)(m_gridManger->GetTaskListPoint()), NULL );
	}

	
	return 0;
}

void CDepartTaskView::OnSize(UINT nType, int cx, int cy)
{
	CBaseFormView::OnSize( nType, cx, cy, m_taskGrid );
	if ( m_taskGrid.GetSafeHwnd() )
	{
		m_taskGrid.MoveWindow( 0, 100, cx, cy-100);
	}
}

void CDepartTaskView::OnCbnSelchangeComboFind()
{
	OnInitOwnerList();
}
void    CDepartTaskView:: SetSelDepartById( int id, int taskSt /*= -1*/  )
{
	UpdateData( TRUE );
	if ( m_gridManger.get()  )
	{
		const ListDepart lst =  m_gridManger->GetOwnerDepartment();
		int index = 0;
		for ( ListDepartConstItor itor = lst.begin(); itor != lst.end(); itor++ )
		{
			if ( itor->GetId() == id )
			{
				m_ctrlDepart.SetCurSel( index+1 ); //跳过所有部门
				OnInitOwnerList();
				//int selSt;
				//if ( taskSt < 0 )
				//{
				//	selSt = 0;
				//}
				//if ( selSt < m_taskStatusCtrl.GetCount() )
				//{
				//	m_taskStatusCtrl.SetCurSel( selSt );
				//}
				int culSel = m_comboQuarter.GetCurSel();
				m_gridManger->SetSelDepart( id, taskSt, 0, m_diffPerCondition, m_editSerarchYear, culSel); //statMpa 的第几个
				this->Invalidate( );
				return;
			}
			index++;
		}
	}
}
void CDepartTaskView::OnGridClick( NMHDR* pNMHDR,LRESULT* pResult )
{
	if ( m_gridManger.get() )
	{
		m_gridManger->OnGridClick( pNMHDR, pResult );
	}
}

void CDepartTaskView::OnCbnSelchangeComboTaskStatus()
{
	//if ( m_gridManger.get()  )
	//{
	//	int selIndex = m_taskStatusCtrl.GetCurSel() ;
	//	m_gridManger->UpdateTask( m_taskStatusCtrl.GetCurSel() );
	//}
}


void CDepartTaskView::OnBnClickedBtnImport()
{
	const ListDepart lst =  m_gridManger->GetOwnerDepartment();
	int index = m_ctrlDepart.GetCurSel();
	if ( lst.size() > 0 && m_gridManger.get() && index > 0 )
	{
		ListDepartConstItor itor = lst.begin();
		advance( itor, index - 1 );
		if ( itor != lst.end() )
		{
		   m_gridManger->ImportDepartTask();
		}else
		{
			AfxMessageBox( _T("未选择项目") );
		}
	}else
	{
		AfxMessageBox( _T("未选择项目") );
	}
}

void CDepartTaskView::OnBnClickedBtnExport()
{
	const ListDepart lst =  m_gridManger->GetOwnerDepartment();
	int index = m_ctrlDepart.GetCurSel();
	if ( lst.size() > 0 && m_gridManger.get() && index >= 0 )
	{
		ListDepartConstItor itor = lst.begin();
		advance( itor, index );
		if ( itor != lst.end() )
		{
			m_gridManger->ExportDepartTask( );
		}else
		{
			AfxMessageBox( _T("未选择项目") );
		}
	}
}


void CDepartTaskView::OnBnClickedButtonSearch()
{
	UpdateData( TRUE );
	int selIndex = m_ctrlDepart.GetCurSel();
	if ( m_gridManger.get() && selIndex >= 0 )
	{
		int stIndex = m_taskStatusCtrl.GetCurSel();
		int prjId = 0;
		const ListDepart &list = m_gridManger->GetOwnerDepartment();
		if ( list.size() > 0 )
		{
			ListDepartConstItor itor = list.begin();
			if ( selIndex > 0 )
			{
				advance( itor, selIndex - 1);
				selIndex = itor->GetId();
			}
			int selUserId = 0;
			int selUserIndex = m_ctrlOwnern.GetCurSel();
			if ( selUserIndex > 0 && m_lstUser.size() > 0 )
			{
				ListUserItor itorUser = m_lstUser.begin();
				advance( itorUser, selUserIndex - 1 );
				if ( itorUser != m_lstUser.end() )
				{
					selUserId = itorUser->GetId();
				}
			}
			int selQuarter = m_comboQuarter.GetCurSel();
			m_gridManger->SetSelDepart( selIndex, stIndex, selUserId, m_diffPerCondition, m_editSerarchYear, selQuarter );
			::SendMessage( GetParent()->GetParent()->GetSafeHwnd(), 
				PRJ_VIEW_MSG_CHANGE_TASKLIST, (WPARAM)(m_gridManger->GetTaskListPoint()), NULL );
		}
	}
}
void CDepartTaskView::OnGridSelChange( NMHDR* pNMHDR,LRESULT* pResult )
{
	if ( m_gridManger.get() )
	{
		m_gridManger->OnGridSelChange( pNMHDR, pResult );
	}
}
void CDepartTaskView::OnDepartReview()
{
	if ( m_gridManger.get() )
	{
		m_gridManger->OnGridDepartReview();
	}
}
void CDepartTaskView::OnMoidyScore()
{
	if ( m_gridManger.get() )
	{
		m_gridManger->OnGridDepartMoidyScore();
	}
}
void CDepartTaskView::OnRclickGrid( NMHDR* pNMHDR,LRESULT* pResult )
{
	if ( m_gridManger.get() )
	{
		m_gridManger->OnGridRclick( pNMHDR, pResult );
	}
}
//导出绩效考核表
void CDepartTaskView::OnBnClickedBtnExportTable()
{
   UpdateData( TRUE );
   int quart = m_comboQuarter.GetCurSel();
   if ( m_editSerarchYear <= 2000)
   {
	   AfxMessageBox(_T("填写年错误,年必须大于2000年"));
	   return;
   }
   int selDepart = m_ctrlDepart.GetCurSel();
   const ListDepart &list = m_gridManger->GetOwnerDepartment();
   int selUserId =  m_ctrlOwnern.GetCurSel();
   if ( list.size() > 0 )
   {
	   ListDepartConstItor itor = list.begin();
	   if ( selDepart > 0 )
	   {
		   advance( itor, selDepart - 1);
		   selDepart = itor->GetId();
	   }
	   selUserId = m_ctrlOwnern.GetCurSel();
	   if ( selUserId > 0 && m_lstUser.size() > 0 )
	   {
		   ListUserItor itorUser = m_lstUser.begin();
		   advance( itorUser, selUserId - 1 );
		   if ( itorUser != m_lstUser.end() )
		   {
			   selUserId = itorUser->GetId();
		   }
	   }
   }
   if ( m_gridManger.get())
   {
	     
	    m_gridManger->ExportPerformanTask( m_editSerarchYear, quart, selDepart, selUserId);
   }
  
}
ListTask*  CDepartTaskView::GetTaskListPoint()
{
	if ( m_gridManger.get() )
	{
		return m_gridManger->GetTaskListPoint();
	}
	return NULL;
}
