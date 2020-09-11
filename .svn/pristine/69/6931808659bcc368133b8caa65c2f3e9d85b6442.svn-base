#include "stdafx.h"
#include "WorkHourDb.h"


CWorkHourDbImp::CWorkHourDbImp( CDaoSupportInterface* pDao )
	             :CWorkHourDbInc(pDao),m_timeDataTableName(_T("time_data"))
{ 
	m_pDaoSupportInc = pDao;
}
CWorkHourDbImp::~CWorkHourDbImp()
{

}
BOOL CWorkHourDbImp::ValidateTimeDataExist( const CWorkHourData &data )
{
	CString str;
	str.Format(_T("select * from %s where type_id = %d and user_id = %d and update_date = '%s' "),
		m_timeDataTableName, data.GetTimeType(), data.GetUserId(), data.GetDateStr() );
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
BOOL CWorkHourDbImp::UpdateTimeData(  CWorkHourData &data )
{
	BOOL ret = ValidateTimeDataExist(data);
	if ( excSqlError == ret )
	{
		return excSqlError;
	}
	if ( insertNameRepeat == ret )
	{
		data.SetId(  (*(m_pDaoSupportInc->GetRecordSet()))[_T("Id")] );
	}
	CString str;
	if ( data.GetId() > 0 )
	{//更新
		str.Format(_T("update %s t set t.nor_task_hour = %0.1f \
					  where type_id = %d and user_id = %d and update_date = '%s' "),   
			           m_timeDataTableName,  data.GetNorTaskWorkHour(),
					   data.GetTimeType(), data.GetUserId(), data.GetDateStr() );
	}else
	{//插入
		str.Format( _T("insert into %s (type_id, user_id, nor_task_hour,update_date)\
					   values ( %d, %d, %0.1f, '%s')"), m_timeDataTableName, 
					   data.GetTimeType(), data.GetUserId(), data.GetNorTaskWorkHour(), data.GetDateStr()/*, data.GetStrBak()*/);
	}
	if ( FALSE == m_pDaoSupportInc->Execute(str) )
	{
		return excSqlError;
	}
	return userExcOk;
}
//CString CTaskDaoImp::GetFieldStringByType(int fieldType )
//{
//	CString  strFild;
//	switch( fieldType )
//	{
//	case taskName:
//		strFild = _T("task_name");
//		break;
//	case taskStatus:
//		strFild = _T("task_statusid");
//		break;
//	case  taskFactStartTime:
//		strFild = _T("task_plan_starttime");
//		break;
//	}
//	return strFild;
//}
//BOOL CWorkHourDbImp::UpdateTimeData( int dataId, const TaskIdStringMap &fields )
//{
//	CString str;
//	str.Format( _T("update %s t set "), m_timeDataTableName );
//	TaskIdStringMap::const_iterator itor = fields.begin();
//	CString tempStr,fieldStr;
//	while( itor != fields.end()  )
//	{
//		fieldStr = GetFieldStringByType( itor->first);
//		if ( fieldStr.GetLength() <= 0 )
//		{
//			return paraError;
//		}
//		tempStr.Format(_T("t.%s = %s,"), fieldStr, itor->second );
//		str += tempStr;
//		itor++;
//	}
//	str.Delete( str.GetLength() - 1 );
//	CString sqlString;
//	sqlString.Format( _T("%s where t.task_id = %d"), str, taskId );
//	if ( FALSE == m_pDaoSupportInc->Execute(sqlString) )
//	{
//		return excSqlError;
//	}
//	return userExcOk;
//}
BOOL CWorkHourDbImp::GetTimeData( int timeType, int userId, const COleDateTime &time, CWorkHourData &obj )
{
	CString str;
	CString dateStr = time.Format(_T("%Y-%m-%d"));
	str.Format(_T("select * from %s where type_id = %d and user_id = %d and update_date = '%s' "),
		m_timeDataTableName, timeType, userId, dateStr );
	if ( FALSE == m_pDaoSupportInc->ExecuteSql(str) )
	{
		return excSqlError;
	}
	CAdoRecordset* pRecordSet = m_pDaoSupportInc->GetRecordSet();
	if ( pRecordSet->IsEOF() )
	{
		return notfind;
	}
	FillTimeDataFromFild( pRecordSet, obj  );
	return userExcOk;
}
BOOL CWorkHourDbImp::GetNorTaskHourSum(  int userId, const COleDateTime &time, double &relsut )
{//timetype < 0查询所有的
	CString str;
	CString dateStr =  time.Format(_T("%Y-%m-%d"));
	str.Format( _T("select sum(nor_task_hour) as valueSum from %s where user_id = %d and update_date = '%s'") ,
		 m_timeDataTableName, userId, dateStr );
	if ( FALSE == m_pDaoSupportInc->ExecuteSql(str) )
	{
		return excSqlError;
	}
	CAdoRecordset* pRecordSet = m_pDaoSupportInc->GetRecordSet();
	relsut = 0.0;
	if ( !pRecordSet->IsEOF() )
	{
		DbField value = (*pRecordSet)[_T("valueSum")];
		if ( !value.IsNull() )
		{
			relsut = value;
		}
	}
	return userExcOk;
}

void CWorkHourDbImp::FillTimeDataFromFild( CAdoRecordset* pRecordSet, CWorkHourData &obj  ) const
{
	obj.SetId( (*pRecordSet)[_T("Id")] );
	obj.SetTimeType( (*pRecordSet)[_T("type_id")] );
	obj.SetUserId( (*pRecordSet)[_T("user_id")]);
	//obj.SetTaskWorkHour( (*pRecordSet)[_T("task_work_hour")] );
	obj.SetDateByString( (*pRecordSet)[_T("update_date")].ToString() );
	//obj.SetStrBak( (*pRecordSet)[_T("data_dec")].ToString() );
	obj.SetNorTaskWorkHour( (*pRecordSet)[_T("nor_task_hour")]);
}