#include "stdafx.h"
#include "CTaskDaoImp.h"

CTaskDaoImp::CTaskDaoImp( CDaoSupportInterface* pDao):CTaskDaoInc( pDao )
{
	m_taskTableName = _T("task");
	m_scheduleName = _T("schedule");
	m_fatherTableName = _T("fathertask");
	m_workTypeTableName = _T("timetype");
	m_unUpateDateTableName = _T("task_check_update");
}
//增
BOOL CTaskDaoImp::InsertTask( CTaskObj &obj )
{
	CString str;
	str.Format(_T("select task_id from %s where task_name = '%s' and task_project_id = %d and task_attri >= 0"), // and task_attri = %d
               m_taskTableName, obj.GetName(), obj.GetProjctId()/*, obj.GetAttribute()*/ );
	if ( FALSE == m_pDaoSupportInc->ExecuteSql(str) )
	{
		return excSqlError;
	}
	if ( !m_pDaoSupportInc->GetRecordSet()->IsEOF() && obj.GetAttribute() >= 0 )
	{
		obj.SetId( (*(m_pDaoSupportInc->GetRecordSet()))[_T("task_id")] );
		return insertNameRepeat;
	}
	str.Format( _T("insert into %s (task_attri, task_name, task_des, task_ownerid,task_project_id,\
				   task_plan_starttime,task_plan_closetime, task_plan_period,\
				   task_ac_starttime, task_ac_closetime, task_ac_period, task_type, task_score,\
				   forecast_task_score,task_score_des,task_auditorid,task_review_date,task_score_date,task_statusid,\
				   task_dif,task_createTime,task_timeType_id,creatorid,task_check_begin,task_flag) values(\
				   %d, '%s', '%s', %d, %d,\
				   '%s', '%s', %0.1f, '%s','%s',%0.1f,'%s', %d,\
				   %d, '%s', %d, '%s','%s', %d,\
				   %0.1f, '%s', %d,%d,'%s',%d)"), m_taskTableName, 
				   obj.GetAttribute(), obj.GetName(), obj.GetDec(), obj.GetOwnerId(), obj.GetProjctId(),
				   obj.GetPanStartDateStr(), obj.GetPanEndDateStr(), obj.GetPlanWorkHour(), obj.GetFactStartDateStr(), obj.GetFactEndDateStr(), obj.GetFactWorkHour(),
				   obj.GetType(), obj.GetScore(),
				   obj.GetForecastScore(),obj.GetScoreDec(),obj.GetMakeScoreUserId(),obj.GetPublishTimeStr(),obj.GetScoreTimeStr(),obj.GetTaskStatus(),
				   obj.GetDifficultyParam(), obj.GetCreateTimeStr(), obj.GetWorkTypeId(),obj.GetCreateId(), obj.GetCheckBeginDateStr(), obj.GetTaskFlag() );
	if ( FALSE == m_pDaoSupportInc->Execute(str) )
	{
		return excSqlError;
	}
	return userExcOk;
}
BOOL CTaskDaoImp::AddTaskSchedule( const CSchedule &obj )
{
	//
	CString str;
	str.Format( _T("select schedule_id from %s where update_time = '%s' and task_id = %d"), m_scheduleName,
		        obj.GetUpdateTimeStr(), obj.GetTaskId() );
	if ( FALSE == m_pDaoSupportInc->ExecuteSql(str) )
	{
		return excSqlError;
	}
	CAdoRecordset *pRecord = m_pDaoSupportInc->GetRecordSet();
	if ( pRecord->IsEOF() )
	{
		str.Format( _T("insert into %s (task_id, schedule_des,schedule_tim,schedule_percent,update_time,timeType_id)\
					   values(%d,'%s',%0.1f,%d,'%s',%d)"),
					   m_scheduleName,obj.GetTaskId(),obj.GetUpdateContent(),obj.GetWorkHours(),
					   obj.GetPercent(), obj.GetUpdateTimeStr(), obj.GetWorkTypeId()  );
	}else
	{
		int schId = (*pRecord)[_T("schedule_id")] ;
		str.Format( _T("update  %s t set t.task_id = %d , t.schedule_des = '%s',\
					    t.schedule_tim = %0.1f, t.schedule_percent=%d, t.timeType_id = %d where schedule_id = %d "),
					   m_scheduleName,obj.GetTaskId(),obj.GetUpdateContent(),obj.GetWorkHours(),
					   obj.GetPercent(), obj.GetWorkTypeId(), schId );
	}
	if ( FALSE == m_pDaoSupportInc->Execute(str) )
	{
		return excSqlError;
	}
	return userExcOk;
}
BOOL CTaskDaoImp::AddEmptySchedule( int taskId, const COleDateTime &date )
{
	CString timeStr;
	timeStr = date.Format( _T("%Y-%m-%d") );
	CString str;
	str.Format(_T("select schedule_id from %s where task_id = %d and update_time = 's'"), 
		m_scheduleName, taskId, timeStr );
	if ( FALSE == m_pDaoSupportInc->ExecuteSql(str) )
	{
		return excSqlError;
	}
	if ( m_pDaoSupportInc->GetRecordSet()->IsEOF() )
	{//插入
		str.Format( _T("insert into %s (task_id, update_time) values (%d,'%s') "), m_scheduleName, taskId, timeStr );
		if ( FALSE == m_pDaoSupportInc->Execute(str) )
		{
			return excSqlError;
		}
	}
	return userExcOk;
}

