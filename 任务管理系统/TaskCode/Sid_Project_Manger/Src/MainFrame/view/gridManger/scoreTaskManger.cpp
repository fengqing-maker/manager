#include "stdafx.h"
#include "scoreTaskManger.h"
#include "Src/MainFrame/messageDef.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

CScoreTaskManger::CScoreTaskManger( CActionServerFactory *pSeverFactory )
	                 :CBaseGridManger( pSeverFactory )
{
	
}
void CScoreTaskManger::CreateColumnMap()
{
	InsertTaskNameColumn();
	InsertProjectTypeColum();
	InsertTaskOwernColum();
	InsertTaskHistoryUpdateColumn();
	InsertTaskPlanColumn();
	InsertTaskFactTimeColumn();
	InsertUnUpdateCnt(); //
	InsertForcastScore();
	InsertScoreColumn();
	InsertSuggestionColumn();
	InsertAgreeBtnColum();
	InsertUnAgreeBtnColum();
}
void CScoreTaskManger::SetShowTask( const CString &taskType /*=_T("")*/  )
{
	m_showTaskMap.clear();
	m_showView.clear();
	ListTaskItor itor = m_taskList.begin();
	int cnt = 0;
	int index = 0;
	while( itor != m_taskList.end() )
	{
		BOOL isInsert = false;
		if ( taskType.IsEmpty() )
		{
			isInsert = true;
		}else if ( taskType == itor->GetType() )
		{
			isInsert = true;
		}
		if ( isInsert )
		{
			CString obj;
			m_showView.push_back( obj );
			m_showTaskMap.insert( std::make_pair( index++, cnt ) );
		}
		itor++;
		cnt++;
	}
}

void CScoreTaskManger::UpdateShowAllTask()
{
	SetShowTask();
	UpdateGridView();
}
void CScoreTaskManger::UpdateShowDepartTask()
{
	SetShowTask(TASKTYPE_DEPARTSTR);
	UpdateGridView();
}
void CScoreTaskManger::UpdateShowProjectTask()
{
	SetShowTask(TASKTYPE_PRJSTR);
	UpdateGridView();
}
void CScoreTaskManger::RefreshTaskList()
{
	m_pTaskSever->UpdateScoreTask();
	m_taskList = m_pTaskSever->GetScoreTask();
}
void CScoreTaskManger::CreateRowItemMap()
{
	m_pTaskSever->UpdateScoreTask();
	m_taskList = m_pTaskSever->GetScoreTask();
	SetShowTask();
}
void CScoreTaskManger::UpdateDb()
{
	m_pTaskSever->UpdateScoreTask();
	m_taskList = m_pTaskSever->GetScoreTask();
}
CTaskObj* CScoreTaskManger::GetTaskByRow( int nRow )
{
	return CBaseGridManger::GetTaskByRow(nRow, m_taskList, m_showTaskMap );
}
CString* CScoreTaskManger::GetEditSuggestion( int nRow )
{
	return GetCStringByPos(  nRow, m_showView, m_showTaskMap );
}

