#include "stdafx.h"
#include "DepartTaskManger.h"
#include "Src/MainFrame/MainFrm.h"
#include "Src/MainFrame/messageDef.h"
#include "Src/action/ExcelAction/CTaskExcelAction.h"
#include "Src/MainFrame/Dlg/review/DepartProcessReview.h"
#include "Src/MainFrame/Dlg/review/CModiyTaskScoreDlg.h"
#include "Src/action/ExcelAction/CExportPerformanceTable.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

CDepartTaskManger::CDepartTaskManger( CActionServerFactory *pSeverFactory ):CDepartBaseGridManger( pSeverFactory )
{
	 m_showDepartId = 0;
	 m_showStatus = ongoing;
	 m_selUserId = 0;
	 m_diffPer = -100;
	 m_hasReviewPower = FALSE;
	 m_year = 0;
	 m_quart = 0;
}
//任务表格
void CDepartTaskManger::CreateColumnMap()
{
	InsertProjectTypeColum();
	InsertTaskNameColumn();
	InsertTaskStatusColumn();
	InsertTaskOwernColum();
	InsertTaskPercentColumn();
	InsertTaskUpdateColumn();
	InsertSchduleDate();
//	InsertTaskHistoryUpdateColumn(); //进度
	InsertTaskPlanColumn();
	InsertTaskFactTimeColumn();
	InsertDiffParaColum();
	InsertProjectReview();
	InsertDepartReview();
	InsertDatePercent();
	//InsertSuggestionColumn();
	//InsertAgreeBtnColum();
	//InsertUnAgreeBtnColum();
}
void      CDepartTaskManger::SetDefDepart(  int id, int taskST )
{
	m_showDepartId = id;
	m_showStatus = taskST;
}

void CDepartTaskManger::SetSelDepart( int departId,int taskStatusIndex, int selUserId, int diffPer, int year, int quarter )
{
	((CMainFrame *)AfxGetMainWnd())->ShowProgressMsg( MSG_READ_TASK_DATA );
	m_selUserId = selUserId;
	m_showDepartId = departId;
	m_diffPer = diffPer;
	m_year = year;
	m_quart = quarter;
	SetTaskStatus( taskStatusIndex );
	UpdateDbData();
	RefreshShowMap();
	UpdateGridView();
	((CMainFrame *)AfxGetMainWnd())->CloseMsgDlg( );
	
}
void CDepartTaskManger::UpdateDbData()
{
	m_taskList.clear();
	int hasChildCount = 0;
	if ( m_selUserId <= 0 )
	{
		ListUser userObj;
		if ( m_showDepartId > 0 )
		{
			m_pDepartSever->GetDepartUser( m_showDepartId, userObj, FALSE );
			for (ListUser::const_iterator itor = userObj.begin(); itor != userObj.end(); itor++ )
			{
				m_pTaskSever->GetTaskByStatus( itor->GetId(), m_showStatus, m_taskList, m_year, m_quart );
			}
		}else
		{
			for (ListDepartConstItor itorDepart = m_allDepart.begin(); itorDepart != m_allDepart.end(); itorDepart++ )
			{
			    userObj.clear();
				m_pDepartSever->GetDepartUser( itorDepart->GetId(), userObj, FALSE );
				for (ListUser::const_iterator itor = userObj.begin(); itor != userObj.end(); itor++ )
				{
					m_pTaskSever->GetTaskByStatus( itor->GetId(), m_showStatus, m_taskList, m_year, m_quart );
				}
			}
		}

	}else
	{
		m_pTaskSever->GetTaskByStatus( m_selUserId, m_showStatus, m_taskList, m_year, m_quart );
	}
}

//列表内容
void CDepartTaskManger::CreateRowItemMap()
{
	if ( m_pSeverFactory->GetLoginServer()->FindLoginPower( taskAgree ) )
	{
		m_hasReviewPower = TRUE;
	}
	UpdateDepart();
	if( m_allDepart.size() > 0 )
	{
		m_showDepartId = (m_allDepart.begin())->GetId();
	}
	UpdateDbData();
	RefreshShowMap();   
}

int CDepartTaskManger::GetFlexRowCount()
{
	return m_showTaskMap.size();
}

