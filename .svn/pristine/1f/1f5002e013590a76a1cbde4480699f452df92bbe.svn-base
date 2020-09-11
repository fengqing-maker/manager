#include "stdafx.h"
#include "taskSeverImp.h"
#include "Src/Tools/UtilityTool/UtilityTool.h"

using namespace std;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CTaskSeverImp::CTaskSeverImp(CDaoFactory  *pObj):CTaskSeverInc(pObj)
{
	ASSERT( pObj != NULL );
	m_pTaskDbDAo = pObj->GetTaskDbDao();
	m_newingTaskCount = 0;
	m_goingTaskCount = 0;
	m_waitStartTaskCount = 0;
	m_pChangeDbDao = pObj->GetChangeDbDao();
	m_ownChangeCount = 0;
}
BOOL CTaskSeverImp::GetTaskByTaskId( int taskId, CTaskObj &obj )
{
	return m_pTaskDbDAo->GetTaskContentById( taskId, obj );
}
BOOL CTaskSeverImp::GetTaskScheudle( int taskId, ListSchedule &obj)
{
	return m_pTaskDbDAo->GetScheduleContentByTaskId( taskId, obj );
}
BOOL CTaskSeverImp::GetTaskByStatus( int ownerId, int st, ListTask &list, int year /*= 0*/, int quarter /*= 0*/ )
{
	if ( st > 0 )
	{
		return m_pTaskDbDAo->GetTaskByOwernerId(ownerId, (EnumStaus)st, list, year, quarter );
	}else
	{
		return m_pTaskDbDAo->GetTaskByOwnerId(ownerId, list, year, quarter );
	}
}
BOOL CTaskSeverImp::GetUserHandleTask( int useId, int year, int quater, ListTask &list )
{//获取用户负责任务
	BOOL ret = m_pTaskDbDAo->GetTaskByOwnerId( useId, list, year, quater );
	//for ( ListTaskItor itor = list.begin(); itor != list.end();  )
	//{
	//	if ( newTasking == itor->GetTaskStatus() && itor->GetCreateId() != m_loginUser.GetId() )
	//	{
	//		itor = list.erase( itor );
	//	}else
	//	{
	//		itor++;
	//	}
	//}
	if ( userExcOk == ret )
	{//获取此用户创建的且责任不是本人的任务
		int beginNew = list.size();
		ret = m_pTaskDbDAo->GetNewTaskButNotOwer( useId, list );
		COleDateTime beginDate, endDate;
		if ( list.size() > beginNew  )
		{
			if ( TRUE == CUtilityTool::GetObj()->GetQuarterDateRange( year, quater, beginDate, endDate) )
			{
				ListTaskItor tmpItor = list.begin();
				advance( tmpItor, beginNew );
				ListTaskItor tmpItor2 = tmpItor++;
				for ( ; tmpItor != list.end(); )
				{
					if ( !(tmpItor->GetPlanStartTime() >= beginDate && tmpItor->GetPlanStartTime() <= endDate) )
					{
						list.erase( tmpItor );
						tmpItor = tmpItor2;
					}else
					{
						tmpItor++;
						tmpItor2++;
					}
				}
			}
		}
	}
	return ret;
}
BOOL CTaskSeverImp::GetTaskByOwnerId( int useId, ListTask &lst )
{
	return m_pTaskDbDAo->GetTaskByOwnerId( useId, lst );
}
BOOL CTaskSeverImp::GetUserReviewTask( int reviewId, ListTask &list  )
{//获取用户负责审核的任务
	BOOL ret = m_pTaskDbDAo->GetTaskByReviewId( reviewId, waitPublish,list );
	if ( userExcOk == ret )
	{
		 m_pTaskDbDAo->GetTaskByProjectManger( reviewId, finishReviewWait, list ); //获取本人管理的项目待完成点评的任务
	}
	return ret;
}
BOOL CTaskSeverImp::GetNewingTaskByCreateId( int creatId, ListTask &list )
{
	return m_pTaskDbDAo->GetNewingTaskByCreateId( creatId, list );
}
BOOL CTaskSeverImp::GetUserRemarkOnTask(  int reviewId, MapTaskSchedule &mapObj   )
{//获取用户负责的主管进度审核任务
	ListTask tasks;
	BOOL ret = m_pTaskDbDAo->GetTaskByReviewId( reviewId, ongoing, tasks);
	if ( userExcOk ==  ret)
	{//进行获取进度
		//mapObj.clear();
		ListTaskItor itor;
		itor = tasks.begin();
		while( itor != tasks.end() )
		{
			ListSchedule obj;
			ret = m_pTaskDbDAo->GetScheduleContentByTaskId( itor->GetId(), notStartReview, obj);
			if ( obj.size() > 0 )
			{
				InsetMapSchudle(itor->GetId() , obj, mapObj );
			}
			itor++;
		}
	}
	return ret;
}
BOOL CTaskSeverImp::GetOnGoingTaskByPrjId( int prjId, ListTask &list )
{
	return m_pTaskDbDAo->GetTaskByProjectId( prjId, ongoing, list);
}
void CTaskSeverImp::InsetMapSchudle(int taskId ,ListSchedule &obj, MapTaskSchedule &mapObj )
{
	ListSchedule& temp = mapObj[taskId];
	if ( temp.size() > 0 )
	{
		ListScheduleItor itorI = obj.begin();
		while( itorI != obj.end() )
		{
			int scheduleId = itorI->GetId();

			ListScheduleItor itorJ = temp.begin();
			while( itorJ != temp.end() )
			{
				if ( itorJ->GetId() == scheduleId )
				{
					break;
				}
				itorJ++;
			}
			if ( itorJ == temp.end() )
			{
				temp.push_back( *itorI );
			}
			itorI++;
		}

	}else
	{
		mapObj[ taskId ].swap( obj );
	}
}
BOOL CTaskSeverImp::GetUserPrjRemarkOnTask( int prjId, MapTaskSchedule &mapObj )
{//获取用户负责的项目的进度审核
	ListTask tasks;
	BOOL ret = m_pTaskDbDAo->GetTaskByProjectId( prjId, ongoing, tasks);
	if ( userExcOk ==  ret)
	{//进行获取进度
		//mapObj.clear();
		ListTaskItor itor;
		itor = tasks.begin();
		while( itor != tasks.end() )
		{
			ListSchedule obj;

			ret = m_pTaskDbDAo->GetScheduleContentByPrjTaskId( itor->GetId(), notStartReview, obj);
			if ( obj.size() > 0 )
			{
				InsetMapSchudle(itor->GetId() , obj, mapObj );
			}
			itor++;
		}
	}
	return ret;
}
BOOL CTaskSeverImp::GetMyChange( int userId, ListChange &list )
{
	BOOL ret = m_pChangeDbDao->GetChangerItemsByTaskOwner( userId, list );		//获取我变更的任务信息
	if ( ret != userExcOk )
	{
		return ret;
	}
	m_ownChangeCount = list.size();
	ret = m_pChangeDbDao->GetChangerItemsByProjectManger( userId, list); //2015-0-26 add 项目经理点评的变更（获取我改变的任务中经过领导操作的任务）
	ret = m_pChangeDbDao->GetChangerItemsByReviewUser( userId, list );			//已经通过领导审批的
	return ret;
}
BOOL CTaskSeverImp::GetChangeFirstReviewTask( int reviewId, ListChange &list )
{//获取用户负责的一级变更审核
	return m_pChangeDbDao->GetChangeItemsByReviewId( reviewId, list );
}
BOOL CTaskSeverImp::GetChangeSecondReviewTask( int gerneralId, ListChange &list )
{//获取用户负责的二级变更审核
	return m_pChangeDbDao->GetChangeItemsByGeneralId( gerneralId, list );
}