CString CScoreTaskManger::GetFlexItemText(int nRow, int nColumn)
{
	CString text;
	GridColumnMapItor colItor = m_gridColumnMap.find(nColumn);
	if (colItor == m_gridColumnMap.end())
		return text;
	CTaskObj *pTask = GetTaskByRow( nRow );
	if ( NULL == pTask )
	{
		return text;
	}
	const CTaskObj &obj = *pTask;
	switch( colItor->second->dataType )
	{
	case TaskName_col:
		text = obj.GetName();
		break;
	case  taskOwern:
		text = GetOwnerNameStr(obj.GetOwnerId());
		break;
	case ProjectType_Colom:
		text = GetProjectShortName( obj  );
		break;
	case  TaskHistoryContent:
		GetHistoryString( obj.GetId(), text );
		break;
	case  TaskPlanTime:
		text = GetPanTimeString( obj );
		break;
	case  TaskFactTime:
		text = GetFactTimeString( obj );
		break;
	case  ForcostScoreColum:
		text.Format( _T("%d"), obj.GetForecastScore() );
		break;
	case ScoreColum:
		text.Format( _T("%d"), obj.GetScore() );
		break;
	case  SuggestionColumn:
		{
		  const	CString *pStr = GetEditSuggestion( nRow );
		  text = pStr ? *pStr:_T("");
		}
		break;
	case  leaveUpdateCnt:
		{// 漏更新次数
			int num = 0;
			m_pSeverFactory->GetWorkHourSever()->GetUnUpdateCnt( obj.GetId(), num );
			text.Format( _T("%d"), num );
			break;
		}
	case  TaskOperate1:
		if ( waitScore == obj.GetTaskStatus() )
		{
			text = _T("同意");
		}	
		break;
	case  TaskOperate2:
		if ( waitScore == obj.GetTaskStatus() )
		{
			text = _T("不同意");
		}	
		break;

	}
	SetBkColorByTask( nRow,  nColumn, obj, 0 );
	return text;
}
BOOL     CScoreTaskManger::SetFlexItemText(int nRow,int nColumn,CString text)
{
	GridColumnMapItor colItor = m_gridColumnMap.find(nColumn);
	if (colItor == m_gridColumnMap.end())
		return TRUE;
	CTaskObj *pTask = GetTaskByRow( nRow );
	if ( NULL == pTask )
	{
		return TRUE;
	}
	CTaskObj &obj = *pTask;
	switch( colItor->second->dataType )
	{
	case ScoreColum:
		{
			int baseRow = GetFixedRowCount();
			int baseCol = GetFixedColumnCount();

			CGridCellNumeric* pCell = 
				dynamic_cast<CGridCellNumeric*>(m_gridCtrl->GetCell(nRow+baseRow,nColumn+baseCol));
			if( pCell )
			{
				obj.SetScore( (int)pCell->GetNumber() );
			}
		}
		break;
	case ForcostScoreColum:
		{
			int baseRow = GetFixedRowCount();
			int baseCol = GetFixedColumnCount();

			CGridCellNumeric* pCell = 
				dynamic_cast<CGridCellNumeric*>(m_gridCtrl->GetCell(nRow+baseRow,nColumn+baseCol));
			if( pCell )
			{
				obj.SetForecastScore( (int)pCell->GetNumber() );
			}
		}
		break;
	case  SuggestionColumn:
		{
			CString *pStr = GetEditSuggestion( nRow );
			if ( pStr )
			{
			   *pStr = text;
			}
		}
		break;
	}
	return TRUE;
}
BOOL     CScoreTaskManger::GetFlexItemEditable(int nRow,int nColumn)
{
	GridColumnMapItor colItor = m_gridColumnMap.find(nColumn);
	CTaskObj* pTask = GetTaskByRow( nRow );
	if (colItor != m_gridColumnMap.end() && NULL != pTask )
	{
		if ( waitScore == pTask->GetTaskStatus() )
		{
			switch( colItor->second->dataType )
			{
			/*case  ScoreColum:*/
			case  SuggestionColumn:
			case ForcostScoreColum:
				return TRUE;
			}
		}
	}
	return FALSE;
}
BOOL    CScoreTaskManger::InitFlexNumEditItem(CGridCellNumeric* pCell,int nRow,int nColumn)
{
	GridColumnMapItor itor = m_gridColumnMap.find( nColumn );
	if ( itor != m_gridColumnMap.end())
	{
		if ( ScoreColum == itor->second->dataType || ForcostScoreColum == itor->second->dataType )
		{//分数
			pCell->SetFlags( CGridCellNumeric::Integer | CGridCellNumeric::Negative );
		}
	}
	return TRUE;
}
void CScoreTaskManger::OnClickGrid( NMHDR* pNMHDR,LRESULT* pResult )
{
	NM_GRIDVIEW *gridView = reinterpret_cast<NM_GRIDVIEW*>(pNMHDR);
	int nRow = gridView->iRow - GetFixedRowCount();
	int nColumn = gridView->iColumn - GetFixedColumnCount();
	//获取列信息
	GridColumnMapItor itor = m_gridColumnMap.find( nColumn );
	CTaskObj* pObj = GetTaskByRow( nRow );
	if ( itor != m_gridColumnMap.end() && pObj != NULL )
	{
		if ( waitScore != pObj->GetTaskStatus() )
		{
			return;
		}
		int agree = -1;
		if ( TaskOperate1 == itor->second->dataType )
		{
			agree = true;
		}else if ( TaskOperate2 == itor->second->dataType )
		{
			agree = false;
		}
		if ( agree >= 0 )
		{
			bool isAgree = agree;
			CString *pReview = GetEditSuggestion( nRow );
			CString revStr;
			if ( pReview )
			{
				revStr = *pReview;
			}
			CString name = m_pSeverFactory->GetLoginServer()->GetLoginUser().GetUserName_R();
			BOOL ret = m_pTaskSever->TaskScoreAction( isAgree, revStr,name, *pObj );
			if ( ret != userExcOk && ret != paraError )
			{
				AfxMessageBox( m_pSeverFactory->GetErrorString( ret) );
				return;
			}
			if ( userExcOk == ret )
			{
				m_pTaskSever->GetTaskByTaskId( pObj->GetId(), *pObj );
				UpdateGridRowView( nRow );
			}

		}
	}
}
void CScoreTaskManger::OnGridSelChange( NMHDR* pNMHDR,LRESULT* pResult )
{
	NM_GRIDVIEW *gridView = reinterpret_cast<NM_GRIDVIEW*>(pNMHDR);
	int nRow = gridView->iRow - GetFixedRowCount();
	CTaskObj* pObj = GetTaskByRow( nRow );
	if ( pObj && m_outputShowId != pObj->GetId() )
	{
		m_outputShowId = pObj->GetId();
		SendMessage( AfxGetMainWnd()->GetSafeHwnd(), MAINFRAME_MSG_TASKID, m_outputShowId, NULL );
	}
}
void  CScoreTaskManger::UpdateTaskContent( int taskId )
{
	if ( taskId > 0 )
	{
		ListTaskItor itor;
		int row = FindTaskById(  taskId, m_taskList, itor, m_showTaskMap  );
		if ( row >= 0 )
		{
			m_pTaskSever->GetTaskByTaskId( taskId, *itor );
			UpdateGridRowView( row );
			SelectRow( row );
		}
	}
}
void  CScoreTaskManger::SelectTaskLine( int taskId )
{
	if ( taskId > 0 )
	{
		ListTaskItor itor;
		int row =  FindTaskById(  taskId, m_taskList, itor, m_showTaskMap  );
		if ( row >= 0 )
		{
			SelectRow( row );
		}
	}
}
BOOL CScoreTaskManger::EndEditGridView(int nRow,int nColumn)
{
	int fixedRowCount = GetFixedRowCount();
	int fixedColumnCount = GetFixedColumnCount();
	int flexRowIndex = nRow - fixedRowCount;
	int flexColumnIndex = nColumn - fixedColumnCount;
	BOOL result = TRUE;
	ASSERT(flexRowIndex >= 0 && flexColumnIndex >= 0);
	if (flexRowIndex >= 0 && flexColumnIndex >= 0)
	{
		CString text = m_gridCtrl->GetItemText(nRow,nColumn);
		result = SetFlexItemText(flexRowIndex,flexColumnIndex,text);
	}
	return result;
}