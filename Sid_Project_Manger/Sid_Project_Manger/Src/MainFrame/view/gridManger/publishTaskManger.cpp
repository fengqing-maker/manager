#include "stdafx.h"
#include "publishTaskManger.h"
#include "Src/MainFrame/messageDef.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

CPublishTaskManger::CPublishTaskManger( CActionServerFactory *pSeverFactory ):CBaseGridManger( pSeverFactory )
{
}
//任务表格
void CPublishTaskManger::CreateColumnMap()
{
	InsertTaskNameColumn();
	InsertTaskStatusColumn();
	InsertTaskOwernColum();
	InsertDiffParaColum();
	InsertTaskPlanColumn();
	InsertProjectTypeColum();
	InsertTaskPrjStep();
	InsertWorkTimeTypeColum();
	InsertSuggestionColumn();
	InsertAgreeBtnColum();
	InsertUnAgreeBtnColum();
}
void CPublishTaskManger::UpdateDbData()
{
	CreateRowItemMap();
	UpdateGridView();
}
//列表内容
void CPublishTaskManger::CreateRowItemMap()
{
	m_pTaskSever->UpdateReviewTask();
	m_taskList = m_pTaskSever->GetReviewTask();
	m_showTaskMap.clear();
	m_showView.clear();
	ListTaskItor itor = m_taskList.begin();
	int cnt = 0;
	while( itor != m_taskList.end() )
	{
		CString obj;
		m_showView.push_back( obj );
		m_showTaskMap.insert( std::make_pair( cnt, cnt ) );
		itor++;
		cnt++;
	}
}

int CPublishTaskManger::GetFlexRowCount()
{
	return m_showTaskMap.size();
}
//获取显示
CString CPublishTaskManger::GetFlexItemText(int nRow, int nColumn)
{
	CString text;
	GridColumnMapItor colItor = m_gridColumnMap.find(nColumn);
	if (colItor == m_gridColumnMap.end())
		return text;
	CTaskObj *pObj = GetTaskByRow( nRow );
	if ( NULL == pObj )
	{
		return text;
	}
	const CTaskObj &obj = *pObj;
	switch( colItor->second->dataType )
	{
	case TaskName_col:
		text = obj.GetName();
		break;
	case  TaskStatus_col:
		GetTaskStatusSting( obj, text );
	    break;
	case  taskOwern:
		text = GetOwnerNameStr(obj.GetOwnerId());
		break;
	case  DiffPara_Colom:
		text.Format( _T("%0.1f"), obj.GetDifficultyParam() );
		break;
	case  TaskPlanTime:
		text = GetPanTimeString( obj );
		break;
	case ProjectType_Colom:
		text = GetProjectShortName( obj );
		break;
	case  TaskTimeType:
		text = GetWorkHourStringById( obj.GetWorkTypeId() );
		break;
	case  SuggestionColumn:
		{
			CString* pView = GetEditSuggestion( nRow );
			if ( pView )
			{
				text = *pView;
			}
		}
		break;
	case TaskOperate1:
		if ( waitPublish == obj.GetTaskStatus() || finishReviewWait ==  obj.GetTaskStatus() )
		{
			text = _T("同意");
		}
		break;
	case TaskOperate2:
		if ( waitPublish == obj.GetTaskStatus() || finishReviewWait ==  obj.GetTaskStatus() )
		{
			text = _T("不同意");
		}
		break;
	case  taskBelongPrjStep: //任务所属的项目阶段
		GetTaskBelongToPrjStep( obj.GetAttribute(), text );
		break;
	}
	SetBkColorByTask(nRow, nColumn, obj, 0 );
	return text;
}
void      CPublishTaskManger::UpdateAllTask()
{
	RefreshShowMap( true, _T("") );
}
void      CPublishTaskManger::UpdateDepartTask()
{
	RefreshShowMap( false, TASKTYPE_DEPARTSTR );
}
void      CPublishTaskManger::UpdateProjectTask()
{
	RefreshShowMap( false, TASKTYPE_PRJSTR );
}

void CPublishTaskManger::RefreshShowMap( bool isAll, const CString &str )
{
	m_showTaskMap.clear();
	ListTaskItor itor = m_taskList.begin();
	int cnt = 0;
	int index = 0;
	while( itor != m_taskList.end() )
	{
		if ( (waitPublish == itor->GetTaskStatus() || finishReviewWait ==  itor->GetTaskStatus() ) &&( isAll || str == itor->GetType())  )
		{
			m_showTaskMap.insert( std::make_pair( index++, cnt ) );
		}
		itor++;
		cnt++;
	}
	UpdateGridView();
}

