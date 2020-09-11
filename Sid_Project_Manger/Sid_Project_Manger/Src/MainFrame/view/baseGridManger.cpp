#include "stdafx.h"
#include "baseGridManger.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

CBaseGridManger::CBaseGridManger(  CActionServerFactory *pSeverFactory )
{
	ASSERT( pSeverFactory ); 
	m_pSeverFactory = pSeverFactory;
	m_pTaskSever = m_pSeverFactory->GetTaskSever();

	m_newingColor = RGB( 255, 232, 255); //粉红
	m_waitPublishColor = RGB( 255, 255, 210); //浅黄
	m_waitStartColor = RGB( 221, 238, 255 ); //浅蓝
	m_otherColor = RGB( 255, 255, 255 );    //白色
	m_goingonColor = RGB( 232, 255, 232 ); //浅绿
	m_finishColor = RGB( 128, 255, 128 ); //绿
	m_diffColor = RGB(255, 236, 236 ); //浅红
	m_overdueColor = RGB(255, 45, 45 ); //深红
	m_outputShowId = -1;
}
//////////////////////////////////////////////////////////////
//重写
void CBaseGridManger::PreCreateGridView()
{
	CreateColumnMap();
	CreateRowItemMap();
}
//
void CBaseGridManger::CreateColumnMap()
{
	InsertProjectTypeColum();
    InsertTaskNameColumn();
	InsertTaskStatusColumn();
	InsertTaskOwernColum();
	InsertTaskPercentColumn(); //最新进度
	InsertMaxPerceneColumn();  //更新的进度
	InsertTaskUpdateColumn();
	//InsertTaskHistoryUpdateColumn();
	InsertTaskWorkHoursColumn();
	InsertColumn( _T("工时类型▼"), 90, DT_LEFT, GridItemType_Combo,TaskTimeType );
	InsertTaskPlanColumn();
	InsertTaskFactTimeColumn();
	InsertDatePercent();
}
void CBaseGridManger::AftCreateGridView()
{
	if (m_gridCtrl != NULL)
	{	
		m_gridCtrl->EnableSelection(TRUE);				/* 允许选择表项 */
		m_gridCtrl->SetFixedRowSelection(TRUE);			/* 允许整行选择 */
		m_gridCtrl->SetFixedColumnSelection(TRUE);		/* 允许整列选择 */	
		/* 设置列宽 */
		GridColumnMapItor itor = m_gridColumnMap.begin();
		while(itor != m_gridColumnMap.end())
		{
			int columnIndex = itor->first+m_fixedColumnCount;
			int nWidth = itor->second->width;
			m_gridCtrl->SetColumnWidth(columnIndex,nWidth);
			itor++;
		}
	    //m_gridCtrl->AutoSizeRows();  //111111111111
		//m_gridCtrl->AutoSize(); //lhz add
	}
}
void     CBaseGridManger::AftUpdateGridView()
{
	AftCreateGridView();
}
CString CBaseGridManger::GetFixedItemText(int nRow, int nColumn)
{
	CString text;
	if ( nRow == GetFixedColumnCount() && nColumn >0 )
	{
		int index = nColumn-m_fixedColumnCount;	/* 可动表项的第一列索引 */
		GridColumnMapItor itor = m_gridColumnMap.find(index);
		if (itor != m_gridColumnMap.end())
		{
			text = itor->second->title;
		}
		return text;
	}
	else
	{
		text =  AbstractGridViewManager::GetFixedItemText(nRow,nColumn);
	}
	return text;
}
//////////////////////////////////////////////////////////////
//
void CBaseGridManger::InsertUniqueColumn(GridColumnPtr columnInfo)
{
	GridColumnMapItor itor = m_gridColumnMap.begin();
	while (itor != m_gridColumnMap.end())
	{
		if (itor->second->title == columnInfo->title)
		{
			return;
		}
		itor++;
	}
	m_gridColumnMap.insert(std::make_pair(m_gridColumnMap.size(),columnInfo));
}
//
void CBaseGridManger::InsertTaskNameColumn()
{
	//InsertRichEditColumn(  _T("任务名称"), 80, DT_LEFT, TaskName );
	//InsertEditColumn(  _T("任务名称"), 210, DT_LEFT, TaskName );
	InsertColumn( _T("任务名称▼"), 250, DT_LEFT, GridItemType_MutilEdit,  TaskName_col );
}
void CBaseGridManger::InsertTaskStatusColumn()
{
	InsertEditColumn(  _T("任务状态"), 80, DT_LEFT,TaskStatus_col );
}
void CBaseGridManger::InsertTaskPrjStep()
{
	InsertEditColumn(  _T("任务所属项目阶段"), 110, DT_LEFT, taskBelongPrjStep );
}