BOOL CTaskDaoImp::UpdateTask( const CTaskObj &obj )
{
	BOOL ret  = ValidateTaskExit( obj.GetId())  ;
	if ( userExcOk != ret )
	{
		return ret;
	}
	CString str;
	int id = obj.GetId();

	CString createrSetStr; 
	createrSetStr.Format( _T(",t.task_createTime = '%s'"), obj.GetCreateTimeStr() );
	str.Format( _T("update  %s t set t.task_attri = %d, t.task_name = '%s', t.task_des = '%s', t.task_ownerid = %d, t.task_project_id=%d,\
				   t.task_plan_starttime = '%s',t.task_plan_closetime = '%s', t.task_plan_period =%0.1f,t.task_ac_starttime='%s',t.task_type='%s',\
				   t.task_auditorid=%d,t.task_statusid=%d, t.task_timeType_id = %d,t.creatorid=%d,t.task_flag=%d\
				   "), 
				   m_taskTableName, obj.GetAttribute(), obj.GetName(), obj.GetDec(), obj.GetOwnerId(),obj.GetProjctId(), 
				   obj.GetPanStartDateStr(), obj.GetPanEndDateStr(), obj.GetPlanWorkHour(), obj.GetFactStartDateStr(),obj.GetType(),
				   obj.GetMakeScoreUserId(),obj.GetTaskStatus(), obj.GetWorkTypeId(),obj.GetCreateId(), obj.GetTaskFlag()
				    );
	str += createrSetStr;
	createrSetStr.Format( _T(" where t.task_id = %d"), id );
	str += createrSetStr;
	if ( FALSE == m_pDaoSupportInc->Execute(str) )
	{
		return excSqlError;
	}
	return userExcOk;
}
CString CTaskDaoImp::GetFieldStringByType(int fieldType )
{
	CString  strFild;
	switch( fieldType )
	{
	case taskName:
		strFild = _T("task_name");
		break;
	case taskStatus:
		strFild = _T("task_statusid");
		break;
	case  taskFactStartTime:
		strFild = _T("task_plan_starttime");
		break;
	case  taskFactWorkHour:
		strFild = _T("task_ac_period");
		break;
	case  taskDiffParam:
		strFild = _T("task_dif");
		break;
	case  taskDec:
		strFild = _T("task_des");
		break;
	case  task_review_date:
		strFild = _T("task_review_date");
		break;
	case  taskFactEndTime:
		strFild = _T("task_ac_closetime");
		break;
	case fild_task_plan_starttime:
		strFild = _T("task_plan_starttime");
		break;
	case fild_task_plan_closetime:
		strFild = _T("task_plan_closetime");
		break;
	case  fild_task_plan_period:
		strFild = _T("task_plan_period");
		break;
	case  fild_forecast_task_score:
		strFild = _T("forecast_task_score");
		break;
	case fild_task_ac_starttime:
		strFild = _T("task_ac_starttime");
		break;
	case fild_task_ac_closetime:
		strFild = _T("task_ac_closetime");
		break;
	case fild_task_ac_period:
		strFild = _T("task_ac_period");
		break;
	case  fild_task_score_date:
		strFild = _T("task_score_date");
		break;
	case fild_task_score:
		strFild = _T("task_score");
		break;
	case  fild_task_ownerid:
		strFild = _T("task_ownerid");
		break;
	case  fild_task_timeType_id:
		strFild = _T("task_timeType_id");
		break;
	case  fild_task_auditorid:
		strFild = _T("task_auditorid");
		break;
	case  fild_promanager_comment:
		strFild = _T("promanager_comment");
		break;
	case  fild_depmanager_comment:
		strFild = _T("depmanager_comment");
		break;
	case fild_task_update_watch:
		strFild = _T("task_update_watch");
		break;
	case  fild_task_check_begin:
		strFild = _T("task_check_begin");
		break;
	case  file_task_attri:
		strFild = _T("task_attri");
		break;
	case  file_task_type:
		strFild = _T("task_type");
		break;
	case file_task_project_id:
		strFild = _T("task_project_id");
		break;
	case task_score_des:
		strFild = _T("task_score_des");
		break;
	}
	return strFild;
}
BOOL CTaskDaoImp::UpdateTaskField( int taskId, const TaskIdStringMap &fields )
{
	BOOL ret  = ValidateTaskExit( taskId)  ;
	if ( userExcOk != ret )
	{
		return ret;
	}
	CString str;
	str.Format( _T("update %s t set "), m_taskTableName );
	TaskIdStringMap::const_iterator itor = fields.begin();
	CString tempStr,fieldStr;
	while( itor != fields.end()  )
	{
		fieldStr = GetFieldStringByType( itor->first);
		if ( fieldStr.GetLength() <= 0 )
		{
			return paraError;
		}
		tempStr.Format(_T("t.%s = %s,"), fieldStr, itor->second );
		str += tempStr;
		itor++;
	}
	str.Delete( str.GetLength() - 1 );
	CString sqlString;
	sqlString.Format( _T("%s where t.task_id = %d"), str, taskId );
	if ( FALSE == m_pDaoSupportInc->Execute(sqlString) )
	{
		return excSqlError;
	}
	return userExcOk;
}

BOOL CTaskDaoImp::UpdateTaskField(int taskId ,int fieldType, const CString &value )
{
	BOOL ret  = ValidateTaskExit( taskId)  ;
	if ( userExcOk != ret )
	{
		return ret;
	}
	CString strFild = GetFieldStringByType( fieldType );
	if ( strFild.GetLength() <= 0 )
	{
		return paraError;
	}
	CString str;
	str.Format( _T("update %s t set t.%s = %s where t.task_id = %d"),
		m_taskTableName,  strFild, value, taskId);
	if ( FALSE == m_pDaoSupportInc->Execute(str) )
	{
		return excSqlError;
	}
	return userExcOk;
}
BOOL CTaskDaoImp::UpdateFactTimeTask( const CTaskObj &obj )
{
	BOOL ret  = ValidateTaskExit( obj.GetId())  ;
	if ( userExcOk != ret )
	{
		return ret;
	}
	CString str;
	int id = obj.GetId();
	str.Format( _T("update %s t set t.task_ac_starttime = '%s', t.task_ac_closetime='%s',\
				   t.task_ac_period = %0.1f,t.task_statusid=%d where t.task_id = %d" ), 
				   m_taskTableName,obj.GetFactStartDateStr(),obj.GetFactEndDateStr(),obj.GetFactWorkHour(),
				   obj.GetTaskStatus(), id);
	if ( FALSE == m_pDaoSupportInc->Execute(str) )
	{
		return excSqlError;
	}
	return userExcOk;
}
BOOL CTaskDaoImp::UpdateTaskFather( int taskId, int fatherId )
{
	BOOL ret  = ValidateTaskExit( taskId )  ;
	if ( userExcOk != ret )
	{
		return ret;
	}
	if ( fatherId < 0 )
	{//删除父子关系
		CString str;
		str.Format( _T("delete  from %s where task_id = %d "), m_fatherTableName, taskId );
		if ( FALSE == m_pDaoSupportInc->Execute(str) )
		{
			return excSqlError;
		}
		return userExcOk;
	}else
	{//修改父子关系
		ret = ValidateTaskExit( fatherId );
		if ( userExcOk != ret )
		{
			return ret;
		}
		CString str;
		str.Format( _T("select * from %s where task_id = %d "), m_fatherTableName, taskId );
		if ( FALSE == (ret = m_pDaoSupportInc->ExecuteSql(str)) )
		{
			return excSqlError;
		}
		CAdoRecordset* pRecordSet = m_pDaoSupportInc->GetRecordSet();
		if ( pRecordSet->IsEOF() )
		{//插入
			str.Format( _T("insert into %s (father_id, task_id) values (%d,%d)"), m_fatherTableName, fatherId, taskId );
		}else
		{//更新
			while( !pRecordSet->IsEOF() )
			{
				if ( (int)((*pRecordSet)[_T("father_id")]) == fatherId )
				{
					return userExcOk;
				}
				pRecordSet->MoveNext();
			}
			str.Format( _T("update %s t set t.father_id = %d where t.task_id = %d"), m_fatherTableName, fatherId,taskId );
		}
		if ( FALSE == m_pDaoSupportInc->Execute(str) )
		{
			return excSqlError;
		}
		return userExcOk;
	}

}


