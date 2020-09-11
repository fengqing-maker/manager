#include "stdafx.h"
#include "ProjectStep.h"

CProjectStep::CProjectStep()
{
	m_id = -1;
	m_num = -1;
}
CProjectStep::CProjectStep( const CProjectStep &obj )
{
	CopyObj( obj );
}
CProjectStep& CProjectStep::operator==( const CProjectStep &obj )
{
	CopyObj( obj );
	return *this;
}
void CProjectStep::CopyObj( const CProjectStep &obj )
{
	m_id = obj.m_id;
	m_num = obj.m_num;
	m_name = obj.m_name;
	m_strType = obj.m_strType;
}