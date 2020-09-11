#include "stdafx.h"
#include "Project.h"

CProject::CProject()
{
	m_id = -1;
	m_mangerId = -1;
	m_lineId = -1;
	m_prjStepType = _T("新产品研发类");
}
void CProject::CopyObj( const CProject &obj )
{
	m_id = obj.m_id;
	m_mangerId = obj.m_mangerId;
	m_lineId = obj.m_lineId;
	m_shortName = obj.m_shortName;
	m_strName = obj.m_strName;
	m_strLongName = obj.m_strLongName;
	m_prjStepType = obj.m_prjStepType;
}
CProject::CProject( const CProject &obj  )
{
	CopyObj( obj );
}
CProject& CProject::operator=( const CProject &obj)
{
	CopyObj( obj );
	return *this;
}