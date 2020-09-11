#ifndef  PROJECT_DB_DAO_IMP_H_3I4U89
#define  PROJECT_DB_DAO_IMP_H_3I4U89

#include "../inc/projectDbDaoInterface.h"

class CProjectDbDaoImp:public CProjectDbDaoInc
{
public:
	CProjectDbDaoImp(  CDaoSupportInterface* pDao );
	//插入
	virtual BOOL InsertProjct( const CProject &obj );
	virtual BOOL InsertProjctLine( const CProductLine &obj );
	virtual BOOL InsertPrjoctMember( int prjId, int userId );
	//更新项目
    virtual BOOL UpdateProjectField( int id, const TaskIdStringMap &fields );
	//更新产品线
	virtual BOOL UpdateProductLineField( int , const TaskIdStringMap &fields );
	//产品
	virtual BOOL GetProjectByMangerId( int mangerId, ListProject& );
	virtual BOOL GetTaskPartProject( int mangerId, ListProject&listObj );
	virtual BOOL GetProjectByLine( int lineId, ListProject& );
	virtual BOOL GetProjectByPrjLineNotManger( int lineId, int userId, ListProject& ); //获取不是某人负责的项目
	virtual BOOL GetPrjectById( int id, CProject &obj );
	virtual BOOL GetProjectByName( const CString &str, CProject &obj );
	virtual BOOL GetAllProject( ListProject &prj );
	virtual BOOL GetProjectByMember( int memberId, ListProject &pj );
	//产品线
	virtual BOOL GetAllProjectLine( ListProjectLine &obj );
	virtual BOOL GetProjectLineByMangerId( int mangerId, ListProjectLine& );
	virtual BOOL GetProjectLineById( int id, CProductLine& );
	virtual BOOL GetProjectLineByName( const CString &str, CProductLine& obj  );

	//删除
	virtual BOOL DeleteMemberByPrj( int prjId, int userId = -1  );
	virtual BOOL DeleteProjectById( int prjId );
	virtual BOOL DeleteProductLineById( int id );
	//任务阶段
	virtual BOOL GetCntPrjTask( int prjId, int stepId, int taskSt, int &cnt ); // taskSt = -1，查询所有的
	virtual BOOL GetCntPrjTaskByEndTime( int prjId, int stepId, const COleDateTime &, int &cnt ); //查询大于此日期
protected:
	void    FillProjectFromFild( CAdoRecordset* , CProject &obj);
	void    FillProductLineFromFild( CAdoRecordset* , CProductLine &obj);
	BOOL    ExecSqLReslut( const CString &str, ListProjectLine &listObj);
	BOOL    ExecSqLReslut( const CString &str, ListProject &listObj);
	BOOL    ValidateProjectExit( int id );
	BOOL    ValidateProductLintExit( int id );
	CString GetFieldStringByType(int fieldType );
	CString GetProductLineStringFieldByType(int fieldType );

protected:
	CString m_prjTableName;
	CString m_prjLineTableName;
	const CString m_taskTableName;
};
#endif