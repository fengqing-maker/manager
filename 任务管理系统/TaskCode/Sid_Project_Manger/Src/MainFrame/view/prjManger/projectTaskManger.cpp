#include "stdafx.h"
#include "projectTaskManger.h"
#include "Src/MainFrame/MainFrm.h"
#include "Src/MainFrame/messageDef.h"
#include "Src/action/ExcelAction/CTaskExcelAction.h"
#include "Src/MainFrame/Dlg/review/ProjectProcessReviewDlg.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

CProjectTaskManger::CProjectTaskManger( CActionServerFactory *pSeverFactory ):CPrjBaseGridManger( pSeverFactory )
{
	 m_showPrj = 0;
	 m_showStatus = ongoing;
	 m_fatherColor = RGB( 173, 185, 205 );
	 m_extentAllTask = FALSE;
	 m_hasChildCount = 0;
     m_selUserId = 0;
	 m_diffPer = -100;
	 m_hasReviewPower = FALSE;
	 m_isOnlyShow = FALSE;
	 m_pLoginUser = &(m_pSeverFactory->GetLoginServer()->GetLoginUser());
}
//任务表格
void CProjectTaskManger::CreateColumnMap()
{
	InsertTaskNameColumn();
	InsertProjectTypeColum();
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
	InsertDatePercent();
	//InsertSuggestionColumn();
	//InsertAgreeBtnColum();
	//InsertUnAgreeBtnColum();
}
void      CProjectTaskManger::SetDefProject(  int prjId, int taskST )
{
	m_showPrj = prjId;
	m_showStatus = taskST;
}

void CProjectTaskManger::SetSelProject( int prjId,int taskStatusIndex, int selUserId, int diffPer, BOOL onlyShowQuality/* = false*/ )
{
	((CMainFrame *)AfxGetMainWnd())->ShowProgressMsg( MSG_READ_TASK_DATA );
	bool isModiy = prjId != m_showPrj;
	m_showPrj = prjId;
	if ( !isModiy )
	{
		isModiy = m_selUserId != selUserId;
	}
	m_selUserId = selUserId;
	bool diffMody = m_diffPer != diffPer;
	m_diffPer = diffPer;
	int taskSt = m_showStatus;
	SetTaskStatus( taskStatusIndex ) ;
	if ( !isModiy )
	{
		isModiy = m_showStatus != taskSt;
	}
	if( !isModiy )
	{
		isModiy = m_isOnlyShow != onlyShowQuality;
	}
	m_isOnlyShow = onlyShowQuality;
	if ( isModiy || m_taskList.size() <= 0 )
	{
		UpdateDbData();
	}
	RefreshShowMap();
	UpdateGridView();
	((CMainFrame *)AfxGetMainWnd())->CloseMsgDlg( );
}
void CProjectTaskManger::ResetChildInfo( int beginRow, int childCount )
{
	ListTaskItor itor = m_taskList.begin();
	advance( itor, beginRow );
	int cnt = beginRow;
	for ( ; itor != m_taskList.end(); itor++ )
	{
		m_childInfo[itor->GetId()].showIndex = cnt;
		if ( (cnt-beginRow) < childCount )
		{
			m_childInfo[itor->GetId()].hasChild = true;
		}else
		{
			m_childInfo[itor->GetId()].hasChild = false;
		}
		cnt++;
	}
}
void CProjectTaskManger::UpdateDbData()
{
	m_taskList.clear();
	m_childInfo.clear();
	m_hasChildCount = 0;
	int cnt = 0;
	if ( m_showPrj > 0 )
	{
		m_pProjectSever->GetProjectTaskNoFather( m_showPrj, m_showStatus, m_selUserId, m_taskList,  m_hasChildCount, m_isOnlyShow );
		ResetChildInfo(  cnt, m_hasChildCount );
	}else
	{
		const ListProject& prj = GetProjectList();
		int taskSize = m_taskList.size();
		if ( taskSize > 1 )
		{
			m_pProjectSever->GetProjectTaskNoFather( prj.begin()->GetId(), m_showStatus, m_selUserId, m_taskList,  m_hasChildCount, m_isOnlyShow );
			ResetChildInfo(  cnt, m_hasChildCount );
		}else
		{
			for ( ListProject::const_iterator itor = prj.begin(); itor != prj.end(); itor++ )
			{
				int childCount = 0;
				m_pProjectSever->GetProjectTaskNoFather( itor->GetId(), m_showStatus,m_selUserId, m_taskList,  childCount, m_isOnlyShow );
				if ( taskSize != m_taskList.size() )
				{
					ResetChildInfo(  cnt, childCount );
					m_hasChildCount += childCount;
					taskSize = m_taskList.size();
					cnt = taskSize;
				}
			}
		}
	
	}
	


	//UpdateGridView();
}