CString CTaskDaoImp::GetSCheduleFieldString(int fieldType )
{
	CString str;
	switch( fieldType )
	{
	case 	fild_task_id:
		str = _T("task_id");
		break;
	case fild_schedule_des:
		str = _T("schedule_des");
		break;
	case fild_schedule_tim:
		str = _T("schedule_tim");
		break;
	case fild_schedule_percent:
		str = _T("schedule_percent");
		break;
	case fild_update_time:
		str = _T("update_time");
		break;
	}
	return str;
}
BOOL CTaskDaoImp::GetScheduleByPer( int taskId, int percent, CSchedule &obj )
{
	CString str;
	str.Format(_T("select * from %s where task_id = %d and schedule_percent = %d"),
		 m_scheduleName, taskId, percent);
	if ( false == m_pDaoSupportInc->ExecuteSql( str) )
	{
		return excSqlError;
	}
	CAdoRecordset* pRecordSet = m_pDaoSupportInc->GetRecordSet();
	if (  pRecordSet->IsEOF() )
	{
		return notfind;
	}
	FillScheduleFromFild( pRecordSet, obj );
	return userExcOk;
}
BOOL CTaskDaoImp::GetScheduleByUpdateDate( int taskId, const COleDateTime &day, CSchedule &obj )
{
	CString str;
	CString dateStr = day.Format( _T("%Y-%m-%d") );
	str.Format(_T("select * from %s where task_id = %d and update_time = '%s'"),
		m_scheduleName, taskId, dateStr );
	if ( false == m_pDaoSupportInc->ExecuteSql( str) )
	{
		return excSqlError;
	}
	CAdoRecordset* pRecordSet = m_pDaoSupportInc->GetRecordSet();
	if (  pRecordSet->IsEOF() )
	{
		return notfind;
	}
	FillScheduleFromFild( pRecordSet, obj );
	return userExcOk;
}
BOOL CTaskDaoImp::UpdateSchedule( int id, const TaskIdStringMap &fields )
{//比如更新点评
	//BOOL ret  = ValidateTaskExit( id )  ;
	//if ( userExcOk != ret )
	//{
	//	return ret;
	//}
	CString str;
	str.Format( _T("update %s t set "), m_scheduleName );
	TaskIdStringMap::const_iterator itor = fields.begin();
	CString tempStr,fieldStr;
	while( itor != fields.end()  )
	{
		fieldStr = GetSCheduleFieldString( itor->first);
		if ( fieldStr.GetLength() <= 0 )
		{
			return paraError;
		}
		tempStr.Format(_T("t.%s = %s,"), fieldStr, itor->second );
		str += tempStr;
		itor++;
	}
	str.Delete( str.GetLength() - 1 );
	CString sqlString;
	sqlString.Format( _T("%s where t.schedule_id = %d"), str, id );
	if ( FALSE == m_pDaoSupportInc->Execute(sqlString) )
	{
		return excSqlError;
	}
	return userExcOk;
}
//
BOOL  CTaskDaoImp::GetChildTask( int taskId, int st ,int ownerId, ListTask &task )
{//获取子任务
	CString str;
	str.Format( _T("select * from %s where "), m_taskTableName );
	CString temp;
	BOOL hasAnd = FALSE;
	if ( st > 0 )
	{
		temp.Format(_T(" task_statusid = %d  and "), st );
		str += temp;
	}
	if ( ownerId > 0 )
	{
		temp.Format(_T(" task_ownerid = %d and "), ownerId );
		str += temp;
	}
	temp.Format( _T(" task_id in ( select task_id from %s where father_id = %d ) and task_attri >= 0 order by task_plan_starttime "),
		 m_fatherTableName, taskId );
	str += temp;
	return ExcSqlTaskList( str,task );
}
BOOL  CTaskDaoImp::IsHasChildTask( int taskId, BOOL &isHasChild )
{
	CString str;
	str.Format( _T("select id from %s where father_id = %d "), m_fatherTableName, taskId );
	if ( FALSE == m_pDaoSupportInc->ExecuteSql(str) )
	{
		return excSqlError;
	}
	if (  m_pDaoSupportInc->GetRecordSet()->IsEOF() )
	{
		isHasChild = FALSE;
	}else
	{
		isHasChild = TRUE;
	}
	return userExcOk;
}

BOOL  CTaskDaoImp::GetFatherTask( int taskId, CTaskObj &father )
{//获取父任务
	CString str;
	str.Format( _T("select * from %s where task_attri > 0 and\
				    task_id in (select father_id from %s where task_id = %d) "),
					m_taskTableName, m_fatherTableName, taskId );
	if ( FALSE == m_pDaoSupportInc->ExecuteSql(str) )
	{
		return excSqlError;
	}
	CAdoRecordset* pRecordSet = m_pDaoSupportInc->GetRecordSet();
	int cnt = 0;
	cnt = pRecordSet->GetRecordCount();
	if (  pRecordSet->IsEOF() )
	{
		return notfind;
	}
	FillTaskFromFild( pRecordSet, father );
	return userExcOk;
}
//const CString  taskTableRowName[] = 
//{
//	_T("task_id"), _T("task_attri"), _T("task_name"),
//	_T("task_des"), _T("task_ownerid"), _T("task_project_id"),
//	_T("task_plan_starttime"),_T("task_plan_closetime"), _T("task_plan_period"),
//	_T("task_ac_starttime"),_T("task_ac_closetime"),_T("task_ac_period"),
//	_T("task_type"),_T("task_score"),_T("forecast_task_score"),
//	_T("task_score_des"),_T("task_auditorid"),_T("task_review_date"),
//	_T("task_score_date"),_T("task_statusid"),_T("task_dif"),
//	_T("task_createTime"),_T("task_timeType_id"),
//
//};

