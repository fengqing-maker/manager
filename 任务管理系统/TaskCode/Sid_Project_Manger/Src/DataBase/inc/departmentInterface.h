#ifndef DEPARTMENT_INTERFACE_HEARD_384U
#define DEPARTMENT_INTERFACE_HEARD_384U
/*
部门及工作组的数据操作接口
*/
#include "DaoSupportInterface.h"
#include "../../Model/department.h"
#include <list>
typedef std::list<CDepartment> ListDepart; 
typedef ListDepart::iterator   ListDepartItor; 
typedef ListDepart::const_iterator   ListDepartConstItor; 


class CDepartmentAdoInc
{
public:
	CDepartmentAdoInc( CDaoSupportInterface* pDao ){ m_pDaoSupportInc = pDao;}
	virtual ~CDepartmentAdoInc(){}

	virtual BOOL AddDepartement( const CDepartment& obj) = 0;  //返回0成功

	virtual BOOL UpdateDepartement( const CDepartment& obj) = 0;  //,主要用于全部内容，返回0成功
	virtual BOOL UpdateDepartField( int id, const TaskIdStringMap &fields ) = 0;

    virtual BOOL GetDepartmentById( int id, CDepartment &obj) = 0;
	virtual BOOL GetDepartmentByName( const CString &name, CDepartment &obj ) = 0;
	virtual BOOL GetAllDepart( ListDepart& ) = 0;
	virtual BOOL GetDepartmentByMangerId( int mangerId, ListDepart &obj, int topId ) =0 ;
    virtual BOOL GetChildDepart( int departId, ListDepart &listObj ) = 0;
	virtual BOOL FindDepartByBakMangerId( int userId, ListDepart &depart ) = 0; //查找代理人生效的部门

	virtual BOOL DeleteDepart( int departId ) = 0;
	virtual BOOL    GetConect() { 
		if ( m_pDaoSupportInc)
		{	m_pDaoSupportInc->IsOpen();}
		return FALSE;}
protected:
	CDaoSupportInterface* m_pDaoSupportInc;
};

#endif