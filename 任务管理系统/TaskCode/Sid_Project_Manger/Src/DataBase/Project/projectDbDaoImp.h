#ifndef  PROJECT_DB_DAO_IMP_H_3I4U89
#define  PROJECT_DB_DAO_IMP_H_3I4U89

#include "../inc/projectDbDaoInterface.h"

class CProjectDbDaoImp:public CProjectDbDaoInc
{
public:
	CProjectDbDaoImp(  CDaoSupportInterface* pDao );
	//����
	virtual BOOL InsertProjct( const CProject &obj );
	virtual BOOL InsertProjctLine( const CProductLine &obj );
	virtual BOOL InsertPrjoctMember( int prjId, int userId );
	//������Ŀ
    virtual BOOL UpdateProjectField( int id, const TaskIdStringMap &fields );
	//���²�Ʒ��
	virtual BOOL UpdateProductLineField( int , const TaskIdStringMap &fields );
	//��Ʒ
	virtual BOOL GetProjectByMangerId( int mangerId, ListProject& );
	virtual BOOL GetTaskPartProject( int mangerId, ListProject&listObj );
	virtual BOOL GetProjectByLine( int lineId, ListProject& );
	virtual BOOL GetProjectByPrjLineNotManger( int lineId, int userId, ListProject& ); //��ȡ����ĳ�˸������Ŀ
	virtual BOOL GetPrjectById( int id, CProject &obj );
	virtual BOOL GetProjectByName( const CString &str, CProject &obj );
	virtual BOOL GetAllProject( ListProject &prj );
	virtual BOOL GetProjectByMember( int memberId, ListProject &pj );
	//��Ʒ��
	virtual BOOL GetAllProjectLine( ListProjectLine &obj );
	virtual BOOL GetProjectLineByMangerId( int mangerId, ListProjectLine& );
	virtual BOOL GetProjectLineById( int id, CProductLine& );
	virtual BOOL GetProjectLineByName( const CString &str, CProductLine& obj  );

	//ɾ��
	virtual BOOL DeleteMemberByPrj( int prjId, int userId = -1  );
	virtual BOOL DeleteProjectById( int prjId );
	virtual BOOL DeleteProductLineById( int id );
	//����׶�
	virtual BOOL GetCntPrjTask( int prjId, int stepId, int taskSt, int &cnt ); // taskSt = -1����ѯ���е�
	virtual BOOL GetCntPrjTaskByEndTime( int prjId, int stepId, const COleDateTime &, int &cnt ); //��ѯ���ڴ�����
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