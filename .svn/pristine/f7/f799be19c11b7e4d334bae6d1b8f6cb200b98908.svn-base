#include "stdafx.h"
#include "WorkHourData.h"

CWorkHourData::CWorkHourData()
{
	m_id = -1;
	m_timeType = 0;
	m_userId = 0;
	m_taskWorkHour = 0.0;
	m_id = 0;
	m_norTaskWorkHour = 0.0;
}
CWorkHourData::CWorkHourData( const CWorkHourData &obj )
{
	CopyObj( obj );
}
CWorkHourData& CWorkHourData::operator==( const CWorkHourData &obj )
{
	CopyObj( obj );
	return *this;
}
void CWorkHourData::CopyObj( const CWorkHourData &obj )
{
	m_timeType = obj.m_timeType;
	m_userId = obj.m_userId;
	m_taskWorkHour = obj.m_taskWorkHour;
	m_date = obj.m_date;
//	m_strBak = obj.m_strBak;
	m_norTaskWorkHour = obj.m_norTaskWorkHour;
}
void CWorkHourData::SetDateByString( const CString &str )
{
    m_date.ParseDateTime( str, VAR_DATEVALUEONLY );
}
CString CWorkHourData::GetDateStr() const
{
	return m_date.Format( _T("%Y-%m-%d") );
}
