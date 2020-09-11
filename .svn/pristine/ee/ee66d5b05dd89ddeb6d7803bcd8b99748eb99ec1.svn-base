#ifndef CHANGE_DAO_INTERFACE_343984
#define CHANGE_DAO_INTERFACE_343984
/*
 任务变更的操作数据接口
*/
#include "DaoSupportInterface.h"
#include "../../Model/TaskChange.h"
#include <list>

typedef std::list<CTaskChange> ListChange;
typedef ListChange::const_iterator ListChangeConstItor;

class CChangeDaoInc
{
public:
	CChangeDaoInc( CDaoSupportInterface* pDao ){ m_pDaoSupportInc = pDao;}
    virtual ~CChangeDaoInc(){}; 
   
	virtual BOOL InsertChangeItem( const CTaskChange &obj ) = 0;
	virtual BOOL UpdateFirstReview( const ReviewContent &obj ) = 0;
	virtual BOOL UpdateSecondReview( const ReviewContent &obj  ) = 0;
	virtual BOOL UpdateChange(int id, TaskIdStringMap fileds ) = 0;

	virtual BOOL GetChangeItem( int changId, CTaskChange &obj ) = 0;
    virtual BOOL GetMaxNewChangeItem( int taskId, CTaskChange &obj ) = 0;
	virtual BOOL GetMaxNewChangeItem( int taskId, int type, CTaskChange &obj ) = 0;
	virtual BOOL GetChangeItemsByTaskId( int taskId, ListChange& objList ) = 0;
	virtual BOOL GetChangeByTaskId( int taskId, int status,ListChange& objList) = 0;
	virtual BOOL GetChangeItemsByReviewId( int reviewId, ListChange& objList ) = 0;
	virtual BOOL GetChangeItemsByGeneralId( int generalId, ListChange& objList ) = 0;
	virtual BOOL GetChangerItemsByReviewUser( int id, ListChange &objList ) = 0;
	virtual BOOL GetChangerItemsByTaskOwner( int userId, ListChange &objList ) = 0;
	virtual BOOL GetChangerItemsByProjectManger( int userId, ListChange &objList ) = 0;
	virtual BOOL GetAllChange( ListChange &list ) = 0;
	virtual BOOL DeleteChange( int id ) = 0;

    virtual BOOL    GetConect() { 
    if ( m_pDaoSupportInc)
	{	m_pDaoSupportInc->IsOpen();}
	return FALSE;}
protected:
    CDaoSupportInterface* m_pDaoSupportInc;
};

#endif