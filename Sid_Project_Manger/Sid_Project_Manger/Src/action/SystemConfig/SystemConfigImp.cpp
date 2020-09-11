#include "stdafx.h"
#include "SystemConfigImp.h"

CSystemCfgSeverImp::CSystemCfgSeverImp(  CDaoFactory *pFactory )
	:CSystemCfgSeverInc( pFactory )
{
	m_pSystemCfgDb = pFactory->GetSystemCfgDbDao();
}
BOOL CSystemCfgSeverImp::GetProjectStep( const CString &stepName, CProjectStep &step )
{
	return m_pSystemCfgDb->GetProjectStep( stepName, step );
}
BOOL CSystemCfgSeverImp::GetProjectStepList( const CString &type, ListProjectStep &obj) 
{
	return m_pSystemCfgDb->GetProjectStepList( type, obj );
}
BOOL CSystemCfgSeverImp::GetProjectStep( const CString &type, const CString &stepName, CProjectStep &step )
{
	return m_pSystemCfgDb->GetProjectStep( type,stepName, step );
}
BOOL CSystemCfgSeverImp::GetProjectStep( int stepId, CProjectStep &step )
{
	return m_pSystemCfgDb->GetProjectStep( stepId, step );
}
BOOL CSystemCfgSeverImp::IsHoliday( const COleDateTime &date ,BOOL &reslut )
{
	return m_pSystemCfgDb->IsHoliday( date, reslut );
}
BOOL CSystemCfgSeverImp::AddHoliday( const COleDateTime &date )
{
	return m_pSystemCfgDb->AddHoliday( date );
}
BOOL CSystemCfgSeverImp::DeleteHoliday( const COleDateTime &date )
{
	return m_pSystemCfgDb->DeleteHoliday( date );
}
//计算一个任务平均每天占用多少工时
double  CSystemCfgSeverImp::CaluateTaskWorkHourEveday( const CTaskObj &obj )
{
	int dayCnt = 0;
	COleDateTime tmpDate = obj.GetPlanStartTime();
	COleDateTimeSpan span( 1, 0, 0, 0 );
	BOOL tmpBool = FALSE;
	while ( tmpDate <= obj.GetPlanEndTime() )
	{
		tmpBool = FALSE;
		IsHoliday( tmpDate, tmpBool);
		if ( FALSE == tmpBool )
		{
			dayCnt ++;
		}
		tmpDate += span;
	}
	if ( dayCnt > 0 )
	{
		return obj.GetPlanWorkHour()/dayCnt;
	}else
	{
		return  0.0f;
	}
}
//计算某员工某天预计工时
double  CSystemCfgSeverImp::CaluateForecastWorkHour( int userId, const COleDateTime &beginDate )
{
	ListTask lstTask;
	m_pDbDao->GetTaskDbDao()->GetTaskByDate( userId, beginDate, lstTask );
	double workHourSum = 0.0, workHour = 0.0;
	for ( ListTaskConstItor itor = lstTask.begin(); itor != lstTask.end(); itor++ )
	{
		workHour = CaluateTaskWorkHourEveday( *itor );
		workHourSum += workHour;
	}
	return workHourSum;
}