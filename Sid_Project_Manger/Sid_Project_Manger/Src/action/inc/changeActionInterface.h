#ifndef  CHANGE_ACTION_INC_H_3847
#define  CHANGE_ACTION_INC_H_3847

#include "../../DataBase/factory/CDaoFactory.h"
class CChangeActionInc
{
public:
	CChangeActionInc( CDaoFactory  *pObj ) { m_pDbDao = pObj; }
	virtual ~CChangeActionInc(){};
	virtual BOOL InsertChange( const CTaskChange &) = 0;
    
    virtual BOOL GetChangeById( int id, CTaskChange &obj ) = 0;
	virtual BOOL GetChangeByTaskId( int taskId, ListChange &list ) = 0;
	virtual BOOL GetNewingChangeByTaskId( int taskId, ListChange &list ) = 0;
	virtual BOOL GetMaxNewSupentChangeByTaskId( int taskId, CTaskChange &obj ) = 0;
	virtual BOOL GetAllChange( ListChange &list ) = 0;
	virtual BOOL DeleteChange( const CTaskChange &chObj ) = 0;
	
	//action
	virtual BOOL SaveChange( CTaskChange &chObj,  CTaskObj &bakObj, BOOL isApply ) = 0;
	virtual BOOL ChangeDepartReview( BOOL isAgree,const CString &reviewName,/* const CTaskObj &taskObj,*/ const CTaskChange &chObj ) = 0; //部门评审
	virtual BOOL ChangeGernalReview( BOOL isAgree,const CString &reviewName,/* const CTaskObj &taskObj,*/ const CTaskChange &chObj ) = 0; //总经理评审
	virtual BOOL ChangePrjectReview(const CString &reviewName, const CTaskChange &chObj ) = 0; //项目经理点评
	virtual BOOL GetMaxNewChangeByTaskId( int taskId, CTaskChange &obj ) = 0;

   
protected:
	CDaoFactory  *m_pDbDao;
};
#endif