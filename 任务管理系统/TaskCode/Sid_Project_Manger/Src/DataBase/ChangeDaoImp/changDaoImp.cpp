#include "stdafx.h"
#include "changeDaoImp.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CChangeDaoImp::CChangeDaoImp( CDaoSupportInterface* pDao ):CChangeDaoInc( pDao )
{
	m_changeTableName = _T( "change_task" );
}
 BOOL CChangeDaoImp::InsertChangeItem( const CTaskChange &obj ) 
 {/*添加status字段，并操作怎么知道插入记录后的id号*/
	 CString str;
	 str.Format( _T("insert into %s (change_item,change_content,change_time,change_reason,\
					change_taskid,change_bakId,review_content,general_manager_review,general_manager_id,\
					auditor_id,audior_date,general_date,status, is_impact_schdule, task_type, project_review, change_reson_option)\
					values(%d,'%s','%s','%s',%d,%d,'%s','%s',%d,%d,'%s','%s',%d, %d, %d, '%s', %d)"), 
					m_changeTableName,obj.GetType(),obj.GetChangeContent(),obj.GetChangeDateTimeStr(), obj.GetChangeReason(), 
					obj.GetTaskId(), obj.GetBakTaskId(), obj.GetReviewContent(), obj.GetGeneralReviewContent(),obj.GetGeneralAuditorId(),
					obj.GetAuditorId(), obj.GetReviewTimeStr(), obj.GetGeneralReviewTimeStr(),obj.GetStatus(),
					obj.GetImpactSchudle(), obj.GetChangeTaskType(), obj.GetProjectReview(), obj.GetChangeResonOption() );
	 if ( FALSE == m_pDaoSupportInc->Execute(str) )
	 {
		 return excSqlError;
	 }
	 return userExcOk;
 }
 BOOL CChangeDaoImp::UpdateFirstReview( const ReviewContent &obj ) 
 {
	 BOOL ret; 
	 if ( userExcOk != (ret = ValidateCChangeExitById( obj.m_id )) )
	 {
		 return ret;
	 }
	 CString str;
	 str.Format( _T("update %s t set t.review_content='%s',t.audior_date = '%s',t.status = %d where t.change_id = %d"),
		        m_changeTableName, obj.m_reviewContent, obj.GetReiewTimeString(),obj.m_result, obj.m_id );
	 if ( FALSE == m_pDaoSupportInc->ExecuteSql(str) )
	 {
		 return excSqlError;
	 }
	 return userExcOk;
 }
 BOOL CChangeDaoImp::UpdateSecondReview( const ReviewContent &obj  )
 {
	 BOOL ret; 
	 if ( userExcOk != (ret = ValidateCChangeExitById( obj.m_id )) )
	 {
		 return ret;
	 }
	 CString str;
	 str.Format( _T("update %s t set t.general_manager_review='%s', t.general_date = '%s',t.status = %d where t.change_id = %d"),
		 m_changeTableName, obj.m_reviewContent, obj.GetReiewTimeString(),obj.m_result, obj.m_id);
	 if ( FALSE == m_pDaoSupportInc->ExecuteSql(str) )
	 {
		 return excSqlError;
	 }
	 return userExcOk;
 }
 BOOL CChangeDaoImp::UpdateChange(int id,  TaskIdStringMap fields )
 {
	 BOOL ret  = ValidateCChangeExitById( id )  ;
	 if ( userExcOk != ret )
	 {
		 return ret;
	 }
	 CString str;
	 str.Format( _T("update %s t set "), m_changeTableName );
	 TaskIdStringMap::const_iterator itor = fields.begin();
	 CString tempStr,fieldStr;
	 while( itor != fields.end()  )
	 {
		 fieldStr = GetFieldStringByType( itor->first);
		 if ( fieldStr.GetLength() <= 0 )
		 {
			 return paraError;
		 }
		 tempStr.Format(_T("t.%s = %s,"), fieldStr, itor->second );
		 str += tempStr;
		 itor++;
	 }
	 str.Delete( str.GetLength() - 1 );
	 CString sqlString;
	 sqlString.Format( _T("%s where t.change_id = %d"), str, id );
	 if ( FALSE == m_pDaoSupportInc->Execute(sqlString) )
	 {
		 return excSqlError;
	 }
	 return userExcOk;
 }
 BOOL CChangeDaoImp::GetChangeItem( int changId, CTaskChange &obj  )
 {
	 CString str;
	 str.Format(_T("select * from %s where change_id = %d"), m_changeTableName, changId );
	 if ( FALSE == m_pDaoSupportInc->ExecuteSql(str) )
	 {
		 return excSqlError;
	 }
	 CAdoRecordset* pRecordSet = m_pDaoSupportInc->GetRecordSet();
	 if ( pRecordSet->IsBOF() )
	 {
		 return notfind;
	 }
	 FillChangeFromField( pRecordSet, obj);
	 return userExcOk;
 }
 BOOL CChangeDaoImp::GetChangeItemsByTaskId( int taskId, ListChange &objList )
 {
	 CString str;
	 str.Format(_T("select * from %s where change_taskid = %d"), m_changeTableName, taskId );
	return ExcSqlChangeList( str, objList );
 }
