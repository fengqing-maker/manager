#include "stdafx.h"
#include "scheduleManger.h"
#include "Src/MainFrame/messageDef.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

CTaskScheduleManger::CTaskScheduleManger( CActionServerFactory *pSeverFactory ):CBaseGridManger( pSeverFactory )
{
	m_showRowCount = 0;
	
}
//任务表格
#define  TASK_COL_NUM  5 
void CTaskScheduleManger::CreateColumnMap()
{
  InsertTaskNameColumn();
  InsertProjectTypeColum();
  InsertTaskOwernColum();
  InsertTaskPlanColumn();
  InsertTaskFactTimeColumn();

  InsertReviewUpateColum(); //进度更新内容
  InsertProjectReview(); //项目点评
  InsertScoreColumn(); //进度评分
  InsertDepartReview(); //评分说明
  InsertUpDownColum();
}
void  CTaskScheduleManger::CreateRowItemMap()
{
	UpdateDbDataMapAndShowAll();
}
void CTaskScheduleManger::UpdateDbDataMapAndShowAll()
{
	m_pTaskSever->UpdateAllReviewSchedule();
	m_showSchedule = m_pTaskSever->GetReviewSchedule();
	ShowAllSchedul();
}
void CTaskScheduleManger::ShowAllSchedul( const CString &strType/* =_T("")*/, int prjId /*= -1*/ )
{
	m_showTaskMap.clear();
	MapTaskSchedule::const_iterator itor = m_showSchedule.begin();
	int cnt = 0;
	SchedulePos pos;
	int            basePos = 0;
	m_showRowCount = 0;
	while( itor != m_showSchedule.end() )
	{
		pos.dataPos = cnt;
		pos.baseShowRow = basePos;
		pos.scheduleCount = itor->second.size();
		bool insert = false;
		
		if ( strType.IsEmpty() )
		{
		   insert = true;
		}else 
		{
			CTaskObj obj;
			m_pTaskSever->GetTaskByTaskId( itor->first, obj );
			if ( prjId < 0  )
			{
				if ( strType == obj.GetType() )
				{
					 insert = true;
				}
			}else if ( prjId == obj.GetId())
			{
				insert = true;
			}
		}
		if ( insert )
		{
			m_showTaskMap.insert( std::make_pair( cnt++, pos ) );
			basePos += pos.scheduleCount;
			m_showRowCount += pos.scheduleCount;
		}
		itor++;
	
	}
}
void CTaskScheduleManger::UpdateShowAllSchedul()
{
	ShowAllSchedul();
	UpdateGridView();
}
void CTaskScheduleManger::UpdateShowDepartSchedul()
{
	ShowAllSchedul( TASKTYPE_DEPARTSTR );
	UpdateGridView();
}
void CTaskScheduleManger::UpdateShowAllPrjSchedul()
{
	ShowAllSchedul( TASKTYPE_PRJSTR );
	UpdateGridView();
}
void CTaskScheduleManger::UpdateShowPrjSchedul( const CProject &prjObj )
{
	ShowAllSchedul( _T(""),  prjObj.GetId() );
	UpdateGridView();
}
void CTaskScheduleManger::AftCreateGridView()
{
	CBaseGridManger::AftCreateGridView();
	int  baseRow = GetFixedRowCount();
	int  rowCount = baseRow + GetFixedRowCount();
	SchedulePosMap::const_iterator itor = m_showTaskMap.begin();
	int beginCol = GetFixedColumnCount();
	int endCol = beginCol + TASK_COL_NUM;
	while ( itor != m_showTaskMap.end() )
	{
		int row = itor->second.baseShowRow;
		row += baseRow;
		BOOL SetRowHeight(int row, int height);
		for ( int i = beginCol; i < endCol; i++ )
		{
			int yNum = itor->second.scheduleCount;
			//1111111111
			//m_gridCtrl->SetCellCombine( row, i, 0, yNum, TRUE );
		}
		itor++;
	}
}
int CTaskScheduleManger::GetFlexRowCount()
{
	return m_showRowCount;
}
const SchedulePos* CTaskScheduleManger::GetSchedulePos( int row )
{
	SchedulePosMap::const_iterator itor = m_showTaskMap.begin();
	while( itor != m_showTaskMap.end() )
	{
		if ( row < (itor->second.scheduleCount + itor->second.baseShowRow) && row >= itor->second.baseShowRow )
		{
			return &( itor->second );
		}
		itor++;
	}
	return NULL;
}

