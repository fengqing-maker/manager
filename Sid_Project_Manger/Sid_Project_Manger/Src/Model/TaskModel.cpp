m_dec
#include "stdafx.h"
#include "TaskModel.h"
#include "excelObj/CExcelTaskModel.h"
#include "Src/DataBase/inc/daocomonDef.h"

CTaskObj::CTaskObj()
{
	InitObj();
}
void CTaskObj::InitObj()
{
	m_attr = NORMAL_TASK;
	m_ownerId = -1;
	m_prjId = -1;
	m_planWorkHour = 0.0;
	m_factWorkHour = 0.0;
	m_taskScore = 0;
	m_forecastScore = 0;
	m_scoreUser = -1;

	m_taskStatus = newTasking;         //新建中
	m_difficultyParm = 1.0;
	m_timeType = 0;
	m_createId = -1;

	m_isModify = FALSE;
	m_id = -1;
	m_cntLeakUpdate = 0;
	m_checkBeginDate.SetDate( 2015, 5, 28 );
	m_taskPublishTime.SetDate( 2000, 1, 1 );
	m_scoreTime.SetDate( 2000, 1, 1 );
	m_createTime.SetDate( 2000, 1, 1 );

	m_taskFlag = 0;
}
void CTaskObj::SetPlanStartTime( const COleDateTime &time )
{
	if (m_planStartTime != time)
	{
		m_planStartTime = time;
		m_isModify = TRUE;
	}
}
const COleDateTime  CTaskObj::GetPlanStartTime() const
{
	return m_planStartTime;
}

void CTaskObj::SetPlanEndTime( const COleDateTime &time )
{
	if ( time != m_planEndTime)
	{
		m_planEndTime = time;
		m_isModify = TRUE;
	}
}
const COleDateTime  CTaskObj::GetPlanEndTime() const
{
	return m_planEndTime;
}
//
CString    CTaskObj::GetPanStartDateStr() const
{
	//return m_planStartTime.Format(  _T("%Y-%m-%d") );
	return m_planStartTime.m_status == COleDateTime::valid?m_planStartTime.Format(  _T("%Y-%m-%d") ):_T("1899-12-30");
}
CString    CTaskObj::GetPanEndDateStr() const
{
   //return   m_planEndTime.Format(  _T("%Y-%m-%d") );
	return   m_planEndTime.m_status == COleDateTime::valid?m_planEndTime.Format(  _T("%Y-%m-%d") ):_T("1899-12-30");
}
CString    CTaskObj::GetPanStartDateShowStr() const
{
	return m_planStartTime.m_status == COleDateTime::valid && m_planStartTime.GetYear() > 2000 ?m_planStartTime.Format(  _T("%Y-%m-%d") ):_T("");
}
CString    CTaskObj::GetPanEndDateShowStr() const
{
	return   m_planEndTime.m_status == COleDateTime::valid && m_planEndTime.GetYear() > 2000 ?m_planEndTime.Format(  _T("%Y-%m-%d") ):_T("");
}
CString    CTaskObj::GetFactStartDateStr() const
{
	//return  m_factStartTime.Format( _T("%Y-%m-%d") );
	return m_factStartTime.m_status == COleDateTime::valid? m_factStartTime.Format( _T("%Y-%m-%d") ):_T("1899-12-30");
}
CString    CTaskObj::GetFactStartDateShowStr() const
{
	return  m_factStartTime.m_status == COleDateTime::valid && m_factStartTime.GetYear() > 2000 ? m_factStartTime.Format( _T("%Y-%m-%d") ):_T("");
}
CString    CTaskObj::GetFactEndDateStr() const
{
	//return m_factEndTime.Format( _T("%Y-%m-%d") );
	return m_factEndTime.m_dt > 0 && m_factEndTime.m_status == COleDateTime::valid ? m_factEndTime.Format( _T("%Y-%m-%d") ):_T("1899-12-30");
}
CString    CTaskObj::GetFactEndDateShowStr() const
{
	return m_factEndTime.m_dt > 0 && m_factEndTime.m_status == COleDateTime::valid && m_factEndTime.GetYear() > 2000 ? m_factEndTime.Format( _T("%Y-%m-%d") ):_T("");
}

