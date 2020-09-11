#include "stdafx.h"
#include "CExcelTaskDao.h"
#include "Src/Tools/UtilityTool/UtilityTool.h"

CExcelTaskDao::CExcelTaskDao()
{
	m_strSheetName = _T("task");
}
CExcelTaskDao::~CExcelTaskDao()
{

}
BOOL CExcelTaskDao::GetExcelTask( ListExcelTaskMode &modeList )
{//
	ASSERT( m_pDaoSupport.get() );
	if ( !m_pDaoSupport->IsOpen() )
	{
		return not_connect;
	}
	CString sql;
	sql.Format(_T("select * from [%s$]"), m_strSheetName);
	if ( !m_pDaoSupport->ExecuteSql( sql ) )
	{
		return excSqlError;
	}
	CAdoRecordset *pRecordSet = m_pDaoSupport->GetRecordSet();
	while( !pRecordSet->IsEOF() )
	{
		CExcelTaskModel obj;
		if (FALSE == FillExcelTaskFild( pRecordSet, obj ) )
		{
			return paraError;
		}
		modeList.push_back( obj );
		pRecordSet->MoveNext();
	}
	return userExcOk;
}
BOOL CExcelTaskDao::ExportTask(const CString &prjName,const ListExcelTaskMode &lst ) 
{
	ASSERT( m_pDaoSupport.get() );
	if ( !m_pDaoSupport->IsOpen() )
	{
		return not_connect;
	}
	CString sql;
	CString tableName( _T("Sheet1") );
	sql.Format(_T("CREATE TABLE %s ([编号] int, [任务内容描述] text, [责任人] text,\
				  [任务状态] text, [涉及项目] text,[最新进度] int, [最新进度工时] float, [进度描述] text,\
				  [进度更新时间] text,\
				  [计划工时] float, [计划开始] text, [计划结束] text, [实际工时] float, \
				  [实际开始] text, [实际结束] text,[工时类型] text, [备注] text)"), tableName/*prjName*/ );
    if ( !m_pDaoSupport->Execute( sql ) )
    {
		return excSqlError;
    }
	//sql.Format(_T("select * from [%s$]"),prjName);
	sql.Format(_T("select * from %s"),tableName);
	if ( !m_pDaoSupport->ExecuteSql( sql ) )
	{
		return excSqlError;
	}
	CAdoRecordset *pRecordSet = m_pDaoSupport->GetRecordSet();
	bool isOpen = pRecordSet->IsOpen();
	for ( ListExcelTaskMode::const_iterator itor = lst.begin(); itor != lst.end(); itor++ )
	{
		try
		{
			pRecordSet->AddNew();
			FillFildData(  pRecordSet,  *itor );
			pRecordSet->Update();
		}
		catch( _com_error e )
		{
			TRACE( e.ErrorMessage() );
			pRecordSet->Close();
			return excSqlError;
		}
		catch (...)
		{
			pRecordSet->Close();
			return excSqlError;
		}
	}
	return userExcOk;
}
void CExcelTaskDao::FillFildData(  CAdoRecordset* pRecordSet, const CExcelTaskModel &obj ) const
{
	(*pRecordSet)[ _T("编号")]  = obj.GetNum();
	//obj.SetFatherNum( (*pRecordSet)[ _T("父级编号]")] );
	(*pRecordSet)[ _T("任务内容描述")] = obj.GetTaskName() ;
	(*pRecordSet)[ _T("工时类型")] = obj.GetWorkHourTypeStr();
	(*pRecordSet)[ _T("进度更新时间")] = obj.GetUpdateTimeStr();
	(*pRecordSet)[_T("涉及项目")] = obj.GetTaskTypeString();
	//obj.SetScore( (*pRecordSet)[ _T("评分")]  );
	//obj.SetDifficultyParam( (*pRecordSet)[ _T("难度系数")] );
	(*pRecordSet)[ _T("责任人")] = obj.GetOwnerName();
	(*pRecordSet)[_T("任务状态")] = obj.GetTaskStatus();
	(*pRecordSet)[_T("最新进度")] = obj.GetPercent();
	(*pRecordSet)[_T("最新进度工时")] = obj.GetWorkHours();
	(*pRecordSet)[_T("进度描述")] = obj.GetUpdateContent( );
	(*pRecordSet)[_T("计划工时")] = obj.GetPlanWorkHour();
	 (*pRecordSet)[_T("计划开始")] =  obj.GetPanStartDateStr( );
	(*pRecordSet)[_T("计划结束")] = obj.GetPanEndDateStr();
	(*pRecordSet)[_T("实际工时")] = obj.GetFactWorkHour();
	(*pRecordSet)[_T("实际开始")] = obj.GetFactStartDateStr();
	(*pRecordSet)[_T("实际结束")] = obj.GetFactEndDateStr();
	(*pRecordSet)[_T("备注")] = obj.GetDec();
}
BOOL CExcelTaskDao::FillExcelTaskFild( CAdoRecordset* pRecordSet, CExcelTaskModel &obj ) const
{
	CString value = (*pRecordSet)[ _T("编号")].ToString();
	if ( !value.IsEmpty() )
	{
		obj.SetNum( (*pRecordSet)[ _T("编号")] );
	}
	value =  (*pRecordSet)[ _T("父级编号")].ToString() ;
	if ( value.GetLength() > 0 )
	{
		obj.SetFatherNum( (*pRecordSet)[ _T("父级编号")] );
	}
	value = (*pRecordSet)[ _T("任务内容描述")].ToString();
	if ( value.IsEmpty() )
	{
		AfxMessageBox( _T("[任务内容描述]不能为空") );
		return FALSE;
	}
	obj.SetTaskName( value );
	value = (*pRecordSet)[ _T("工时类型")].ToString();
	CString str;
	if ( value.IsEmpty() )
	{
		str.Format(_T("任务'%s'的[工时类型]为空"), obj.GetTaskName() );
		AfxMessageBox( str );
		return FALSE;
	}
	obj.SetWorkHourTypeStr( value );
	value =  (*pRecordSet)[ _T("评分")].ToString() ;
	if ( !value.IsEmpty() )
	{
		obj.SetScore( (*pRecordSet)[ _T("评分")]  );
	}
	value = (*pRecordSet)[ _T("难度系数")].ToString();
	if ( !value.IsEmpty() )
	{
		obj.SetDifficultyParam( (*pRecordSet)[ _T("难度系数")] );
	}
	
	obj.SetOwnerName( (*pRecordSet)[_T("责任人")].ToString() );
	obj.SetTaskStatus( (*pRecordSet)[_T("任务状态")].ToString() );
	value = (*pRecordSet)[_T("最新进度")].ToString(); 
	int valueInt = 0;
	if ( !value.IsEmpty() )
	{
		if ( !CUtilityTool::GetObj()->ConvertStringToInt( value, valueInt )  )
		{
			str.Format(_T("任务'%s',[最新进度]字段包含非数字字符"), obj.GetTaskName() );
			AfxMessageBox(str);
			return FALSE;
		}
		obj.SetPercent(  (*pRecordSet)[_T("最新进度")] );
	}
	value = (*pRecordSet)[_T("最新进度工时")].ToString();
	if ( obj.GetPercent() > 0 && value.IsEmpty() )
	{
		str.Format(_T("任务'%s',最新进度不为空时,[最新进度工时]也不能为空"), obj.GetTaskName() );
		AfxMessageBox( str );
		return FALSE;
	}
	if ( !value.IsEmpty() )
	{
		obj.SetWorkHours( (*pRecordSet)[_T("最新进度工时")] );
	}
	value = (*pRecordSet)[_T("进度描述")].ToString() ;
	if ( obj.GetPercent() > 0 && value.IsEmpty() )
	{
		str.Format(_T("任务'%s',[最新进度]不为空时，[进度描述]也不能为空"), obj.GetTaskName() );
		AfxMessageBox( str );
		return FALSE;
	}
	obj.SetUpdateContent( value );
	value = (*pRecordSet)[_T("计划工时")].ToString();
	if ( value.IsEmpty() )
	{
		str.Format(_T("任务'%s',[计划工时]不能为空"), obj.GetTaskName() );
		AfxMessageBox( str );
		return FALSE;
	}
	obj.SetPlanWorkHour( (*pRecordSet)[_T("计划工时")] );
	value = (*pRecordSet)[_T("计划开始")].ToString();
	if ( value.IsEmpty() )
	{
		str.Format(_T("任务'%s',[计划开始]日期不能为空"), obj.GetTaskName() );
		AfxMessageBox( str );
		return FALSE;
	}
	obj.SetPanStartDateStr( value );
	value = (*pRecordSet)[_T("计划结束")].ToString();
	if ( value.IsEmpty() )
	{
		str.Format(_T("任务'%s',[计划结束]日期不能为空"), obj.GetTaskName() );
		AfxMessageBox( str );
		return FALSE;
	}
	obj.SetPanEndDateStr( value );
	if ( obj.GetPlanEndTime() < obj.GetPlanStartTime() )
	{
		str.Format(_T("任务'%s',[计划结束]日期比[计划开始]早"), obj.GetTaskName() );
		AfxMessageBox( str );
		return FALSE;
	}
	value =  (*pRecordSet)[ _T("实际工时")].ToString() ;
	if (value.GetLength() > 0 )
	{
		obj.SetFactWorkHour( (*pRecordSet)[_T("实际工时")] );
	}
	obj.SetFactStartDateStr( (*pRecordSet)[_T("实际开始")].ToString());
	obj.SetFactEndDateStr( (*pRecordSet)[_T("实际结束")].ToString() );
	obj.SetDec( (*pRecordSet)[_T("备注")].ToString() );
	CString attrrbutStr = (*pRecordSet)[_T("任务所属阶段")].ToString();
	obj.SetTaskAtrrbuteString( (*pRecordSet)[_T("任务所属阶段")].ToString() );
	return TRUE;
}
BOOL CExcelTaskDao::ExportWorkHour( const CString &tableName, const COleDateTime &beginDate, const COleDateTime &endDate,WorkHourList &datas )
{
	if ( tableName.IsEmpty() )
	{
		AfxMessageBox( _T("导出表名不能为空") );
		return paraError;
	}
	CString sql;
	CString workHourColumName, bakColumName ;
	workHourColumName.Format( _T("工时(%s~%s)"), beginDate.Format(_T("%Y-%m-%d")), endDate.Format(_T("%Y-%m-%d")) );
	bakColumName.Format(_T("备注(%s)"), COleDateTime::GetCurrentTime().Format(_T("%Y-%m-%d")) );
	
	sql.Format(_T("CREATE TABLE %s ([工时类型名称] text, [%s] float, [%s] text)"),  
		       tableName, workHourColumName, bakColumName );

	if ( !m_pDaoSupport->Execute( sql ) )
	{
		return excSqlError;
	}
	sql.Format(_T("select * from %s"),tableName);
	if ( !m_pDaoSupport->ExecuteSql( sql ) )
	{
		return excSqlError;
	}
	CAdoRecordset *pRecordSet = m_pDaoSupport->GetRecordSet();
	bool isOpen = pRecordSet->IsOpen();
	for ( WorkHourList::const_iterator itor = datas.begin(); itor != datas.end(); itor++ )
	{
		try
		{
			pRecordSet->AddNew();
			(*pRecordSet)[_T("工时类型名称")] = itor->workHourType;
			(*pRecordSet)[workHourColumName] = itor->totalWorkHour;
			(*pRecordSet)[bakColumName] = itor->schudulInfo;
			pRecordSet->Update();
		}
		catch( _com_error e )
		{
			TRACE( e.ErrorMessage() );
			pRecordSet->Close();
			return excSqlError;
		}
		catch (...)
		{
			pRecordSet->Close();
			return excSqlError;
		}
	}
	return userExcOk;
}
BOOL CExcelTaskDao::ExportWorkHour( const CString &tableName, CWorkHourData *pData, const ListUser& lstUser, const TaskIdStringMap &typeMap )
{
	if ( tableName.IsEmpty() )
	{
		AfxMessageBox( _T("导出表名不能为空") );
		return paraError;
	}
	CString sql;
	sql.Format(_T("CREATE TABLE %s ([工时类型名称] text,"), tableName );
	int colCnt = lstUser.size()+1;
	int rowCnt = typeMap.size();
	CString userName;
	ListUser::const_iterator itor = lstUser.begin();
	for( ; itor != lstUser.end(); itor++)
	{
		userName.Format(_T("[%s] float, "), itor->GetUserName_R() );
		sql += userName;
	}
	sql += _T("[合计] float)");
	if ( !m_pDaoSupport->Execute( sql ) )
	{
		return excSqlError;
	}
	sql.Format(_T("select * from %s"),tableName);
	if ( !m_pDaoSupport->ExecuteSql( sql ) )
	{
		return excSqlError;
	}
	CAdoRecordset *pRecordSet = m_pDaoSupport->GetRecordSet();
	bool isOpen = pRecordSet->IsOpen();
	int   pos = 0;
	for ( TaskIdStringMapConstItor itorType = typeMap.begin(); itorType != typeMap.end(); itorType++ )
	{
		try
		{
			pRecordSet->AddNew();
			(*pRecordSet)[_T("工时类型名称")] = itorType->second;
			for ( itor = lstUser.begin(); itor != lstUser.end(); itor++ )
			{
				(*pRecordSet)[itor->GetUserName_R()] = pData[pos].GetTaskWorkHour();
				pos++;
			}
			(*pRecordSet)[_T("合计")] = pData[pos].GetTaskWorkHour();
			pos++;
			pRecordSet->Update();
		}
		catch( _com_error e )
		{
			TRACE( e.ErrorMessage() );
			pRecordSet->Close();
			return excSqlError;
		}
		catch (...)
		{
			pRecordSet->Close();
			return excSqlError;
		}
	}
	return userExcOk;
}