BOOL CTaskDaoImp::GetTaskByOwnerId( int userId, ListTask &listObj, int year /*= 0*/, int quater /*= 0*/ ) 
{
	CString str;
	str.Format( _T("select * from %s where task_ownerid = %d and task_attri >= 0") , m_taskTableName, userId );
	str += GetQuarterOfYearFilfterStr( year, quater, 0 );
	return ExcSqlTaskList( str, listObj );
}
BOOL CTaskDaoImp::GetTaskByOwernerId( int owernId, EnumStaus status, ListTask &listObj, int year /*= 0*/, int quater /*= 0*/)
{
	CString str;
	if ( status > 0)
	{
		str.Format( _T("select * from %s where task_ownerid = %d and task_statusid = %d and task_attri >= 0") , m_taskTableName, owernId, status );				//获取正在进行的任务
	}else
	{
		str.Format( _T("select * from %s where task_ownerid = %d  and task_attri >= 0") , m_taskTableName, owernId );											//获取所有的任务
	}
	str += GetQuarterOfYearFilfterStr( year, quater, status );
	return ExcSqlTaskList( str, listObj );
}
BOOL CTaskDaoImp::GetPerformProjectTask( int year, int quater, int owerId, ListTask &listObj )
{
	CString str;
	str.Format( _T("select * from %s where task_ownerid = %d  and task_attri >= 0 /*and task_project_id > 0*/ and task_statusid != %d ") , 
		m_taskTableName, owerId, newTasking );
	str += GetQuarterOfYearFilfterStr( year, quater, 0 );
	return ExcSqlTaskList( str, listObj );
}
BOOL CTaskDaoImp::GetPerformDepartTask( int year, int quater, int owerId, ListTask &listObj )
{
	CString str;
	str.Format( _T("select * from %s where task_ownerid = %d  and task_attri >= 0 and task_project_id <= 0 and task_statusid != %d ") , 
		m_taskTableName, owerId, newTasking );
	str += GetQuarterOfYearFilfterStr( year, quater, 0 );
	return ExcSqlTaskList( str, listObj );
}
//查找任务时间包含某天的任务,且任务不是关闭或中止
BOOL CTaskDaoImp::GetTaskByDate( int ownerId, const COleDateTime &day, ListTask &listObj )
{
	CString str;
	str.Format( _T("select * from %s where task_ownerid = %d  and task_attri >= 0 and task_statusid != %d and task_statusid != %d and task_statusid != %d and task_plan_starttime <= '%s' and task_plan_closetime >= '%s' "),
		m_taskTableName, ownerId, waitScore, midStop, task_close, day.Format(_T("%Y-%m-%d")), day.Format(_T("%Y-%m-%d")) );
	return ExcSqlTaskList( str, listObj );
}
//2015-9-10 add 添加季度筛选任务的条件字符串
CString CTaskDaoImp::GetQuarterOfYearFilfterStr( int year, int  quarter, int st )
{
	CString str;
	CString beginDayStr, endDayStr;
	if ( year <= 2000 )
	{
		return str;
	}
	beginDayStr.Format(_T("%04d-"), year );
	endDayStr = beginDayStr;
    switch (quarter)
    {
	case 1:
		beginDayStr += _T("1-1");
		endDayStr   += _T("3-31");
		break;
	case 2:
		beginDayStr += _T("4-1");
		endDayStr   += _T("6-30");
		break;
	case 3:
		beginDayStr += _T("7-1");
		endDayStr   += _T("9-30");
		break;
	case  4:
		beginDayStr += _T("10-1");
		endDayStr   += _T("12-31");
		break;
	default:
		return str;
    }
	if ( st > 0 )
	{
		str.Format(_T(" and ((task_ac_starttime >= '%s' and task_ac_starttime <= '%s') or ( task_statusid <= 3 and task_plan_starttime >= '%s' and task_plan_starttime <= '%s')\
			             or (task_ac_closetime >= '%s' and task_ac_closetime <= '%s') or ( task_score <= 0 and task_plan_closetime >= '%s' and task_plan_closetime <= '%s') )"),
			beginDayStr, endDayStr, beginDayStr, endDayStr, beginDayStr, endDayStr, beginDayStr, endDayStr );
	}else
	{
		str.Format(_T(" and ( (task_ac_starttime >= '%s' and task_ac_starttime <= '%s') or ( task_statusid <= 3 and task_plan_starttime >= '%s' and task_plan_starttime <= '%s') or\
					  (task_ac_closetime >= '%s' and task_ac_closetime <= '%s') or ( task_score <= 0 and task_plan_closetime >= '%s' and task_plan_closetime <= '%s') \
					  or (task_statusid = %d and task_ac_starttime <= '%s') \
					  or (task_statusid = %d and task_plan_starttime <= '%s') )"),

					  beginDayStr, endDayStr, beginDayStr, endDayStr,
					  beginDayStr, endDayStr, beginDayStr, endDayStr, ongoing, endDayStr, task_suspend, endDayStr);

		/*str.Format(_T(" and ( ( task_statusid > 3 and task_ac_starttime <= '%s') or ( task_statusid <= 3 and task_plan_starttime <= '%s') or\
					  (task_ac_closetime >= '%s' and task_ac_closetime <= '%s') or ( (task_statusid != %d && task_statusid != %d ) and task_plan_closetime >= '%s' and task_plan_closetime <= '%s') \
					     or (task_statusid = %d and task_ac_starttime <= '%s') \
						 or (task_statusid = %d and task_ac_starttime <= '%s') )"),
			             endDayStr,  endDayStr,
			             beginDayStr, endDayStr, task_close, midStop, beginDayStr, endDayStr, ongoing, endDayStr, task_suspend, endDayStr);*/
	}

	return str;
}

