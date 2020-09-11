#ifndef DEPARTMENT_DAO_IMP
#define DEPARTMENT_DAO_IMP

#include "../inc/departmentInterface.h"


class CDepartmentAdoImp:public CDepartmentAdoInc
{
public:
	CDepartmentAdoImp( CDaoSupportInterface* pDao );

	virtual BOOL AddDepartement( const CDepartment& obj) ;  //����0�ɹ�
	virtual BOOL UpdateDepartement( const CDepartment& obj) ;  //,��Ҫ�����޸ĸ����ˣ�����0�ɹ�
	virtual BOOL UpdateDepartField( int id, const TaskIdStringMap &fields );

	virtual BOOL GetDepartmentById( int id, CDepartment &obj);
	virtual BOOL GetDepartmentByName( const CString &name, CDepartment &obj );
	virtual BOOL GetAllDepart( ListDepart&);
	virtual BOOL GetDepartmentByMangerId( int mangerId, ListDepart &obj, int topId ) ; //isTop = -1�������ж�������ɸѡ
	virtual BOOL GetChildDepart( int departId, ListDepart &listObj );
	virtual BOOL FindDepartByBakMangerId( int userId, ListDepart &depart );


	virtual BOOL DeleteDepart( int departId );
	

protected:
	void    FillDepartFromFild( CAdoRecordset* , CDepartment &obj);
    BOOL    ExecSqLReslut( const CString &str, ListDepart &listObj);
	CString GetFieldStringByType(int fieldType );
private:
	CString               m_strDepartTableName;
	CString               m_strGroupTableName;

};

#endif