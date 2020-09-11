#ifndef  CHANGE_DAO_IMP_3947_H
#define  CHANGE_DAO_IMP_3947_H

#include "../inc/changeDaointerface.h"

class CChangeDaoImp:public CChangeDaoInc
{
public:
	CChangeDaoImp(  CDaoSupportInterface* pDao );

	virtual BOOL InsertChangeItem( const CTaskChange &obj ) ;
	virtual BOOL UpdateFirstReview( const ReviewContent &obj ) ;
	virtual BOOL UpdateSecondReview( const ReviewContent &obj  );
	virtual BOOL UpdateChange(int id, TaskIdStringMap fileds );

	virtual BOOL GetChangeItem( int changId, CTaskChange &obj );
	virtual BOOL GetMaxNewChangeItem( int taskId, CTaskChange &obj );
	virtual BOOL GetMaxNewChangeItem( int taskId, int type, CTaskChange &obj );
	virtual BOOL GetChangeItemsByTaskId( int taskId, ListChange& objList );
	virtual BOOL GetChangeByTaskId( int taskId, int status,ListChange& objList);
	virtual BOOL GetChangeItemsByReviewId( int geviewId, ListChange& objList );
	virtual BOOL GetChangeItemsByGeneralId( int generalId, ListChange& objList );
	virtual BOOL GetChangerItemsByReviewUser( int id, ListChange &objList );   //获取待一二级评审任务
	virtual BOOL GetChangerItemsByTaskOwner( int userId, ListChange &objList );
	virtual BOOL GetChangerItemsByProjectManger( int userId, ListChange &objList ); //获取待项目经理点评任务
	virtual BOOL GetAllChange( ListChange &list );
	virtual BOOL DeleteChange( int id );

	
protected:
	 BOOL ValidateCChangeExitById( int id );
	 void FillChangeFromField( CAdoRecordset* pRecordSet, CTaskChange &obj);
	 BOOL ExcSqlChangeList( const CString &str, ListChange &obj );
	 CString GetFieldStringByType(int fieldType );
private:
	CString m_changeTableName;
};
#endif