BOOL CTaskDaoImp::GetNewingTaskByCreateId( int createId, ListTask &listObj )
{
	CString str;
	str.Format( _T("select * from %s where creatorid = %d and task_statusid = %d and task_attri >= 0 ") , m_taskTableName, createId, newTasking );
	return ExcSqlTaskList( str, listObj );
}
BOOL CTaskDaoImp::GetNewTaskButNotOwer( int createId, ListTask &listObj )
{
	CString str;
	str.Format( _T("select * from %s where creatorid = %d and task_statusid = %d and task_ownerid != %d and task_attri >= 0 ") , m_taskTableName, createId, newTasking, createId );
	return ExcSqlTaskList( str, listObj );
}
void CTaskDaoImp::FillReslutList( CAdoRecordset* pRecordSet , ListTask &listObj) const
{
	//pRecordSet->MoveFirst();
	while( !pRecordSet->IsEOF() )
	{
		CTaskObj obj;
		FillTaskFromFild( pRecordSet, obj );
		listObj.push_back( obj );
		pRecordSet->MoveNext();
	}
}
BOOL CTaskDaoImp::ValidateTaskExit( int id )
{
	CString str;
	str.Format( _T("select * from %s where task_id = %d") , m_taskTableName, id );
	if ( FALSE == m_pDaoSupportInc->ExecuteSql(str) )
	{
		return excSqlError;
	}
	CAdoRecordset* pRecordSet = m_pDaoSupportInc->GetRecordSet();
	if ( pRecordSet->IsEOF() )
	{
		return notfind;
	}
	return userExcOk;
}
BOOL CTaskDaoImp::GetTaskContentById( int id, CTaskObj& obj )
{
	CString str;
	str.Format( _T("select * from %s where task_id = %d") , m_taskTableName, id );
	if ( FALSE == m_pDaoSupportInc->ExecuteSql(str) )
	{
		return excSqlError;
	}
	CAdoRecordset* pRecordSet = m_pDaoSupportInc->GetRecordSet();
	if ( pRecordSet->IsEOF() )
	{
		return notfind;
	}
	FillTaskFromFild( pRecordSet, obj );

	return userExcOk;

}
void CTaskDaoImp::FillTaskFromFild(  CAdoRecordset* pRecordSet, CTaskObj &obj ) const
{
	CString value = (*pRecordSet)[_T("task_id")].ToString();
	if ( value.IsEmpty() == FALSE )
	{
		obj.SetId( (*pRecordSet)[_T("task_id")] );
		obj.SetAttribute( (*pRecordSet)[_T("task_attri")] );
		obj.SetName( (*pRecordSet)[_T("task_name")].ToString() );
		obj.SetDec( (*pRecordSet)[_T("task_des")].ToString() );
		obj.SetOwnerId( (*pRecordSet)[_T("task_ownerid")] );
		obj.SetCreateId( (*pRecordSet)[_T("creatorid")] );
		obj.SetProjctId( (*pRecordSet)[_T("task_project_id")] );

		obj.SetPanStartDateStr( (*pRecordSet)[_T("task_plan_starttime")].ToString() );
		obj.SetPanEndDateStr( (*pRecordSet)[_T("task_plan_closetime")].ToString() );
		obj.SetPlanWorkHour( (*pRecordSet)[_T("task_plan_period")] );
		obj.SetFactStartDateStr( (*pRecordSet)[_T("task_ac_starttime")].ToString() );
		obj.SetFactEndDateStr( (*pRecordSet)[_T("task_ac_closetime")].ToString());
		obj.SetFactWorkHour( (*pRecordSet)[_T("task_ac_period")] );

		obj.SetType( (*pRecordSet)[_T("task_type")].ToString() );
		obj.SetScore( (*pRecordSet)[_T("task_score")] );
		obj.SetForecastScore( (*pRecordSet)[_T("forecast_task_score")] );
		obj.SetScoreDec( (*pRecordSet)[_T("task_score_des")] );
		obj.SetMakeScoreUserId( (*pRecordSet)[_T("task_auditorid")] );
		obj.SetPublishTimeStr( (*pRecordSet)[_T("task_review_date")].ToString() );
		obj.SetScoreTimeStr( (*pRecordSet)[_T("task_score_date")].ToString() );
		obj.SetTaskStatus( (*pRecordSet)[_T("task_statusid")] );
		obj.SetDifficultyParam( (*pRecordSet)[_T("task_dif")] );
		obj.SetCreateTimeStr( (*pRecordSet)[_T("task_createTime")].ToString() );
		obj.SetWorkTypeId(  (*pRecordSet)[_T("task_timeType_id")] );

		obj.SetPrjReview( (*pRecordSet)[_T("promanager_comment")].ToString() );
		obj.SetReview( (*pRecordSet)[_T("depmanager_comment")].ToString() );
		obj.SetCntLeakUpdate( (*pRecordSet)[_T("task_update_watch")] );
		//
		obj.SetCheckBeginDateStr( (*pRecordSet)[_T("task_check_begin")].ToString() );
		obj.SetTaskFlag( (*pRecordSet)[_T("task_flag")]  );
	}
	

}
BOOL CTaskDaoImp::GetScheduleContentByTaskId( int taskId, ListSchedule& obj )
{
	CString str;
	str.Format( _T("select * from %s where task_id = %d and  schedule_des != '' order by update_time desc") , m_scheduleName, taskId );
	return  ExcSqlScheduleList( str, obj );
}
BOOL CTaskDaoImp::GetScheduleContentById( int id, CSchedule& obj )
{
	CString str;
	str.Format( _T("select * from %s where schedule_id = %d") , m_scheduleName, id );
	if ( FALSE == m_pDaoSupportInc->ExecuteSql(str) )
	{
		return excSqlError;
	}
	CAdoRecordset* pRecordSet = m_pDaoSupportInc->GetRecordSet();
	if (  pRecordSet->IsEOF() )
	{
		return notfind;
	}
	FillScheduleFromFild( pRecordSet, obj );
	return userExcOk;
}
//获取最新记录
BOOL CTaskDaoImp::GetMaxNewScheduleByTaskId( int taskid, CSchedule& obj )
{
	CString str;
	str.Format( _T("select * from %s where  update_time=(select max(update_time) from %s where task_id = %d and schedule_percent > 0) and task_id = %d") , m_scheduleName, m_scheduleName, taskid, taskid );
	if ( FALSE == m_pDaoSupportInc->ExecuteSql(str) )
	{
		return excSqlError;
	}
	CAdoRecordset* pRecordSet = m_pDaoSupportInc->GetRecordSet();
	if (  pRecordSet->IsEOF() )
	{
		return notfind;
	}
	FillScheduleFromFild( pRecordSet, obj );
	return userExcOk;
}
BOOL CTaskDaoImp::GetWorkTimeTypeStr( int id, CString &retStr )
{//获取工时类型
	CString str;
	str.Format( _T("select * from %s where id = %d") , m_workTypeTableName, id );
	if ( FALSE == m_pDaoSupportInc->ExecuteSql(str) )
	{
		return excSqlError;
	}
	CAdoRecordset* pRecordSet = m_pDaoSupportInc->GetRecordSet();

	if (  pRecordSet->IsEOF() )
	{
		return notfind;
	}
	pRecordSet->MoveFirst();
	retStr = (*pRecordSet)[_T("timeType_name")].ToString();

	return userExcOk;
}
BOOL  CTaskDaoImp::GetAllWorkTimeType( TaskIdStringMap &obj) 
{
	CString str;
	str.Format( _T("select * from %s") , m_workTypeTableName );
	if ( FALSE == m_pDaoSupportInc->ExecuteSql(str) )
	{
		return excSqlError;
	}
	CAdoRecordset* pRecordSet = m_pDaoSupportInc->GetRecordSet();
	//pRecordSet->MoveFirst();
	while ( !pRecordSet->IsEOF() )
	{
		int id = (*pRecordSet)[_T("id")];
		CString str = (*pRecordSet)[_T("timeType_name")].ToString();
		obj.insert( std::make_pair( id, str)  );
		pRecordSet->MoveNext();
	}
	return userExcOk;
}
BOOL  CTaskDaoImp::UpdateWorkTimeType( int id, const CString &name )
{
	CString str;
	str.Format( _T("select * from %s where timeType_name = '%s'") , m_workTypeTableName,
		         name );
	if ( FALSE == m_pDaoSupportInc->ExecuteSql(str) )
	{
		return excSqlError;
	}
	CAdoRecordset* pRecordSet = m_pDaoSupportInc->GetRecordSet();
	if ( !pRecordSet->IsEOF() )
	{
		return insertNameRepeat;
	}
	str.Format( _T("update  %s t set t.timeType_name = '%s' where t.id = %d "), m_workTypeTableName, name, id );
	if ( FALSE == m_pDaoSupportInc->Execute(str) )
	{
		return excSqlError;
	}
	return userExcOk;
}
BOOL  CTaskDaoImp::AddWorkType(const CString &name )
{
	CString str;
	str.Format( _T("select * from %s where timeType_name = '%s'") , m_workTypeTableName,
		name );
	if ( FALSE == m_pDaoSupportInc->ExecuteSql(str) )
	{
		return excSqlError;
	}
	CAdoRecordset* pRecordSet = m_pDaoSupportInc->GetRecordSet();
	if ( !pRecordSet->IsEOF() )
	{
		return insertNameRepeat;
	}
	str.Format( _T("insert %s ( timeType_name ) values ( '%s')") , m_workTypeTableName,
		name );
	if ( FALSE == m_pDaoSupportInc->Execute(str) )
	{
		return excSqlError;
	}
	return userExcOk;
}
BOOL   CTaskDaoImp::DeleteWorkTypeString( int id )
{
	CString str;
	str.Format( _T("delete  from %s where id = %d") , m_workTypeTableName,
		id );
	if ( FALSE == m_pDaoSupportInc->Execute(str) )
	{
		return excSqlError;
	}
	return userExcOk;
}