void CBaseGridManger::InsertTaskPercentColumn()
{//指最新进度
	InsertNumEditColumn(   _T("最新进度(%)")/*_T("更新的进度(%)▼")*//*_T("任务进度▼")*/, 68, DT_LEFT,TaskPrecent_col );
}
void CBaseGridManger::InsertMaxPerceneColumn()
{
	InsertNumEditColumn(  _T("更新进度(%)▼")/*_T("任务进度▼")*/, 68, DT_LEFT,TaskUpdatePrecent );
}
void CBaseGridManger::InsertTaskUpdateColumn()
{//指当前描述
	InsertColumn(_T("更新描述▼"), 250, DT_LEFT, GridItemType_MutilEdit,  TaskUpcontent );
}
void CBaseGridManger::InsertTaskHistoryUpdateColumn()
{//历史描述
	//InsertRichEditColumn(  _T("历史更新"), 210, DT_LEFT,TaskHistoryContent );
	InsertColumn( _T("历史更新"), 250, DT_LEFT, GridItemType_MutilEdit,  TaskHistoryContent );
}
void CBaseGridManger::InsertTaskWorkHoursColumn()
{//指此次更新的工时，
	InsertNumEditColumn(  _T("工时(h)▼"), 60, DT_LEFT,TaskWorkHour );
}

void CBaseGridManger::InsertTaskPlanColumn()
{//计划工时
	//InsertEditColumn(  _T("计划时间"), 200, DT_LEFT,TaskPlanTime );
	InsertColumn( _T("计划时间"), 135, DT_LEFT, GridItemType_MutilEdit,  TaskPlanTime );
}

void CBaseGridManger::InsertTaskFactTimeColumn()
{//实际工时
	//InsertEditColumn(  _T("实际时间"), 200, DT_LEFT, TaskFactTime );
	InsertColumn( _T("实际时间"), 135, DT_LEFT, GridItemType_MutilEdit,  TaskFactTime );
}
//插入更新按钮
void CBaseGridManger::InsertUpDownColum( )
{
	InsertColumn(_T("操作▼"), 65,  DT_CENTER, GridItemType_Button, TaskOperate1);
}

void CBaseGridManger::InsertProjectTypeColum()
{//项目
	InsertEditColumn(  _T("涉及项目"), 90, DT_LEFT,ProjectType_Colom );
}
void CBaseGridManger::InsertWorkTimeTypeColum()
{
	InsertEditColumn(  _T("工时类型"), 90, DT_LEFT,TaskTimeType );
}
void CBaseGridManger::InsertTaskOwernColum()
{//责任人
	InsertEditColumn(  _T("负责人"), 60, DT_LEFT,taskOwern );
}
void CBaseGridManger::InsertDiffParaColum()
{//难度系数▼
	InsertNumEditColumn(  _T("难度系数▼"), 80, DT_LEFT, DiffPara_Colom );
}
void CBaseGridManger::InsertSuggestionColumn()
{//评审意见
	InsertColumn( _T("意见▼"), 200, DT_LEFT, GridItemType_MutilEdit,  SuggestionColumn );
}
void CBaseGridManger::InsertAgreeBtnColum()
{//同样按键
	InsertColumn(  _T("同意▼"), 60,  DT_CENTER, GridItemType_Button, TaskOperate1);
}
void CBaseGridManger::InsertUnAgreeBtnColum()
{//不同样按键
	InsertColumn(  _T("不同意▼"), 60,  DT_CENTER, GridItemType_Button, TaskOperate2);
}
void CBaseGridManger::InsertReviewUpateColum()
{
	InsertColumn( _T("点评的进度"), 200, DT_LEFT, GridItemType_MutilEdit,  ReviewCurrtentUpdate );
}
void CBaseGridManger::InsertProjectReview()
{//项目点评
	InsertColumn( _T("项目过程点评"), 200, DT_LEFT, GridItemType_MutilEdit,  SheduleProjectReview );
}

