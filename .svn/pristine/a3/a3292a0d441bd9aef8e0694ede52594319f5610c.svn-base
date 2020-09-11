#include "stdafx.h"
#include "myTaskGirdManager.h"
#include "../../Dlg/NewTaskDlg.h"
#include "../../Dlg/StartTaskDlg.h"
#include "../../Dlg/ChangeApplyFor.h"
#include "../../Dlg/FinisTaskDlg.h"
#include "Src/MainFrame/messageDef.h"
#include "../../Dlg/workHour/AskForLeaveDlg.h"
#include "Src/MainFrame/MainFrm.h"
#include <Src/Tools/UtilityTool/UtilityTool.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

CTaskGridManager::CTaskGridManager( CActionServerFactory *pSeverFactory ):CBaseGridManger(pSeverFactory)
{
	//m_operateFlag.reset( NULL );
	m_isSelAllCol = false;
	m_pSchduleAction = pSeverFactory->GetScheduleSever();
	m_pWorkHourAction = pSeverFactory->GetWorkHourSever();
	m_loginUserId = m_pSeverFactory->GetLoginServer()->GetLoginUser().GetId();
	m_selUpdateDate.SetDate( 1899, 12, 30 );
}

void CTaskGridManager::UpdateShowAllTask( int year, int quart )
{
	((CMainFrame *)AfxGetMainWnd())->ShowProgressMsg( MSG_READ_TASK_DATA );
	//const CUser obj = m_pSeverFactory->GetLoginServer()->GetLoginUser();
	//获取所有任务责任人是登陆者
	m_showTaskList.clear();
	m_pTaskSever->GetUserHandleTask( m_loginUserId, year, quart, m_showTaskList );
	RefreshSchduleMap();
	UpdateGridView();
	((CMainFrame *)AfxGetMainWnd())->CloseMsgDlg( );
}
void CTaskGridManager::UpdateNewingTask()
{
	//const CUser obj = m_pSeverFactory->GetLoginServer()->GetLoginUser();
	//获取当前用户创建的任务
	m_showTaskList.clear();
	m_pTaskSever->GetNewingTaskByCreateId( m_loginUserId, m_showTaskList );
	m_pTaskSever->GetTaskByStatus( m_loginUserId, newTasking, m_showTaskList) ;
	RefreshSchduleMap();
	//SetGridContent();
	UpdateGridView();
}
void CTaskGridManager::UpdateWaitReveiwTask()
{
	//const CUser obj = m_pSeverFactory->GetLoginServer()->GetLoginUser();
	//获取当前用户创建的任务
	m_showTaskList.clear();
	m_pTaskSever->GetTaskByStatus( m_loginUserId, waitPublish, m_showTaskList) ;
	m_pTaskSever->GetTaskByStatus( m_loginUserId, waitScore, m_showTaskList) ;
	m_pTaskSever->GetTaskByStatus( m_loginUserId, changeWait1, m_showTaskList) ;
	m_pTaskSever->GetTaskByStatus( m_loginUserId, changeWait2, m_showTaskList) ;
	m_pTaskSever->GetTaskByStatus( m_loginUserId, changeWait0, m_showTaskList) ;
	m_pTaskSever->GetTaskByStatus( m_loginUserId, finishReviewWait, m_showTaskList) ;
	RefreshSchduleMap();
	//SetGridContent();
	ReSetUpdateDateList();
	UpdateGridView();
}
void CTaskGridManager::UpdateOngoingTask()
{
	//const CUser obj = m_pSeverFactory->GetLoginServer()->GetLoginUser();
	m_showTaskList.clear();
	m_pTaskSever->GetTaskByStatus( m_loginUserId, ongoing, m_showTaskList);
	RefreshSchduleMap();
	//SetGridContent();
	ReSetUpdateDateList();
	UpdateGridView();
}

void CTaskGridManager::ReSetUpdateDateList()
{
	m_setUpdateDateList.clear();
	m_taskUpdateDateMap.clear();
    ListDate lstTmp;
	COleDateTime lastDay = m_pSchduleAction->GetLastSetUpdateData();
	for( ListTaskItor itor = m_showTaskList.begin(); itor != m_showTaskList.end(); itor++ )
	{
		if ( ongoing == itor->GetTaskStatus() )
		{
			ListDate taskDates;
			m_pSeverFactory->GetScheduleSever()->GetUnUpdateDateList( *itor, taskDates);
			if ( taskDates.size() > 0)
			{
				m_taskUpdateDateMap[itor->GetId()] = taskDates;
				lstTmp.merge( taskDates );
		
			}else if( itor->GetPlanStartTime() <= lastDay ) 
			{
				m_taskUpdateDateMap[itor->GetId()].push_back( lastDay );
			}
			//if ( m_setUpdateDateList.size() == 0 )
			//{
			//	m_setUpdateDateList = m_taskUpdateDateMap[itor->GetId()];
			//}
		}
	}
	//
	for ( ListDateItor itor = lstTmp.begin(); itor != lstTmp.end(); itor++ )
	{
		ListDateItor itor2;
		for ( itor2 = m_setUpdateDateList.begin(); itor2 != m_setUpdateDateList.end(); itor2++ )
		{
			if ( *itor == *itor2 )
			{
				break;
			}
		}
		if ( itor2 == m_setUpdateDateList.end() )
		{
			m_setUpdateDateList.push_back( *itor );
		}
	}
	if ( m_setUpdateDateList.size() <= 0 )
	{//将前一条的日期放入进度更新框
		m_setUpdateDateList.push_back( lastDay );
	}
}
void CTaskGridManager::UpdateFinishTask()
{
	((CMainFrame *)AfxGetMainWnd())->ShowProgressMsg( MSG_READ_TASK_DATA );
	/*const CUser obj = m_pSeverFactory->GetLoginServer()->GetLoginUser();*/
	m_showTaskList.clear();
	m_pTaskSever->GetTaskByStatus( m_loginUserId, task_close, m_showTaskList);
	RefreshSchduleMap();
	//SetGridContent();
	UpdateGridView();
	((CMainFrame *)AfxGetMainWnd())->CloseMsgDlg( );
}