CTaskObj* CPublishTaskManger::GetTaskByRow( int nRow )
{
	return CBaseGridManger::GetTaskByRow(nRow, m_taskList, m_showTaskMap );
}
CString* CPublishTaskManger::GetEditSuggestion( int nRow )
{
	return GetCStringByPos(  nRow, m_showView, m_showTaskMap );
}
BOOL    CPublishTaskManger::SetFlexItemText(int nRow,int nColumn,CString text)
{
	GridColumnMapItor colItor = m_gridColumnMap.find(nColumn);
	CTaskObj* pTask = GetTaskByRow( nRow );
	if (colItor != m_gridColumnMap.end() && NULL != pTask )
	{
		int baseRow = GetFixedRowCount();
		int baseCol = GetFixedColumnCount();
		CTaskObj &data = *pTask;
		CGridCellNumeric* pCell = 
			dynamic_cast<CGridCellNumeric*>(m_gridCtrl->GetCell(nRow+baseRow,nColumn+baseCol));
		switch( colItor->second->dataType )
		{
		case DiffPara_Colom:
			  if ( pCell )
			  {
				  data.SetDifficultyParam( (float)pCell->GetNumber() );
			  }
			  break;
		case  SuggestionColumn:
			{
				CString* pView = GetEditSuggestion( nRow );
				if ( pView )
				{
					*pView = text;
				}
			}
		    break;

		}
	}
	return TRUE;
}
 BOOL   CPublishTaskManger::GetFlexItemEditable(int nRow,int nColumn)
 {
	 GridColumnMapItor colItor = m_gridColumnMap.find(nColumn);
	 CTaskObj* pTask = GetTaskByRow( nRow );
	 if (colItor != m_gridColumnMap.end() && NULL != pTask )
	 {
		 if ( waitPublish == pTask->GetTaskStatus() )
		 {
			 switch( colItor->second->dataType )
			 {
			 case DiffPara_Colom:
			 case  SuggestionColumn:
				 return TRUE;
			 }
		 }else if ( finishReviewWait == pTask->GetTaskStatus() 
			 && SuggestionColumn == colItor->second->dataType )
		 {
			 return TRUE;
		 }
	 }
	 return FALSE;
 }
 BOOL    CPublishTaskManger::InitFlexNumEditItem(CGridCellNumeric* pCell,int nRow,int nColumn)
 {
	 GridColumnMapItor itor = m_gridColumnMap.find( nColumn );
	 if ( itor != m_gridColumnMap.end())
	 {
		 if ( TaskPrecent_col == itor->second->dataType )
		 {//百分比
			 pCell->SetFlags( CGridCellNumeric::Integer );

		 }else if ( DiffPara_Colom == itor->second->dataType)
		 {//工时
			 pCell->SetFlags( CGridCellNumeric::Real );
		 }
	 }
	 return TRUE;
 }
  //
 void CPublishTaskManger::OnClickGrid( NMHDR* pNMHDR,LRESULT* pResult )
 {
	 NM_GRIDVIEW *gridView = reinterpret_cast<NM_GRIDVIEW*>(pNMHDR);
	 int nRow = gridView->iRow - GetFixedRowCount();
	 int nColumn = gridView->iColumn - GetFixedColumnCount();
	 //获取列信息
	 GridColumnMapItor itor = m_gridColumnMap.find( nColumn );
	 CTaskObj* pObj = GetTaskByRow( nRow );
	 if ( itor != m_gridColumnMap.end() && pObj != NULL )
	 {
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
			 BOOL ret ;
			 if ( finishReviewWait == pObj->GetTaskStatus() )
			 {
				 ret = m_pTaskSever->TaskReviewFinishAction( isAgree, revStr,name, *pObj );
			 }else  if( waitPublish == pObj->GetTaskStatus() )
			 {
				  ret = m_pTaskSever->TaskReviewAction( isAgree, revStr,name, *pObj );
			 }else
			 {
				 return;
			 }
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
 void CPublishTaskManger::OnGridSelChange( NMHDR* pNMHDR,LRESULT* pResult )
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


 void  CPublishTaskManger::UpdateTaskContent( int taskId )
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
 void  CPublishTaskManger::SelectTaskLine( int taskId )
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