//列表内容
void CProjectTaskManger::CreateRowItemMap()
{
	if ( m_pSeverFactory->GetLoginServer()->FindLoginPower( projectReview ) )
	{
		m_hasReviewPower = TRUE;
	}
	UpdateProject();
	if ( m_allProject.size() > 0 )
	{
		m_showPrj = m_allProject.begin()->GetId();
	}
	UpdateDbData();
	RefreshShowMap();   
}

int CProjectTaskManger::GetFlexRowCount()
{
	return m_showTaskMap.size();
}
void CProjectTaskManger::GetTaskShowNum( int taskId, CString &text )
{
	ShowInfoMap::iterator itor = m_childInfo.find( taskId );
	if ( itor!= m_childInfo.end()  )
	{
		CString str;
		str.Format( _T("%d"), itor->second.showIndex );
		text += str + _T(".");
		if ( itor->second.fatherId > 0 )
		{
			GetTaskShowNum(  itor->second.fatherId, text );
		}
	}
}
CString CProjectTaskManger::GetFixedItemText(int nRow, int nColumn)
{
	CString text;
	int fixRowCnt = GetFixedRowCount();
    if( nColumn != GetFixedColumnCount() -1 || nRow < fixRowCnt-1 )
	{
		text = CPrjBaseGridManger::GetFixedItemText( nRow,  nColumn);
		return text;
	}
	if ( nRow == fixRowCnt - 1 && m_hasChildCount > 0 )
	{
		if ( m_extentAllTask )
		{
			text = _T("-");
		}else
		{
			text = _T("+");
		}
	}else if ( nRow >= fixRowCnt )
	{
		CTaskObj *pObj = GetTaskByRow( nRow - GetFixedRowCount() );
		if ( NULL == pObj )
		{
			return text;
		}
		const CTaskObj &obj = *pObj;
		ShowInfoMap::iterator itor = m_childInfo.find( obj.GetId() );
		if ( itor!= m_childInfo.end()  )
		{
			GetTaskShowNum( itor->first, text );
			if ( text.GetLength() > 0 )
			{
				text.Delete( text.GetLength() - 1);
				CString orginText = text;
				text = _T("");
				int pos = 0;
				while( (pos = orginText.ReverseFind( _T('.') )) > 0 )
				{
					int rPos = orginText.GetLength() - pos - 1;
					text += orginText.Right( rPos );
					text += _T(".");
					orginText = orginText.Left( pos );
				}
				text += orginText;
			}
			COLORREF back;
			if ( itor->second.hasChild )
			{
				text += itor->second.showChild ? _T("-"):_T("+");
			    back = m_fatherColor;
			}else if ( itor->second.fatherId > 0 )
			{
				 back = RGB( 238, 243, 250);
			}else
			{
				 back = RGB( 255, 255, 255 );
			}
			if ( nColumn = GetFixedColumnCount() )
			{
				int maxColum = m_gridColumnMap.size() + nColumn;
				for ( int i = nColumn; i < maxColum;i++)
				{
					m_gridCtrl->SetItemBkColour(nRow, i,back );
				}
			}
		}
	}else
	{
		
	}
	return text;
}
//获取显示
CString CProjectTaskManger::GetFlexItemText(int nRow, int nColumn)
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
	COleDateTime currTime= COleDateTime::GetCurrentTime();
	COleDateTime currDate( currTime.GetYear(), currTime.GetMonth(), currTime.GetDay(), 0,0,0);
	int diffPer = obj.CaludateTimePercent( currTime ) - shedule.GetPercent();;
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
			text.Format( _T("%03d"), shedule.GetPercent() );
			break;
	case   SCH_UPDATE_DATE:
		 if ( userExcOk == ret )
		 {
		 	text = shedule.GetUpdateTimeStr();
		 }
		 break;
	case  TaskUpcontent://最新进度描述
		if ( userExcOk == ret )
		{
			text = GetScheduleString( shedule );
		}else
		{
			text = _T("无");
		}
		break;
	case  PERCENT_DIFF:
		text.Format( _T("%03d"), diffPer);
		break;
	}
	if ( diffPer > SHOW_EXCEPT_COLOR )
	{
		m_gridCtrl->SetItemBkColour( nRow+GetFixedRowCount(), nColumn + GetFixedColumnCount(), m_diffColor);
	}