void CBaseGridManger::InsertScoreColumn()
{//进度评分
	InsertNumEditColumn(  _T("最终评分"), 50, DT_LEFT,ScoreColum );
}
void CBaseGridManger::InsertForcastScore()
{//系统预测分
	InsertNumEditColumn(  _T("经理评分▼"), 60, DT_LEFT, ForcostScoreColum );
}
void CBaseGridManger::InsertUnUpdateCnt()
{
	InsertEditColumn(  _T("漏更新次数"), 60, DT_LEFT, leaveUpdateCnt );
}
void CBaseGridManger::InsertDepartReview()
{//评分说明
	InsertColumn( _T("部门过程点评"), 120, DT_LEFT, GridItemType_MutilEdit,  SheduleDepartReview );
}
void CBaseGridManger::InsertChangeTime()
{//变更时间
	InsertEditColumn(  _T("申请时间"), 130, DT_LEFT, ChangeAppTime );
}
void CBaseGridManger::InsertChangeStatus()
{
	InsertEditColumn(  _T("变更状态"), 120, DT_LEFT, ChangeStatus );
}
void CBaseGridManger::InsertChangeReson()
{//变更原因
	InsertColumn( _T("变更原因"), 200, DT_LEFT, GridItemType_MutilEdit,  ChangeReson );
}
void CBaseGridManger::InsertChangeContent()
{//变更内容
	InsertColumn( _T("变更内容"), 240, DT_LEFT, GridItemType_MutilEdit,  ChangeContent );
}
void CBaseGridManger::InsertDatePercent()
{
	InsertColumn( _T("偏差"), 40, DT_LEFT, GridItemType_Edit,  PERCENT_DIFF  );
}
void CBaseGridManger::InsertSchduleDate()
{
	InsertColumn( _T("进度日期"), 80, DT_LEFT, GridItemType_Edit,  SCH_UPDATE_DATE  );
}
void  CBaseGridManger::InsertNumEditColumn( const CString &str, int width, int alignemnt, int dataType )
{
	InsertColumn( str, width,  alignemnt, GridItemType_NumEidt, dataType  );
}
void  CBaseGridManger::InsertRichEditColumn( const CString &str, int width, int alignemnt, int dataType )
{
	InsertColumn( str, width,  alignemnt, GridItemType_RichEidt, dataType  );
}
void   CBaseGridManger::InsertEditColumn( const CString &str, int width, int alignemnt, int dataType )
{
	InsertColumn( str, width,  alignemnt, GridItemType_Edit, dataType  );
}
void CBaseGridManger::InsertCheckColumn( const CString &str, int alignemnt, int dataType )
{
	InsertColumn( str, 25,  alignemnt, GridItemType_Check, dataType  );
}
void  CBaseGridManger::InsertDateColumn( const CString &str, int width, int alignemnt, int dataType )
{
	InsertColumn( str, width,  alignemnt, GridItemType_DateTime, dataType );
}
void CBaseGridManger::InsertColumn( const CString &str, int width, int alignemnt, int type, int dataType )
{
	GridColumnPtr columnInfo(new GridColumnInfo());
	columnInfo->title = str;
	columnInfo->type = type;
	columnInfo->width = width;
	columnInfo->alignment = alignemnt;
	columnInfo->dataType = dataType;
	InsertUniqueColumn(columnInfo);
}
GridItemType CBaseGridManger::GetFlexItemType(int nRow,int nColumn)
{
	 GridColumnMapItor itor = m_gridColumnMap.find( nColumn );
	 if ( itor != m_gridColumnMap.end())
	 {
		 return (GridItemType)itor->second->type;
	 }else
	 {
		 return AbstractGridViewManager::GetFlexItemType(nRow, nColumn);
	 }
}
 void CBaseGridManger::GetHistoryString( CTaskSeverInc *pSever, int taskId, CString &str )
{
	ASSERT( pSever );
	ListSchedule listSch;
	pSever->GetTaskScheudle( taskId, listSch);
	ListScheduleItor itor = listSch.begin();
	while( itor != listSch.end())
	{
		CString schStr;
	/*	CString strPrj = itor->GetPrjReview();
		CString strDepart = itor->GetReview();
		if ( strPrj.GetLength() <= 0 )
		{
			strPrj = _T("无");
		}
		if ( strDepart.GetLength() <= 0 )
		{
			strDepart = _T("无");
		}*/
		schStr.Format(_T(" T:%s P:%03d%%,%03.1fh DC:%s \r\n"), /*项目点评:%s 部门点评:%s 评分:%d*/
			itor->GetUpdateTimeStr(), itor->GetPercent(), itor->GetWorkHours(), itor->GetUpdateContent()
			/*,strPrj, strDepart, itor->GetScore()*/ );
		itor++;
		if ( itor != listSch.end() )
		{
			schStr += _T(".....................................................................\r\n");
		}

		str += schStr;

	}
}
void CBaseGridManger::GetHistoryString( CTaskSeverInc *pSever, int taskId, std::list<CString> &strLst )
{
	ASSERT( pSever );
	ListSchedule listSch;
	pSever->GetTaskScheudle( taskId, listSch);
	ListScheduleItor itor = listSch.begin();
	while( itor != listSch.end())
	{
		CString schStr;
	/*	CString strPrj = itor->GetPrjReview();
		CString strDepart = itor->GetReview();
		if ( strPrj.GetLength() <= 0 )
		{
			strPrj = _T("无");
		}
		if ( strDepart.GetLength() <= 0 )
		{
			strDepart = _T("无");
		}*/
		schStr.Format(_T(" T:%s P:%03d%%,%03.1fh DC:%s \r\n"), /*项目点评:%s 部门点评:%s 评分:%d*/
			itor->GetUpdateTimeStr(), itor->GetPercent(), itor->GetWorkHours(), itor->GetUpdateContent()
			/*,strPrj, strDepart, itor->GetScore()*/ );
		itor++;
		if ( itor != listSch.end() )
		{
			schStr += _T(".....................................................................\r\n");
		}
		strLst.push_back( schStr );
		//str += schStr;

	}
}
void CBaseGridManger::GetHistoryString( int taskId, CString &str ) const
{
	GetHistoryString( m_pTaskSever,  taskId, str );
}
CString CBaseGridManger::GetScheduleString( const CSchedule &obj ) const
{
	CString schStr;
	
	schStr.Format(_T("T:%s  P:%03d%%,%03.1fh \r\nDC:%s"), obj.GetUpdateTimeStr(), obj.GetPercent(), obj.GetWorkHours(),
		 obj.GetUpdateContent() );
	return schStr;
}
int CBaseGridManger::GetFlexColumnCount()
{
	if (m_gridColumnMap.size() != 0)
	{
		return m_gridColumnMap.size();
	}
	else
	{
		return GetFlexColumnCount();
	}
}
CString CBaseGridManger::GetPanTimeString(const CTaskObj &obj )
{
	CString text;
	text.Format( _T("%s %0.1fh\r\n%s"), obj.GetPanStartDateShowStr(), obj.GetPlanWorkHour(),
		obj.GetPanEndDateShowStr() );
	return text;
}
CString CBaseGridManger::GetFactTimeString( const CTaskObj &obj)
{
	CString text;
	text.Format( _T("%s %0.1fh\r\n%s"), obj.GetFactStartDateShowStr(), obj.GetFactWorkHour(),
		obj.GetFactEndDateShowStr() );
	return text;
}
void  CBaseGridManger::GetTaskStatusSting(const CTaskObj &obj, CString &text )
{
	const CTaskStaus& stMap = m_pTaskSever->GetTaskStausMap();
	stMap.GetStrStausById( obj.GetTaskStatus(), text );
	CTaskChange chObj;
    if ( userExcOk == m_pSeverFactory->GetChangeSever()->GetMaxNewChangeByTaskId( obj.GetId(), chObj) 
		&& (departUnAgree == chObj.GetStatus() || genernalUnAgree == chObj.GetStatus()) )
    {
		CString str = chObj.GetChangeStatusString( chObj.GetStatus()  );
		text += _T("(") + str +_T(")");
    }
}

