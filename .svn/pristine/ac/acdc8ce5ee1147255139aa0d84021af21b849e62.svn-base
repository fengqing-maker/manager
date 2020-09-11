#include "stdafx.h"
#include "SystemConfigDbAdo.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

CSystemDbAdoImp::CSystemDbAdoImp( CDaoSupportInterface* pDao )
	               :CSystemCfgDbInc(pDao),m_tableHolidaysName( _T("holidays") ),
				     m_prjProcTableName(_T("project_period"))
{
}

BOOL CSystemDbAdoImp::ValidateProjectStepExist( const CProjectStep &data )
{
	CString str;
	str.Format(_T("select * from %s where number = %d or name = '%s' and product_type = '%s' "),
		m_prjProcTableName, data.GetNum(), data.GetName(), data.GetTypeName() );
	if ( FALSE == m_pDaoSupportInc->ExecuteSql(str) )
	{
		return excSqlError;
	}
	if ( !m_pDaoSupportInc->GetRecordSet()->IsEOF() )
	{
		return insertNameRepeat;
	}
	return userExcOk;
}

BOOL CSystemDbAdoImp::InsertProjectStep( CProjectStep &step )
{
	BOOL ret = ValidateProjectStepExist(  step );
	if ( userExcOk != ret  )
	{
		return ret;
	}
	CString str;
	str.Format( _T("insert into %s (number, name, product_type)\
				   values ( %d, '%s', '%s')"), m_prjProcTableName,
				   step.GetNum(), step.GetName(), step.GetTypeName() );
	if ( FALSE == m_pDaoSupportInc->Execute(str) )
	{
		return excSqlError;
	}
	return userExcOk;
}
BOOL CSystemDbAdoImp::UpdateProjectStep( const CProjectStep &step )
{
	BOOL ret = ValidateProjectStepExist( step );
	if ( ret != userExcOk )
	{
		return ret;
	}
	CString str;
	str.Format(_T("update %s t set t.number = %d, t.name = '%s'"), 
		m_prjProcTableName, step.GetNum(), step.GetName() );
	if ( FALSE == m_pDaoSupportInc->Execute(str) )
	{
		return excSqlError;
	}
	return userExcOk;
}

BOOL CSystemDbAdoImp::ExecProjectStepSql( const CString &str, ListProjectStep &obj )
{
	if ( FALSE == m_pDaoSupportInc->ExecuteSql(str) )
	{
		return excSqlError;
	}
	CAdoRecordset* pRecordSet = m_pDaoSupportInc->GetRecordSet();
	while( !pRecordSet->IsEOF() )
	{
		CProjectStep tempObj;
		FillTimeDataFromFild( pRecordSet, tempObj );
		obj.push_back( tempObj );
		pRecordSet->MoveNext();
	}
	return userExcOk;
}
BOOL CSystemDbAdoImp::GetProjectStepList( const CString &type, ListProjectStep &obj )
{
	CString str;
	str.Format(_T("select * from %s where product_type = '%s' order by number asc"),
		m_prjProcTableName, type );
	return ExecProjectStepSql( str, obj );
}

BOOL CSystemDbAdoImp::GetProjectStepType( ListString &lst )
{
	CString str;
	str.Format(_T("select product_type from %s group by product_type "), m_prjProcTableName );
	if ( FALSE == m_pDaoSupportInc->ExecuteSql(str) )
	{
		return excSqlError;
	}
	CAdoRecordset* pRecordSet = m_pDaoSupportInc->GetRecordSet();
	while( !pRecordSet->IsEOF() )
	{
		lst.push_back( (*pRecordSet)[_T("product_type")].ToString() );
		pRecordSet->MoveNext();
	}
	return userExcOk;
}
BOOL CSystemDbAdoImp::GetProjectStep( const CString stepName, CProjectStep &step )
{
	CString str;
	str.Format(_T("select * from %s where name = '%s' "), m_prjProcTableName, stepName );
	if ( FALSE == m_pDaoSupportInc->ExecuteSql(str) )
	{
		return excSqlError;
	}
	CAdoRecordset* pRecordSet = m_pDaoSupportInc->GetRecordSet();
	if ( pRecordSet->IsEOF()  )
	{
		return notfind;
	}
	FillTimeDataFromFild( pRecordSet, step  );
	return userExcOk;
}
BOOL CSystemDbAdoImp::GetProjectStep(const CString& type, const CString stepName, CProjectStep &step )
{
	CString str;
	str.Format(_T("select * from %s where name = '%s' and product_type = '%s' "),
		m_prjProcTableName, stepName, type );
	if ( FALSE == m_pDaoSupportInc->ExecuteSql(str) )
	{
		return excSqlError;
	}
	CAdoRecordset* pRecordSet = m_pDaoSupportInc->GetRecordSet();
	if ( pRecordSet->IsEOF()  )
	{
		return notfind;
	}
	FillTimeDataFromFild( pRecordSet, step  );
	return userExcOk;
}
BOOL CSystemDbAdoImp::GetProjectStep( int stepId, CProjectStep &step )
{
	CString str;
	str.Format(_T("select * from %s where id = %d "),
		m_prjProcTableName, stepId );
	if ( FALSE == m_pDaoSupportInc->ExecuteSql(str) )
	{
		return excSqlError;
	}
	CAdoRecordset* pRecordSet = m_pDaoSupportInc->GetRecordSet();
	if ( pRecordSet->IsEOF()  )
	{
		return notfind;
	}
	FillTimeDataFromFild( pRecordSet, step  );
	return userExcOk;
}

void CSystemDbAdoImp::FillTimeDataFromFild( CAdoRecordset* pRecordSet, CProjectStep &obj  ) const
{
	obj.SetId( (*pRecordSet)[_T("Id")] );
	obj.SetNum( (*pRecordSet)[_T("number")] );
	obj.SetName( (*pRecordSet)[_T("name")].ToString() );
	obj.SetTypeName( (*pRecordSet)[_T("product_type")].ToString() );
}
BOOL CSystemDbAdoImp::IsHoliday( const COleDateTime &date ,BOOL &reslut )
{
	CString str;
	str.Format(_T("select * from %s where filed_holiday = '%s' "),
		m_tableHolidaysName, date.Format(_T("%Y-%m-%d")) );
	if ( FALSE == m_pDaoSupportInc->ExecuteSql(str) )
	{
		return excSqlError;
	}
	reslut = !m_pDaoSupportInc->GetRecordSet()->IsEOF();
	return userExcOk;
}
BOOL CSystemDbAdoImp::AddHoliday( const COleDateTime &date )
{
	BOOL reslut = FALSE;
	BOOL ret = IsHoliday( date, reslut );
	if ( ret != userExcOk )
	{
		return ret;
	}
	if ( FALSE == reslut )
	{//Ìí¼Ó
		CString str;
		str.Format( _T("insert into %s ( filed_holiday ) values ( '%s' )"), 
			m_tableHolidaysName, date.Format(_T("%Y-%m-%d")));
		if ( FALSE == m_pDaoSupportInc->Execute(str) )
		{
			return excSqlError;
		}
	}
	return userExcOk;
}
BOOL CSystemDbAdoImp::DeleteHoliday( const COleDateTime &date )
{
	CString str;
	str.Format(_T("delete from %s where filed_holiday = '%s'"), 
		        m_tableHolidaysName, date.Format(_T("%Y-%m-%d")) );
	if ( FALSE == m_pDaoSupportInc->Execute(str) )
	{
		return excSqlError;
	}
	return userExcOk;
}
