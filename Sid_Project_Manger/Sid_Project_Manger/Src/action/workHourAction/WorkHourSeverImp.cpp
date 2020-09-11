#include "stdafx.h"
#include "WorkHourSeverImp.h"
#include "Src/Tools/UtilityTool/UtilityTool.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CWorkHourSverImp::CWorkHourSverImp(  CDaoFactory *pFactory )
	:CWorkHourSeverInc( pFactory )
{
	m_pWorkDbDao = pFactory->GetWorkHourDbDao();
}
BOOL CWorkHourSverImp::GetWorkTimeData( int timeType, int userId, const COleDateTime &time, CWorkHourData &obj )
{
	return m_pWorkDbDao->GetTimeData( timeType, userId, time, obj );
}
BOOL CWorkHourSverImp::GetHolidayHour( int userId,  const COleDateTime &time, CWorkHourData &obj )
{
	return m_pWorkDbDao->GetTimeData( 0, userId, time, obj );
}
BOOL CWorkHourSverImp::AddOrUpdateWorkHour( CWorkHourData &data )
{
	return m_pWorkDbDao->UpdateTimeData( data );
}
BOOL CWorkHourSverImp::GetNorTaskHourSum(   int userId, const COleDateTime &time, double &relsut )
{
	return m_pWorkDbDao->GetNorTaskHourSum( userId, time, relsut );
}
BOOL CWorkHourSverImp::GetTaskHourSum( int userId, int timeType, const COleDateTime &time, double &relust )
{
    return m_pDbDao->GetTaskDbDao()->GetTaskWorkHourSum( userId, timeType, time, relust );
}
BOOL CWorkHourSverImp::GetTaskHourSumExceptOngoing( int userId, const COleDateTime &time, double &relust )
{
	 return m_pDbDao->GetTaskDbDao()->GetTaskHourSumExceptSt( userId, ongoing, time, relust );
}
BOOL CWorkHourSverImp::GetUnUpdateDate( int taskId, ListDate &lst )
{
	return m_pDbDao->GetTaskDbDao()->GetUnUpdateDate( taskId, lst );
}
BOOL CWorkHourSverImp::AddUnUpdateDate( int taskId, const COleDateTime &date )
{
	return m_pDbDao->GetTaskDbDao()->AddUnUpdateDate( taskId, date );
}
BOOL CWorkHourSverImp::DeleteUnUpdateDate( int taskId, const COleDateTime &date )
{
    return m_pDbDao->GetTaskDbDao()->DeleteUnUpdateDate( taskId, date );
}
BOOL CWorkHourSverImp::DeleteUnUpdateDateByUserId( int userId, const COleDateTime &date )
{
	 double norWorkHour = 0.0;
	 GetNorTaskHourSum( userId, date, norWorkHour );
	 if ( 0 == CUtilityTool::GetObj()->DoubleCmp( norWorkHour - 7.5) )
	 {
		 return m_pDbDao->GetTaskDbDao()->DeleteUnUpdateDateByUserId( userId, date );
	 }
	 return userExcOk;
}
BOOL CWorkHourSverImp::GetUnUpdateCnt( int taskId, int &num )
{
	  return m_pDbDao->GetTaskDbDao()->GetUnUpdateCnt( taskId, num );
}
