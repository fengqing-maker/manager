#ifndef PRJECT_DB_DAO_I34809
#define PRJECT_DB_DAO_I34809
/*
项目及产品数据处理接口
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
	//插入
	virtual BOOL InsertProjct( const CProject &obj ) = 0;
	virtual BOOL InsertProjctLine( const CProductLine &obj ) = 0;
	virtual BOOL InsertPrjoctMember( int prjId, int userId ) = 0;

	//更新项目
	virtual BOOL UpdateProjectField( int prjId, const TaskIdStringMap &fields ) = 0;
	//更新产品线
	virtual BOOL UpdateProductLineField( int , const TaskIdStringMap &fields ) = 0; 
	//产品
	virtual BOOL GetProjectByMangerId( int mangerId, ListProject& ) = 0;
	virtual BOOL GetTaskPartProject( int mangerId, ListProject& ) = 0;
	virtual BOOL GetProjectByLine( int lineId, ListProject& ) = 0;
	virtual BOOL GetProjectByPrjLineNotManger( int lineId, int userId, ListProject& ) = 0; //获取不是某人负责的项目
	virtual BOOL GetPrjectById( int id, CProject &obj ) = 0;
	virtual BOOL GetProjectByName( const CString &str, CProject &obj ) = 0;
	virtual BOOL GetAllProject( ListProject &list ) = 0;
	//产品线
	virtual BOOL GetAllProjectLine( ListProjectLine &obj ) = 0;
	virtual BOOL GetProjectLineByMangerId( int mangerId, ListProjectLine& ) = 0;
	virtual BOOL GetProjectLineById( int id, CProductLine& ) = 0;
	virtual BOOL GetProjectLineByName( const CString &str, CProductLine& obj  ) = 0;
	//virtual BOOL GetPrjByMember( int memberId, ListProject &lst ) = 0; //获取该用户参与的项目
	//删除
	virtual BOOL DeleteMemberByPrj( int prjId, int userId = -1  ) = 0;
	virtual BOOL DeleteProjectById( int prjId ) = 0;
	virtual BOOL DeleteProductLineById( int id ) = 0;
	//任务阶段
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