BOOL    CTaskDaoImp::GetTaskStatusMap( CTaskStaus &obj )
{
	CString str( _T("select * from status") );
	if ( FALSE == m_pDaoSupportInc->ExecuteSql(str) )
	{
		return excSqlError;
	}
	CAdoRecordset* pRecordSet = m_pDaoSupportInc->GetRecordSet();
	int id = 0;
	while( !pRecordSet->IsEOF() )
	{
		id = (*pRecordSet)[_T("id")];
		str = (*pRecordSet)[_T("status_name")].ToString();
		obj.InsertStaus( id, str );
		pRecordSet->MoveNext();
	}
	return userExcOk;
}
BOOL    CTaskDaoImp::GetLastInsertId( int & ret )
{
	CString str = _T("select last_insert_id()");
	if ( FALSE == m_pDaoSupportInc->ExecuteSql(str) )
	{
		return excSqlError;
	}
	CAdoRecordset* pRecordSet = m_pDaoSupportInc->GetRecordSet();
	if ( pRecordSet->IsEOF() )
	{
		return notfind;
	}
	ret = (*pRecordSet)[_T("last_insert_id()")];
	return userExcOk;
}
BOOL  CTaskDaoImp::GetTaskByReviewId( int reviewId, EnumStaus status, ListTask &listObj )
{
	CString str;
	str.Format( _T("select * from %s where task_auditorid = %d and task_statusid = %d and task_attri >= 0 ") , m_taskTableName, reviewId, status );
	return  ExcSqlTaskList( str, listObj );
}
BOOL CTaskDaoImp::GetTaskByProjectManger( int prjMangerId, EnumStaus status,ListTask &listObj )
{
	CString str;
	str.Format( _T("select * from %s where task_statusid = %d and task_attri >= 0 and task_project_id > 0 \
		and task_project_id in (select project_id from project where project_managerid = %d)" ) , 
		m_taskTableName,  status, prjMangerId );
	return  ExcSqlTaskList( str, listObj );
}
void CTaskDaoImp::FillScheduleFromFild( CAdoRecordset* pRecordSet, CSchedule &schedule  ) const
{
	schedule.SetId( (*pRecordSet)[_T("schedule_id")] );
	schedule.SetTaskId( (*pRecordSet)[_T("task_id")] );
	schedule.SetUpdateContent( (*pRecordSet)[_T("schedule_des")].ToString());
	schedule.SetWorkHours( (*pRecordSet)[_T("schedule_tim")] );
	schedule.SetPercent( (*pRecordSet)[_T("schedule_percent")]);
	schedule.SetUpdateTimeStr( (*pRecordSet)[_T("update_time")].ToString() );
	schedule.SetWorkTypeId( (*pRecordSet)[_T("timeType_id")] );
}
void   CTaskDaoImp::FillScheduleList( CAdoRecordset* pRecordSet , ListSchedule &listObj ) const
{
	//pRecordSet->MoveFirst();
	while( !pRecordSet->IsEOF() )
	{
		CSchedule obj;
		FillScheduleFromFild( pRecordSet, obj );
		listObj.push_back( obj );
		pRecordSet->MoveNext();
	}
}
BOOL CTaskDaoImp::GetScheduleContentByTaskId( int taskId, ReviewType st, ListSchedule& obj )
{
	CString str;
	str.Format( _T("select * from %s where task_id = %d and skipReview = %d order by update_time desc ") , m_scheduleName, taskId, (int)st );
	return ExcSqlScheduleList( str, obj );
}
BOOL CTaskDaoImp::ExcSqlScheduleList( const CString &str, ListSchedule &obj )
{
	if ( FALSE == m_pDaoSupportInc->ExecuteSql(str) )
	{
		return excSqlError;
	}
	CAdoRecordset* pRecordSet = m_pDaoSupportInc->GetRecordSet();
	FillScheduleList( pRecordSet , obj );
	return userExcOk;
}
BOOL   CTaskDaoImp::ExcSqlTaskList( const CString &str,  ListTask &listObj )
{
	if ( FALSE == m_pDaoSupportInc->ExecuteSql(str) )
	{
		return excSqlError;
	}
	CAdoRecordset* pRecordSet = m_pDaoSupportInc->GetRecordSet();
	FillReslutList( pRecordSet , listObj );
	return userExcOk;
}
BOOL  CTaskDaoImp::GetScheduleContentByPrjTaskId( int taskId, ReviewType st, ListSchedule & obj )
{
	CString str;
	str.Format( _T("select * from %s where task_id = %d and skipPrjReview = %d  order by update_time ") , m_scheduleName, taskId, st );
	return ExcSqlScheduleList( str, obj );
}
BOOL CTaskDaoImp::GetScheduleContentByDate( int ownerId, const COleDateTime &beginTime, 
	           const COleDateTime &endTime, ListSchedule &obj)
{
	CString str;
	str.Format( _T("select * from %s where \
				   task_id in (select task_id from %s where task_ownerid = %d and task_attri >= 0 order by task_timeType_id) and\
		                update_time >= '%s' and update_time <= '%s'") , m_scheduleName, m_taskTableName,ownerId,
		           beginTime.Format(_T("%Y-%m-%d %H:%M:%S")), endTime.Format(_T("%Y-%m-%d %H:%M:%S")) );
	return ExcSqlScheduleList( str, obj );
}

