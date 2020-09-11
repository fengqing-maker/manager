#include "stdafx.h"
#include "changeReviewManger.h"
#include "../../Dlg/ChangeReviewDlg.h"
#include "Src/MainFrame/messageDef.h"
#include "Src/MainFrame/MainFrm.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

CChangeReviewManger::CChangeReviewManger( CActionServerFactory *pSeverFactory )
	:CBaseGridManger( pSeverFactory )
{
}
void CChangeReviewManger::InsertFirstReview()
{
	InsertEditColumn(  _T("一级评审"), 70, DT_LEFT,ChangeReviewUser1 );
}
void CChangeReviewManger::InsertSecondReview()
{
	InsertEditColumn(  _T("二级评审"), 70, DT_LEFT,ChangeReviewUser2 );
}
void    CChangeReviewManger::CreateColumnMap()
{//创建列头
	InsertTaskNameColumn();
	InsertProjectTypeColum();
	InsertTaskOwernColum();
	InsertTaskStatusColumn();
	//变更
	InsertFirstReview();
	InsertSecondReview();
	InsertChangeStatus();
	InsertChangeTime(); //变更时间
	InsertChangeReson(); //变更原因
	InsertChangeContent(); //变更内容
	InsertUpDownColum();
}
void  CChangeReviewManger::SetShowTask( const CString &taskType , BOOL isReview  )
{
	m_showTaskMap.clear();
	ListChange::const_iterator itor = m_changeList.begin();
	int cnt = 0;
	int index = 0;
	const CUser &user = m_pSeverFactory->GetLoginServer()->GetLoginUser();
	int userId = user.GetId();
	BOOL hasReviewFirst =  m_pSeverFactory->GetLoginServer()->FindLoginPower( changeLevelAgree1 );
	BOOL hasReviewSecond = m_pSeverFactory->GetLoginServer()->FindLoginPower( changeLevelAgree2 );
	while( itor != m_changeList.end() )
	{
		BOOL isInsert = false;
		int st = itor->GetStatus();
		CTaskObj obj;
		BOOL ret = m_pTaskSever->GetTaskByTaskId( itor->GetTaskId(), obj );
		CProject prj;
		m_pSeverFactory->GetProjectSever()->GetProjectById( obj.GetProjctId(), prj );
		if ( taskType.IsEmpty() )
		{
			if ( TRUE == isReview )
			{
				if ( (prj_remark_on == itor->GetStatus() && userId == prj.GetMangerId()) || 
					  (hasReviewFirst && up_apper == itor->GetStatus() && userId == itor->GetAuditorId() )
					  || ( hasReviewSecond && (departAgree == itor->GetStatus() && false == itor->IsFinishChange()) && userId == itor->GetGeneralAuditorId()) )
				{
					isInsert = true;
				}
			}else if ( FALSE == isReview && obj.GetOwnerId() == userId )
			{
				isInsert = true;
			}
			
		}else
		{
			if ( isReview && 
				 ( userId == itor->GetAuditorId() || userId == itor->GetGeneralAuditorId() || userId == prj.GetMangerId() )  )
			{
				if ( userExcOk == ret && taskType == obj.GetType() )
					isInsert = true;
			}else if ( FALSE == isReview && obj.GetOwnerId() == userId )
			{
				isInsert = true;
			}
			
		}
		if ( -1 == isReview )
		{
			isInsert = TRUE;
		}
		if ( isInsert )
		{
			m_showTaskMap.insert( std::make_pair( index++, cnt ) );
		}
		itor++;
		cnt++;
	}
}
void    CChangeReviewManger::CreateRowItemMap()
{//
	((CMainFrame *)AfxGetMainWnd())->ShowProgressMsg( MSG_READ_TASK_DATA );
    UpdateDbData();
	SetShowTask(_T(""), TRUE );
	((CMainFrame *)AfxGetMainWnd())->CloseMsgDlg( );
}
void      CChangeReviewManger::UpdateDbData()
{
	m_pTaskSever->UpdateAllChange();
	m_changeList = m_pTaskSever->GetReviewChange();
	if ( _T("管理员") == m_pSeverFactory->GetLoginServer()->GetLoginRole().GetRoleName() )
	{//修改查看所有变更
		m_changeList.clear();
		m_pSeverFactory->GetChangeSever()->GetAllChange( m_changeList );
	}
}
void  CChangeReviewManger::UpdateShowAllChange()
{
	//等待加载
	((CMainFrame *)AfxGetMainWnd())->ShowProgressMsg( MSG_READ_TASK_DATA );
	SetShowTask( _T(""), -1 );;
	UpdateGridView();
	((CMainFrame *)AfxGetMainWnd())->CloseMsgDlg( );
	//
}
void  CChangeReviewManger::UpdateShowAllReviewChange()
{
	SetShowTask( _T(""), TRUE );;
	UpdateGridView();
}
void  CChangeReviewManger::UpdateShowDepartChange()
{
	//等待加载
	((CMainFrame *)AfxGetMainWnd())->ShowProgressMsg( MSG_READ_TASK_DATA );
	SetShowTask( TASKTYPE_DEPARTSTR, TRUE );
	UpdateGridView();
	((CMainFrame *)AfxGetMainWnd())->CloseMsgDlg( );
}
void  CChangeReviewManger::UpdateShowProjectChange()
{
	//等待加载
	((CMainFrame *)AfxGetMainWnd())->ShowProgressMsg( MSG_READ_TASK_DATA );
	SetShowTask( TASKTYPE_PRJSTR, TRUE );
	UpdateGridView();
	((CMainFrame *)AfxGetMainWnd())->CloseMsgDlg( );
}
void  CChangeReviewManger::UpdateOwnerChange()
{
	//等待加载
	((CMainFrame *)AfxGetMainWnd())->ShowProgressMsg( MSG_READ_TASK_DATA );
	SetShowTask( _T(""), FALSE );
	UpdateGridView();
	((CMainFrame *)AfxGetMainWnd())->CloseMsgDlg( );
}

