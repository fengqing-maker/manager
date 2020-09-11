#include "stdafx.h"
#include "CProjectTaskView.h"
#include "../../MainFrm.h"
#include "Src/action/factory/actionServerFactory.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


// CEventView


IMPLEMENT_DYNCREATE(CProjectTaskView, CBaseFormView)

CProjectTaskView::CProjectTaskView()
	: CBaseFormView(CProjectTaskView::IDD)
	, m_diffPerCondition(-100)
{
	m_pActionFactory = FALSE;

}

CProjectTaskView::~CProjectTaskView()
{
}

void CProjectTaskView::DoDataExchange(CDataExchange* pDX)
{
	CBaseFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CEventView)
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_COMBO_FIND, m_searchTask);
	DDX_Control(pDX, IDC_COMBO_TASK_STATUS, m_taskStatusCtrl);
	DDX_Text(pDX, IDC_EDIT_DIFF_PER, m_diffPerCondition);
	DDX_Control(pDX, IDC_COMBO_FIND_OWNER, m_ctrlOwnern);
}

BEGIN_MESSAGE_MAP(CProjectTaskView, CBaseFormView)
	//{{AFX_MSG_MAP(CEventView)
	//}}AFX_MSG_MAP
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_WM_CREATE()
	ON_CBN_SELCHANGE(IDC_COMBO_FIND, &CProjectTaskView::OnCbnSelchangeComboFind)
	//ON_BN_CLICKED(IDC_BTN_FRESH, &CProjectView::OnBnClickedBtnFresh)
	ON_NOTIFY( NM_CLICK, ID_GRID_PROJECT,  OnGridClick )
	ON_NOTIFY( NM_RCLICK, ID_GRID_PROJECT,  OnRclickGrid )
	ON_NOTIFY( GVN_SELCHANGED, ID_GRID_PROJECT,  OnGridSelChange )
	ON_CBN_SELCHANGE(IDC_COMBO_TASK_STATUS, &CProjectTaskView::OnCbnSelchangeComboTaskStatus)
	ON_BN_CLICKED(IDC_BTN_IMPORT, &CProjectTaskView::OnBnClickedBtnImport)
	ON_BN_CLICKED(IDC_BTN_EXPORT, &CProjectTaskView::OnBnClickedBtnExport)

	ON_COMMAND( IDM_TASK_PROJECT_REVIEW, OnPrjoctProcessReview )
	ON_BN_CLICKED(IDC_BUTTON_SEARCH, &CProjectTaskView::OnBnClickedButtonSearch)
END_MESSAGE_MAP()


// CEventView 诊断

