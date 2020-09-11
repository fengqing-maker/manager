#ifndef  PRJ_SERVER_INTERFACE_39480239
#define  PRJ_SERVER_INTERFACE_39480239

#include "../../DataBase/factory/CDaoFactory.h"

class CProjectSeverInc
{
public:
	CProjectSeverInc( CDaoFactory  *pObj ) { m_pDbDao = pObj; }
	virtual ~CProjectSeverInc(){}

	//新建项目
	virtual BOOL NewProject(const CProject &prj, const ListUser & ) = 0;
	virtual BOOL NewProductLine( const CProductLine &obj ) = 0;

	virtual BOOL GetProjectByManger( int mangerId, ListProject &obj ) = 0;
	virtual BOOL GetProjectByMember( int memberId, ListProject &obj ) = 0; //参与或管理的项目
	virtual BOOL GetProjectByLine( int lineId, ListProject &obj ) = 0;
	virtual BOOL GetProjectMember( int prjId, ListUser& obj ) = 0;
	virtual BOOL GetAllProject( ListProject &list ) = 0;
    virtual BOOL GetProjectById( int prjId, CProject& obj) = 0;
	virtual BOOL GetProjectByUser(  const CUser &useObj, BOOL isPowerReadAll, ListProject &list ) = 0;
	
    virtual BOOL GetAllProjectLine( ListProjectLine &list ) = 0;
    virtual BOOL GetProjectLineById( int lineId, CProductLine &obj ) = 0;
	virtual BOOL GetProductLineByName( const CString &str, CProductLine &obj ) = 0;

	virtual BOOL UpdatePrjShortName( int prjId, const CString &str ) = 0;
	virtual BOOL UpdatePrjLongName( int prjId, const CString &str ) = 0;
	virtual BOOL UpdatePrjProductLine( int prjId, int lineId ) = 0;
	virtual BOOL UpdatePrjOwner( const CProject &prj ) = 0;
	virtual BOOL UpdatePrjMember( const CProject &prj ) = 0;

	virtual BOOL UpdateProductName( int id, const CString &str ) = 0;
	virtual BOOL UpdateProductDC( int id, const CString &str ) = 0;
	virtual BOOL UpdateProductOwner( const CProductLine &obj ) = 0;

	//项目任务
	virtual BOOL GetProjectTaskNoFather( int prjId, int taskSt, int ownerId,ListTask &obj, int &hasChildCount, BOOL isOnlyShow ) = 0;
	virtual BOOL GetProjectTask( int prjId, int taskSt, int ownerId, ListTask &obj ) = 0;

	virtual BOOL GetChildTask( int taskId, int st, int ownerId, ListTask &obj ) = 0;
	virtual BOOL IsHasChildTask( int taskId, BOOL &isHasChild ) = 0;
    virtual BOOL GetFatherTask( int taskId, CTaskObj &fatherTask ) = 0;

	virtual BOOL DeleteProject( int prjId ) = 0;
	virtual BOOL DeleteProduct( int id ) = 0;
	//流程
	virtual BOOL GetProjectFactStep( const CProject& prj, CString &str ) = 0; //获取项目实际所属阶段
	virtual BOOL GetProjectBelongStep( const CProject& prj, CString &str ) = 0; //获取项目应属阶段

protected:
	CDaoFactory  *m_pDbDao;
};

#endif