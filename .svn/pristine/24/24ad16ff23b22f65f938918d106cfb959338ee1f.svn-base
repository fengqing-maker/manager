#include "stdafx.h"
#include "ProductLine.h"

CProductLine::CProductLine()
{
	m_id = -1;
	m_mangerId = -1;
}
void CProductLine::CopyObj( const CProductLine &obj )
{
	 m_id = obj.m_id;
     m_mangerId = obj.m_mangerId;
     m_bakStr = obj.m_bakStr;
     m_strName = obj.m_strName;
}
CProductLine::CProductLine( const CProductLine &obj  )
{
	CopyObj( obj );
}
CProductLine& CProductLine::operator=( const CProductLine &obj)
{
	CopyObj(obj);
	return *this;
}