BOOL CChangeDaoImp::GetChangerItemsByReviewUser( int userId, ListChange &objList )
{
	CString str;
	str.Format(_T("select * from %s where (auditor_id = %d and status = %d ) or (general_manager_id = %d and status = %d)"),
		m_changeTableName, userId, up_apper, userId, departAgree );
	return ExcSqlChangeList( str, objList );
}
//获取项目负责点评的变更且不属于自己申请的任务
BOOL CChangeDaoImp::GetChangerItemsByProjectManger( int userId, ListChange &objList )
{
	CString str;
	str.Format(_T("select * from %s where status = %d and  change_taskid in \
				  ( select task_id from task where task_ownerid != %d and task_project_id in\
				          (select project_id from project where project_managerid = %d) ) "),
		m_changeTableName, prj_remark_on, userId, userId); //2016-1-30 修改将筛选待项目经理点评的任务
	return ExcSqlChangeList( str, objList );
}
BOOL CChangeDaoImp::GetChangerItemsByTaskOwner( int userId, ListChange &objList )
{
	CString str;
	str.Format(_T("select * from %s where change_taskid in (select task_id from task where task_ownerid = %d )"),
				  m_changeTableName , userId );
	return ExcSqlChangeList( str, objList );
}
BOOL CChangeDaoImp::GetAllChange( ListChange &objList )
{
	CString str;
	str.Format(_T("select * from %s order by change_time desc"),
		m_changeTableName  );
	return ExcSqlChangeList( str, objList );
}
 //
  BOOL CChangeDaoImp::ExcSqlChangeList( const CString &str,  ListChange &objList )
  {
	  if ( FALSE == m_pDaoSupportInc->ExecuteSql(str) )
	  {
		  return excSqlError;
	  }
	  CAdoRecordset* pRecordSet = m_pDaoSupportInc->GetRecordSet();
	  //
	  int count = pRecordSet->GetRecordCount() ;
	 // pRecordSet->MoveFirst();
	  while( !pRecordSet->IsEOF() )
	  {
		  CTaskChange obj;
		  FillChangeFromField( pRecordSet, obj);
		  objList.push_back( obj );
		  pRecordSet->MoveNext();
	  }
	  return userExcOk;
  }
 BOOL CChangeDaoImp::ValidateCChangeExitById( int id )
 {
	 CString str;
	 str.Format(_T("select * from %s where change_id = %d"), m_changeTableName, id );
	 if ( FALSE == m_pDaoSupportInc->ExecuteSql(str) )
	 {
		 return excSqlError;
	 }
	 CAdoRecordset* pRecordSet = m_pDaoSupportInc->GetRecordSet();
	 if ( pRecordSet->IsBOF() )
	 {
		 return notfind;
	 }
	 return userExcOk;
 }
 BOOL CChangeDaoImp::GetChangeItemsByReviewId( int userId, ListChange& objList )
 {
	 CString str;
	 str.Format(_T("select * from %s where auditor_id = %d and status = %d"), m_changeTableName, userId, up_apper );
	 return ExcSqlChangeList( str, objList );
 }
 BOOL CChangeDaoImp::GetChangeItemsByGeneralId( int userId, ListChange&objList )
 {
	 CString str;
	 str.Format(_T("select * from %s where general_manager_id = %d and status = %d"), m_changeTableName, userId, departAgree );
	 return ExcSqlChangeList( str, objList );
 }