void CTaskGridManager::UpdateWaitStartTask()
{
	//const CUser obj = m_pSeverFactory->GetLoginServer()->GetLoginUser();
	m_showTaskList.clear();
	m_pTaskSever->GetTaskByStatus( m_loginUserId, waitStart, m_showTaskList);
	RefreshSchduleMap();
	//SetGridContent();
	UpdateGridView();
}
void CTaskGridManager::UpdateOwnerTask()
{
	((CMainFrame *)AfxGetMainWnd())->ShowProgressMsg( MSG_READ_TASK_DATA );
	m_pTaskSever->UpdateOwernTask();
	m_showTaskList.clear();
	m_showTaskList = m_pTaskSever->GetOwernTask();
	m_pTaskSever->GetTaskByStatus( m_loginUserId, task_suspend, m_showTaskList );
	RefreshSchduleMap();
	ReSetUpdateDateList();
	//SetGridContent();
	UpdateGridView();
	((CMainFrame *)AfxGetMainWnd())->CloseMsgDlg( );
}

//创建列头
void CTaskGridManager::CreateColumnMap()
{
	CBaseGridManger::CreateColumnMap();
	InsertUpDownColum(); //提交按钮
}
//填充数据
void CTaskGridManager::CreateRowItemMap()
{
	UpdateOwnerTask();
	RefreshSchduleMap( TRUE );
	//if ( m_setUpdateDateList.size() > 0 )
	//{
	//	m_selUpdateDate = *(m_setUpdateDateList.begin());
	//}
}