//	SetBkColorByTask(nRow, nColumn, obj );
	return text;
}
void      CProjectTaskManger::SetTaskStatus( int taskStatusIndex )
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

void CProjectTaskManger::RefreshShowMap(  )
{
	m_showTaskMap.clear();
	ListTaskItor itor = m_taskList.begin();
	int cnt = 0;
	int index = 0;
	COleDateTime currTime;
	currTime = COleDateTime::GetCurrentTime();
	COleDateTime currDate( currTime.GetYear(), currTime.GetMonth(), currTime.GetDay(), 0,0,0);
	while( itor != m_taskList.end() )
	{
		CSchedule chObj;
		m_pSeverFactory->GetScheduleSever()->GetMaxNewScheduleByTaskId( itor->GetId(), chObj );
		int diff = itor->CaludateTimePercent( currDate ) - chObj.GetPercent();
		bool isDiffPer = /*itor->GetTaskStatus() != ongoing ||*/  diff >= m_diffPer;
		if ( 
			/*(m_selUserId <= 0 || m_selUserId == itor->GetOwnerId() ) && */isDiffPer && m_childInfo[itor->GetId()].isShow )
		{
			m_showTaskMap.insert( std::make_pair( index++, cnt ) );
			m_childInfo[ itor->GetId() ].isShow = true;
		}else
		{
			m_childInfo[ itor->GetId() ].isShow = false;
		}
		itor++;
		cnt++;
	}
	
}

