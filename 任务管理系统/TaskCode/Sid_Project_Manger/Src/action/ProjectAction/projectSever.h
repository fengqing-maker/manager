#ifndef  PROJECT_SERVER_IMP_9347882
#define  PROJECT_SERVER_IMP_9347882

#include "../inc/projectSeverInterface.h"

class CProjectSever:public CProjectSeverInc
{
public:
	CProjectSever( CDaoFactory  *pObj );

	virtual BOOL GetProjectByManger( int mangerId, ListProject& obj );
	virtual BOOL GetProjectByLine( int lineId, ListProject &obj ) ;

	//新建项目
	virtual BOOL NewProject(const CProject &prj,  const ListUser & );
	virtual BOOL NewProductLine( const CProductLine &obj );

	virtual BOOL GetProjectMember( int prjId, ListUser &obj );//获取该用户参与的项目
	virtual BOOL GetProjectByMember( int memberId, ListProject &obj );
	virtual BOOL GetAllProject( ListProject &list );
	virtual BOOL GetProjectById( int prjId, CProject& obj);

	virtual BOOL GetProjectByUser( const CUser &useObj, BOOL isPowerReadAll, ListProject &list );
	virtual BOOL GetAllProjectLine( ListProjectLine &list );
	virtual BOOL GetProjectLineById( int lineId, CProductLine &obj );
	virtual BOOL GetProductLineByName( const CString &str,  CProductLine &obj );
	//项目
	virtual BOOL UpdatePrjShortName( int prjId, const CString &str );
	virtual BOOL UpdatePrjLongName( int prjId, const CString &str );
	virtual BOOL UpdatePrjProductLine( int prjId, int lineId );
	virtual BOOL UpdatePrjOwner( const CProject &prj );
	virtual BOOL UpdatePrjMember( const CProject &prj );
	//产品线
	virtual BOOL UpdateProductName( int id, const CString &str );
	virtual BOOL UpdateProductDC( int id, const CString &str );
	virtual BOOL UpdateProductOwner( const CProductLine &obj );

	//项目任务
	virtual BOOL GetProjectTask( int prjId, int taskSt, int ownerId, ListTask &obj );
	virtual BOOL GetProjectTaskNoFather( int prjId, int taskSt, int userId, ListTask &obj, int &hasChildCount, BOOL isOnlyShow  );
	virtual BOOL GetChildTask( int taskId, int st, int ownerId, ListTask &obj );
	virtual BOOL IsHasChildTask( int taskId, BOOL &isHasChild );
	virtual BOOL GetFatherTask( int taskId, CTaskObj &fatherTask );

	//删除任务
	virtual BOOL DeleteProject( int prjId );
	virtual BOOL DeleteProduct( int id );
	//项目流程
	virtual BOOL GetProjectFactStep( const CProject& prj, CString &str ); //获取项目实际所属阶段
	virtual BOOL GetProjectBelongStep( const CProject& prj, CString &str ); //获取项目应属阶段

protected:
	CDaoFactory      *m_pDaoFactory;
	CProjectDbDaoInc *m_prjDbDao;
	CSystemCfgDbInc  *m_systemCfgDbDao;
};

#endif