//获取显示
CString CDepartTaskManger::GetFlexItemText(int nRow, int nColumn)
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
	CSchedule shedule;
	BOOL ret = m_pSeverFactory->GetScheduleSever()->GetMaxNewScheduleByTaskId( obj.GetId(), shedule );
	int diffPer = 0;
	COleDateTime currTime= COleDateTime::GetCurrentTime();
	COleDateTime currDate( currTime.GetYear(), currTime.GetMonth(), currTime.GetDay(), 0,0,0);
	diffPer = obj.CaludateTimePercent( currTime ) - shedule.GetPercent();
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
	case  TaskHistoryContent:
		GetHistoryString( obj.GetId(), text );
		break;
	case  TaskFactTime:
		text = GetFactTimeString( obj );
		break;
	case  TaskPrecent_col: //最新百分比
		{
			text.Format( _T("%03d"), shedule.GetPercent() );
			break;
		}
	case   SCH_UPDATE_DATE:
		if ( userExcOk == ret )
		{
			text = shedule.GetUpdateTimeStr();
		}
		break;
	case  TaskUpcontent://最新进度描述
		{
			if ( userExcOk == ret )
			{
				text = GetScheduleString( shedule );
			}else
			{
				text = _T("无");
			}
			break;
		}
	case  SheduleProjectReview:
		text = obj.GetPrjReview();
		break;
	case SheduleDepartReview:
		text = obj.GetReview();
		break;
	case  PERCENT_DIFF:
		{
			text.Format( _T("%03d"), obj.CaludateTimePercent( currTime ) - shedule.GetPercent() );
		}
		break;
	}
	if ( diffPer > SHOW_EXCEPT_COLOR )
	{
		m_gridCtrl->SetItemBkColour( nRow+GetFixedRowCount(), nColumn + GetFixedColumnCount(), m_diffColor);
	}else
	{
		m_gridCtrl->SetItemBkColour( nRow+GetFixedRowCount(), nColumn + GetFixedColumnCount(), RGB(255, 255, 255) );
	}
//	SetBkColorByTask(nRow, nColumn, obj );
	return text;
}
void      CDepartTaskManger::SetTaskStatus( int taskStatusIndex )
{
	if (  taskStatusIndex > 0 )
	{
		const CTaskStaus obj = m_pTaskSever->GetTaskStausMap();
		const TaskIdStringMap maps = obj.GetTaskStringMap();
		if ( maps.size() <= 0 )
		{
			return;
		}
		int cnt = 0;
		TaskIdStringMap::const_iterator itor = maps.begin();
		advance( itor, taskStatusIndex - 1 );
		if ( itor != maps.end() )
		{
			m_showStatus = itor->first;
		}
	}else if ( 0 == taskStatusIndex )
	{
		m_showStatus = -1;
	}
	
}

void CDepartTaskManger::RefreshShowMap(  )
{
	m_showTaskMap.clear();
	ListTaskItor itor = m_taskList.begin();
	int cnt = 0;
	int index = 0;
	int diff = 0;
	COleDateTime currTime;
	currTime = COleDateTime::GetCurrentTime();
	COleDateTime currDate( currTime.GetYear(), currTime.GetMonth(), currTime.GetDay(), 0,0,0);
	while( itor != m_taskList.end() )
	{
		CSchedule chObj;
		m_pSeverFactory->GetScheduleSever()->GetMaxNewScheduleByTaskId( itor->GetId(), chObj );
		diff = itor->CaludateTimePercent( currDate ) - chObj.GetPercent();
		bool isDiffPer = /*itor->GetTaskStatus() != ongoing ||*/  diff >= m_diffPer;
		if  ( isDiffPer )
		{
			m_showTaskMap.insert( std::make_pair( index++, cnt ) );
		}
		itor++;
		cnt++;
	}
}

CTaskObj* CDepartTaskManger::GetTaskByRow( int nRow )
{
	return CBaseGridManger::GetTaskByRow(nRow, m_taskList, m_showTaskMap );
}


 BOOL    CDepartTaskManger::InitFlexNumEditItem(CGridCellNumeric* pCell,int nRow,int nColumn)
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

