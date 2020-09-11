#ifndef DEPART_SEVER_IMP_HERAD_34U
#define DEPART_SEVER_IMP_HERAD_34U

#include "../inc/departSeverInterface.h"

class CDepartSeverImp:public CDepartSeverInc
{
public:
	CDepartSeverImp(  CDaoFactory *pFactory );
	virtual BOOL InsertDepart( CDepartment &obj );

	//BOOL     GetOrganizationString( const CUser &obj, CString name );
	virtual BOOL GetDepartById( int id, CDepartment& );
	virtual BOOL GetDepartByName( const CString str, CDepartment &);
	virtual BOOL GetTopDepartByMangerId( int userId, ListDepart &list );
	virtual BOOL GetAllDepart( ListDepart &list );
	
	virtual BOOL GetDepartUser( int departId, ListUser &usr, BOOL isIncludeChildDepart );
    virtual BOOL GetDepartByMangerId( int userId, const CString &roleName, ListDepart &list );
	//¸üÐÂ
	virtual BOOL UpdateDepartName( int id, const CString &name );
	virtual BOOL UpdateDepartDec( int id, const CString &str );
	virtual BOOL UpdateBakValidate( int id, BOOL validate );
	virtual BOOL UpdateDepartManger( const CDepartment &depart );
	virtual BOOL UpdateDepartBakManger( const CDepartment &depart );
	virtual BOOL UpdateFatherDepart( int departId, const CString &fatherNameStr );
	//
	virtual BOOL DeleteDepart( int departId );

protected:
};

#endif