CTaskObj* CProjectTaskManger::GetTaskByRow( int nRow )
{
	return CBaseGridManger::GetTaskByRow(nRow, m_taskList, m_showTaskMap );
}


 BOOL    CProjectTaskManger::InitFlexNumEditItem(CGridCellNumeric* pCell,int nRow,int nColumn)
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
BOOL CProjectTaskManger::AddShowChild( ShowInfoMap::iterator &fahterItor, BOOL updateGrid /*= TRUE*/ )
{
	if ( false == fahterItor->second.isLoadChild )
	{
		ListTask lstChild;
		BOOL ret = m_pProjectSever->GetChildTask( fahterItor->first, m_showStatus, m_selUserId, lstChild );
		if ( ret != userExcOk )
		{
			AfxMessageBox( m_pSeverFactory->GetErrorString( ret) );
			return FALSE;
		}
		TaskChildInfo showInf;
		showInf.fatherId = fahterItor->first;
		int cntNum = 0;
		ListTask::iterator itorTmp = m_taskList.begin();
		ListTask::iterator itorTmp2 = m_taskList.begin();
		ListTask::iterator insertPos = m_taskList.begin();
		ListTask  tmpChild = lstChild;
		while( itorTmp != m_taskList.end() )
		{
            if ( itorTmp->GetId() == fahterItor->first )
            {
				insertPos = itorTmp;
				insertPos++;
            }
			ListTask::iterator itorJ;
			for ( itorJ = tmpChild.begin(); itorJ != tmpChild.end(); itorJ++ )
			{
				if ( itorTmp->GetId() == itorJ->GetId() )
				{
					break;
				}
			}
			if ( itorJ != tmpChild.end() )
			{//有重复任务 删除
				m_taskList.erase( itorTmp );
				itorTmp = itorTmp2;
			}else
			{
				itorTmp2 = itorTmp;
				itorTmp++;
			}
		}
		for ( ListTaskItor itor = lstChild.begin(); itor != lstChild.end(); itor++)
		{
			m_taskList.insert( insertPos, *itor);
			BOOL hasChild = FALSE;
			m_pProjectSever->IsHasChildTask( itor->GetId(), showInf.hasChild);
			showInf.showIndex = cntNum;
			m_childInfo.insert( std::make_pair(itor->GetId(), showInf) );
			cntNum++;
			//insertPos++;
		}
		fahterItor->second.isLoadChild = TRUE;
	}else
	{
		for ( ShowInfoMap::iterator itor = m_childInfo.begin(); itor != m_childInfo.end(); itor++ )
		{
			if ( itor->second.fatherId == fahterItor->first )
			{
				itor->second.isShow = true;
			}
		}
	}
	fahterItor->second.showChild = true;
	RefreshShowMap();
	if ( updateGrid )
	{
		UpdateGridView();
	}
	return TRUE;
}
//隐藏孩子
BOOL CProjectTaskManger::ShowHideChild(  ShowInfoMap::iterator &fahterItor, BOOL updateGrid /*= TRUE*/ )
{
	//if ( false == fahterItor->second.isLoadChild )
	//{
		for ( ShowInfoMap::iterator itor = m_childInfo.begin(); itor != m_childInfo.end(); itor++ )
		{
			if ( itor->second.fatherId == fahterItor->first )
			{
				itor->second.isShow = false;
			}
		}
		fahterItor->second.showChild = false;
		RefreshShowMap();
		if ( updateGrid )
		{
			UpdateGridView();
		}
		return TRUE;
	//}
	//return FALSE;
}
void CProjectTaskManger::ExtentShowAllChild( int taskId, BOOL isShow, BOOL updateGrid /*= TRUE*/ )
{
	ShowInfoMap::iterator itor = m_childInfo.find( taskId);
	if ( itor != m_childInfo.end() )
	{//有子孩子
		if ( TRUE == itor->second.hasChild )
		{
			if ( (bool)isShow != itor->second.showChild )
			{
				if ( isShow )
				{
				   AddShowChild( itor, FALSE ) ;
				}else
				{
					ShowHideChild( itor, FALSE );
				}
			}
			
			for ( itor = m_childInfo.begin(); itor != m_childInfo.end(); itor++ )
			{
				if ( true == itor->second.showChild && taskId == itor->second.fatherId 
					 &&  (bool)isShow != itor->second.showChild )
				{
					ExtentShowAllChild( itor->first,  isShow );
				}
			}
			if ( updateGrid )
			{
				//
				UpdateGridView();
			}
		}
		
	}
}
void CProjectTaskManger::OnGridClick( NMHDR* pNMHDR,LRESULT* pResult )
{
	NM_GRIDVIEW *gridView = reinterpret_cast<NM_GRIDVIEW*>(pNMHDR);
	int fixRowCnt = GetFixedRowCount();
	int nRow = gridView->iRow - fixRowCnt;
	int fixColumnCnt = GetFixedColumnCount();
	if ( gridView->iColumn != (fixColumnCnt - 1) )
	{
		return;
	}
	if ( gridView->iRow == fixRowCnt-1 && m_hasChildCount > 0 )
	{//扩展或收缩所有的
		((CMainFrame *)AfxGetMainWnd())->ShowProgressMsg( MSG_READ_TASK_DATA );
		if ( FALSE ==  m_extentAllTask )
		{//展开所有子任务
			m_extentAllTask = TRUE;
		}else
		{//隐藏所有子任务
			m_extentAllTask = FALSE;
		}
		ListInt lst;
		for ( IntToIntMap::const_iterator itor = m_showTaskMap.begin(); itor != m_showTaskMap.end(); itor++ )
		{
			CTaskObj* pTask = GetTaskByRow( itor->first );
			if ( pTask )
			{
				lst.push_back( pTask->GetId() );
			}
		}
		for ( ListIntItor itor = lst.begin(); itor != lst.end(); itor++ )
		{
			ExtentShowAllChild( *itor,  m_extentAllTask, FALSE );
		}
		UpdateGridView();
		((CMainFrame *)AfxGetMainWnd())->CloseMsgDlg();
	}else if ( nRow >= 0 ) 
	{
		CTaskObj* pTask = GetTaskByRow( nRow );
		 ShowInfoMap::iterator itor = m_childInfo.find( pTask->GetId() );
		 if ( pTask && itor != m_childInfo.end())
		 {
			if ( itor->second.hasChild )
			{
				if ( true == itor->second.showChild )
				{
					 ShowHideChild(itor);
				}else
				{
					 AddShowChild( itor) ;
				}
			}
		}
	}
	::SendMessage( m_gridCtrl->GetParent()->GetParent()->GetParent()->GetSafeHwnd(), 
		PRJ_VIEW_MSG_CHANGE_TASKLIST, (WPARAM)(GetTaskListPoint()), NULL );
}
//导入项目任务
void      CProjectTaskManger::ImportProjectTask( const CProject &prj )
{
	if ( prj.GetShortName().IsEmpty() )
	{
		AfxMessageBox( _T("项目名为空") );
		return;
	}
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
		BOOL ret = action.ImportTask( path, prj );
		if (  ret == userExcOk )
		{//更新任务
			 AfxMessageBox(_T("导入成功"));
			 SetSelProject( prj.GetId(), m_showStatus, m_selUserId, m_diffPer );
		}else if ( ret != paraError)
		{
			AfxMessageBox( m_pSeverFactory->GetErrorString(ret) );
		}
	}
}
void    CProjectTaskManger::ExportProjectTask( const CProject &prj )
{
	CFileDialog saveFile = CFileDialog(FALSE, _T(".xls"), _T(""), OFN_CREATEPROMPT | OFN_PATHMUSTEXIST ,_T( "(*.xls)|*xls" ));  
	CString defaultFileName = prj.GetShortName();  
	saveFile.m_ofn.lpstrFile = defaultFileName.GetBuffer(MAX_PATH);  
	saveFile.m_ofn.nMaxFile = MAX_PATH;  
	if(IDOK == saveFile.DoModal())
	{  
		CString path = saveFile.GetPathName();
		CFileFind fileFinder;
		CTaskExcelAction action( m_pSeverFactory );
		BOOL ret = action.ExportTask( path, prj.GetShortName(), m_taskList );
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
void CProjectTaskManger::OnGridSelChange( NMHDR* pNMHDR,LRESULT* pResult )
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
void CProjectTaskManger::OnGridRclick( NMHDR* pNMHDR,LRESULT* pResult )
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
	{//
		CTaskObj *pTaskObj =GetTaskByRow( range.GetMinRow() - GetFixedRowCount() );
		if ( pTaskObj && ongoing == pTaskObj->GetTaskStatus() )
		{
			CProject project;
			m_pProjectSever->GetProjectById( pTaskObj->GetProjctId(), project );
			if ( project.GetMangerId() == m_pLoginUser->GetId() )
			{
				CString str;
				CMenu PopupMenu;
				PopupMenu.CreatePopupMenu();
				str.LoadString( IDS_STRING_REVIEW );
				PopupMenu.AppendMenu(MF_ENABLED, IDM_TASK_PROJECT_REVIEW, str);
				PopupMenu.TrackPopupMenu(TPM_LEFTALIGN |TPM_RIGHTBUTTON , pt.x, pt.y, m_gridCtrl->GetParent());
			}
		}
	}
}
void CProjectTaskManger::OnGridProjectReview()
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
			CProjectProcessReviewDlg dlg( *pTaskObj, newSch ); 
			if ( IDOK == dlg.DoModal() )
			{
				m_outputShowId = pTaskObj->GetId();
				SendMessage( AfxGetMainWnd()->GetSafeHwnd(), MAINFRAME_MSG_TASKID, m_outputShowId, NULL );
				m_pSeverFactory->GetTaskSever()->GetTaskByTaskId( pTaskObj->GetId(), *pTaskObj );
				UpdateGridRowView( range.GetMinRow() - GetFixedRowCount() );
			}
		}
	}
}