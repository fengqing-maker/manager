#include "stdafx.h"
#include "Schedule.h"

CSchedule::CSchedule()
{
	m_id = -1;
	m_taskId = -1;
	m_workHours = 0.0;
	m_percent = 0;
	m_isChange = FALSE;
	m_timeType = 0;

}
void CSchedule::ReInit()
{
	m_id = -1;
	m_taskId = -1;
	m_workHours = 0.0;
	m_percent = 0;
	m_isChange = FALSE;
	m_strdec = _T("");
	m_updateTime.SetDate( 0, 0, 0);
	m_timeType = 0;
}
CSchedule::CSchedule(const CSchedule &obj)
{
	CopyObj( obj );
	CleanModify();
}
bool CSchedule::SetPercent(int per)
{
	m_percent = per;
	return true;
}
void CSchedule::SetUpdateTimeStr( const CString timeStr)
{
	m_updateTime.ParseDateTime( timeStr );
}
CString CSchedule::GetUpdateTimeStr() const
{
	return m_updateTime.Format( _T("%Y-%m-%d") );
}


void CSchedule::CopyObj( const CSchedule &obj )
{
	m_id = obj.m_id;
	m_taskId = obj.m_taskId;
	m_strdec = obj.m_strdec;
	m_workHours = obj.m_workHours;
	m_percent = obj.m_percent;
	m_updateTime = obj.m_updateTime;
	m_isChange = obj.m_isChange;
	m_timeType = obj.m_timeType;
}
CSchedule & CSchedule::operator = (const CSchedule & pt)
{
	CopyObj( pt );
	return (*this);
}
bool CSchedule::operator == (const CSchedule & obj) const
{
	if ( m_id != obj.m_id )
	{
		return false;
	}
    if ( m_taskId != obj.m_taskId )
    {
		return false;
    }
	if ( m_strdec != obj.m_strdec )
	{
		return false;
	}
    if ( m_workHours != obj.m_workHours)
    {
		return false;
    }
	if ( m_percent != obj.m_percent )
	{
		return false;
	} 
	if ( m_updateTime != obj.m_updateTime )
	{
		return false;
	}
	return true;
}