BOOL CTaskDaoImp::GetTaskByProjectId( int prjId, EnumStaus st,ListTask &listTask )
{
	CString str;
	str.Format( _T("select * from %s where task_project_id = %d and task_statusid = %d and task_attri >= 0 "), m_taskTableName , prjId, st );
	return ExcSqlTaskList( str, listTask );
}
BOOL CTaskDaoImp::GetTaskByProjectId( int prjId, ListTask &listTask )
{
	CString str;
	str.Format( _T("select * from %s where task_project_id = %d  and task_attri >= 0 "), m_taskTableName, prjId );
	return ExcSqlTaskList( str, listTask );
}
BOOL CTaskDaoImp::DeleteTaskById( int taskId )
{
	CString str;
	str.Format(_T("delete from %s where task_id = %d "), m_fatherTableName, taskId );
	if ( TRUE != m_pDaoSupportInc->Execute( str ))
	{
		return excSqlError;
	}
	str.Format(_T("delete from %s where task_id = %d"), m_scheduleName,taskId );
	if ( TRUE != m_pDaoSupportInc->Execute( str ))
	{
		return excSqlError;
	}
	str.Format(_T("delete from change_task where change_taskid = %d"), taskId );
	if ( TRUE != m_pDaoSupportInc->Execute( str ))
	{
		return excSqlError;
	}
	str.Format( _T( "delete from %s where task_id = %d"), m_taskTableName, taskId );
	if ( TRUE != m_pDaoSupportInc->Execute( str ))
	{
		return excSqlError;
	}
	return userExcOk;
}
BOOL CTaskDaoImp::DeleteNoneShcdule( int taskId )
{
	CString str;
	str.Format( _T( "delete from %s where task_id = %d and schedule_des='' "), m_scheduleName, taskId );
	if ( TRUE != m_pDaoSupportInc->Execute( str ))
	{
		return excSqlError;
	}
	return userExcOk;
}
BOOL CTaskDaoImp::GetProjectTaskNoFatherHasChild( int prjId, int taskSt, int ownerId, ListTask &obj, BOOL taskFlag/* = 0*/  )
{
	CString str;
	str.Format( _T( "select * from %s where task_project_id = %d and task_attri >= 0" ), m_taskTableName, prjId);
	CString tempStr;
	if ( taskFlag > 0 )
	{
		tempStr.Format(_T(" and task_flag = %d "), taskFlag );
		str += tempStr;
	}
 	if ( taskSt > 0 )
	{
	   tempStr.Format(_T(" and task_statusid = %d "), taskSt );
	   str += tempStr;
	}
	if ( ownerId > 0 )
	{
		tempStr.Format(_T(" and task_ownerid = %d "), ownerId );
		str += tempStr;
	}
	tempStr.Format(_T(" and task_id not in (select task_id from %s) and \
					  task_id in (select father_id from %s) order by task_plan_starttime "),  m_fatherTableName, m_fatherTableName );
	str += tempStr;
	return ExcSqlTaskList( str, obj );
}


BOOL CTaskDaoImp::GetProjectTaskHasFahter( int prjId, int taskSt, int ownerId, ListTask &fahterId, ListTask &obj, BOOL taskFlag/* = 0*/ )
{
	CString str;
	str.Format( _T( "select * from %s where task_project_id = %d and task_attri >= 0" ), m_taskTableName, prjId);
	CString tempStr;
	if ( taskFlag > 0 )
	{
	    tempStr.Format( _T(" and task_flag = %d "), taskFlag );
		str += tempStr;
	}
	if ( taskSt > 0 )
	{
		tempStr.Format(_T(" and task_statusid = %d "), taskSt );
		str += tempStr;
	}
	if ( ownerId > 0 )
	{
		tempStr.Format(_T(" and task_ownerid = %d "), ownerId );
		str += tempStr;
	}
	//if ( fahterId.size() > 0 )
	//{
		BOOL ret ;
		CString fatherIdStr;
	    CString sqlStr;
		int cnt = 0;
	/*	for ( ListTaskItor itor = fahterId.begin(); itor != fahterId.end();  )
		{*/
			fatherIdStr.Format( _T("and task_id  in ( select task_id from %s ) "),
				 m_fatherTableName/*,itor->GetId()*/ );
  			sqlStr = str + fatherIdStr; 
			if ( userExcOk != (ret = ExcSqlTaskList( sqlStr, obj )) )
			{
				return ret;
			}
		//	itor++;
		//	cnt++;
		//}
		return ret;
	//}
	return ExcSqlTaskList( str, obj );
}

