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
	sql.Format(_T("CREATE TABLE %s ([���] int, [������������] text, [������] text,\
				  [����״̬] text, [�漰��Ŀ] text,[���½���] int, [���½��ȹ�ʱ] float, [��������] text,\
				  [���ȸ���ʱ��] text,\
				  [�ƻ���ʱ] float, [�ƻ���ʼ] text, [�ƻ�����] text, [ʵ�ʹ�ʱ] float, \
				  [ʵ�ʿ�ʼ] text, [ʵ�ʽ���] text,[��ʱ����] text, [��ע] text)"), tableName/*prjName*/ );
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
	(*pRecordSet)[ _T("���")]  = obj.GetNum();
	//obj.SetFatherNum( (*pRecordSet)[ _T("�������]")] );
	(*pRecordSet)[ _T("������������")] = obj.GetTaskName() ;
	(*pRecordSet)[ _T("��ʱ����")] = obj.GetWorkHourTypeStr();
	(*pRecordSet)[ _T("���ȸ���ʱ��")] = obj.GetUpdateTimeStr();
	(*pRecordSet)[_T("�漰��Ŀ")] = obj.GetTaskTypeString();
	//obj.SetScore( (*pRecordSet)[ _T("����")]  );
	//obj.SetDifficultyParam( (*pRecordSet)[ _T("�Ѷ�ϵ��")] );
	(*pRecordSet)[ _T("������")] = obj.GetOwnerName();
	(*pRecordSet)[_T("����״̬")] = obj.GetTaskStatus();
	(*pRecordSet)[_T("���½���")] = obj.GetPercent();
	(*pRecordSet)[_T("���½��ȹ�ʱ")] = obj.GetWorkHours();
	(*pRecordSet)[_T("��������")] = obj.GetUpdateContent( );
	(*pRecordSet)[_T("�ƻ���ʱ")] = obj.GetPlanWorkHour();
	 (*pRecordSet)[_T("�ƻ���ʼ")] =  obj.GetPanStartDateStr( );
	(*pRecordSet)[_T("�ƻ�����")] = obj.GetPanEndDateStr();
	(*pRecordSet)[_T("ʵ�ʹ�ʱ")] = obj.GetFactWorkHour();
	(*pRecordSet)[_T("ʵ�ʿ�ʼ")] = obj.GetFactStartDateStr();
	(*pRecordSet)[_T("ʵ�ʽ���")] = obj.GetFactEndDateStr();
	(*pRecordSet)[_T("��ע")] = obj.GetDec();
}
BOOL CExcelTaskDao::FillExcelTaskFild( CAdoRecordset* pRecordSet, CExcelTaskModel &obj ) const
{
	CString value = (*pRecordSet)[ _T("���")].ToString();
	if ( !value.IsEmpty() )
	{
		obj.SetNum( (*pRecordSet)[ _T("���")] );
	}
	value =  (*pRecordSet)[ _T("�������")].ToString() ;
	if ( value.GetLength() > 0 )
	{
		obj.SetFatherNum( (*pRecordSet)[ _T("�������")] );
	}
	value = (*pRecordSet)[ _T("������������")].ToString();
	if ( value.IsEmpty() )
	{
		AfxMessageBox( _T("[������������]����Ϊ��") );
		return FALSE;
	}
	obj.SetTaskName( value );
	value = (*pRecordSet)[ _T("��ʱ����")].ToString();
	CString str;
	if ( value.IsEmpty() )
	{
		str.Format(_T("����'%s'��[��ʱ����]Ϊ��"), obj.GetTaskName() );
		AfxMessageBox( str );
		return FALSE;
	}
	obj.SetWorkHourTypeStr( value );
	value =  (*pRecordSet)[ _T("����")].ToString() ;
	if ( !value.IsEmpty() )
	{
		obj.SetScore( (*pRecordSet)[ _T("����")]  );
	}
	value = (*pRecordSet)[ _T("�Ѷ�ϵ��")].ToString();
	if ( !value.IsEmpty() )
	{
		obj.SetDifficultyParam( (*pRecordSet)[ _T("�Ѷ�ϵ��")] );
	}
	
	obj.SetOwnerName( (*pRecordSet)[_T("������")].ToString() );
	obj.SetTaskStatus( (*pRecordSet)[_T("����״̬")].ToString() );
	value = (*pRecordSet)[_T("���½���")].ToString(); 
	int valueInt = 0;
	if ( !value.IsEmpty() )
	{
		if ( !CUtilityTool::GetObj()->ConvertStringToInt( value, valueInt )  )
		{
			str.Format(_T("����'%s',[���½���]�ֶΰ����������ַ�"), obj.GetTaskName() );
			AfxMessageBox(str);
			return FALSE;
		}
		obj.SetPercent(  (*pRecordSet)[_T("���½���")] );
	}
	value = (*pRecordSet)[_T("���½��ȹ�ʱ")].ToString();
	if ( obj.GetPercent() > 0 && value.IsEmpty() )
	{
		str.Format(_T("����'%s',���½��Ȳ�Ϊ��ʱ,[���½��ȹ�ʱ]Ҳ����Ϊ��"), obj.GetTaskName() );
		AfxMessageBox( str );
		return FALSE;
	}
	if ( !value.IsEmpty() )
	{
		obj.SetWorkHours( (*pRecordSet)[_T("���½��ȹ�ʱ")] );
	}
	value = (*pRecordSet)[_T("��������")].ToString() ;
	if ( obj.GetPercent() > 0 && value.IsEmpty() )
	{
		str.Format(_T("����'%s',[���½���]��Ϊ��ʱ��[��������]Ҳ����Ϊ��"), obj.GetTaskName() );
		AfxMessageBox( str );
		return FALSE;
	}
	obj.SetUpdateContent( value );
	value = (*pRecordSet)[_T("�ƻ���ʱ")].ToString();
	if ( value.IsEmpty() )
	{
		str.Format(_T("����'%s',[�ƻ���ʱ]����Ϊ��"), obj.GetTaskName() );
		AfxMessageBox( str );
		return FALSE;
	}
	obj.SetPlanWorkHour( (*pRecordSet)[_T("�ƻ���ʱ")] );
	value = (*pRecordSet)[_T("�ƻ���ʼ")].ToString();
	if ( value.IsEmpty() )
	{
		str.Format(_T("����'%s',[�ƻ���ʼ]���ڲ���Ϊ��"), obj.GetTaskName() );
		AfxMessageBox( str );
		return FALSE;
	}
	obj.SetPanStartDateStr( value );
	value = (*pRecordSet)[_T("�ƻ�����")].ToString();
	if ( value.IsEmpty() )
	{
		str.Format(_T("����'%s',[�ƻ�����]���ڲ���Ϊ��"), obj.GetTaskName() );
		AfxMessageBox( str );
		return FALSE;
	}
	obj.SetPanEndDateStr( value );
	if ( obj.GetPlanEndTime() < obj.GetPlanStartTime() )
	{
		str.Format(_T("����'%s',[�ƻ�����]���ڱ�[�ƻ���ʼ]��"), obj.GetTaskName() );
		AfxMessageBox( str );
		return FALSE;
	}
	value =  (*pRecordSet)[ _T("ʵ�ʹ�ʱ")].ToString() ;
	if (value.GetLength() > 0 )
	{
		obj.SetFactWorkHour( (*pRecordSet)[_T("ʵ�ʹ�ʱ")] );
	}
	obj.SetFactStartDateStr( (*pRecordSet)[_T("ʵ�ʿ�ʼ")].ToString());
	obj.SetFactEndDateStr( (*pRecordSet)[_T("ʵ�ʽ���")].ToString() );
	obj.SetDec( (*pRecordSet)[_T("��ע")].ToString() );
	CString attrrbutStr = (*pRecordSet)[_T("���������׶�")].ToString();
	obj.SetTaskAtrrbuteString( (*pRecordSet)[_T("���������׶�")].ToString() );
	return TRUE;
}
BOOL CExcelTaskDao::ExportWorkHour( const CString &tableName, const COleDateTime &beginDate, const COleDateTime &endDate,WorkHourList &datas )
{
	if ( tableName.IsEmpty() )
	{
		AfxMessageBox( _T("������������Ϊ��") );
		return paraError;
	}
	CString sql;
	CString workHourColumName, bakColumName ;
	workHourColumName.Format( _T("��ʱ(%s~%s)"), beginDate.Format(_T("%Y-%m-%d")), endDate.Format(_T("%Y-%m-%d")) );
	bakColumName.Format(_T("��ע(%s)"), COleDateTime::GetCurrentTime().Format(_T("%Y-%m-%d")) );
	
	sql.Format(_T("CREATE TABLE %s ([��ʱ��������] text, [%s] float, [%s] text)"),  
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
			(*pRecordSet)[_T("��ʱ��������")] = itor->workHourType;
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
		AfxMessageBox( _T("������������Ϊ��") );
		return paraError;
	}
	CString sql;
	sql.Format(_T("CREATE TABLE %s ([��ʱ��������] text,"), tableName );
	int colCnt = lstUser.size()+1;
	int rowCnt = typeMap.size();
	CString userName;
	ListUser::const_iterator itor = lstUser.begin();
	for( ; itor != lstUser.end(); itor++)
	{
		userName.Format(_T("[%s] float, "), itor->GetUserName_R() );
		sql += userName;
	}
	sql += _T("[�ϼ�] float)");
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
			(*pRecordSet)[_T("��ʱ��������")] = itorType->second;
			for ( itor = lstUser.begin(); itor != lstUser.end(); itor++ )
			{
				(*pRecordSet)[itor->GetUserName_R()] = pData[pos].GetTaskWorkHour();
				pos++;
			}
			(*pRecordSet)[_T("�ϼ�")] = pData[pos].GetTaskWorkHour();
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
