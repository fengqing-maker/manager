#include "stdafx.h"
#include "WorkTimeType.h"


bool CWorkTimeType::GetWorkTimeTypeById( int id, CString &strStatus ) const
{
	TaskIdStringMapConstItor itor;
	if ( (itor = m_workTimeMap.find(id)) != m_workTimeMap.end())
	{
		strStatus = itor->second;
		return true ;
	}
	return false;
}
int      CWorkTimeType::FindWorkTimeIdByString( const CString &str ) const
{
	int ret = -1;
	for ( TaskIdStringMap::const_iterator itor = m_workTimeMap.begin(); itor != m_workTimeMap.end(); itor++ )
	{
		if ( itor->second == str )
		{
			ret = itor->first;
			break;
		}
	}
	return ret;
}