#include "stdafx.h"
#include "Role.h"

CRole::CRole( const CRole &obj )
{
	CopyObj( obj );
}
void CRole::CopyObj( const CRole &obj )
{
	 m_id = obj.m_id;
	 m_roleName = obj.m_roleName;
	 m_strDec = obj.m_strDec;
}
CRole& CRole::operator==( const CRole &obj )
{
	CopyObj( obj );
	return *this;
}