void CTaskGridManager::RefreshSchduleMap(  BOOL reSetValue/* = FALSE*/ )
{
	CSchedule schObj; //空白进度
	ListTaskItor itor = m_showTaskList.begin();
	while( itor !=  m_showTaskList.end() )
	{
		int st = itor->GetTaskStatus();
		if ( ongoing == st || waitStart == st )
		{
			schObj.ReInit();
			schObj.SetWorkTypeId( itor->GetWorkTypeId() );
			if ( m_selUpdateDate.m_dt == 0 || m_selUpdateDate.GetStatus() != COleDateTime::valid )
			{
				m_pSchduleAction->GetMaxNewScheduleByTaskId( itor->GetId(), schObj );
			}else
			{
				m_pSchduleAction->GetSchduleByDate( itor->GetId(), m_selUpdateDate, schObj  );
			}
			if ( m_currentUpdate.find( itor->GetId()) == m_currentUpdate.end() )
			{
				m_currentUpdate.insert( std::make_pair( itor->GetId(), schObj) );
			}else
			{
				m_currentUpdate[itor->GetId()] = schObj;
			}
		}
		itor++;
	}
}
//填充数据内容
CString CTaskGridManager::GetFlexItemText(int nRow, int nColumn)
{
	CString text;
	if ( nRow < 0 || m_showTaskList.size() <= 0 || (UINT)nRow > m_showTaskList.size() )
	{
		return text;
	}
	
	GridColumnMapItor colItor = m_gridColumnMap.find(nColumn);
	if (colItor == m_gridColumnMap.end())
		return text;
	ListTaskItor itor = m_showTaskList.begin();

	advance( itor, nRow );
	if ( itor == m_showTaskList.end())
	{
		return text;
	}
	CTaskObj& obj = *itor;
	TaskScheduleMapItor itorMap;
	itorMap =m_currentUpdate.find(obj.GetId());
	CSchedule chobj;
	m_pSchduleAction->GetMaxNewScheduleByTaskId( obj.GetId(), chobj );
	bool setColor = true;
	int diffPer = 0;
	COleDateTime currTime= COleDateTime::GetCurrentTime();
	COleDateTime currDate( currTime.GetYear(), currTime.GetMonth(), currTime.GetDay(), 0,0,0);
	diffPer = obj.CaludateTimePercent( currTime ) - chobj.GetPercent();
	switch( colItor->second->dataType )
	{
	case TaskName_col:
		text = obj.GetName();
		break;
	case  TaskStatus_col:
		GetTaskStatusSting( obj, text );
		break;
	case TaskPrecent_col://最新进度
		if ( ongoing == obj.GetTaskStatus() )
		{
			m_pSchduleAction->GetMaxNewScheduleByTaskId( obj.GetId(), chobj );
			text.Format(_T("%d"), chobj.GetPercent() );
		}
		break;
	case  taskOwern:
		text = GetOwnerNameStr( obj.GetOwnerId() );
		break;
	case TaskUpdatePrecent:	//更新的进度
		if ( m_currentUpdate.end() != itorMap )
		{
			text.Format( _T("%d"),  itorMap->second.GetPercent());
		}else if ( chobj.GetId() > 0 )
		{
			text.Format( _T("%d"), chobj.GetPercent() );
		}
		break;
	case TaskUpcontent:
		if ( m_currentUpdate.end() != itorMap )
		{
			text = itorMap->second.GetUpdateContent();
		}else if ( chobj.GetId() > 0 )
		{
			text = chobj.GetUpdateContent();
		}
		break;
	case  TaskHistoryContent:
		GetHistoryString( obj.GetId(), text );
		break;
	case  TaskWorkHour:
		if ( m_currentUpdate.end() != itorMap )
		{
			text.Format( _T("%0.1f"),  itorMap->second.GetWorkHours());
		}else if ( chobj.GetId() > 0 )
		{
			text.Format( _T("%0.1f"),  chobj.GetWorkHours());
		}
		break;
	case TaskPlanTime:
		text = GetPanTimeString( obj );
		break;
	case TaskFactTime:
		m_pSchduleAction->SetTaskFactWorkHour( obj );
		text = GetFactTimeString( obj );
		break;
	case TaskOperate1:
		{
			int status = obj.GetTaskStatus(); 
			if ( waitStart == status )
			{
				text = _T("启动任务");
			}else if ( ongoing == status )
			{
				CGridCellCheck *pCell = (CGridCellCheck *)m_gridCtrl->GetCell( nRow+GetFixedRowCount(), nColumn+GetFixedColumnCount());
				setColor = !pCell->GetCheck();
				if( IsEnableUpdateSchdule( obj.GetId()) )
				{
					text = _T("更新进度");
				}
			}else if ( newTasking == status && obj.GetCreateId() == m_loginUserId )
			{
				text = _T("申请任务");
			}
		}
		break;
	case ProjectType_Colom:
		text = GetProjectShortName( obj );
		break;
	case  PERCENT_DIFF:
		{
			text.Format( _T("%03d"), diffPer );
		}
		break;
	case  TaskTimeType:
		if ( m_currentUpdate.end() != itorMap && itorMap->second.GetWorkTypeId() > 0 )
		{
			text = GetWorkHourStringById( itorMap->second.GetWorkTypeId() );
		}else
		{
			text = GetWorkHourStringById( obj.GetWorkTypeId() );
		}
		
		break;
	}

	if ( setColor )
	{
		SetBkColorByTask(nRow, nColumn, obj, diffPer );
	}
	return text;
}

BOOL  CTaskGridManager::SetFlexItemText(int nRow,int nColumn,CString text)
{
	if ( m_showTaskList.size() <= 0 || nRow < 0)
	{
		return TRUE;
	}
	GridColumnMapItor colItor = m_gridColumnMap.find(nColumn);
	ListTaskItor itor = m_showTaskList.begin();
	advance( itor, nRow );
	if (colItor != m_gridColumnMap.end() && itor != m_showTaskList.end())
	{
		TaskScheduleMapItor itorMap;
		itorMap =m_currentUpdate.find(itor->GetId());
        int baseRow = GetFixedRowCount();
		int baseCol = GetFixedColumnCount();
	   switch( colItor->second->dataType )
	   {
	   case TaskName_col:
		   //isChange = itor->GetName() != text;
		   itor->SetName( text );
		   //
		   m_pTaskSever->GetTaskByTaskId( itor->GetId(), *itor );
		   UpdateGridRowView( nRow );
		   break;
	   case TaskUpdatePrecent://进度
		   if ( itorMap != m_currentUpdate.end() )
		   {
			   CGridCellNumeric* pCell = 
				   dynamic_cast<CGridCellNumeric*>(m_gridCtrl->GetCell(nRow+baseRow,nColumn+baseCol));
			   double value = pCell->GetNumber();
			    
			   //isChange = itorMap->second.GetPercent() != (int)value;
			   itorMap->second.SetPercent( (int)value );
		   }
		   break;
	   case TaskWorkHour:
		   if ( itorMap != m_currentUpdate.end() )
		   {
			   CGridCellNumeric* pCell = 
				   dynamic_cast<CGridCellNumeric*>(m_gridCtrl->GetCell(nRow+baseRow,nColumn+baseCol));
			   double value = pCell->GetNumber();
			  // isChange = itorMap->second.GetWorkHours() != (float)value;
			  CUtilityTool::GetObj()->ConvertStringToDouble( text, value );
			  itorMap->second.SetWorkHours( value );
			  
		   }
		   break;
	   case TaskUpcontent:
		   if ( itorMap != m_currentUpdate.end() )
		   {
			   // isChange = itorMap->second.GetUpdateContent() != text;
			   itorMap->second.SetUpdateContent( text );
			 
		   }
		   break;
	   default:
		   return TRUE;
	   } 

	}
	return TRUE;
}