BOOL CTaskDaoImp::GetProjectTask( int prjId, int taskSt, int ownerId, ListTask &obj )
{
	CString str;
	str.Format( _T( "select * from %s where task_project_id = %d and task_attri >= 0" ), m_taskTableName, prjId);
	CString tempStr;
	if ( taskSt > 0 )
	{
		tempStr.Format(_T(" and task_statusid = %d "), taskSt );
		str += tempStr;
	}
	if ( ownerId > 0 )
	{
		tempStr.Format(_T(" and task_ownerid = %d "), ownerId );
		str += tempStr;
	}
	
	return ExcSqlTaskList( str, obj );
}
BOOL CTaskDaoImp::GetProjectTaskNoFatherNoChild( int prjId, int taskSt, int ownerId, ListTask &obj, BOOL taskFlag /*= 0*/ )
{
	CString str;
	str.Format( _T( "select * from %s where task_project_id = %d and task_attri >= 0" ), m_taskTableName, prjId);
	CString tempStr;
	if ( taskFlag > 0 )
	{
		tempStr.Format(_T(" and task_flag = %d "), taskFlag );
		str += tempStr;
	}
	if ( taskSt > 0 )
	{
		tempStr.Format(_T(" and task_statusid = %d "), taskSt );
		str += tempStr;
	}
	if ( ownerId > 0 )
	{
		tempStr.Format(_T(" and task_ownerid = %d "), ownerId );
		str += tempStr;
	}
	tempStr.Format( _T(" and task_id not in (select task_id from %s) and task_id not in (select father_id from %s) order by task_plan_starttime"), 
						m_fatherTableName, m_fatherTableName );
	str += tempStr;
	return ExcSqlTaskList( str, obj );
}
BOOL CTaskDaoImp::GetUnUpdateDate( int taskId, ListDate &lst )
{
	CString str;
	str.Format(_T("select leak_date from %s where task_id = %d"), m_unUpateDateTableName, taskId );
	if ( FALSE == m_pDaoSupportInc->ExecuteSql(str) )
	{
		return excSqlError;
	}
	CAdoRecordset* pRecordSet = m_pDaoSupportInc->GetRecordSet();
	while( !pRecordSet->IsEOF() )
	{
		str = (*pRecordSet)[_T("leak_date")].ToString();
		COleDateTime date;
		date.ParseDateTime( str, VAR_DATEVALUEONLY );
		lst.push_back( date );
		pRecordSet->MoveNext();
	}
	return userExcOk;
}
BOOL CTaskDaoImp::AddUnUpdateDate( int taskId, const COleDateTime &date )
{
	CString str;
	CString dateStr;
	dateStr = date.Format(_T("%Y-%m-%d"));
	str.Format(_T("select Id from %s where task_id = %d and leak_date = '%s'"), 
		m_unUpateDateTableName, taskId, dateStr );
	if ( FALSE == m_pDaoSupportInc->ExecuteSql(str) )
	{
		return excSqlError;
	}
	CAdoRecordset* pRecordSet = m_pDaoSupportInc->GetRecordSet();
    if( !pRecordSet->IsEOF() )
	{
		return userExcOk;
	}
	str.Format(_T("insert into %s (task_id, leak_date) values (%d, '%s') "),
		 m_unUpateDateTableName, taskId, dateStr );
	if ( FALSE == m_pDaoSupportInc->Execute(str) )
	{
		return excSqlError;
	}
	return userExcOk;
}
BOOL CTaskDaoImp::DeleteUnUpdateDate( int taskId, const COleDateTime &date )
{
	CString str;
	str.Format(_T("delete from %s where task_id = %d and leak_date = '%s' "),
		m_unUpateDateTableName, taskId, date.Format(_T("%Y-%m-%d")) );
	if ( FALSE == m_pDaoSupportInc->ExecuteSql(str) )
	{
		return excSqlError;
	}
	return userExcOk;
}
BOOL CTaskDaoImp::DeleteUnUpdateDateByUserId( int userId, const COleDateTime &date )
{
	CString str;
	str.Format(_T("delete from %s where leak_date = '%s' and task_id in (select task_id from %s where task_ownerid = %d) "),
		m_unUpateDateTableName,  date.Format(_T("%Y-%m-%d")), m_taskTableName, userId );

	if ( FALSE == m_pDaoSupportInc->ExecuteSql(str) )
	{
		return excSqlError;
	}
	return userExcOk;
}
BOOL CTaskDaoImp::GetUnUpdateCnt( int taskId, int &cnt )
{
	CString str;
	str.Format(_T("select count(*) as num from %s where task_id = %d "),
		m_unUpateDateTableName, taskId );
	cnt = 0;
	if ( FALSE == m_pDaoSupportInc->ExecuteSql(str) )
	{
		return excSqlError;
	}
	CAdoRecordset* pRecordSet = m_pDaoSupportInc->GetRecordSet();
	if( !pRecordSet->IsEOF() )
	{
		cnt = (*pRecordSet)[_T("num")] ;
	}
	return userExcOk;
}
BOOL CTaskDaoImp::GetTaskWorkHourSum(int ownerId, int timeType, const COleDateTime &time, double &relust )
{
	CString str;
	if ( timeType < 0 )
	{
		str.Format( _T("select sum(schedule_tim) as task_hour from %s where update_time = '%s' and \
					   task_id in (select task_id from %s where task_ownerid = %d and task_attri >= 0)"),
					   m_scheduleName, time.Format(_T("%Y-%m-%d")),m_taskTableName, ownerId );
	}else
	{
		//str.Format( _T("select sum(schedule_tim) as task_hour from %s where update_time = '%s' and \
		//			   task_id in (select task_id from %s where task_ownerid = %d and task_timeType_id = %d and task_attri >= 0)"),
		//			   m_scheduleName, time.Format(_T("%Y-%m-%d")),m_taskTableName, ownerId, timeType );
		//2015-9-28 由任务对象获取工时类型 改 由进度上获取工时类型
		str.Format( _T("select sum(schedule_tim) as task_hour from %s where update_time = '%s' and timeType_id = %d and \
					   task_id in (select task_id from %s where task_ownerid = %d and task_attri >= 0)"),
					   m_scheduleName, time.Format(_T("%Y-%m-%d")), timeType, m_taskTableName, ownerId  );
	}
	if ( FALSE == m_pDaoSupportInc->ExecuteSql(str) )
	{
		return excSqlError;
	}
	CAdoRecordset* pRecordSet = m_pDaoSupportInc->GetRecordSet();
	if ( pRecordSet->IsEOF() )
	{
		return notfind;
	}
	DbField value = (*pRecordSet)[_T("task_hour")];
	if ( !value.IsNull() )
	{
		relust = value;
	}else
	{
		relust = 0.0;
	}
	return userExcOk;
}
BOOL  CTaskDaoImp::GetTaskWorkHourSum( int taskId,  double &relust )
{
	CString str;
	str.Format( _T("select sum(schedule_tim) as task_hour from %s where task_id = %d"),
				   m_scheduleName, taskId );
	if ( FALSE == m_pDaoSupportInc->ExecuteSql(str) )
	{
		return excSqlError;
	}
	CAdoRecordset* pRecordSet = m_pDaoSupportInc->GetRecordSet();
	if ( pRecordSet->IsEOF() )
	{
		return notfind;
	}
	DbField value = (*pRecordSet)[_T("task_hour")];
	if ( !value.IsNull() )
	{
		relust = value;
	}else
	{
		relust = 0.0;
	}
	return userExcOk;
}
BOOL CTaskDaoImp::GetTaskWorkHourSum( int taskId, const COleDateTime &beginDate, const COleDateTime &endDate,double &relust )
{//[beginDate ~endDate]
	CString str;
	str.Format( _T("select sum(schedule_tim) as task_hour from %s where task_id = %d and update_time >= '%s' and update_time <= '%s'"),
		m_scheduleName, taskId, beginDate.Format(_T("%Y-%m-%d")),  endDate.Format(_T("%Y-%m-%d")));
	if ( FALSE == m_pDaoSupportInc->ExecuteSql(str) )
	{
		return excSqlError;
	}
	CAdoRecordset* pRecordSet = m_pDaoSupportInc->GetRecordSet();
	if ( pRecordSet->IsEOF() )
	{
		return notfind;
	}
	DbField value = (*pRecordSet)[_T("task_hour")];
	if ( !value.IsNull() )
	{
		relust = value;
	}else
	{
		relust = 0.0;
	}
	return userExcOk;
}
BOOL CTaskDaoImp::GetTaskHourSumExceptSt(int ownerId, int st, const COleDateTime &time, double &relust )
{
	CString str;
	str.Format( _T("select sum(schedule_tim) as task_hour from %s where update_time = '%s' and \
				   task_id in (select task_id from %s where task_ownerid = %d and task_statusid != %d and task_attri >= 0)"),
				   m_scheduleName, time.Format(_T("%Y-%m-%d")),m_taskTableName, ownerId, st );
	if ( FALSE == m_pDaoSupportInc->ExecuteSql(str) )
	{
		return excSqlError;
	}
	CAdoRecordset* pRecordSet = m_pDaoSupportInc->GetRecordSet();
	if ( pRecordSet->IsEOF() )
	{
		return notfind;
	}
	DbField value = (*pRecordSet)[_T("task_hour")];
	if ( !value.IsNull() )
	{
		relust = value;
	}else
	{
		relust = 0.0;
	}

	return userExcOk;
}