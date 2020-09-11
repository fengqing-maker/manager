#ifndef CHANGE_ACTION_HERAD_3498
#define CHANGE_ACTION_HERAD_3498

#include "../inc/changeActionInterface.h"

class ChangeActionImp:public CChangeActionInc
{
public:
	ChangeActionImp(  CDaoFactory *pFactory );

	virtual BOOL InsertChange( const CTaskChange &);

	virtual BOOL GetChangeById( int id, CTaskChange &obj );
    virtual BOOL GetChangeByTaskId( int taskId, ListChange &list );
	virtual BOOL GetNewingChangeByTaskId( int taskId, ListChange &list );
	virtual BOOL GetMaxNewSupentChangeByTaskId( int taskId, CTaskChange &obj );
	virtual BOOL GetAllChange( ListChange &list );

    virtual BOOL DeleteChange( const CTaskChange &chObj );

	//action
	virtual BOOL SaveChange(  CTaskChange &chObj, CTaskObj &bakObj, BOOL isApply  );
	virtual BOOL ChangeDepartReview( BOOL isAgree,const CString &reviewName,const CTaskChange &chObj ); //部门评审
	virtual BOOL ChangeGernalReview( BOOL isAgree,const CString &reviewName,const CTaskChange &chObj ); //总经理评审
	virtual BOOL ChangePrjectReview(const CString &reviewName, const CTaskChange &chObj ); //项目经理点评
	virtual BOOL GetMaxNewChangeByTaskId( int taskId, CTaskChange &obj );

protected:
	BOOL  ChangeUnAgree( const CTaskChange &chObj  );
	BOOL  ChangeAgree( const CTaskChange &chObj,int *pScore = NULL );
protected:
	CChangeDaoInc*	m_pChangeDbDao;
	CTaskDaoInc*    m_pTaskDbDao;
};

#endif