BOOL CTaskGridManager::GetFlexItemEditable(int nRow,int nColumn)
{
	if ( m_showTaskList.size() <= 0  || nRow >= (int)m_showTaskList.size() || nRow < 0 )
	{
		return FALSE;
	}
	//BOOL bEditable = GetColumnEditable(nColumn) && GetRowEditable(nRow);
	GridColumnMapItor colItor;
	CTaskObj *pObj = ValidateRow( nRow, nColumn, colItor );
    BOOL bEditable = FALSE;
	if ( pObj )
	{
		//if ( newTasking == pObj->GetTaskStatus() && pObj->GetCreateId() == m_loginUserId )
		//{
		//	//if ( TaskName_col == colItor->second->dataType )
		//	//{
		//	//	bEditable = TRUE;
		//	//}
		//	if (  TaskOperate1 == colItor->second->dataType  && pObj->GetCreateId() == m_loginUserId  )
		//	{
		//		bEditable = pObj->GetIsModify();
		//	}
		//}else
		if ( ongoing == pObj->GetTaskStatus() )
		{//进行中 
			
			if ( /*IsFinishTask( pObj->GetId() ) == FALSE && */IsEnableUpdateSchdule( pObj->GetId() ) )
			{
				switch( colItor->second->dataType )
				{
				case TaskUpdatePrecent:
				case TaskWorkHour:
				case TaskUpcontent:
				case TaskOperate1:
				case TaskTimeType:
					if ( TaskTimeType == colItor->second->dataType && pObj->GetProjctId() > 0)
					{
						bEditable = FALSE;
					}else
					{
						bEditable = TRUE;
					}
				}
			}
		}/*else if ( waitStart == pObj->GetTaskStatus() )
		{
			if (  TaskOperate1 == colItor->second->dataType )
			{
				bEditable = TRUE;
			}
		}*/
	}
	return bEditable;
}