#ifdef _DEBUG
void CProjectTaskView::AssertValid() const
{
	CBaseFormView::AssertValid();
}
#ifndef _WIN32_WCE
void CProjectTaskView::Dump(CDumpContext& dc) const
{
	CBaseFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CEventView 消息处理程序
void CProjectTaskView::OnInitialUpdate() 
{
	BOOL isChange = m_bFirstActive;
	CBaseFormView::OnInitialUpdate();
	if ( isChange )
	{
		InitFindCtrl();
	}
	m_taskGrid.GetFocus();
	UpdateData( FALSE );
}

//初始化产品线组合框
void CProjectTaskView::InitFindCtrl()
{
	//初始化项目索引
	if ( m_gridManger.get() )
	{
		CActionServerFactory* pActionFactory = ((CMainFrame *)AfxGetMainWnd())->GetSeverFactory();
		ASSERT( NULL != pActionFactory );

		const ListProject & listPrj = m_gridManger->GetProjectList();
		m_searchTask.InsertString( 0, _T("所有项目") );
		int cnt = 1;
		for ( ListProject::const_iterator itor = listPrj.begin(); itor != listPrj.end(); itor++ )
		{
			m_searchTask.InsertString(cnt++, itor->GetShortName() );
		}
		if ( listPrj.size() > 0 )
		{
			m_searchTask.SetCurSel( 1 );
		}
		const CTaskStaus obj = pActionFactory->GetTaskSever()->GetTaskStausMap();
		//每个用户参加的产品线信息缓存 
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
		if ( pActionFactory->GetLoginServer()->FindLoginPower( backgroundManger ) )			//如果是后台管理用户可以新建任务
		{
			GetDlgItem( IDC_BTN_IMPORT )->ShowWindow( SW_SHOW );

		}
		else
		{
			GetDlgItem( IDC_BTN_IMPORT )->ShowWindow( SW_HIDE );
		}
		OnInitOwnerList();
	}
}
void CProjectTaskView::OnInitOwnerList()
{	
	if ( m_pActionFactory )
	{
		//获取组合框的选中
		int selPos = m_searchTask.GetCurSel();
		//获取工程列表
		const ListProject & listPrj= m_gridManger->GetProjectList();
		if ( listPrj.size() > 0 )
		{
			if ( selPos > 0  )
			{
				ListProject::const_iterator itor = listPrj.begin(); 
				advance( itor, selPos - 1 );
				if ( itor != listPrj.end() )
				{
					m_lstUser.clear();
					m_pActionFactory->GetProjectSever()->GetProjectMember( itor->GetId(), m_lstUser );
				}
			}else
			{
				m_lstUser.clear();
				for (  ListProject::const_iterator itor = listPrj.begin(); itor != listPrj.end(); itor++ )
				{
					ListUser tempUsr;
					m_pActionFactory->GetProjectSever()->GetProjectMember( itor->GetId(), tempUsr );
					if ( m_lstUser.size() == 0 )
					{
						m_lstUser = tempUsr;
						continue;
					}
					for ( ListUserItor itor = tempUsr.begin(); itor != tempUsr.end(); itor++ )
					{
						ListUserItor itor2 = m_lstUser.begin();
						for ( ; itor2 != m_lstUser.end(); itor2++ )
						{
							if ( itor->GetId() == itor2->GetId() )
							{
								break;
							}
						}
						if ( itor2 == m_lstUser.end() )
						{
							m_lstUser.push_back( *itor );
						}
					}
				}
			}
			m_ctrlOwnern.ResetContent();
			m_ctrlOwnern.InsertString( 0, _T("所有项目成员") );
			int count = 1;
			for ( ListUserItor itorUser = m_lstUser.begin(); itorUser != m_lstUser.end(); itorUser++ )
			{
				m_ctrlOwnern.InsertString( count++ , itorUser->GetUserName_R() );
			}
			m_ctrlOwnern.SetCurSel( 0 );
		}
	}
}
int CProjectTaskView::CreateCtrl()
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
	m_gridManger.reset( new CProjectTaskManger( m_pActionFactory ));
	if ( m_gridManger.get() )
	{
		m_gridManger->RegisterAsGridView( &m_taskGrid );
		m_gridManger->CreateGridView();
		::SendMessage( GetParent()->GetParent()->GetSafeHwnd(), 
			PRJ_VIEW_MSG_CHANGE_TASKLIST, (WPARAM)(m_gridManger->GetTaskListPoint()), NULL );
	}
	return 0;
}
int CProjectTaskView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	return CreateCtrl();
}
void CProjectTaskView::OnSize(UINT nType, int cx, int cy)
{
	CBaseFormView::OnSize(nType, cx, cy, m_taskGrid);
	//if ( m_taskGrid.GetSafeHwnd() )
	//{
	//	m_taskGrid.MoveWindow( 0, 100, cx, cy-100);
	//}
}

void CProjectTaskView::OnCbnSelchangeComboFind()
{
	//int selIndex = m_searchTask.GetCurSel();
	//if ( m_gridManger.get() && selIndex >= 0 )
	//{
	//    int stIndex = m_taskStatusCtrl.GetCurSel();
	//	int prjId = 0;
	//	const ListProject &list = m_gridManger->GetProjectList();
	//	if ( list.size() > 0)
	//	{
	//		ListProject::const_iterator itor = list.begin();
	//		advance( itor, selIndex );
	//		if ( itor != list.end() )
	//		{
	//			m_gridManger->SetSelProject( itor->GetId(), stIndex );
	//			//记录项目
	//			::SendMessage( GetParent()->GetParent()->GetSafeHwnd(), 
	//				                  PRJ_VIEW_MSG_RECODE_PRJID,itor->GetId(), NULL );
	//		}
	//		
	//	}
	//}
	OnInitOwnerList();
}
//就是筛选你现在的产品线是什么
void    CProjectTaskView:: SetSelProjectByPrjId( int prjId, int taskSt /*= -1*/  )
{
	if ( m_gridManger.get()  )
	{
		//获取任务信息容器
		const ListProject lst =  m_gridManger->GetProjectList();
		int index = 0;
		for ( ListProject::const_iterator itor = lst.begin(); itor != lst.end(); itor++ )
		{
			if ( itor->GetId() == prjId )
			{
				m_searchTask.SetCurSel( index + 1); //跳过所有项目
				OnInitOwnerList();
				m_gridManger->SetSelProject( prjId, taskSt, 0, m_diffPerCondition ); //statMpa 的第几个
				return;
			}
			index++;
		}
	}
}
void CProjectTaskView::OnGridClick( NMHDR* pNMHDR,LRESULT* pResult )
{
	if ( m_gridManger.get() )
	{
		m_gridManger->OnGridClick( pNMHDR, pResult );
	}
}