CString   CTaskObj::GetCreateTimeStr() const
{
	CString temp;
	if ( !(m_createTime.m_dt > 0 && m_createTime.m_status == COleDateTime::valid ))
	{
		return temp;
	}
	temp.Format(_T("%d-%d-%d %d:%d:%d"),
		        m_createTime.GetYear(),m_createTime.GetMonth(), m_createTime.GetDay(), 
				 m_createTime.GetHour(), m_createTime.GetMinute(), m_createTime.GetSecond());
	return temp;
}
CString   CTaskObj::GetPublishTimeStr() const
{
	CString temp;
	if ( !(m_taskPublishTime.m_dt > 0 && m_taskPublishTime.m_status == COleDateTime::valid ))
	{
		return temp;
	}
	temp.Format(_T("%d-%d-%d %d:%d:%d"),
		m_taskPublishTime.GetYear(),m_taskPublishTime.GetMonth(), m_taskPublishTime.GetDay(), 
		m_taskPublishTime.GetHour(), m_taskPublishTime.GetMinute(), m_taskPublishTime.GetSecond());
	return temp;
}
CString   CTaskObj::GetScoreTimeStr() const
{
	CString temp;
	if ( !(m_scoreTime.m_dt > 0 && m_scoreTime.m_status == COleDateTime::valid ))
	{
		return temp;
	}
	temp.Format(_T("%d-%d-%d %d:%d:%d"),
		m_scoreTime.GetYear(),m_scoreTime.GetMonth(), m_scoreTime.GetDay(), 
		m_scoreTime.GetHour(), m_scoreTime.GetMinute(), m_scoreTime.GetSecond());
	return temp;
}

//set
void      CTaskObj::SetPanStartDateStr(const CString timeStr)
{
	m_planStartTime.ParseDateTime( timeStr, VAR_DATEVALUEONLY );
}
void     CTaskObj::SetPanEndDateStr( const CString timeStr)
{
	m_planEndTime.ParseDateTime( timeStr, VAR_DATEVALUEONLY );
}
void      CTaskObj::SetFactStartDateStr( const CString timeStr)
{
	m_factStartTime.ParseDateTime( timeStr, VAR_DATEVALUEONLY );
}
void      CTaskObj::SetFactEndDateStr( const CString timeStr )
{
	m_factEndTime.ParseDateTime( timeStr, VAR_DATEVALUEONLY );
}
void      CTaskObj::SetCreateTimeStr( const CString timeStr ) 
{
	m_createTime.ParseDateTime( timeStr, LOCALE_NOUSEROVERRIDE );
}
void       CTaskObj::SetPublishTimeStr( const CString timeStr ) 
{
	m_taskPublishTime.ParseDateTime( timeStr, LOCALE_NOUSEROVERRIDE );
}
void       CTaskObj::SetScoreTimeStr( const CString timeStr ) 
{
	m_scoreTime.ParseDateTime( timeStr, LOCALE_NOUSEROVERRIDE);
}
CTaskObj::CTaskObj( const CTaskObj &obj)
{
	CopyObj( obj );
	CleanModify();
}
CTaskObj& CTaskObj::operator=( const CTaskObj &obj )
{
	CopyObj( obj );
	return *this;
}
void  CTaskObj::CopyObj( const CTaskObj &obj )
{
	m_id = obj.m_id;
	m_attr = obj.m_attr;//0：正常任务   1备份任务
	m_name = obj.m_name;
	m_dec = obj.m_dec;
	m_type = obj.m_type; // 项目任务
	m_ownerId = obj.m_ownerId;
	m_prjId = obj.m_prjId;
	m_createTime = obj.m_createTime;      //创建时间
	m_taskPublishTime = obj.m_taskPublishTime; //任务发布时间

	m_planStartTime = obj.m_planStartTime; //计划开启时间
	m_planEndTime = obj.m_planEndTime;   //计划结束时间
	m_planWorkHour = obj.m_planWorkHour;  //计划工时
	m_factStartTime = obj.m_factStartTime;
	m_factEndTime = obj.m_factEndTime;
	m_factWorkHour = obj.m_factWorkHour;

	m_taskScore = obj.m_taskScore;     //任务总分
	m_forecastScore = obj.m_forecastScore;  //系统预测分，以100为基准
	m_taskScoreDc = obj.m_taskScoreDc; //评分说明
	m_scoreUser = obj.m_scoreUser;  //打分人
	m_scoreTime = obj.m_scoreTime; //评分时间

	m_taskStatus = obj.m_taskStatus;
	m_difficultyParm = obj.m_difficultyParm; //任务难度系数，评审人同样发布后必填的值
	m_timeType = obj.m_timeType;       //工时类型

	m_createId = obj.m_createId;
	m_isModify = obj.m_isModify;

	m_strPrjReviw = obj.m_strPrjReviw;
	m_strReveiw = obj.m_strReveiw;
	m_cntLeakUpdate = obj.m_cntLeakUpdate;
	m_checkBeginDate = obj.m_checkBeginDate;

	m_taskFlag = obj.m_taskFlag; //任务属性标识
}
void   CTaskObj::FillObjFromExcel( const CExcelTaskModel &obj )
{
	m_name = obj.GetTaskName();
	m_taskScore = obj.GetScore();
	m_difficultyParm = obj.GetDifficultyParam();
	m_planStartTime = obj.GetPlanStartTime();
	m_planEndTime = obj.GetPlanEndTime();
	m_planWorkHour = obj.GetPlanWorkHour();
	m_factStartTime = obj.GetFactStartTime();
	m_factEndTime = obj.GetFactEndTime();
	m_factWorkHour = obj.GetFactWorkHour();
	m_dec = obj.GetDec();
}