int CTaskGridManager::GetFlexRowCount()
{
	return m_showTaskList.size();
}
BOOL    CTaskGridManager::InitFlexNumEditItem(CGridCellNumeric* pCell,int nRow,int nColumn)
{
	GridColumnMapItor itor = m_gridColumnMap.find( nColumn );
	if ( itor != m_gridColumnMap.end())
	{
		if ( TaskPrecent_col == itor->second->dataType || TaskUpdatePrecent == itor->second->dataType )
		{//百分比
			pCell->SetFlags( CGridCellNumeric::Integer );

		}else if ( TaskWorkHour == itor->second->dataType)
		{//工时
			pCell->SetFlags( CGridCellNumeric::Real );
		}
	}
	return TRUE;
}
BOOL    CTaskGridManager::InitFlexComboItem(CGridCellCombo* pCell,int nRow,int nColumn)
{
	 GridColumnMapItor itor = m_gridColumnMap.find( nColumn );
	 if ( TaskTimeType == itor->second->dataType )
	 {
		 const TaskIdStringMap workTypeMap = m_pTaskSever->GetWorkHourString();
		 TaskIdStringMap::const_iterator dataItor = workTypeMap.begin();
		 CStringArray strs;
		 for ( ; dataItor != workTypeMap.end(); dataItor++ )
		 {
			 strs.Add( dataItor->second );
		 }
		 pCell->SetOptions( strs );
	 }
	 return TRUE;
}
void CTaskGridManager::OnOpreateBtnClick( CTaskObj &editTask, int nRow, int ncol )
{
	const CTaskObj& obj = editTask;
	int st = obj.GetTaskStatus();
	bool update = false;
	if ( waitStart == st  )
	{//启动任务
		CStartTaskDlg dlg( obj, m_selUpdateDate );
		if (IDCANCEL != dlg.DoModal() )
		{
			update = true;
		}
	}else if ( ongoing == st && IsEnableUpdateSchdule( obj.GetId()) )
	{
		/*if ( FALSE == IsFinishTask( obj.GetId()) )
		{*///选择更新进度
			CGridCellCheck *pCell = (CGridCellCheck *)m_gridCtrl->GetCell( nRow+GetFixedRowCount(), ncol+GetFixedColumnCount());
			if ( pCell )
			{
				if ( FALSE == pCell->GetCheck() )
				{
					CSchedule obj;
					m_pSchduleAction->GetSchduleByDate( obj.GetId(), m_selUpdateDate, obj );
					if ( m_currentUpdate.find( obj.GetId()) != m_currentUpdate.end() )
					{
						if ( obj.GetId() > 0 )
						{
							m_currentUpdate[obj.GetId()] = obj;
							UpdateGridRowView( nRow );
							return;
						}
					}
					update = true;
				}else
				{//将背景设变白色
					for ( GridColumnMapItor itor = m_gridColumnMap.begin(); itor != m_gridColumnMap.end(); itor++ )
					{
						switch( itor->second->dataType )
						{//
						case TaskUpdatePrecent:
						case TaskWorkHour:
						case TaskUpcontent:
							m_gridCtrl->SetItemBkColour( nRow+GetFixedRowCount(), itor->first+GetFixedColumnCount(), RGB(255, 255, 255) );
							m_gridCtrl->RedrawCell( nRow+GetFixedRowCount(), itor->first+GetFixedColumnCount() );
							break;
						}
					}
                  
				}
		     }
		/*}*/
		//else
		//{//申请完成任务
  //         CFinisTaskDlg dlg( obj, m_selUpdateDate );
		//   if (IDCANCEL != dlg.DoModal() )
		//   {
		//	   update = true;
		//   }
	 //   }
	}else if ( newTasking == st )
	{
		CNewTaskDlg dlg( &obj );
		if ( IDCANCEL != dlg.DoModal() )
		{//更新此行任务
			update = TRUE;
		}
	}
	CTaskObj newObj;
	m_pTaskSever->GetTaskByTaskId( obj.GetId(), newObj ) ;
	if ( newObj.GetId() > 0 )
	{
		editTask = newObj;
	}
	//更新行内容
	if( update )
	{
		if( waitStart == st )
		{//重新初始化更新日期
			ReSetUpdateDateList();
			::PostMessage( m_gridCtrl->GetParent()->GetSafeHwnd(), GRID_MYTASK_UPDATE_DATE, NULL, NULL );
		}
		UpdateGridRowView( nRow );
	}
}
void  CTaskGridManager::OnGridSelChange( NMHDR* pNMHDR,LRESULT* pResult )
{
	NM_GRIDVIEW *gridView = reinterpret_cast<NM_GRIDVIEW*>(pNMHDR);
	m_isSelAllCol = gridView->iRow < GetFixedRowCount() && gridView->iColumn < GetFixedColumnCount();
	if ( m_isSelAllCol )
	{
		return;
	}
	int nRow = gridView->iRow - GetFixedRowCount();
	int nColumn = gridView->iColumn - GetFixedColumnCount();
	GridColumnMapItor itor ;
	CTaskObj* pObj = ValidateRow(  nRow,  nColumn, itor );
	if ( NULL == pObj && pObj->GetId() != m_outputShowId )
	{
		return;
	}
	m_outputShowId = pObj->GetId();
	SendMessage( AfxGetMainWnd()->GetSafeHwnd(), MAINFRAME_MSG_TASKID, m_outputShowId, NULL );
}
CTaskObj*      CTaskGridManager::GetTaskByRow( int nRow )
{
	if ( m_showTaskList.size() <= 0 || nRow < 0)
	{
		return NULL;
	}
	ListTaskItor itor = m_showTaskList.begin();
	advance( itor, nRow );
	if ( itor != m_showTaskList.end() )
	{
		return &(*itor);
	}
	return NULL;
}
CTaskObj*    CTaskGridManager::ValidateRow( int nRow, int nColumn,  GridColumnMapItor &colItor  )
{
	colItor = m_gridColumnMap.find(nColumn);
	if (colItor == m_gridColumnMap.end())
		return NULL;
	return GetTaskByRow( nRow );
}
void CTaskGridManager::OnClickGrid( NMHDR* pNMHDR,LRESULT* pResult )
{
	NM_GRIDVIEW *gridView = reinterpret_cast<NM_GRIDVIEW*>(pNMHDR);

	int nRow = gridView->iRow - GetFixedRowCount();
	int nColumn = gridView->iColumn - GetFixedColumnCount();
	//获取列信息
	GridColumnMapItor itor ;
	CTaskObj* pObj = ValidateRow(  nRow,  nColumn, itor );
	if ( NULL == pObj )
	{
		return;
	}
	int dataType = itor->second->dataType;
	if ( TaskOperate1 == dataType )
	{
		OnOpreateBtnClick( *pObj, nRow, nColumn );
		return;
	}
	
}