void  CBaseGridManger::SetBkColorByTask(int nRow, int nColumn, const CTaskObj &obj, int diffPer )
{
	int factRow =  nRow + GetFixedRowCount();
	int factCol = nColumn + GetFixedColumnCount();
	COLORREF color = m_otherColor;
	switch( obj.GetTaskStatus() )
	{
	case waitStart:
		color = m_waitStartColor;
		break;
	case  ongoing:
		if ( obj.GetPlanEndTime() >= COleDateTime::GetCurrentTime() )
		{
			color = m_goingonColor;
		}else
		{
			color = m_overdueColor; 
		}
		break;
	case  newTasking:
		color = m_newingColor;
		break;
	case  task_close:
		color = m_finishColor;
		break;
	case waitPublish:
		color = m_waitPublishColor;
		break;
	}
	if( diffPer > SHOW_EXCEPT_COLOR )
	{
		color = m_diffColor;
	}
	m_gridCtrl->SetItemBkColour( factRow, factCol, color );
}
CString CBaseGridManger::GetWorkHourStringById( int id )
{
	CString str;
	const TaskIdStringMap list = m_pSeverFactory->GetTaskSever()->GetWorkHourString();
	TaskIdStringMapConstItor itor = list.find( id );
	if ( itor != list.end() )
	{
		str = itor->second;
	}
	return str;
}
CString CBaseGridManger::GetProjectShortName( const CTaskObj &obj  )
{
	int prjId = obj.GetProjctId();
	if ( obj.GetType() == TASKTYPE_DEPARTSTR )
	{
		return TASKTYPE_DEPARTSTR;
	}else if ( obj.GetProjctId() >= 0)
	{
		CProject prj;
		m_pSeverFactory->GetProjectSever()->GetProjectById( prjId, prj );
		return prj.GetShortName();
	}else
	{
		return _T("无");
	}
}
CString CBaseGridManger::GetOwnerNameStr( int userId )
{
	CString text;
	CUser useObj;
	m_pSeverFactory->GetLoginServer()->GetUserById( userId, useObj );
	text = useObj.GetUserName_R();
	return text;
}
void    CBaseGridManger::GetTaskBelongToPrjStep( int taskAttrbute, CString &str )
{
     if ( taskAttrbute > 0 )
     {
		 CProjectStep step;
		 m_pSeverFactory->GetSystemCfgSever()->GetProjectStep( taskAttrbute, step );
		 str = step.GetName();
     }else
	 {
		 str = _T("无");
	 }
}

