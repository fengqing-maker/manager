#ifndef PRJECT_DB_DAO_I34809
#define PRJECT_DB_DAO_I34809
/*
��Ŀ����Ʒ���ݴ���ӿ�
*/
#include "DaoSupportInterface.h"
#include "../../Model/Project.h"
#include "../../Model/ProductLine.h"
#include <list>
typedef std::list<CProject> ListProject;
typedef std::list<CProductLine> ListProjectLine;


class CProjectDbDaoInc
{
public:
	CProjectDbDaoInc( CDaoSupportInterface* pDao ){ m_pDaoSupportInc = pDao;}
	virtual ~CProjectDbDaoInc(){};
	//����
	virtual BOOL InsertProjct( const CProject &obj ) = 0;
	virtual BOOL InsertProjctLine( const CProductLine &obj ) = 0;
	virtual BOOL InsertPrjoctMember( int prjId, int userId ) = 0;

	//������Ŀ
	virtual BOOL UpdateProjectField( int prjId, const TaskIdStringMap &fields ) = 0;
	//���²�Ʒ��
	virtual BOOL UpdateProductLineField( int , const TaskIdStringMap &fields ) = 0; 
	//��Ʒ
	virtual BOOL GetProjectByMangerId( int mangerId, ListProject& ) = 0;
	virtual BOOL GetTaskPartProject( int mangerId, ListProject& ) = 0;
	virtual BOOL GetProjectByLine( int lineId, ListProject& ) = 0;
	virtual BOOL GetProjectByPrjLineNotManger( int lineId, int userId, ListProject& ) = 0; //��ȡ����ĳ�˸������Ŀ
	virtual BOOL GetPrjectById( int id, CProject &obj ) = 0;
	virtual BOOL GetProjectByName( const CString &str, CProject &obj ) = 0;
	virtual BOOL GetAllProject( ListProject &list ) = 0;
	//��Ʒ��
	virtual BOOL GetAllProjectLine( ListProjectLine &obj ) = 0;
	virtual BOOL GetProjectLineByMangerId( int mangerId, ListProjectLine& ) = 0;
	virtual BOOL GetProjectLineById( int id, CProductLine& ) = 0;
	virtual BOOL GetProjectLineByName( const CString &str, CProductLine& obj  ) = 0;
	//virtual BOOL GetPrjByMember( int memberId, ListProject &lst ) = 0; //��ȡ���û��������Ŀ
	//ɾ��
	virtual BOOL DeleteMemberByPrj( int prjId, int userId = -1  ) = 0;
	virtual BOOL DeleteProjectById( int prjId ) = 0;
	virtual BOOL DeleteProductLineById( int id ) = 0;
	//����׶�
	virtual BOOL GetCntPrjTask( int prjId, int stepId, int taskSt, int &cnt ) = 0;
	virtual BOOL GetCntPrjTaskByEndTime( int prjId, int stepId, const COleDateTime &, int &cnt ) = 0;
	virtual BOOL    GetConect() { 
		if ( m_pDaoSupportInc)
		{	m_pDaoSupportInc->IsOpen();}
		return FALSE;}
protected:
	CDaoSupportInterface* m_pDaoSupportInc;

};
#endif