BOOL       CTaskObj::operator==( const CTaskObj &obj ) const
{
	if ( m_id != obj.m_id )
	{
		return false;
	}
	if ( m_taskStatus != obj.m_taskStatus )
	{
		return false;
	}
	if ( m_difficultyParm != obj.m_difficultyParm )
	{//任务难度系数，评审人同样发布后必填的值
		return false;
	}
	if ( m_timeType != obj.m_timeType )
	{  //工时类型
		return false;
	}
	if ( m_attr != obj.m_attr )
	{
		return false;
	}
    if ( m_name != obj.m_name )
    {
		return false;
    }
	if ( m_dec != obj.m_dec )
	{
		return false;
	}
	if ( m_type != obj.m_type )
	{ // 项目任务
		return false;
	}
	if ( m_ownerId != obj.m_ownerId )
	{
		return false;
	}
	if ( m_prjId != obj.m_prjId )
	{
		return false;
	}

	//m_createTime = obj.m_createTime;      //创建时间
	//m_taskPublishTime = obj.m_taskPublishTime; //任务发布时间

	if ( m_planStartTime.m_dt > 0 && m_planStartTime.m_status == COleDateTime::valid && m_planStartTime != obj.m_planStartTime )
	{ //计划开启时间
		return false;
	}	
	if (m_planEndTime.m_dt > 0 && m_planEndTime.m_status == COleDateTime::valid && m_planEndTime != obj.m_planEndTime )
	{//计划结束时间
		return false;
	}
	if ( m_planWorkHour != obj.m_planWorkHour )
	{ //计划工时
		return false;
	}
    if ( m_factStartTime.m_dt > 0 && m_factStartTime.m_status == COleDateTime::valid && m_factStartTime != obj.m_factStartTime )
    {
		return false;
    }
	if ( m_factEndTime.m_dt > 0 && m_factEndTime.m_status == COleDateTime::valid && m_factEndTime != obj.m_factEndTime )
    {
		return false;
    }
	if ( m_factWorkHour != obj.m_factWorkHour )
	{
		return false;
	}
	
	if ( m_taskScore != obj.m_taskScore )
	{ //任务总分
		return false;
	}
	//1111111111 去掉预测分判断
	//if ( m_forecastScore != obj.m_forecastScore )
	//{ //系统预测分，以100为基准
	//	return false;
	//}
	if ( m_taskScoreDc != obj.m_taskScoreDc )
	{//评分说明
		return false;
	}
	if ( m_scoreUser != obj.m_scoreUser )
	{
		return false;
	}

     //以下改变不进行，输出消息
	//if ( m_createId != obj.m_createId )
	//{
	//	return false;
	//}
	//if ( m_strPrjReviw != obj.m_strPrjReviw )
	//{
	//	return false;
	//}
	//if ( m_strReveiw != obj.m_strReveiw )
	//{
	//	return false;
	//}
	//if( m_cntLeakUpdate != obj.m_cntLeakUpdate )
	//{
	//	return false;
	//}
	//if ( m_checkBeginDate != obj.m_checkBeginDate )
	//{
	//	return false;
	//}	
	//if ( m_taskFlag != obj.m_taskFlag )
	//{
	//	return false;
	//}
	return true;
}
int  CTaskObj::CaludateTimePercent( const COleDateTime &updateDate ) const
{
	COleDateTime curDate = updateDate;
	COleDateTimeSpan span( 1, 0, 0, 0 );
	if (  ongoing == this->m_taskStatus   )
	{
	    curDate -= span;
	}
	COleDateTimeSpan span1 = curDate - m_planStartTime;
    COleDateTimeSpan span2 = m_planEndTime - m_planStartTime ;
	if ( span2.m_span == 0 )
	{
		span2.SetDateTimeSpan( 1, 0, 0, 0 );
	}
	int per = (int)( (span1.m_span*100.0/span2.m_span));
	if ( per > 100 )
	{
		per = 100;
	}
	if ( per < 0 )
	{
		per = 0;
	}
	return per;
}
COleDateTime CTaskObj::g_minCheckDate( 2015, 5, 28, 0, 0, 0 );
void   CTaskObj::SetCheckBeginDate( const COleDateTime& data )
{
	if ( data > g_minCheckDate )
	{
		m_checkBeginDate = data;
	}
}
void   CTaskObj::SetCheckBeginDateStr( const CString &str )
{
	COleDateTime time;
	time.ParseDateTime( str, VAR_DATEVALUEONLY );
	if ( time > g_minCheckDate )
	{
		m_checkBeginDate = time;
	}
}
const CString  CTaskObj::GetCheckBeginDateStr() const
{
	return  m_checkBeginDate.Format(_T("%Y-%m-%d"));
}
int      CTaskObj::GetTaskPanDay() const
{//任务计划完成天数
	COleDateTimeSpan span( 1, 0, 0, 0 ); 
	COleDateTimeSpan span2 = m_planEndTime-m_planStartTime;
	span2 += span;
	return span2.GetDays();
}
//2015-6-15 add 
CString       CTaskObj::GetDiffContent( const CTaskObj &obj, const TaskIdStringMap &statusMap ) const
{
	CString str;
	//if ( m_id != obj.m_id )
	//{
	//	return false;
	//}
	if ( m_dec != obj.m_dec )
	{
		str.Format(_T("【任务%d %s】中任务备注:"), m_id, m_name );
		int pos = m_dec.Find( obj.m_dec );
		if ( pos >= 0)
		{
			str += m_dec.Right( m_dec.GetLength() - pos - obj.m_dec.GetLength() );
		}else
		{
			str += m_dec;
		}
		return str;
	}
	if ( m_taskStatus != obj.m_taskStatus )
	{
		CString st1, st2;
		TaskIdStringMapConstItor itor;
		if ( (itor = statusMap.find( m_taskStatus)) != statusMap.end() )
		{
			st1 = itor->second;
		}
		if ( (itor =statusMap.find( obj.m_taskStatus)) != statusMap.end() )
		{
			st2 = itor->second;
		}
		str.Format(_T("【任务%d %s】中任务状态由'%s'->'%s'"), m_id, m_name, st1, st2);
		return str;
	}
	if ( m_difficultyParm != obj.m_difficultyParm )
	{//任务难度系数，评审人同样发布后必填的值
		str.Format(_T("【任务%d %s】中任务难度系数由'%d'->'%d'"), m_id, m_name,m_difficultyParm, obj.m_difficultyParm );
		return str;
	}
	if ( m_timeType != obj.m_timeType )
	{  //工时类型
		str.Format(_T("【任务%d %s】中'工时类型'改变"), m_id, m_name );
		return str;
	}
	if ( m_attr != obj.m_attr )
	{
		str.Format(_T("【任务%d %s】中'任务所属项目阶段'改变"), m_id, m_name );
		return str;
	}
	if ( m_name != obj.m_name )
	{
		str.Format(_T("【任务%d】任务名称由'%s'->'%s'"), m_id, m_name, obj.m_name );
		return str;
	}

	if ( m_type != obj.m_type )
	{ // 项目任务
		str.Format(_T("【任务%d %s】中任务类型由'%s'->'%s'"),  m_id, m_name,m_type, obj.m_type );
		return str;
	}
	if ( m_ownerId != obj.m_ownerId )
	{
		str.Format(_T("【任务%d %s】中'负责人'改变"), m_id, m_name );
		return str;
	}
	if ( m_prjId != obj.m_prjId )
	{
		str.Format(_T("【任务%d %s】中'所属项目'改变"), m_id, m_name );
		return str;
	}

	//m_createTime = obj.m_createTime;      //创建时间
	//m_taskPublishTime = obj.m_taskPublishTime; //任务发布时间

	if ( m_planStartTime.m_dt > 0 && m_planStartTime.m_status == COleDateTime::valid && m_planStartTime != obj.m_planStartTime )
	{ //计划开启时间
		str.Format(_T("【任务%d %s】中计划开始日期由 '%s' 更新为 '%s'"), m_id, m_name, GetPanStartDateShowStr(), obj.GetPanStartDateShowStr() );
		return str;
	}	
	if (m_planEndTime.m_dt > 0 && m_planEndTime.m_status == COleDateTime::valid && m_planEndTime != obj.m_planEndTime )
	{//计划结束时间
		str.Format(_T("【任务%d %s】中计划开始日期由 '%s' 更新为 '%s'"), m_id, m_name, GetPanEndDateShowStr(), obj.GetPanEndDateShowStr() );
		return str;
	}
	if ( m_planWorkHour != obj.m_planWorkHour )
	{ //计划工时
		str.Format(_T("【任务%d %s】中计划工时由'%0.1f'->'%0.1f'"),  m_id, m_name, m_planWorkHour, obj.m_planWorkHour );
		return str;
	}
	if ( m_factStartTime.m_dt > 0 && m_factStartTime.m_status == COleDateTime::valid && m_factStartTime != obj.m_factStartTime )
	{
		str.Format(_T("【任务%d %s】中计划开始日期由 '%s' 更新为 '%s'"), m_id, m_name, GetFactStartDateShowStr(), obj.GetFactStartDateShowStr() );
		return str;
	}
	if ( m_factEndTime.m_dt > 0 && m_factEndTime.m_status == COleDateTime::valid && m_factEndTime != obj.m_factEndTime )
	{
		str.Format(_T("【任务%d %s】中计划开始日期由 '%s' 更新为 '%s'"), m_id, m_name, GetFactEndDateShowStr(), obj.GetFactEndDateShowStr() );
		return str;
	}
	if ( m_factWorkHour != obj.m_factWorkHour )
	{
	   str.Format(_T("【任务%d %s】中实际工时由'%0.1f'->'%0.1f'"), m_id, m_name, m_factWorkHour, obj.m_factWorkHour );
	   return str;
	}

	if ( m_taskScore != obj.m_taskScore )
	{ //任务总分
		str.Format(_T("【任务%d %s】中任务评分更新为%d"), m_id, m_name, m_taskScore );
		return str;
	}
	if ( m_taskScoreDc != obj.m_taskScoreDc )
	{//评分说明
		str.Format(_T("【任务%d %s】中任务评分更新为%d"), m_id, m_name, m_taskScore );
		return str;
	}
	if ( m_scoreUser != obj.m_scoreUser )
	{
		str.Format(_T("【任务%d %s】更改评分人"), m_id, m_name );
		return str;
	}
	//以下改变不进行，输出消息
	//if ( m_createId != obj.m_createId )
	//{
	//	return false;
	//}
	if ( m_strPrjReviw != obj.m_strPrjReviw )
	{
		str.Format(_T("【任务%d %s】项目过程点评更新为%s"), m_id, m_name, m_strPrjReviw );
		return str;
	}
	//if ( m_strReveiw != obj.m_strReveiw )
	//{
	//	return FALSE;
	//}
	//if( m_cntLeakUpdate != obj.m_cntLeakUpdate )
	//{
	//	return FALSE;
	//}
	//if ( m_checkBeginDate != obj.m_checkBeginDate )
	//{
	//	return FALSE;
	//}
	return str;
}