void CBaseGridManger::OnGridEndEidt( NMHDR* pNMHDR,LRESULT* pResult )
{
	NM_GRIDVIEW *gridView = reinterpret_cast<NM_GRIDVIEW*>(pNMHDR);
	EndEditGridView( gridView->iRow, gridView->iColumn );
}
CTaskObj* CBaseGridManger::GetTaskByRow( int nRow, ListTask &taskList, const IntToIntMap &showMap )
{
	if ( taskList.size() <= 0)
	{
		return NULL;
	}
	IntToIntMap::const_iterator dataItor = showMap.find( nRow );
	if ( dataItor != showMap.end() && dataItor->second >= 0 )
	{
		ListTaskItor itor = taskList.begin();
		advance( itor, dataItor->second );
		if ( itor == taskList.end() )
		{
			return NULL;
		}
		return &(*itor);
	}
	return NULL;
}
CString* CBaseGridManger::GetCStringByPos( int nRow, ListString &showView,  const IntToIntMap &showMap )
{
	if ( showView.size() <= 0)
	{
		return NULL;
	}
	IntToIntMap::const_iterator dataItor = showMap.find( nRow );
	if ( dataItor != showMap.end() && dataItor->second >= 0 )
	{
		ListStringItor itor = showView.begin();
		advance( itor, dataItor->second );
		if ( itor == showView.end() )
		{
			return NULL;
		}
		return &(*itor);
	}
	return NULL;
}
int CBaseGridManger::FindTaskListById( int taskId, ListTask &lst, ListTaskItor &itor )
{
	itor = lst.begin();
	int row = 0;
	while( itor != lst.end() )
	{
		if ( itor->GetId() == taskId )
		{
			break;
		}
		row++;
		itor++;
	}
	if ( itor == lst.end() )
	{
		row = -1;
	}
	return row;
}
int CBaseGridManger::FindTaskById( int taskId,  ListTask &lst, ListTaskItor &itor,const IntToIntMap &postMap  )
{
	itor = lst.begin();
	int row = 0;
	while( itor != lst.end() )
	{
		if ( itor->GetId() == taskId )
		{
			break;
		}
		row++;
		itor++;
	}
	if ( itor != lst.end() )
	{
		for ( IntToIntMap::const_iterator lineItor = postMap.begin(); lineItor != postMap.end();
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