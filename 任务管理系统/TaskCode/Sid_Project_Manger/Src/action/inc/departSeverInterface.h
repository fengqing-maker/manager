#ifndef  DEPART_SEVER_INTERFAC_AW3948
#define  DEPART_SEVER_INTERFAC_AW3948

#include "../../DataBase/factory/CDaoFactory.h"

class CDepartSeverInc
{
public:
	CDepartSeverInc( CDaoFactory  *pObj ) { m_pDbDao = pObj; }
	virtual ~CDepartSeverInc(){};

	virtual BOOL InsertDepart( CDepartment &obj ) = 0;

	virtual BOOL GetDepartById( int id, CDepartment&) = 0; 
	virtual BOOL GetDepartByName( const CString str, CDepartment &) = 0;

	virtual BOOL GetTopDepartByMangerId( int userId, ListDepart &list ) = 0;
	virtual BOOL GetAllDepart( ListDepart &list )= 0;

    virtual BOOL GetDepartUser( int departId, ListUser &usr, BOOL isIncludeGroup ) = 0;
	virtual BOOL GetDepartByMangerId( int userId, const CString &roleName, ListDepart &list ) = 0;

	virtual BOOL UpdateDepartName( int id, const CString &name ) = 0;
	virtual BOOL UpdateDepartDec( int id, const CString &str ) = 0;
	virtual BOOL UpdateBakValidate( int id, BOOL validate ) = 0;
	virtual BOOL UpdateDepartManger( const CDepartment &depart )  = 0;
	virtual BOOL UpdateDepartBakManger( const CDepartment &depart )  = 0;
	virtual BOOL UpdateFatherDepart( int departId, const CString &fatherNameStr ) = 0 ;
    virtual BOOL DeleteDepart( int departId ) = 0;
protected:
	CDaoFactory       *m_pDbDao;
	CDepartmentAdoInc *m_pDepartDao;
};

#endif