CString CTaskScheduleManger::GetFlexItemText(int nRow, int nColumn)
{
	CString text;
	if ( m_showSchedule.size() < 0 || m_showSchedule.size() < 0)
	{
		return text;
	}
	GridColumnMapItor colItor = m_gridColumnMap.find(nColumn);
	if (colItor == m_gridColumnMap.end())
		return text;
	const SchedulePos* pPos = GetSchedulePos( nRow );
	if ( NULL == pPos || m_showSchedule.size() <= 0 || pPos->dataPos < 0 )
	{
		return text;
	}
	MapTaskSchedule::const_iterator itorData = m_showSchedule.begin();
	advance( itorData, pPos->dataPos );
	if ( itorData == m_showSchedule.end() )
	{
		return text;
	}
	CTaskObj task;
	m_pTaskSever->GetTaskByTaskId( itorData->first, task );
	if ( pPos->baseShowRow == nRow  )
	{
		if (  nColumn < TASK_COL_NUM   )
		{
			switch( colItor->second->dataType )
			{
			case TaskName:
				text = task.GetName();
				break;
			case ProjectType_Colom:
				text = GetProjectShortName( task  );
				break;
			case  taskOwern:
				text = GetOwnerNameStr(task.GetOwnerId());
				break;
			case  TaskPlanTime:
				text = GetPanTimeString( task );
				break;
			case  TaskFactTime:
				text = GetFactTimeString( task );
				break;
			}
		}
	}
	SetBkColorByTask(nRow, nColumn, task );
	if ( itorData->second.size() <= 0 )
	{
		return text;
	}
	if ( nColumn < TASK_COL_NUM )
	{
		return text;
	}
	int dataPos = nRow - pPos->baseShowRow;
	if ( dataPos < 0  || dataPos >= itorData->second.size() )
	{
		return text;
	}
	ListSchedule::const_iterator itorSch = itorData->second.begin();
	advance( itorSch, dataPos );
	if ( itorSch == itorData->second.end() )
	{
		return text;
	}
	int baseRow = GetFixedRowCount();
	int baseCol = GetFixedColumnCount();

	CGridCellNumeric* pCell = 
		dynamic_cast<CGridCellNumeric*>(m_gridCtrl->GetCell(nRow+baseRow,nColumn+baseCol));
	switch( colItor->second->dataType )
	{
	case ReviewCurrtentUpdate:
		text = GetScheduleString( *itorSch );
		break;
	case  SheduleProjectReview:
		text = itorSch->GetPrjReview();
		break;
	case  ScoreColum:
		if ( pCell )
		{
			text.Format( _T("%d"), itorSch->GetScore() );
		}
		break;
	case  SheduleScoreDec:
		text = itorSch->GetReview();
		break;
	case  TaskOperate1:
		text = _T("更新");
		break;
	}
	
	return text;

}
CSchedule* CTaskScheduleManger::GetScheduleByRow( int nRow )
{
	const SchedulePos* pPos = GetSchedulePos( nRow );
	if ( NULL == pPos || m_showSchedule.size() <= 0 || pPos->dataPos < 0 )
	{
		return NULL;
	}
	MapTaskSchedule::iterator itorData = m_showSchedule.begin();
	advance( itorData, pPos->dataPos );
	if ( itorData == m_showSchedule.end() || itorData->second.size() <= 0)
	{
		return NULL;
	}
	int dataPos = nRow - pPos->baseShowRow;
	if ( dataPos < 0 )
	{
		return NULL;
	}
	ListSchedule::iterator itorSch = itorData->second.begin();
	advance( itorSch, dataPos );
	if ( itorSch == itorData->second.end() )
	{
		return NULL;
	}
	return &(*itorSch);
}
BOOL   CTaskScheduleManger::SetFlexItemText(int nRow,int nColumn,CString text)
{
    if (  nColumn < TASK_COL_NUM || m_showSchedule.size() < 0 || m_showSchedule.size() < 0)
	{
		return TRUE;
	}
	GridColumnMapItor colItor = m_gridColumnMap.find(nColumn);
	if (colItor == m_gridColumnMap.end())
		return TRUE;
    CSchedule *pObj = GetScheduleByRow( nRow );
	if ( NULL== pObj )
	{
		return TRUE;
	}

	switch( colItor->second->dataType )
	{
	case  SheduleProjectReview:
		if ( m_pSeverFactory->GetLoginServer()->FindLoginPower( projectReview ))
		{
		   pObj->SetPrjReview( text )  ;
		}
		break;
	case  ScoreColum:
		{
			if ( m_pSeverFactory->GetLoginServer()->FindLoginPower( taskAgree ) )
			{
				int baseRow = GetFixedRowCount();
				int baseCol = GetFixedColumnCount();

				CGridCellNumeric* pCell = 
					dynamic_cast<CGridCellNumeric*>(m_gridCtrl->GetCell(nRow+baseRow,nColumn+baseCol));
				if ( pCell )
				{
					int value = (int)pCell->GetNumber();
				    pObj->SetScore( value ) ;
				}
			}
		}
		break;
	case  SheduleScoreDec:
		if ( m_pSeverFactory->GetLoginServer()->FindLoginPower( taskAgree )  )
		{
			pObj->SetReview( text ) ;
		}
		break;

	}
	return TRUE;
}
BOOL    CTaskScheduleManger::GetFlexItemEditable(int nRow,int nColumn)
{
  if ( nColumn < TASK_COL_NUM )
  {
	  return FALSE;
  }
  GridColumnMapItor colItor = m_gridColumnMap.find(nColumn);
  if (colItor == m_gridColumnMap.end())
	  return FALSE;
  switch( colItor->second->dataType )
  {
  case  ReviewCurrtentUpdate:
	  return FALSE;
  case  SheduleProjectReview:
	  return m_pSeverFactory->GetLoginServer()->FindLoginPower( projectReview );
  case  SheduleScoreDec:
  case  ScoreColum:
	  return m_pSeverFactory->GetLoginServer()->FindLoginPower( taskAgree );
  }
  return FALSE;
}
BOOL    CTaskScheduleManger::InitFlexNumEditItem(CGridCellNumeric* pCell,int nRow,int nColumn)
{
	GridColumnMapItor itor = m_gridColumnMap.find( nColumn );
	if ( itor != m_gridColumnMap.end())
	{
		if ( ScoreColum == itor->second->dataType )
		{//分数
			pCell->SetFlags( CGridCellNumeric::Integer | CGridCellNumeric::Negative );
		}
	}
	return TRUE;
}
void  CTaskScheduleManger::OnClickGrid( NMHDR* pNMHDR,LRESULT* pResult )
{
	NM_GRIDVIEW *gridView = reinterpret_cast<NM_GRIDVIEW*>(pNMHDR);
	int nRow = gridView->iRow - GetFixedRowCount();
	int nColumn = gridView->iColumn - GetFixedColumnCount();
	GridColumnMapItor itor = m_gridColumnMap.find( nColumn );
	if ( itor == m_gridColumnMap.end() || TaskOperate1 != itor->second->dataType )
	{
		return;
	}
	CSchedule *pObj = GetScheduleByRow( nRow );
	if ( NULL== pObj )
	{
		return ;
	}
	BOOL ret = paraError;
	if ( m_pSeverFactory->GetLoginServer()->FindLoginPower( projectReview ) )
	{
		 ret = m_pSeverFactory->GetScheduleSever()->OnPrjReviewSchedule( *pObj );
		if ( ret != userExcOk )
		{
			if ( ret != paraError )
			{
				AfxMessageBox( m_pSeverFactory->GetErrorString(ret) );
				return;
			}
		}

	}
	if ( m_pSeverFactory->GetLoginServer()->FindLoginPower( taskAgree ) )
	{
		ret = m_pSeverFactory->GetScheduleSever()->OnReviewSchedule( *pObj );
		if ( ret != userExcOk )
		{
			AfxMessageBox( m_pSeverFactory->GetErrorString(ret) );
			return;
		}
	}
	if ( ret == userExcOk )
	{//更新进度
	    m_pSeverFactory->GetScheduleSever()->GetScheduleContentById( pObj->GetId(), *pObj );
		AfxMessageBox( _T("提交成功"));
	}
}
void CTaskScheduleManger::OnGridSelChange( NMHDR* pNMHDR,LRESULT* pResult )
{
	NM_GRIDVIEW *gridView = reinterpret_cast<NM_GRIDVIEW*>(pNMHDR);
	int nRow = gridView->iRow - GetFixedRowCount();
	CSchedule *pObj = GetScheduleByRow( nRow );
	if ( pObj && m_outputShowId != pObj->GetTaskId() )
	{
		m_outputShowId = pObj->GetTaskId();
		SendMessage( AfxGetMainWnd()->GetSafeHwnd(), MAINFRAME_MSG_TASKID, m_outputShowId, NULL );
	}
}
int CTaskScheduleManger::FindScheulById( int scheulId, ListSchedule::iterator &retItor  )
{
	MapTaskSchedule::iterator itor = m_showSchedule.begin();
	int row = 0;
	while( itor != m_showSchedule.end() )
	{
		retItor = itor->second.begin();
		while ( retItor != itor->second.end() )
		{
			if ( retItor->GetId() == scheulId )
			{
				break;
			}
			retItor++;
		}
		if ( retItor != itor->second.end() )
		{
			break;
		}
		row += itor->second.size();
		itor++;
	}
	if ( itor != m_showSchedule.end() )
	{
		for ( SchedulePosMap::iterator lineItor = m_showTaskMap.begin(); lineItor != m_showTaskMap.end();
			lineItor++ )
		{
			if ( lineItor->second.baseShowRow == row )
			{
				return lineItor->second.baseShowRow+lineItor->second.dataPos;
			}
		}
	}
	return -1;
}

void  CTaskScheduleManger::UpdateScheulContent( int schId )
{
	if ( schId > 0 )
	{
		ListSchedule::iterator itor;
		int row = FindScheulById(  schId, itor  );
		if ( row >= 0 )
		{
			m_pSeverFactory->GetScheduleSever()->GetScheduleContentById( itor->GetId(), *itor);
			UpdateGridRowView( row );
			SelectRow( row );
		}
	}
}
void  CTaskScheduleManger::SelectScheuleLine( int schId )
{
	if ( schId > 0 )
	{
		 ListSchedule::iterator itor;
		int row = FindScheulById(  schId, itor  );
		if ( row >= 0 )
		{
			SelectRow( row );
		}
	}
}