void CDepartTaskManger::OnGridClick( NMHDR* pNMHDR,LRESULT* pResult )
{
	NM_GRIDVIEW *gridView = reinterpret_cast<NM_GRIDVIEW*>(pNMHDR);
	int nRow = gridView->iRow - GetFixedRowCount();
}
//导入项目任务
void      CDepartTaskManger::ImportDepartTask(  )
{
	//if ( prj.GetShortName().IsEmpty() )
	//{
	//	AfxMessageBox( _T("项目名为空") );
	//	return;
	//}
	CFileDialog dlg( TRUE,NULL,_T("*.xls"),OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,_T("(*.xls)|*xls") );
	if(IDOK==dlg.DoModal())
	{
		CString path=dlg.GetPathName();
		CString fileName = dlg.GetFileTitle();

		CFile file;
		if(!file.Open(path, CFile::modeRead)/*dataFile==NULL*/)
		{
			AfxMessageBox(_T("打开文件失败！"));
			return;
		}
		file.Close();
		CTaskExcelAction action( m_pSeverFactory );
		CProject prj;
		prj.SetId( -1 );
		BOOL ret = action.ImportTask( path, prj ); 
		if (  ret == userExcOk )
		{//更新任务
			 AfxMessageBox(_T("导入成功"));
			 SetSelDepart( m_showDepartId, m_showStatus, m_selUserId, m_diffPer, m_year, m_quart );
		}else if ( ret != paraError)
		{
			AfxMessageBox( m_pSeverFactory->GetErrorString(ret) );
		}
	}
}
void    CDepartTaskManger::ExportDepartTask( )
{
	CDepartment depart;
	m_pDepartSever->GetDepartById( m_showDepartId, depart);
	if ( depart.GetId() <= 0 )
	{
		return;
	}
	CFileDialog saveFile = CFileDialog(FALSE, _T(".xls"), _T(""), OFN_CREATEPROMPT | OFN_PATHMUSTEXIST ,_T( "(*.xls)|*xls" ));  
	CString defaultFileName = depart.GetName();  
	saveFile.m_ofn.lpstrFile = defaultFileName.GetBuffer(MAX_PATH);  
	saveFile.m_ofn.nMaxFile = MAX_PATH;  
	if(IDOK == saveFile.DoModal())
	{  
		CString path = saveFile.GetPathName();
		CTaskExcelAction action( m_pSeverFactory );
		BOOL ret = action.ExportTask( path, depart.GetName(), m_taskList );
		if ( userExcOk == ret )
		{
			AfxMessageBox(_T("导出成功"));
		}else if ( paraError != ret )
		{
			AfxMessageBox( m_pSeverFactory->GetErrorString(ret) );
		}
	} 
	defaultFileName.ReleaseBuffer();  
}
void   CDepartTaskManger::ExportPerformanTask( int year, int quart, int departId, int selUserId )
{
	CFileDialog saveFile = CFileDialog(FALSE, _T(".xls"), _T(""), OFN_CREATEPROMPT | OFN_PATHMUSTEXIST ,_T( "(*.xls)|*.xls|" ));  
/*	CString defaultFileName = depart.GetName();  
	saveFile.m_ofn.lpstrFile = defaultFileName.GetBuffer(MAX_PATH); */ 
	saveFile.m_ofn.nMaxFile = MAX_PATH;  
	if(IDOK == saveFile.DoModal())
	{
		CString path = saveFile.GetPathName();
		CExportPerformanceTable exportSv(m_pSeverFactory);
		exportSv.ExportPerformanFile( path, year, quart, departId, selUserId );
	}
}
void CDepartTaskManger::OnGridSelChange( NMHDR* pNMHDR,LRESULT* pResult )
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
void CDepartTaskManger::OnGridRclick( NMHDR* pNMHDR,LRESULT* pResult )
{
	if ( FALSE == m_hasReviewPower )
	{
		return;
	}
	NM_GRIDVIEW *gridView = reinterpret_cast<NM_GRIDVIEW*>(pNMHDR);
	int nRow = gridView->iRow - GetFixedRowCount();
	int nColumn = gridView->iColumn - GetFixedColumnCount();
	CPoint pt;
	GetCursorPos( &pt );
	CCellRange range = m_gridCtrl->GetSelectedCellRange();
	if ( range.GetMinRow() >= GetFixedRowCount() && range.GetMinRow() == range.GetMaxRow() &&
		GetFixedColumnCount() == range.GetMinCol() && (range.GetMaxCol() == m_gridColumnMap.size())  )
	{//-1去掉操作按钮不能选择， 删除
		CTaskObj *pTaskObj =GetTaskByRow( range.GetMinRow() - GetFixedRowCount() );
		if ( pTaskObj  )
		{
			if (  ongoing == pTaskObj->GetTaskStatus() )
			{
				/*CUser ownerObj;  
				CDepartment depart;
				m_pSeverFactory->GetLoginServer()->GetUserById( pTaskObj->GetOwnerId(), ownerObj );*/
				//if ( ownerObj.GetDepartId() > 0 )
				//{
				//	m_pDepartSever->GetDepartById( ownerObj.GetDepartId(), depart );
				//	if ( depart.GetMangerId() == m_pSeverFactory->GetLoginServer()->GetLoginUser().GetDepartId() )
				//	{
				CString str;
				CMenu PopupMenu;
				PopupMenu.CreatePopupMenu();
				str.LoadString( IDS_STRING_REVIEW );
				PopupMenu.AppendMenu(MF_ENABLED, IDM_TASK_DEPART_REVIEW, str);
				PopupMenu.TrackPopupMenu(TPM_LEFTALIGN |TPM_RIGHTBUTTON , pt.x, pt.y, m_gridCtrl->GetParent());
				//}
				//}
			}else if ( task_close == pTaskObj->GetTaskStatus() || midStop == pTaskObj->GetTaskStatus() )
			{
				CUser ownerObj;  
				CDepartment depart;
				m_pSeverFactory->GetLoginServer()->GetUserById( pTaskObj->GetOwnerId(), ownerObj );
				if ( ownerObj.GetDepartId() > 0 )
				{
					m_pDepartSever->GetDepartById( ownerObj.GetDepartId(), depart );
					if ( depart.GetMangerId() == m_pSeverFactory->GetLoginServer()->GetLoginUser().GetId() )
					{
						CString str;
						CMenu PopupMenu;
						PopupMenu.CreatePopupMenu();
						str.LoadString( IDS_STR_MODIY_SCORE );
						PopupMenu.AppendMenu(MF_ENABLED, IDM_TASK_MOIDY_SCORE, str);
						PopupMenu.TrackPopupMenu(TPM_LEFTALIGN |TPM_RIGHTBUTTON , pt.x, pt.y, m_gridCtrl->GetParent());
					}
				}
			}
			
			
		}
	}
}
void CDepartTaskManger::OnGridDepartReview()
{
	CCellRange range = m_gridCtrl->GetSelectedCellRange();
	if ( range.GetMinRow() >= GetFixedRowCount() && range.GetMinRow() == range.GetMaxRow() &&
		GetFixedColumnCount() == range.GetMinCol() && (range.GetMaxCol() == m_gridColumnMap.size())  )
	{
		CTaskObj *pTaskObj =GetTaskByRow( range.GetMinRow() - GetFixedRowCount() );
		if ( pTaskObj && ongoing == pTaskObj->GetTaskStatus() )
		{
			CSchedule schObj;
			m_pSeverFactory->GetScheduleSever()->GetMaxNewScheduleByTaskId( pTaskObj->GetId(), schObj );
			CString newSch;
			if ( schObj.GetId() > 0 )
			{
				newSch = GetScheduleString( schObj );
			}
			DepartProcessReview dlg( *pTaskObj, newSch );
			if ( IDOK == dlg.DoModal() )
			{
				/*	m_outputShowId = pTaskObj->GetId();
				SendMessage( AfxGetMainWnd()->GetSafeHwnd(), MAINFRAME_MSG_TASKID, m_outputShowId, NULL );*/
				m_pSeverFactory->GetTaskSever()->GetTaskByTaskId( pTaskObj->GetId(), *pTaskObj );
				UpdateGridRowView( range.GetMinRow() - GetFixedRowCount() );
			}
		}
	}
}
void CDepartTaskManger::OnGridDepartMoidyScore()
{
	CCellRange range = m_gridCtrl->GetSelectedCellRange();
	if ( range.GetMinRow() >= GetFixedRowCount() && range.GetMinRow() == range.GetMaxRow() &&
		GetFixedColumnCount() == range.GetMinCol() && (range.GetMaxCol() == m_gridColumnMap.size())  )
	{
		CTaskObj *pTaskObj =GetTaskByRow( range.GetMinRow() - GetFixedRowCount() );
		if ( pTaskObj && ( task_close == pTaskObj->GetTaskStatus() || midStop == pTaskObj->GetTaskStatus() ) )
		{
			CModiyTaskScoreDlg dlg( *pTaskObj);
			if ( IDOK == dlg.DoModal() )
			{
								m_pSeverFactory->GetTaskSever()->GetTaskByTaskId( pTaskObj->GetId(), *pTaskObj );
				UpdateGridRowView( range.GetMinRow() - GetFixedRowCount() );
			}
			
		}
	}
}