BOOL CTaskSeverImp::UpdateOwernTask()
{
	SmartCSLock lock( &m_ownerTaskCS );
	//return GetUserHandleTask( m_loginUser.GetId(), m_ownerTaskList );
	m_ownerTaskList.clear();
	int  userId = m_loginUser.GetId();
	BOOL ret = m_pTaskDbDAo->GetNewingTaskByCreateId( userId, m_ownerTaskList ) ;	//获取新建中的任务
	if ( userExcOk != ret )
	{
		return ret;
	}
	m_newingTaskCount = m_ownerTaskList.size();
	ret = m_pTaskDbDAo->GetTaskByOwernerId( userId, ongoing ,m_ownerTaskList ) ;	//获取正在进行的任务清单
	if ( userExcOk != ret )
	{
		return ret;
	}
	m_goingTaskCount = m_ownerTaskList.size();
	ret = m_pTaskDbDAo->GetTaskByOwernerId( userId, waitStart,m_ownerTaskList);		//获取未开始的任务清单
	return ret;
}
BOOL CTaskSeverImp::UpdateReviewTask()
{
	SmartCSLock lock( &m_reviewTaskCS );
	m_reviewTaskList.clear();
	return GetUserReviewTask( m_loginUser.GetId(), m_reviewTaskList );
}
BOOL  CTaskSeverImp::UpdateScoreTask()
{
	SmartCSLock lock( &m_scoreTaskCS );
	m_scoreTaskList.clear();
	return m_pTaskDbDAo->GetTaskByReviewId( m_loginUser.GetId(), waitScore, m_scoreTaskList );
}
BOOL CTaskSeverImp::UpdateReviewChange()
{
	return GetChangeFirstReviewTask( m_loginUser.GetId(), m_reviewChange );
}
BOOL CTaskSeverImp::UpdateGernalChange()
{
	return GetChangeSecondReviewTask( m_loginUser.GetId(), m_reviewChange );
}