void CProjectTaskView::OnCbnSelchangeComboTaskStatus()
{
/*	if ( m_gridManger.get()  )
	{
		int selIndex = m_taskStatusCtrl.GetCurSel() ;
		m_gridManger->UpdateTask( m_taskStatusCtrl.GetCurSel() );
		//记录项目
		::SendMessage( GetParent()->GetParent()->GetSafeHwnd(), 
		PRJ_VIEW_MSG_RECODE_TASK_INDEX,selIndex, NULL );
	}
*/
}


void CProjectTaskView::OnBnClickedBtnImport()
{
	const ListProject lst =  m_gridManger->GetProjectList();
	int index = m_searchTask.GetCurSel();
	if ( lst.size() > 0 && m_gridManger.get() && index > 0 )
	{
		ListProject::const_iterator itor = lst.begin();
		advance( itor, index - 1);
		if ( itor != lst.end() )
		{
		   m_gridManger->ImportProjectTask( *itor );
		}else
		{
			AfxMessageBox( _T("未选择项目") );
		}
	}else
	{
		AfxMessageBox( _T("未选择项目") );
	}
}

void CProjectTaskView::OnBnClickedBtnExport()
{
	const ListProject lst =  m_gridManger->GetProjectList();
	int index = m_searchTask.GetCurSel();
	if ( lst.size() > 0 && m_gridManger.get() && index > 0 )
	{
		ListProject::const_iterator itor = lst.begin();
		advance( itor, index - 1);
		if ( itor != lst.end() )
		{
			m_gridManger->ExportProjectTask( *itor );
		}else
		{
			AfxMessageBox( _T("未选择项目") );
		}
	}else
	{
		AfxMessageBox( _T("请选择具体的项目") );
	}
}
void CProjectTaskView::OnGridSelChange( NMHDR* pNMHDR,LRESULT* pResult )
{
	if ( m_gridManger.get() )
	{
		m_gridManger->OnGridSelChange( pNMHDR, pResult );
	}
}
void CProjectTaskView::OnPrjoctProcessReview()
{
	if ( m_gridManger.get() )
	{
		m_gridManger->OnGridProjectReview();
	}
}


void CProjectTaskView::OnBnClickedButtonSearch()
{
	int selIndex = m_searchTask.GetCurSel();
	UpdateData();
	if ( m_gridManger.get() && selIndex >= 0 )
	{
	    int stIndex = m_taskStatusCtrl.GetCurSel();
		int prjId = 0;
		const ListProject &list = m_gridManger->GetProjectList();
		if ( list.size() > 0)
		{
			if ( selIndex > 0 )
			{
				ListProject::const_iterator itor = list.begin();
				advance( itor, selIndex - 1);
				selIndex = itor->GetId();
			}
			int selUser = m_ctrlOwnern.GetCurSel();
			if ( selUser > 0 && m_lstUser.size() >= 0 && (UINT)selUser <= m_lstUser.size() )
			{
				ListUserItor itor = m_lstUser.begin();
				advance( itor, selUser - 1 );
				if ( itor != m_lstUser.end() )
				{
					selUser = itor->GetId();
				}
			}
			bool showQuality = (BST_CHECKED == ( (CButton *)GetDlgItem( IDC_ONLY_QUALITY ) )->GetCheck());
			m_gridManger->SetSelProject( selIndex, stIndex, selUser, m_diffPerCondition, showQuality );
			////记录项目
			//::SendMessage( GetParent()->GetParent()->GetSafeHwnd(), 
			//		                PRJ_VIEW_MSG_RECODE_PRJID, itor->GetId(), NULL );
			
			::SendMessage( GetParent()->GetParent()->GetSafeHwnd(), 
				PRJ_VIEW_MSG_CHANGE_TASKLIST, (WPARAM)(m_gridManger->GetTaskListPoint()), NULL );
		}
	}
}
void CProjectTaskView::OnRclickGrid( NMHDR* pNMHDR,LRESULT* pResult )
{
	if ( m_gridManger.get() )
	{
		m_gridManger->OnGridRclick( pNMHDR, pResult );
	}
}
ListTask*  CProjectTaskView::GetTaskListPoint()
{
	if ( m_gridManger.get() )
	{
		return m_gridManger->GetTaskListPoint();
	}
	return NULL;
}