BOOL CChangeDaoImp::GetChangeByTaskId( int taskId, int status,ListChange& objList)
{
	CString str;
	str.Format(_T("select * from %s where change_taskid = %d and status = %d"), m_changeTableName, taskId, status );
	return ExcSqlChangeList( str, objList );
}
 void CChangeDaoImp::FillChangeFromField( CAdoRecordset* pRecordSet, CTaskChange &obj)
 {
	 obj.SetId( (*pRecordSet)[_T("change_id")]  );
	 obj.SetType( (*pRecordSet)[_T("change_item")] );
	 obj.SetChangeContent( (*pRecordSet)[_T("change_content")].ToString() );
	 obj.SetChangeDateTimeStr( (*pRecordSet)[_T("change_time")].ToString() );
	 obj.SetChangeReason( (*pRecordSet)[_T("change_reason")].ToString() );
	 obj.SetTaskId( (*pRecordSet)[_T("change_taskid")] );
	 obj.SetBakTaskId( (*pRecordSet)[_T("change_bakId")] );
	 obj.SetReviewContent( (*pRecordSet)[_T("review_content")].ToString() );
	 obj.SetGeneralReviewContent( (*pRecordSet)[_T("general_manager_review")].ToString() );
	 obj.SetGeneralAuditorId( (*pRecordSet)[_T("general_manager_id")] );
	 obj.SetAuditorId( (*pRecordSet)[_T("auditor_id")] );
	 obj.SetReviewTimeStr( (*pRecordSet)[_T("audior_date")].ToString() );
	 obj.SetGeneralReviewTimeStr( (*pRecordSet)[_T("general_date")].ToString() );
	 obj.SetStatus( (*pRecordSet)[_T("status")] );
	 //2015-10-26 修改变更流程
	 obj.SetImpactSchudle( (*pRecordSet)[_T("is_impact_schdule")] );
	 obj.SetChangeTaskType( (*pRecordSet)[_T("task_type")] );
	 obj.SetProjectReview( (*pRecordSet)[_T("project_review")].ToString()  );
	 obj.SetChangeResonOption( (*pRecordSet)[_T("change_reson_option")] );
 }
 CString CChangeDaoImp::GetFieldStringByType(int fieldType )
 {
	 CString  strFild;
	 switch( fieldType )
	 {
	 case fild_change_content:
		 strFild = _T("change_content");
		 break;
	 case fild_change_time:
		 strFild = _T("change_time");
		 break;
	 case  fild_change_reason:
		 strFild = _T("change_reason");
		 break;
	 case  fild_auditor_id:
		 strFild = _T("auditor_id");
		 break;
	 case  fild_general_manager_id:
		 strFild = _T("general_manager_id");
		 break;
	 case  fild_change_item:
		 strFild = _T("change_item");
		 break;
	 case  fild_change_bakId:
		 strFild = _T("change_bakId");
		 break;
	 case  fild_status:
		 strFild = _T("status");
		 break;
	 case  fild_is_impact_schdule:
		 strFild = _T("is_impact_schdule");
		 break;
	 case  fild_task_type:
		 strFild = _T("task_type");
		 break;
	 case  fild_project_review:
		 strFild = _T("project_review");
		 break;
	 case  fild_change_reason_option:
		 strFild = _T("change_reson_option");
		 break;
	 }
	 return strFild;
 }
 BOOL CChangeDaoImp::DeleteChange( int id )
 {
	 CString str;
	 str.Format( _T( "delete from %s where change_id = %d"), m_changeTableName, id );
	 if ( TRUE != m_pDaoSupportInc->Execute( str ))
	 {
		 return excSqlError;
	 }
	 return userExcOk;
 }
 BOOL CChangeDaoImp::GetMaxNewChangeItem( int taskId, CTaskChange &obj )
 {
	 CString str;
	 str.Format( _T( "select * from %s where change_taskid = %d and change_time = (select max(change_time) from %s where change_taskid = %d )"),
		 m_changeTableName, taskId, m_changeTableName, taskId );
	 if ( FALSE == m_pDaoSupportInc->ExecuteSql(str) )
	 {
		 return excSqlError;
	 }
	 CAdoRecordset* pRecordSet = m_pDaoSupportInc->GetRecordSet();
	 if ( pRecordSet->IsBOF() )
	 {
		 return notfind;
	 }
	 FillChangeFromField( pRecordSet, obj );
	 return userExcOk;
 }
 BOOL CChangeDaoImp::GetMaxNewChangeItem( int taskId, int type, CTaskChange &obj )
 {
	 CString str;
	 str.Format( _T( "select * from %s where change_taskid = %d and change_time = (select max(change_time) from %s where change_taskid = %d and change_item = %d)"),
		 m_changeTableName, taskId, m_changeTableName, taskId, type );
	 if ( FALSE == m_pDaoSupportInc->ExecuteSql(str) )
	 {
		 return excSqlError;
	 }
	 CAdoRecordset* pRecordSet = m_pDaoSupportInc->GetRecordSet();
	 if ( pRecordSet->IsBOF() )
	 {
		 return notfind;
	 }
	 FillChangeFromField( pRecordSet, obj );
	 return userExcOk;
 }