void CTaskSeverImp::SetLoginUser( const CUser &obj )
{
	ASSERT( onLine == obj.GetOnLine() );
	m_loginUser = obj;
	m_loginPower.clear();
	m_pDbDao->GetUserDao()->GetUserPowersByRoleId( m_loginUser.GetRolId(), m_loginPower );			//获取用户权限
	m_stausMap.Clear();
	m_pTaskDbDAo->GetTaskStatusMap( m_stausMap );													//又获取各个任务的代号和名称干嘛

	LoadTaskByUser();
}
void CTaskSeverImp::UpdateLoginUser()
{
	if ( m_loginUser.GetId() > 0 )
	{
		m_pDbDao->GetUserDao()->GetUserById( m_loginUser.GetId(), m_loginUser );
	}
}

BOOL CTaskSeverImp::LoadTaskByUser()
{
	BOOL           ret = -1; 
	if ( m_loginUser.GetId() >= 0  )
	{
		ret = UpdateOwernTask();		//获取到自己需要处理的任务的信息
		if ( ret != userExcOk )
		{
			return ret;
		}

		RolePowerList::iterator itor = m_loginPower.begin();
		GetMyChange( m_loginUser.GetId(), m_reviewChange ); 
		while( itor != m_loginPower.end() )
		{
			switch(*itor)
			{
			case taskAgree:
				UpdateReviewTask();
				break;
			}
			itor++;
		}
	}
	return ret;
}
BOOL CTaskSeverImp::UpdateAllChange()
{
	SmartCSLock lock( &m_reviewChangeCS ); 
	RolePowerList::iterator itor = m_loginPower.begin();
	BOOL ret = userExcOk;
	m_reviewChange.clear();

	GetMyChange( m_loginUser.GetId(), m_reviewChange );
	//while( itor != m_loginPower.end() )
	//{
	//	if ( *itor == changeLevelAgree1 )
	//	{
	//		ret = UpdateReviewChange();
	//	}
	//	if ( *itor == changeLevelAgree2 )
	//	{
	//		ret = UpdateGernalChange();
	//	}
	//	itor++;
	//}
	return ret;
}
//BOOL CTaskSeverImp::UpdateAllReviewSchedule()
//{
//	SmartCSLock lock( &m_remarkScheduleCS ); 
//	RolePowerList::iterator itor = m_loginPower.begin();
//	BOOL ret = userExcOk;
//	m_remarkSchedule.clear();
//	while( itor != m_loginPower.end() )
//	{
//		if ( *itor == taskAgree )
//		{
//			ret = UpdateReviewSchedule();
//		}
//		if ( *itor == projectReview )
//		{
//			ret = UpdatePrjReviewSchedule();
//		}
//		itor++;
//	}
//	return ret;
//}
const TaskIdStringMap & CTaskSeverImp::GetWorkHourString()
{
	if ( m_workHoureObj.GetWorkTimeTypMap().size() <= 0 )
	{
		m_pTaskDbDAo->GetAllWorkTimeType( m_workHoureObj.GetWorkTimeTypMap() );
	}
	return m_workHoureObj.GetWorkTimeTypMap();
}
BOOL CTaskSeverImp::InsertTask( CTaskObj &obj )
{
	BOOL ret ;
	if ( userExcOk == (ret =m_pTaskDbDAo->InsertTask( obj )) )
	{
		ret = -1;
		m_pTaskDbDAo->GetLastInsertId( ret );
		obj.SetId( ret );
	}
	return ret;
}
BOOL CTaskSeverImp::UpdateTask( CTaskObj &obj )
{
	return m_pTaskDbDAo->UpdateTask( obj );
}
BOOL CTaskSeverImp::UpdateFactStartTime(const CTaskObj &obj )
{
	TaskIdStringMap filds;
	CString value;
	value.Format( _T("'%s'"), obj.GetFactStartDateStr() );
	filds.insert( std::make_pair( fild_task_ac_starttime,  value ) );
	//value.Format( _T("'%s'"), obj.GetFactEndDateStr() );
	//filds.insert( std::make_pair( fild_task_ac_closetime,  value ) );
	//value.Format( _T("'%0.1f'"), obj.GetFactWorkHour() );
	//filds.insert( std::make_pair( fild_task_ac_period,  value ) );
	filds.insert( std::make_pair( fild_task_check_begin,  value ) );
	value.Format(_T("%d"), obj.GetTaskStatus() );
	filds.insert( std::make_pair( taskStatus, value) );
	BOOL ret = m_pTaskDbDAo->UpdateTaskField( obj.GetId(), filds)/*m_pTaskDbDAo->UpdateFactTimeTask(obj)*/;
	 return ret;
}
//fatherId<0删除现有的父子关系
BOOL CTaskSeverImp::UpdateTaskFather( int taskId, int fatherId )
{
	return m_pTaskDbDAo->UpdateTaskFather( taskId, fatherId );
}
//
BOOL CTaskSeverImp::TaskReviewAction( bool isAgree,const CString &viewStr, const CString &userName,const CTaskObj &objInput )
{
	CTaskObj obj = objInput;
	double diffPara = obj.GetDifficultyParam();
	CString strDc;
	COleDateTime time = COleDateTime::GetCurrentTime();
	CString timeStr = time.Format( _T("%Y-%m-%d %H:%M:%S") );
	TaskIdStringMap fields;
	CString temp;
	if ( isAgree )
	{
		if ( diffPara < 1.0 || diffPara > 3.0 )
		{
			AfxMessageBox(_T("任务难度系数范围为[1.0~3.0]"));
			return paraError;
		}
		strDc.Format( _T("%s\r\n%s;%s%s同意任务申请"), obj.GetDec(), viewStr, timeStr, userName);
		temp.Format( _T("%0.1f"), diffPara );
		fields.insert( std::make_pair( taskDiffParam, temp )  );
		obj.SetTaskStatus( waitStart );
	}else
	{
		if ( viewStr.GetLength() <= 0 )
		{
			AfxMessageBox(_T("不同意任务申请，评审意见不能为空"));
			return paraError;
		}
		strDc.Format( _T("%s\r\n%s;%s%s不同意任务申请"), obj.GetDec(), viewStr, timeStr, userName);
		obj.SetTaskStatus( newTasking );
	}
	//更新任务
	obj.SetPublishTime( time );
	temp.Format( _T("'%s'"), timeStr );
	fields.insert( std::make_pair( task_review_date, temp) );
	temp.Format( _T("'%s'"), strDc );
	fields.insert( std::make_pair( taskDec, temp) );
	temp.Format( _T("%d"), obj.GetTaskStatus() );
	fields.insert( std::make_pair( taskStatus, temp) );

	BOOL ret = m_pTaskDbDAo->UpdateTaskField( obj.GetId(), fields ) ;
	if ( paraError ==  ret )
	{
		AfxMessageBox( _T("参数错误"));
	}
	return ret;
}
//任务完成审核 动作处理
BOOL CTaskSeverImp::TaskReviewFinishAction( bool isAgree,const CString &viewStr, const CString &userName, const CTaskObj &objInput )
{
	CTaskObj obj = objInput;
	CString strDc;
	COleDateTime time = COleDateTime::GetCurrentTime();
	CString timeStr = time.Format( _T("%Y-%m-%d %H:%M:%S") );
	TaskIdStringMap fields;
	CString temp;
	if ( isAgree )
	{
		strDc.Format( _T("%s\r\n%s;%s%s项目任务审核完成"), obj.GetDec(), viewStr, timeStr, userName);
		obj.SetTaskStatus( waitScore );
	}else
	{
		if ( viewStr.GetLength() <= 0 )
		{
			AfxMessageBox(_T("不同意任务完成，评审意见不能为空"));
			return paraError;
		}
		strDc.Format( _T("%s\r\n%s;%s%s不同意任务申请完成"), obj.GetDec(), viewStr, timeStr, userName);
		obj.SetTaskStatus( ongoing );
	}
	//更新任务
	temp.Format( _T("'%s'"), strDc );
	fields.insert( std::make_pair( taskDec, temp) );
	temp.Format( _T("%d"), obj.GetTaskStatus() );
	fields.insert( std::make_pair( taskStatus, temp) );

	BOOL ret = m_pTaskDbDAo->UpdateTaskField( obj.GetId(), fields ) ;
	if ( paraError ==  ret )
	{
		AfxMessageBox( _T("参数错误"));
	}
	return ret;
}
BOOL CTaskSeverImp::TaskFinishWaitScore( CTaskObj &obj )
{
	TaskIdStringMap fields;
	CString valueStr;
	valueStr.Format( _T("'%s'"), obj.GetFactEndDateStr() );
	fields.insert( std::make_pair( taskFactEndTime,valueStr));
	valueStr.Format( _T("%d"), obj.GetTaskStatus() );
	fields.insert(std::make_pair( taskStatus, valueStr) );
	valueStr.Format( _T("'%s'"), obj.GetDec() );
	fields.insert(std::make_pair( taskDec, valueStr) );
	return m_pTaskDbDAo->UpdateTaskField( obj.GetId(), fields );

}
BOOL CTaskSeverImp::TaskScoreAction( bool isAgree,const CString &viewStr, const CString &userName, const CTaskObj &objInput)
{
	CTaskObj obj = objInput;
	COleDateTime time = COleDateTime::GetCurrentTime();
	CString timeStr = time.Format( _T("%Y-%m-%d %H:%M:%S") );
	TaskIdStringMap fields;
	CString temp;
	int score = obj.GetForecastScore()/*obj.GetScore()*/;/*2017-2-5用将系统预测分修改为经理评分*/
	CString strDc;
	bool isDeleteNoneSch = false;
	if ( isAgree )
	{
		if ( score < 0 || score > 120 )
		{
			AfxMessageBox(_T("任务分数范围为[0~120]"));
			return paraError;
		}
		if ( ( score < 95 || score > 100 ) && viewStr.IsEmpty() )
		{
			AfxMessageBox(_T("任务评分小于95或大于100需填写评分意见"));
			return paraError;
		}
		if ( obj.GetFactEndTime() > obj.GetPlanEndTime() )
		{//延迟完成
			COleDateTimeSpan span1 = obj.GetFactEndTime() - obj.GetPlanStartTime();
			COleDateTimeSpan span2 = obj.GetPlanEndTime() - obj.GetPlanStartTime();
			score = (score * (span2.GetDays()+1))/(span1.GetDays()+1);
			strDc.Format( _T("%s\r\n%s;%s%s同意任务延期完成(经理原评分为%d,最终评分%d)"), obj.GetDec(), viewStr, timeStr, userName,
				           objInput.GetScore(), score );
		}else
		{
			strDc.Format( _T("%s\r\n%s;%s%s同意任务完成(评分为%d)"), obj.GetDec(), viewStr, timeStr, userName, score);
		}
		temp.Format( _T("%d"), score );
		fields.insert( std::make_pair( fild_task_score, temp )  );
		temp.Format( _T("%d"),  obj.GetForecastScore() );
		fields.insert( std::make_pair( fild_forecast_task_score, temp )  );
		obj.SetTaskStatus( task_close );
		isDeleteNoneSch = true;
	}else
	{
		if ( viewStr.GetLength() <= 0 )
		{
			AfxMessageBox(_T("不同意任务完成，评审意见不能为空"));
			return paraError;
		}
		strDc.Format( _T("%s\r\n%s;%s%s不同意任务完成"), obj.GetDec(), viewStr, timeStr, userName);
		obj.SetTaskStatus( ongoing );
		COleDateTime date = COleDateTime::GetCurrentTime();
		COleDateTimeSpan span( 1, 0, 0, 0 );
		date -= span;
		temp.Format( _T("'%s'"), date.Format(_T("%Y-%m-%d")) );
		fields.insert( std::make_pair( fild_task_check_begin, temp) );
		if (  obj.GetTaskStatus() == ongoing)
		{//2015-7-13 add 
			double relust = 0.0f;
			m_pTaskDbDAo->GetTaskWorkHourSum( obj.GetOwnerId(), -1, date, relust );
			if ( relust > 0 )
			{
				m_pTaskDbDAo->AddEmptySchedule( obj.GetId(), date );
			}
		}
	}
	//更新任务
	obj.SetPublishTime( time );
	temp.Format( _T("'%s'"), timeStr );
	fields.insert( std::make_pair( fild_task_score_date, temp) );
	temp.Format( _T("'%s'"), strDc );
	fields.insert( std::make_pair( taskDec, temp) );
	temp.Format( _T("%d"), obj.GetTaskStatus() );
	fields.insert( std::make_pair( taskStatus, temp) );
	temp.Format(_T("'%s'"), viewStr );
	fields.insert( std::make_pair( task_score_des, temp) );
	BOOL ret = m_pTaskDbDAo->UpdateTaskField( obj.GetId(), fields ) ;
	if ( paraError ==  ret )
	{
		AfxMessageBox( _T("参数错误"));
	}
	if ( isDeleteNoneSch && userExcOk == ret )
	{
		m_pTaskDbDAo->DeleteNoneShcdule( obj.GetId() );
	}
	return ret;
}
BOOL CTaskSeverImp::DeleteTaskById( int taskId )
{
	return m_pTaskDbDAo->DeleteTaskById( taskId );
}
BOOL CTaskSeverImp::GetTaskByPrjId( int prjId, ListTask &task )
{
	return m_pTaskDbDAo->GetTaskByProjectId( prjId, task);
}
BOOL CTaskSeverImp::GetScheduleContentByDate(int ownerId, const COleDateTime &beginTime, 
	const COleDateTime &endTime, ListSchedule &obj)
{
	return m_pTaskDbDAo->GetScheduleContentByDate( ownerId, beginTime, endTime, obj );
}
void CTaskSeverImp::SetOwernTask( const ListTask &lst )
{
	SmartCSLock lock( &m_ownerTaskCS );
	m_ownerTaskList = lst;
}
void CTaskSeverImp::SetReviewTask( const ListTask &lst  )
{
	SmartCSLock lock( &m_reviewTaskCS );
	m_reviewTaskList = lst;
}
void CTaskSeverImp::SetScoreTask( const ListTask &lst  )
{
	SmartCSLock lock( &m_scoreTaskCS );
	m_scoreTaskList = lst;
}
//void CTaskSeverImp::SetAllReviewSchedule( const MapTaskSchedule &lst )
//{
//	SmartCSLock lock( &m_reviewChangeCS );
//	m_remarkSchedule = lst;
//}
void CTaskSeverImp::SetAllChange( const ListChange &lst ) 
{
	SmartCSLock lock( &m_remarkScheduleCS );
	m_reviewChange = lst;
}
BOOL CTaskSeverImp::GetPerformProjectTask( int year, int quater, int owerId, ListTask &listObj )
{
	return m_pTaskDbDAo->GetPerformProjectTask( year, quater, owerId, listObj );
}
BOOL CTaskSeverImp::GetPerformDepartTask( int year, int quater, int owerId, ListTask &listObj )
{
	return m_pTaskDbDAo->GetPerformDepartTask( year, quater, owerId, listObj );
}
