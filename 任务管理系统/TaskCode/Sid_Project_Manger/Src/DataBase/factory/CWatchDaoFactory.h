#ifndef  CFACTORY_TYPE_HEARD_343565
#define  CFACTORY_TYPE_HEARD_343565
/*
Dao数据接口服务类
*/
//#include "../inc/DaoSupportInterface.h"
//#include "../inc/CUserDaoInterface.h"
//#include "../inc/changeDaointerface.h"
//#include "../inc/departmentInterface.h"
#include "../inc/taskdaointerface.h"
//#include "../inc/projectDbDaoInterface.h"
#include <boost/scoped_ptr.hpp>

class CWatchDaoFactory
{
public:
	CWatchDaoFactory();
	~CWatchDaoFactory();
    
	//CUserDaoInterface *    GetUserDao();
 //   CChangeDaoInc *        GetChangeDbDao();
 //   CDepartmentAdoInc *    GetDepartDbDao();
    CTaskDaoInc *          GetTaskDbDao();
    CDaoSupportInterface*  GetDaoSupportDao();
	 /* CProjectDbDaoInc*      GetPrjDbDao();*/
protected:

private:
    boost::scoped_ptr<CDaoSupportInterface> m_pBaseDaoInc;   //数据连接类
	//CUserDaoInterface    *m_pUserInc;
    //CChangeDaoInc        *m_pChangeInc;
    //CDepartmentAdoInc    *m_pDepartInc;
     boost::scoped_ptr<CTaskDaoInc> m_pTaskDaoInc;
	//CProjectDbDaoInc     *m_pPrjDbInc;
};
#endif