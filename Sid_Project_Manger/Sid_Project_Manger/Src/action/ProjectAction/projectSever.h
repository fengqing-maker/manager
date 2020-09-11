#ifndef  PROJECT_SERVER_IMP_9347882
#define  PROJECT_SERVER_IMP_9347882

#include "../inc/projectSeverInterface.h"

class CProjectSever:public CProjectSeverInc
{
public:
	CProjectSever( CDaoFactory  *pObj );

	virtual BOOL GetProjectByManger( int mangerId, ListProject& obj );
	virtual BOOL GetProjectByLine( int lineId, ListProject &obj ) ;

	//�½���Ŀ
	virtual BOOL NewProject(const CProject &prj,  const ListUser & );
	virtual BOOL NewProductLine( const CProductLine &obj );

	virtual BOOL GetProjectMember( int prjId, ListUser &obj );//��ȡ���û��������Ŀ
	virtual BOOL GetProjectByMember( int memberId, ListProject &obj );
	virtual BOOL GetAllProject( ListProject &list );
	virtual BOOL GetProjectById( int prjId, CProject& obj);

	virtual BOOL GetProjectByUser( const CUser &useObj, BOOL isPowerReadAll, ListProject &list );
	virtual BOOL GetAllProjectLine( ListProjectLine &list );
	virtual BOOL GetProjectLineById( int lineId, CProductLine &obj );
	virtual BOOL GetProductLineByName( const CString &str,  CProductLine &obj );
	//��Ŀ
	virtual BOOL UpdatePrjShortName( int prjId, const CString &str );
	virtual BOOL UpdatePrjLongName( int prjId, const CString &str );
	virtual BOOL UpdatePrjProductLine( int prjId, int lineId );
	virtual BOOL UpdatePrjOwner( const CProject &prj );
	virtual BOOL UpdatePrjMember( const CProject &prj );
	//��Ʒ��
	virtual BOOL UpdateProductName( int id, const CString &str );
	virtual BOOL UpdateProductDC( int id, const CString &str );
	virtual BOOL UpdateProductOwner( const CProductLine &obj );

	//��Ŀ����
	virtual BOOL GetProjectTask( int prjId, int taskSt, int ownerId, ListTask &obj );
	virtual BOOL GetProjectTaskNoFather( int prjId, int taskSt, int userId, ListTask &obj, int &hasChildCount, BOOL isOnlyShow  );
	virtual BOOL GetChildTask( int taskId, int st, int ownerId, ListTask &obj );
	virtual BOOL IsHasChildTask( int taskId, BOOL &isHasChild );
	virtual BOOL GetFatherTask( int taskId, CTaskObj &fatherTask );

	//ɾ������
	virtual BOOL DeleteProject( int prjId );
	virtual BOOL DeleteProduct( int id );
	//��Ŀ����
	virtual BOOL GetProjectFactStep( const CProject& prj, CString &str ); //��ȡ��Ŀʵ�������׶�
	virtual BOOL GetProjectBelongStep( const CProject& prj, CString &str ); //��ȡ��ĿӦ���׶�

protected:
	CDaoFactory      *m_pDaoFactory;
	CProjectDbDaoInc *m_prjDbDao;
	CSystemCfgDbInc  *m_systemCfgDbDao;
};

#endif