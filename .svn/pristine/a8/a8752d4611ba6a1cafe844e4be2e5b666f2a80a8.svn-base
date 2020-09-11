#ifndef PRJ_BASE_GRID_MAGER_AW34I785934
#define PRJ_BASE_GRID_MAGER_AW34I785934


#include "../baseGridManger.h"



class CDepartBaseGridManger:public CBaseGridManger
{
public:
	CDepartBaseGridManger(  CActionServerFactory *pSeverFactory );

	const ListDepart& GetOwnerDepartment() const{ return m_allDepart;};
protected:
	void      InsertDepartNameColumn();
	void      InsertDepartDecColumn();
	void      InsertAgentUser();
	void      InsertAgentValidate();
	void      InsertDepartMember();
	void      InsertFahterDepart();
	
	void      UpdateDepart();

	CDepartment* GetDepartByRow( int nRow, ListDepart &lst);
	void   GetDepartNameById( int lineId, CString &str );
    BOOL   GetDepartMemberString( int prjId, CString &str );

protected:
	enum {
	   DepartName, DepartDec, DepartMember, DepartAgentUser, DepartAgentValidate,DepartFather,
	};
protected:
    CDepartSeverInc   * m_pDepartSever;
	const CUser*       m_pLoginUser;
	ListDepart         m_allDepart;
private:

};
#endif