int CChangeReviewManger::GetTaskChangeByRow( int nRow, CTaskObj &obj, CTaskChange &change  )
{
	if ( m_changeList.size() <= 0)
	{
		return FALSE;
	}
	IntToIntMap::const_iterator dataItor = m_showTaskMap.find( nRow );
	if ( dataItor != m_showTaskMap.end() && dataItor->second  >= 0 )
	{
		ListChange::iterator itor = m_changeList.begin();
		advance( itor, dataItor->second );
		if ( itor == m_changeList.end() )
		{
			return -1;
		}
		change = *itor;
		if (userExcOk == m_pTaskSever->GetTaskByTaskId( itor->GetTaskId(), obj ))
		{
			return dataItor->second;
		}
	}
	return -1;
}
CString CChangeReviewManger::GetFlexItemText(int nRow, int nColumn)
{
	CString text;
	//
	GridColumnMapItor colItor = m_gridColumnMap.find(nColumn);
	if (colItor == m_gridColumnMap.end())
		return text;
	//
	CTaskObj obj;
	CTaskChange changeItem;
	if ( GetTaskChangeByRow(nRow, obj, changeItem) < 0)
	{
		return text;
	}

	switch( colItor->second->dataType )
	{
	case TaskName_col:
		text = obj.GetName();
		break;
	case  taskOwern:
		text = GetOwnerNameStr(obj.GetOwnerId());
		break;
	case  ChangeReviewUser1:
		text = GetOwnerNameStr(changeItem.GetAuditorId());
		break;
	case  ChangeReviewUser2:
		text = GetOwnerNameStr(changeItem.GetGeneralAuditorId());
		break;
	case ProjectType_Colom:
		text = GetProjectShortName( obj  );
		break;
	case TaskStatus_col:
		GetTaskStatusSting( obj, text );
		break;
	case  ChangeStatus:
		text = changeItem.GetChangeStatusString( changeItem.GetStatus() );
		break;
	case ChangeAppTime:
		text = changeItem.GetChangeDateTimeStr();
		break;
	case ChangeReson:
		text = changeItem.GetChangeReason();
		break;
	case ChangeContent:
		text = changeItem.GetChangeContent();
		break;
	case  TaskOperate1:
		{
			const CUser &user = m_pSeverFactory->GetLoginServer()->GetLoginUser();
			int userId = user.GetId();
			bool isFirstReview = (userId == changeItem.GetAuditorId() &&  up_apper == changeItem.GetStatus()/*changeWait1 == obj.GetTaskStatus()*/ ); //2015-11-24 moidy
			bool isSecondReview =  (userId == changeItem.GetGeneralAuditorId() && departAgree == changeItem.GetStatus()/*changeWait2 == obj.GetTaskStatus()*/);
			if ( prj_remark_on == changeItem.GetStatus() )
			{
				text = _T("点评");
			}else if ( up_apper == changeItem.GetStatus() || departAgree == changeItem.GetStatus()/*changeWait1 == obj.GetTaskStatus() || changeWait2 == obj.GetTaskStatus()*/ )
			{
				if ( isFirstReview || isSecondReview )
				{
					text = _T("审批");
				}else
				{
					text = _T("查看");
				}
			}else
			{
				text = _T("查看");
			}

		}
	
		break;
	}
	return text;
}
BOOL     CChangeReviewManger::GetFlexItemEditable(int nRow,int nColumn)
{
	BOOL ret = FALSE;
	return ret;
}
void CChangeReviewManger::OnClickGrid( NMHDR* pNMHDR,LRESULT* pResult )
{
	NM_GRIDVIEW *gridView = reinterpret_cast<NM_GRIDVIEW*>(pNMHDR);
	int nRow = gridView->iRow - GetFixedRowCount();
	int nColumn = gridView->iColumn - GetFixedColumnCount();
	//获取列信息
	GridColumnMapItor itor = m_gridColumnMap.find( nColumn );
	if ( itor != m_gridColumnMap.end() && TaskOperate1 == itor->second->dataType  )
	{
		CTaskChange chObj;
		CTaskObj    taskObj;
		int         dataIndex;
		if ( (dataIndex = GetTaskChangeByRow(nRow, taskObj, chObj )) >= 0 && m_changeList.size() > 0 )
		{
			const CUser &user = m_pSeverFactory->GetLoginServer()->GetLoginUser();
			int userId = user.GetId();
			BOOL isHasPow= m_pSeverFactory->GetLoginServer()->FindLoginPower( changeLevelAgree1 );
			ListChange::iterator itor = m_changeList.begin();
			advance( itor, dataIndex);
			if ( itor == m_changeList.end() )
			{
				return;
			}
			if ( userId == chObj.GetAuditorId() && isHasPow && up_apper == chObj.GetStatus() /*changeWait1 == taskObj.GetTaskStatus()*/ )
			{
				CChangeReviewDlg dlg( taskObj,chObj );
				if ( IDOK == dlg.DoModal() )
				{
					m_pSeverFactory->GetChangeSever()->GetChangeById( itor->GetId(), *itor );
					UpdateGridRowView( nRow );
				}
				return;
			}
			isHasPow = m_pSeverFactory->GetLoginServer()->FindLoginPower( changeLevelAgree2 );
			if ( userId == chObj.GetGeneralAuditorId() && isHasPow && departAgree == chObj.GetStatus()/*changeWait2 == taskObj.GetTaskStatus()*/ )
			{
				CChangeReviewDlg dlg( taskObj,chObj );
				if ( IDOK == dlg.DoModal() )
				{
					m_pSeverFactory->GetChangeSever()->GetChangeById( itor->GetId(), *itor );
					UpdateGridRowView( nRow );
				}
				return;
			}
			CProject prj;
			m_pSeverFactory->GetProjectSever()->GetProjectById( taskObj.GetProjctId(), prj );
			if ( userId == prj.GetMangerId()  && prj_remark_on == itor->GetStatus()/*changeWait0 == taskObj.GetTaskStatus()*/ )
			{
				CChangeReviewDlg dlg( taskObj,chObj );
				if ( IDOK == dlg.DoModal() )
				{
					m_pSeverFactory->GetChangeSever()->GetChangeById( itor->GetId(), *itor );
					UpdateGridRowView( nRow );
				}
				return;
			}
			//查看任务
			CChangeReviewDlg dlg( taskObj,chObj, TRUE );
			dlg.DoModal();
			return;
		}
	}
}
int CChangeReviewManger::FindChangeById( int changeId, ListChange::iterator &itor  )
{
	itor = m_changeList.begin();
	int row = 0;
	while( itor != m_changeList.end() )
	{
		if ( itor->GetId() == changeId )
		{
			break;
		}
		row++;
		itor++;
	}
	if ( itor != m_changeList.end() )
	{
		for ( IntToIntMap::iterator lineItor = m_showTaskMap.begin(); lineItor != m_showTaskMap.end();
			lineItor++ )
		{
			if ( lineItor->second == row )
			{
				return lineItor->first;
			}
		}
	}
	return -1;
}
void CChangeReviewManger::OnGridSelChange( NMHDR* pNMHDR,LRESULT* pResult )
{
	NM_GRIDVIEW *gridView = reinterpret_cast<NM_GRIDVIEW*>(pNMHDR);
	int nRow = gridView->iRow - GetFixedRowCount();
	CTaskChange chObj;
	CTaskObj    taskObj;
	int         dataIndex ;
	dataIndex = GetTaskChangeByRow(nRow, taskObj, chObj );
	if (  taskObj.GetId() >= 0 && m_outputShowId != taskObj.GetId() )
	{
		m_outputShowId = taskObj.GetId();
		SendMessage( AfxGetMainWnd()->GetSafeHwnd(), MAINFRAME_MSG_TASKID, m_outputShowId, NULL );
	}
}
void  CChangeReviewManger::UpdateChangeContent( int changId )
{
	if ( changId > 0 )
	{
		ListChange::iterator itor;
		int row = FindChangeById(  changId, itor  );
		if ( row >= 0 )
		{
			m_pSeverFactory->GetChangeSever()->GetChangeById( changId, *itor );
			UpdateGridRowView( row );
			SelectRow( row );
		}
	}
}
void  CChangeReviewManger::SelectChangeLine( int changeId )
{
	if ( changeId > 0 )
	{
		ListChange::iterator itor;
		int row = FindChangeById(  changeId, itor  );
		if ( row >= 0 )
		{
			SelectRow( row );
		}
	}
}
