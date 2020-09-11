#include "stdafx.h"
#include "department.h"

CDepartment::CDepartment()
{
	m_id = -1;
	m_mangerId = -1;
	m_bakMangerId = -1;
	m_isBakValidate = FALSE;
	m_fatherDepartId = 7;
}
CDepartment::CDepartment(const CDepartment &depart )
{
	CopyObj(depart);
}
CDepartment& CDepartment::operator=( const CDepartment &depart )
{
	CopyObj(depart);
	return *this;
}
void CDepartment::CopyObj( const CDepartment &depart )
{
	m_id = depart.m_id;
	m_mangerId = depart.m_mangerId;
	m_bakMangerId = depart.m_bakMangerId;
	m_isBakValidate = depart.m_isBakValidate;
	m_bakStr = depart.m_bakStr;
	m_strName = depart.m_strName;
	m_fatherDepartId = depart.m_fatherDepartId;
}