void CTaskGridManager::OnGridRclick( NMHDR* pNMHDR,LRESULT* pResult )
{
	//if ( m_isSelAllCol )
	//{//选中行
		int rowMin=-1, colMin = -1, rowMax = -1, colMax = -1;
		CCellRange selRange = m_gridCtrl->GetSelectedCellRange();
		if (TRUE != selRange.IsValid() || selRange.GetMinRow() < GetFixedRowCount()
			|| (UINT)selRange.GetMaxCol() < m_gridColumnMap.size() || selRange.GetMinCol() > GetFixedColumnCount() )
		{
			return;
		}
		rowMin = selRange.GetMinRow();
		colMin = selRange.GetMinCol();
		rowMax = selRange.GetMaxRow();
		colMax = selRange.GetMaxCol();
		int row =   rowMin-GetFixedRowCount();
		if( rowMin < 0 || m_showTaskList.size() <= 0 || row < 0 )
		{
			return;
		}
		ListTaskItor itorTask = m_showTaskList.begin();
		advance( itorTask, row );
		if ( itorTask == m_showTaskList.end())
		{
			return;
		}
		BOOL isEnableDel = TRUE;
		int count = rowMin;
		while( itorTask != m_showTaskList.end() && count <= rowMax )
		{
			if ( newTasking != itorTask->GetTaskStatus() || m_loginUserId != itorTask->GetCreateId()  )
			{
				isEnableDel = FALSE; 
				break;
			}
			itorTask++;
			count++;
		}
		CString str;
		CPoint pt;
		GetCursorPos( &pt );
		if ( isEnableDel )
		{//弹出删除对话框
			str.LoadString( IDS_STR_DEL_TASK );
			CMenu PopupMenu;
			PopupMenu.CreatePopupMenu();
			PopupMenu.AppendMenu(MF_ENABLED, IDM_DELETE_TASK, str);
			PopupMenu.TrackPopupMenu(TPM_LEFTALIGN |TPM_RIGHTBUTTON , pt.x, pt.y, m_gridCtrl->GetParent());
			return;
		}
		if( rowMin == rowMax )
		{
			CTaskObj objTmp;
			m_pTaskSever->GetTaskByTaskId( itorTask->GetId(), objTmp );
			int st = itorTask->GetTaskStatus();
			if ( objTmp.GetId() > 0 )
			{
				st = objTmp.GetTaskStatus();
			}
			if ( waitStart == st || ongoing == st || task_suspend == st)
			{
				str.LoadString( IDS_STR_CHANGE_TASK );
				CMenu PopupMenu;
				PopupMenu.CreatePopupMenu();
				PopupMenu.AppendMenu(MF_ENABLED, IDM_CHANGE_TASK, str);
				PopupMenu.TrackPopupMenu(TPM_LEFTALIGN |TPM_RIGHTBUTTON , pt.x, pt.y, m_gridCtrl->GetParent());
			}
		}
	//}
}
void CTaskGridManager::OnDeleteTask()
{
	CCellRange selRange = m_gridCtrl->GetSelectedCellRange();
	if (TRUE != selRange.IsValid() || selRange.GetMinRow() < GetFixedRowCount() )
	{
		return;
	}
	int row = selRange.GetMinRow()-GetFixedRowCount();
	if ( m_showTaskList.size() <= 0 || row < 0 )
	{
		return;
	}
	ListTaskItor itorTask = m_showTaskList.begin();
	advance( itorTask, row );
	if ( itorTask == m_showTaskList.end())
	{
		return;
	}
	int count = selRange.GetMinRow();
	while( itorTask != m_showTaskList.end() && count <= selRange.GetMaxRow() )
	{
		if ( newTasking == itorTask->GetTaskStatus() )
		{
			BOOL ret = m_pTaskSever->DeleteTaskById( itorTask->GetId());
			if( userExcOk != ret )
			{
				CString str;
				str.Format(_T("删除‘%s’任务失败，%s"), itorTask->GetName(), m_pSeverFactory->GetErrorString( ret ) );
				AfxMessageBox( str );
				return;
			}
            break;
		}else
		{
			AfxMessageBox(_T("选中的任务中，任务状态有不是新建中的任务") );
		}
		count++;
	}
}
void CTaskGridManager::OnApplyForChangeTask()
{
	CCellRange selRange = m_gridCtrl->GetSelectedCellRange();
	if (TRUE != selRange.IsValid() || selRange.GetMinRow() < GetFixedRowCount() 
		|| selRange.GetMinRow() != selRange.GetMaxRow() )
	{
		return;
	}
	int nRow = selRange.GetMinRow()-GetFixedRowCount();
	if ( m_showTaskList.size()<= 0 || nRow < 0 )
	{
		return;
	}
	ListTaskItor itorTask = m_showTaskList.begin();
	advance( itorTask, nRow );
	if ( itorTask == m_showTaskList.end())
	{
		return;
	}
	CChangeActionInc* pChangeAction = m_pSeverFactory->GetChangeSever();
	ListChange changList;
	pChangeAction->GetNewingChangeByTaskId( itorTask->GetId() ,changList );
	CTaskChange chObj;
	if ( changList.size() > 0 )
	{
		chObj = *( changList.begin() );
	}else
	{
		chObj.SetTaskId( itorTask->GetId() );
	}
	CChangeApplyFor dlg( *itorTask, chObj );
	if ( IDOK == dlg.DoModal() )
	{
		m_pTaskSever->GetTaskByTaskId( itorTask->GetId(), *itorTask );
		UpdateGridRowView( nRow );
	}
}
void  CTaskGridManager::UpdateTaskContent( int taskId )
{
	if ( taskId > 0 )
	{
		ListTaskItor itor;
	    int row = FindTaskListById(  taskId, m_showTaskList,itor );
		if ( row >= 0 )
		{
		     m_pTaskSever->GetTaskByTaskId( itor->GetId(), *itor );
			 UpdateGridRowView( row );
             SelectRow( row );
		}
	}
}
void  CTaskGridManager::SelectTaskLine( int taskId )
{
	if ( taskId > 0 )
	{
		ListTaskItor itor;
		int row = FindTaskListById(  taskId, m_showTaskList,itor );
		if ( row >= 0 )
		{
			 SelectRow( row );
		}
	}
}
void CTaskGridManager::SetUpdateDate( const CString &date )
{
	if ( !date.IsEmpty() )
	{
		m_selUpdateDate.ParseDateTime( date, VAR_DATEVALUEONLY );
	}else
	{//1899-12-30
		m_selUpdateDate.SetDate( 1899, 12, 30 );
	}
	RefreshSchduleMap( TRUE );
	UpdateGridView();
}
BOOL  CTaskGridManager::IsFinishTask( int taskId )
{
	CSchedule schObj; 
	m_pSchduleAction->GetMaxNewScheduleByTaskId( taskId, schObj );
	if ( schObj.GetPercent() >= 100 )
	{
		return TRUE;
	}
	return FALSE;
}
BOOL  CTaskGridManager::IsEnableUpdateSchdule( int taskId )
{
	if ( m_taskUpdateDateMap.find( taskId ) != m_taskUpdateDateMap.end() )
	{
		ListDate &dateList = m_taskUpdateDateMap[taskId];
		ListDateItor dateItor = dateList.begin();
		while( dateItor != dateList.end() )
		{
			if ( *dateItor == m_selUpdateDate )
			{
				break;
			}
			dateItor++;
		}
		if ( dateItor != dateList.end() )
		{
			return TRUE;
		}
	}
	return FALSE;
}
GridItemType CTaskGridManager::GetFlexItemType(int nRow,int nColumn)
{
	GridColumnMapItor itor ;
	CTaskObj* pObj = ValidateRow(  nRow,  nColumn, itor );
	if ( pObj && itor->second->dataType == TaskOperate1   )
	{
		/*if ( IsFinishTask( pObj->GetId()) )
		{
		return GridItemType_Button;
		}else*/ 
		if ( ongoing == pObj->GetTaskStatus() )
		{
			if ( IsEnableUpdateSchdule( pObj->GetId()) )
			{
				return GridItemType_Check;
			}else
			{
				return GridItemType_Edit;
			}
		}else if ( newTasking == pObj->GetTaskStatus() && pObj->GetCreateId() != m_loginUserId )
		{
			return  GridItemType_Edit;
		}
	}
	return CBaseGridManger::GetFlexItemType( nRow, nColumn );
}
double CTaskGridManager::GetTotalWorkHour( )
{
	double workHour = 0.0;
	//ListTaskItor itor = m_showTaskList.begin();
	//int nRow = GetFixedRowCount();
	//for ( ; itor != m_showTaskList.end(); itor++, nRow++)
	//{
	//	TaskScheduleMapItor itorMap;
	//	itorMap = m_currentUpdate.find( itor->GetId() );
	//	if ( itorMap != m_currentUpdate.end() )
	//	{
	//		if ( IsEnableUpdateSchdule(itor->GetId()) &&  )
	//		{
	//			workHour = itorMap->second.GetWorkHours();
	//		}else
	//		{
	//			CSchedule obj;
	//			m_pSchduleAction->GetSchduleByDate( itorMap->first, m_selUpdateDate, obj );
	//			if ( obj.GetId() <= 0 )
	//			{
	//				return FALSE;
	//			}
	//		}
	//	}
	//}
	ListTaskItor itor = m_showTaskList.begin();
	int nRow = GetFixedRowCount();
	int          nCol = 0;
	nCol = GetOpertorColunmNum();
	for ( ; itor != m_showTaskList.end(); itor++, nRow++)
	{
		TaskScheduleMapItor itorMap;
		itorMap = m_currentUpdate.find( itor->GetId() );
		if ( itorMap != m_currentUpdate.end() && /*FALSE == IsFinishTask( itor->GetId() ) && */IsEnableUpdateSchdule(itor->GetId())  )
		{//插入进度
			CGridCellCheck * pCell = ( CGridCellCheck *)m_gridCtrl->GetCell( nRow,  nCol );
			if ( pCell && pCell->GetCheck() )
			{
				workHour += itorMap->second.GetWorkHours();
			}else
			{
				CSchedule obj;
				m_pSchduleAction->GetSchduleByDate( itorMap->first, m_selUpdateDate, obj );
				workHour += obj.GetWorkHours();
			}
		}else if ( itor->GetTaskStatus() == ongoing )
		{
			CSchedule obj;
			m_pSchduleAction->GetSchduleByDate( itorMap->first, m_selUpdateDate, obj );
			workHour += obj.GetWorkHours();
		}
	}
	double norOngoingTaskWorkhour = 0.0;
	ASSERT( userExcOk == m_pWorkHourAction->GetTaskHourSumExceptOngoing( m_loginUserId, m_selUpdateDate, norOngoingTaskWorkhour ) );
	workHour += norOngoingTaskWorkhour;
	return workHour;
}
int CTaskGridManager::GetOpertorColunmNum()
{
	GridColumnMapItor colItor = m_gridColumnMap.begin();
	for(; colItor != m_gridColumnMap.end(); colItor++ )
	{
		if ( colItor->second->dataType == TaskOperate1 )
		{
			break;
		}
	}
	ASSERT(  colItor != m_gridColumnMap.end() );
	return colItor->first + GetFixedColumnCount();
}
int CTaskGridManager::GetWorkTypeColunmNum()
{
	GridColumnMapItor colItor = m_gridColumnMap.begin();
	for(; colItor != m_gridColumnMap.end(); colItor++ )
	{
		if ( colItor->second->dataType == TaskTimeType )
		{
			break;
		}
	}
	ASSERT(  colItor != m_gridColumnMap.end() );
	return colItor->first + GetFixedColumnCount();
}
void CTaskGridManager::OnUpdateSchdule()
{
	double        workHour = GetTotalWorkHour();
	int    loginUser   = m_pSeverFactory->GetLoginServer()->GetLoginUser().GetId();

	double   norTaskWorkHour = 0.0;
	BOOL ret = m_pWorkHourAction->GetNorTaskHourSum( loginUser, m_selUpdateDate, norTaskWorkHour );
	if ( ret != userExcOk )
	{
		AfxMessageBox( m_pSeverFactory->GetErrorString( ret ) );
		return;
	}
	CString info;
	long bRet = CUtilityTool::GetObj()->DoubleCmp( (workHour+norTaskWorkHour) - 7.5 );
	if ( bRet > 0 )
	{
		info.Format( _T("填写总任务工时为%0.1fh, 非任务工时总和%0.1fh，两者之和超过7.5小时"), workHour,
			norTaskWorkHour );
		AfxMessageBox( info );
		return;
	}
	if ( bRet < 0 )
	{
		if ( IDYES != AfxMessageBox( _T("填写工时总合低于7.5小时，是否填写非任务工时"), MB_YESNO ))
		{
			AfxMessageBox(_T("请重新填写进度"));
			return;
		}
		//添加工时备注(请假说明)
		CAskForLeaveDlg dlg( m_selUpdateDate, workHour );
		if ( IDOK != dlg.DoModal() )
		{
			return;
		}
		//
	}

	ListTaskItor itor = m_showTaskList.begin();
	int          nRow = GetFixedRowCount();
	int          nCol = 0;
	int          nWorkTimeTypeCol = 0;
	nCol = GetOpertorColunmNum();
	nWorkTimeTypeCol = GetWorkTypeColunmNum();
	BOOL updateAllView = FALSE;
	for ( ; itor != m_showTaskList.end(); itor++, nRow++)
	{
		TaskScheduleMapItor itorMap;
		itorMap = m_currentUpdate.find( itor->GetId() );
		if ( itorMap != m_currentUpdate.end() && /*FALSE == IsFinishTask( itor->GetId() ) && */IsEnableUpdateSchdule(itor->GetId())  )
		{//插入进度
			CGridCellCheck * pCell = ( CGridCellCheck *)m_gridCtrl->GetCell( nRow,  nCol );
			if ( pCell && pCell->GetCheck() )
			{
				CSchedule objSch = itorMap->second;
				objSch.SetUpdateTime( m_selUpdateDate );
				objSch.SetTaskId( itorMap->first );
				objSch.SetWorkTypeId( itor->GetWorkTypeId() );
				CGridCellCombo * pComboCell = ( CGridCellCombo *)m_gridCtrl->GetCell( nRow,  nWorkTimeTypeCol );
				if ( pComboCell )
				{
					int selType = pComboCell->GetCurSel();
					const TaskIdStringMap & timeTypeMap = m_pTaskSever->GetWorkHourString();
					if ( selType >= 0 && selType < timeTypeMap.size() )
					{
						TaskIdStringMap::const_iterator timeTypeItor = timeTypeMap.begin();
						advance( timeTypeItor, selType );
						objSch.SetWorkTypeId( timeTypeItor->first );
					}
				}
				BOOL ret = m_pSchduleAction->InsertSchedule( objSch );
				if ( ret != userExcOk )
				{
					if ( ret != paraError )
					{
						AfxMessageBox( m_pSeverFactory->GetErrorString( ret ) );
					}
					UpdateGridRowView( nRow - GetFixedRowCount() );
                    return;
				}/*else
				{
					itorMap->second.ReInit();
				}*/
				if ( 100 == objSch.GetPercent() )
				{
					updateAllView = TRUE;
				}
			}else
			{//
				if ( itorMap->second.GetId() <= 0 )
				{//添加空白进度
					BOOL ret = m_pSchduleAction->InsertEmptySchdule( itorMap->first, m_selUpdateDate );
					if ( ret != userExcOk )
					{
						AfxMessageBox( m_pSeverFactory->GetErrorString( ret ) );
						return;
					}
					m_pSchduleAction->GetSchduleByDate( itorMap->first, m_selUpdateDate, itorMap->second);  
				}
			}
			//2015-5-31 更改检查日期
			if ( m_setUpdateDateList.size() > 0 && m_selUpdateDate == (*m_setUpdateDateList.begin()) )
			{
				COleDateTime updateDate  = m_selUpdateDate;
				/*COleDateTimeSpan span( 1, 0, 0, 0 );
				updateDate += span; */
				if ( updateDate > itor->GetCheckBeginDate() )
				{
					if ( userExcOk == m_pSchduleAction->UpdateCheckBeginDate( *itor, updateDate ) )
					{
						itor->SetCheckBeginDate( updateDate ); 
					}
				}
			}
		    //
		}
	}
	
	ReSetUpdateDateList();
	SendMessage( m_gridCtrl->GetParent()->GetSafeHwnd() ,GRID_MYTASK_UPDATE_DATE, NULL, NULL );
	SendMessage( m_gridCtrl->GetParent()->GetParent()->GetParent()->GetSafeHwnd() ,GRID_UPDATE_WORKHOUR, NULL, NULL );
	//2015-10-16 add 添加同步更新工时表
	//if ( updateAllView )
	//{
		UpdateGridView();
	//}
	AfxMessageBox(_T("更新进度成功"));

}