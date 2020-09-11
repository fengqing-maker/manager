#include "stdafx.h"
#include "CExcelTaskModel.h"

CExcelTaskModel::CExcelTaskModel()
{
	m_num = -1;
	m_fatherNum = -1;
	m_taskScore = 0;
	m_difficultyParm = 0.0;
	m_planWorkHour = 0;
	m_workHours = 0.0f;
	m_factWorkHour = 0.0f;
	m_percent = 0;
	m_relatedTaskId = -1;
	m_factFatherId = -1;
}
void CExcelTaskModel::SetPlanStartTime( const COleDateTime &time )
{
	if (m_planStartTime != time)
	{
		m_planStartTime = time;
		m_isModify = TRUE;
	}
}
const COleDateTime   CExcelTaskModel::GetPlanStartTime() const
{
	return m_planStartTime;
}

void CExcelTaskModel::SetPlanEndTime( const COleDateTime &time )
{
	if ( time != m_planEndTime)
	{
		m_planEndTime = time;
		m_isModify = TRUE;
	}
}
const COleDateTime  CExcelTaskModel::GetPlanEndTime() const
{
	return m_planEndTime;
}
void  CExcelTaskModel::SetPercent(int per)
{
	m_percent = per;
	return ;
}
//set
void      CExcelTaskModel::SetPanStartDateStr(const CString timeStr)
{
	m_planStartTime.ParseDateTime( timeStr, /*VAR_DATEVALUEONLY*/ LOCALE_NOUSEROVERRIDE);
}
void     CExcelTaskModel::SetPanEndDateStr( const CString timeStr)
{
	m_planEndTime.ParseDateTime( timeStr, VAR_DATEVALUEONLY );
}
void      CExcelTaskModel::SetFactStartDateStr( const CString timeStr)
{
	m_factStartTime.ParseDateTime( timeStr, VAR_DATEVALUEONLY );
}
void      CExcelTaskModel::SetFactEndDateStr( const CString timeStr )
{
	m_factEndTime.ParseDateTime( timeStr, VAR_DATEVALUEONLY );
}
CString    CExcelTaskModel::GetPanStartDateStr() const
{
	return m_planStartTime.m_status == COleDateTime::valid?m_planStartTime.Format(  _T("%Y-%m-%d") ):_T("");
}
CString    CExcelTaskModel::GetPanEndDateStr() const
{
     return   m_planEndTime.m_status == COleDateTime::valid?m_planEndTime.Format(  _T("%Y-%m-%d") ):_T("");
}
CString    CExcelTaskModel::GetFactStartDateStr() const
{
	return  m_factStartTime.m_status == COleDateTime::valid ? m_factStartTime.Format( _T("%Y-%m-%d") ):_T("");
}
CString    CExcelTaskModel::GetFactEndDateStr() const
{
	return m_factEndTime.m_status == COleDateTime::valid ? m_factEndTime.Format( _T("%Y-%m-%d") ):_T("");
}
CString   CExcelTaskModel::GetUpdateTimeStr() const
{
	return m_updateTime.m_status == COleDateTime::valid ? m_updateTime.Format( _T("%Y-%m-%d") ):_T("");
}
CExcelTaskModel::CExcelTaskModel( const CExcelTaskModel& obj )
{
	CopyObj( obj );
}
void CExcelTaskModel::CopyObj( const CExcelTaskModel& obj )
{
	m_num = obj.m_num;
	m_fatherNum = obj.m_fatherNum;
	m_status = obj.m_status;
	m_ownerName = obj.m_ownerName;
	m_workHourTypeStr = obj.m_workHourTypeStr;
	m_relatedTaskId = obj.m_relatedTaskId;
	m_factFatherId = obj.m_factFatherId;
	m_taskTypeString = obj.m_taskTypeString;
	//任务
	m_taskName = obj.m_taskName;
	m_taskScore = obj.m_taskScore;     //任务总分
	m_difficultyParm = obj.m_difficultyParm; //任务难度系数，评审人同样发布后必填的值
	m_planStartTime = obj.m_planStartTime; //计划开启时间
	m_planEndTime = obj.m_planEndTime;   //计划结束时间
	m_planWorkHour = obj.m_planWorkHour;  //计划工时
	m_factStartTime = obj.m_factStartTime;
	m_factEndTime = obj.m_factEndTime;
	m_factWorkHour = obj.m_factWorkHour;
	m_dec = obj.m_dec;           //任务备注
	//进度相关
	m_workHours = obj.m_workHours; //工时
	m_percent = obj.m_percent; //完成百分比
	m_strdec = obj.m_strdec; //进度描述
	m_updateTime = obj.m_updateTime;

	m_taskAtrrString = obj.m_taskAtrrString;
	m_isModify = obj.m_isModify;    //是否修改过对象
}
CExcelTaskModel& CExcelTaskModel::operator=( const CExcelTaskModel &obj )
{
   CopyObj( obj );
   return *this;
}
void CExcelTaskModel::FillContentFromTask( const CTaskObj &obj )
{
	//任务
	m_relatedTaskId = obj.GetId();
	m_taskName = obj.GetName();
	m_taskScore = obj.GetScore();     //任务总分
	m_difficultyParm = obj.GetDifficultyParam(); //任务难度系数，评审人同样发布后必填的值
	m_planStartTime = obj.GetPlanStartTime(); //计划开启时间
	m_planEndTime = obj.GetPlanEndTime();   //计划结束时间
	m_planWorkHour = obj.GetPlanWorkHour();  //计划工时
	m_factStartTime = obj.GetFactStartTime();
	m_factEndTime = obj.GetFactEndTime();
	m_factWorkHour = obj.GetFactWorkHour();
	m_dec = obj.GetDec();           //任务备注
}
void      CExcelTaskModel::FillContentFromSchedul( const CSchedule &obj )
{
	m_workHours = obj.GetWorkHours(); //工时
	m_percent = obj.GetPercent(); //完成百分比
	m_strdec = obj.GetUpdateContent(); //进度描述
	m_updateTime = obj.GetUpdateTime();

}