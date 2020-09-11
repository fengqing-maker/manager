#include "stdafx.h"
#include "ScheduleAction.h"
#include "Src/MainFrame/Dlg/FinisTaskDlg.h"
#include "Src/Tools/UtilityTool/UtilityTool.h"

CSCheduleAction::CSCheduleAction( CDaoFactory  *pObj ):CScheduleActionInc(pObj)
{
	ASSERT( pObj != NULL );
	m_pTaskDbDAo = pObj->GetTaskDbDao();
}

CSCheduleAction::~CSCheduleAction()
{

}
BOOL CSCheduleAction::GetMaxNewScheduleByTaskId( int taskId, CSchedule& obj )
{
	return  m_pTaskDbDAo->GetMaxNewScheduleByTaskId( taskId, obj );
}
BOOL CSCheduleAction::TaskFinishApply( const CTaskObj &obj, const COleDateTime &finishDate )
{
	CFinisTaskDlg dlg( obj, finishDate );
	dlg.DoModal();
	return userExcOk;
}
BOOL CSCheduleAction::InsertSchedule( const CSchedule &obj )
{
	CTaskObj taskObj;
	if (  userExcOk != m_pTaskDbDAo->GetTaskContentById( obj.GetTaskId(), taskObj) )
	{
		AfxMessageBox( _T("进度更新的关联任务不存在") );
		return paraError;
	}
	CSchedule oldSchdule;
	m_pTaskDbDAo->GetMaxNewScheduleByTaskId( taskObj.GetId(), oldSchdule );
	BOOL ret = 0;
	if ( oldSchdule.GetPercent() >= 100 && obj.GetUpdateTime() < oldSchdule.GetUpdateTime() )
	{
		return TaskFinishApply( taskObj, obj.GetUpdateTime() );
	}
	if ( obj.GetUpdateContent().GetLength() <= 0 )
	{
		AfxMessageBox(_T("更新进度内容不能为空"));
		return paraError;
	}
	double maxWorkHour = taskObj.GetPlanWorkHour()/* - taskObj.GetFactWorkHour()*/;
	CString str;
	if ( obj.GetWorkHours() < 0 /*|| obj.GetWorkHours() > maxWorkHour*/ )
	{
		str.Format( _T("此次进度工时填写有误，工时范围（0.0~%0.1f）"), maxWorkHour );
		AfxMessageBox( str );
		return paraError;
	}
	int per = obj.GetPercent();
	int minPer = 0/*oldSchdule.GetPercent()*/;
	if ( per <= minPer || per > 100 )
	{
		str.Format( _T("此次进度填写不正确,进度范围(%d~100]"), minPer );
		AfxMessageBox( str );
		return paraError;
	}
	CSchedule curDateSch;
	m_pTaskDbDAo->GetScheduleByUpdateDate( taskObj.GetId(), obj.GetUpdateTime(), curDateSch );
	ret = m_pTaskDbDAo->AddTaskSchedule( obj );
	if ( userExcOk == ret )
	{//更改对应任务的工时
		BOOL ret = userExcOk;
		if ( curDateSch.GetId() <= 0 || obj.GetWorkHours() != curDateSch.GetWorkHours() )
		{
			double workHour = taskObj.GetFactWorkHour() + obj.GetWorkHours() - curDateSch.GetWorkHours();
			CString stValue;
			stValue.Format( _T("%0.1f"), workHour );
			TaskIdStringMap filds;
			filds.insert( std::make_pair(taskFactWorkHour, stValue) );
			ret =  m_pTaskDbDAo->UpdateTaskField( obj.GetTaskId() , filds );
			if ( ret != userExcOk )
			{//
				AfxMessageBox( _T("更新实际工时失败请重试") );
			}
			taskObj.SetFactWorkHour( workHour );
		}
		if ( userExcOk == ret && per >= 100 )
		{//进行完成任务申请

			return TaskFinishApply( taskObj, obj.GetUpdateTime() );
		}

	}
	return ret;
}
BOOL  CSCheduleAction::OnReviewSchedule( const CTaskObj &obj, const CString &reviewName, const CString &reviewStr, int score  )
{//进度点评
	BOOL reRet;
	TaskIdStringMap fields;
	if ( score <-10 && score > 10 )
	{
		AfxMessageBox( _T("分数填写不正确，分数应在[-10~10]直接") );
		return paraError;
	}
	if ( score != 0 && reviewStr.IsEmpty() )
	{
		AfxMessageBox(_T("有点评分则需填写点评意见"));
		return paraError;
	}
	CString valueStr;
	CString tempReviewStr;
	tempReviewStr.Format( _T("%s %s更新 点评分%d 点评人%s"),  reviewStr, 
		COleDateTime::GetCurrentTime().Format(_T("%Y-%m-%d %H-%M-%S")), score, reviewName);
	CString oldReview = obj.GetReview();
	if ( !oldReview.IsEmpty() )
	{
		valueStr = _T("'") + oldReview + _T("\r\n") + tempReviewStr + _T("'"); 
	}else
	{
		valueStr = _T("'") + tempReviewStr + _T("'") ;
	}
	fields.insert( std::make_pair( fild_depmanager_comment, valueStr) );
	if ( score != 0 )
	{
		int tmpValue = obj.GetForecastScore() + score;
		valueStr.Format(_T("%d"), tmpValue );
		fields.insert( std::make_pair( fild_forecast_task_score, valueStr) );
	}
	reRet = m_pTaskDbDAo->UpdateTaskField( obj.GetId(), fields );
	return reRet;
}
BOOL  CSCheduleAction::OnPrjReviewSchedule( const CTaskObj &obj, const CString &reviewName, const CString &reviewStr )
{
	TaskIdStringMap fields;
	if ( reviewStr.IsEmpty() )
	{//分数描述为空
		AfxMessageBox(_T("项目经理点评意见不能为空"));
		return paraError;
	}
	CString valueStr;
	CString tempReviewStr;
	tempReviewStr.Format( _T("%s %s更新"),  reviewStr, 
		COleDateTime::GetCurrentTime().Format(_T("%Y-%m-%d %H-%M-%S")) );
	CString oldReview = obj.GetPrjReview();
	if ( !oldReview.IsEmpty() )
	{
		valueStr = _T("'") + oldReview + _T("\r\n") + tempReviewStr +_T("点评人")+ reviewName + _T("'"); 
	}else
	{
		valueStr = _T("'") + tempReviewStr + reviewName + _T("'") ;
	}
	fields.insert( std::make_pair( fild_promanager_comment, valueStr) );
	return m_pTaskDbDAo->UpdateTaskField( obj.GetId(), fields );
}
BOOL CSCheduleAction::GetScheduleContentById( int id, CSchedule& obj )
{
	return m_pTaskDbDAo->GetScheduleContentById( id, obj );
}
COleDateTime CSCheduleAction::GetLastSetUpdateData()
{
	COleDateTime currDateTime = COleDateTime::GetCurrentTime();
	COleDateTime currDate;
	BOOL isHoliday = FALSE;
	BOOL isNorWorkTime = TRUE;
	COleDateTimeSpan span( 1, 0, 0, 0);
	while( 1 )
	{
		int hour = currDateTime.GetHour();
		int min = currDateTime.GetMinute();
		if ( !( hour >= 18 || ( hour == 17 && min >= 30)) )
		{
			isNorWorkTime  = FALSE;
			currDateTime -= span;
		}
		isHoliday = TRUE;
		BOOL ret = m_pDbDao->GetSystemCfgDbDao()->IsHoliday( currDateTime, isHoliday );
		if ( FALSE == isHoliday || ret != userExcOk )
		{
			break;
		}
		if ( isNorWorkTime )
		{
			currDateTime -= span;
		}
	}
	currDate.SetDate( currDateTime.GetYear(),currDateTime.GetMonth(), currDateTime.GetDay() );
	return currDate;
}
BOOL CSCheduleAction::GetUnUpdateDateList( const CTaskObj &obj, ListDate &lst)
{
	if ( ongoing != obj.GetTaskStatus() )
	{
		return userExcOk;
	}
	COleDateTime lastDay = GetLastSetUpdateData();
	COleDateTime beginCheckDay = obj.GetCheckBeginDate();
	CString tmp = beginCheckDay.Format(_T("%Y-%m-%d"));//1111111111
	CString tmp2 = lastDay.Format(_T("%Y-%m-%d"));
	CSchedule schObj;
	CWorkHourDbInc* pWorkHour = m_pDbDao->GetWorkHourDbDao();
	BOOL reSetCheckDate = FALSE;
	COleDateTime lastPushDate;
	while( beginCheckDay <= lastDay )
	{
		COleDateTimeSpan span( 1, 0, 0, 0);
		//判断日期是否为节假日
		BOOL isHoliday  = FALSE;
		BOOL relust = m_pDbDao->GetSystemCfgDbDao()->IsHoliday( beginCheckDay, isHoliday );
		ASSERT( relust == userExcOk );
		CUser ownerUser;
		m_pDbDao->GetUserDao()->GetUserById( obj.GetOwnerId(), ownerUser );
		/*CSchedule schObj; 
		GetMaxNewScheduleByTaskId( obj.GetId(), schObj );*/
		if ( departure == ownerUser.GetWorkStatus() /*|| schObj.GetPercent() >= 100 */ ) 
		{
			isHoliday = TRUE;
		}
		double norTaskWorkHor = 0.0;
		relust = m_pDbDao->GetWorkHourDbDao()->GetNorTaskHourSum( obj.GetOwnerId(), beginCheckDay, norTaskWorkHor );
		//double taskWorkHour = 0.0;
		//relust = m_pTaskDbDAo->GetTaskWorkHourSum( obj.GetOwnerId(), -1,beginCheckDay, taskWorkHour );
		schObj.ReInit();
		relust = m_pTaskDbDAo->GetScheduleByUpdateDate( obj.GetId(), beginCheckDay, schObj );
		ASSERT( relust == userExcOk || notfind == relust );
		if ( FALSE == isHoliday && !( ( 0 == CUtilityTool::GetObj()->DoubleCmp( norTaskWorkHor /*+ taskWorkHour*/ - 7.5 ) || schObj.GetId() > 0 ) && lastDay != beginCheckDay) )
		{
			lst.push_back( beginCheckDay );
			lastPushDate = beginCheckDay;
			//判断是否填写请假
			/*CWorkHourData data;
			m_pDbDao->GetWorkHourDbDao()->GetTimeData( 0, obj.GetOwnerId(), beginCheckDay, data );*/
			/*if ( data.GetId() > 0 )
			{
			m_pTaskDbDAo->DeleteUnUpdateDate( obj.GetId(), beginCheckDay );
			} else*/ if ( beginCheckDay != lastDay )
			{//添加未有更新记录
				m_pTaskDbDAo->AddUnUpdateDate( obj.GetId(), beginCheckDay );
			}
			//
			if ( reSetCheckDate && beginCheckDay != obj.GetCheckBeginDate() )
			{//更改检查日期
				UpdateCheckBeginDate( obj, beginCheckDay );
				reSetCheckDate = TRUE;
			}
		}
		BOOL testBOOL = CUtilityTool::GetObj()->DoubleCmp( norTaskWorkHor /*+ taskWorkHour*/ - 7.5 );
		if ( TRUE == isHoliday || 0 == CUtilityTool::GetObj()->DoubleCmp( norTaskWorkHor /*+ taskWorkHour*/ - 7.5 )   )
		{
			if ( lastPushDate == beginCheckDay)
			{
				lst.pop_back();
			}
			m_pTaskDbDAo->DeleteUnUpdateDate( obj.GetId(), beginCheckDay );
			//2015/10/15假期 删除前一天的不良记录
			m_pTaskDbDAo->DeleteUnUpdateDate( obj.GetId(), beginCheckDay - span );
		}
		beginCheckDay += span;
	}
	return userExcOk;
}
BOOL CSCheduleAction::GetSchduleByDate( int taskId, const COleDateTime &date, CSchedule &obj )
{
	return m_pTaskDbDAo->GetScheduleByUpdateDate( taskId, date, obj);
}
BOOL CSCheduleAction::InsertEmptySchdule( int taskId, const  COleDateTime &date )
{
	return m_pTaskDbDAo->AddEmptySchedule( taskId, date );
}
BOOL CSCheduleAction::UpdateCheckBeginDate( const CTaskObj &obj,const COleDateTime &date )
{
	if ( obj.GetCheckBeginDate() > date )
	{
		CString value;
		value.Format(_T("'%s'"), date );
		return m_pTaskDbDAo->UpdateTaskField( obj.GetId(), fild_task_check_begin, value );
	}
	return userExcOk;
}
BOOL CSCheduleAction::OnModiyTaskScore(const CTaskObj &obj, int score, const CString &soreDec )
{
	if ( soreDec.IsEmpty() )
	{
		AfxMessageBox( _T("修改评分，必须填写评分说明") );
		return paraError;
	}
	if ( score > 120 || score < 0 )
	{
		AfxMessageBox( _T("评分数值越限") );
		return paraError;
	}
	CString str = obj.GetScoreDec();
	if ( str.IsEmpty() )
	{
		str.Format( _T("任务评分由%d修改为%d (%s),说明：%s"), obj.GetScore(), score, COleDateTime::GetCurrentTime().Format(_T("%Y-%m-%d %H:%M:%S")),  soreDec ) ;
	}else
	{
		str.Format( _T("\r\n任务评分由%d修改为%d (%s),说明：%s"), obj.GetScore(), score,COleDateTime::GetCurrentTime().Format(_T("%Y-%m-%d %H:%M:%S")), soreDec ) ;
	}
	BOOL ret;
	TaskIdStringMap fields;
	CString valueStr;
	valueStr.Format(_T("%d"), score );
	fields.insert( std::make_pair( fild_task_score, valueStr) );
	valueStr.Format(_T("'%s'"), str );
	fields.insert( std::make_pair( task_score_des, valueStr) );
	ret = m_pTaskDbDAo->UpdateTaskField( obj.GetId(), fields );
	return ret;
}
BOOL CSCheduleAction::GetTaskWorkHourSum( int taskId,  double &relust )
{
	return m_pTaskDbDAo->GetTaskWorkHourSum( taskId, relust );
}
BOOL CSCheduleAction::GetTaskWorkHourSum( int taskId, const COleDateTime &beginDate, const COleDateTime &endDate,double &relust )
{
	return m_pTaskDbDAo->GetTaskWorkHourSum( taskId, beginDate, endDate, relust );
}

BOOL CSCheduleAction::SetTaskFactWorkHour( CTaskObj &obj )
{
	double value;
	BOOL ret = GetTaskWorkHourSum( obj.GetId(), value );
	if ( ret == userExcOk )
	{
		if ( obj.GetFactWorkHour() != value )
		{
			CString valueStr;
			TaskIdStringMap fields;
			valueStr.Format(_T("%0.1f"), value );
			fields.insert( std::make_pair( fild_task_ac_period, valueStr) );
			ret = m_pTaskDbDAo->UpdateTaskField( obj.GetId(), fields );
			return TRUE;
		}
	}
	return FALSE;
}
