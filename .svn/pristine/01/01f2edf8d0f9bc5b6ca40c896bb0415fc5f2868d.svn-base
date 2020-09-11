#ifndef  CFACTORY_TYPE_HEARD_343565
#define  CFACTORY_TYPE_HEARD_343565
/*
Dao数据接口服务类
*/
#include "../inc/DaoSupportInterface.h"
#include "../inc/CUserDaoInterface.h"
#include "../inc/changeDaointerface.h"
#include "../inc/departmentInterface.h"
#include "../inc/taskdaointerface.h"
#include "../inc/projectDbDaoInterface.h"
#include "../inc/WorkHourDbInterface.h"
#include "../inc/SystemCfgDbInterface.h"

class CDaoFactory
{
public:
	CDaoFactory();
	~CDaoFactory();
    
	CUserDaoInterface *    GetUserDao();
    CChangeDaoInc *        GetChangeDbDao();
    CDepartmentAdoInc *    GetDepartDbDao();
    CTaskDaoInc *          GetTaskDbDao();
    CDaoSupportInterface*  GetDaoSupportDao();
	CProjectDbDaoInc*      GetPrjDbDao();
	CWorkHourDbInc*         GetWorkHourDbDao();
	CSystemCfgDbInc*       GetSystemCfgDbDao();

protected:

private:
    CDaoSupportInterface *m_pBaseDaoInc;   //数据连接类
	CUserDaoInterface    *m_pUserInc;
    CChangeDaoInc        *m_pChangeInc;
    CDepartmentAdoInc    *m_pDepartInc;
    CTaskDaoInc          *m_pTaskDaoInc;
	CProjectDbDaoInc     *m_pPrjDbInc;

	CWorkHourDbInc        *m_pWorkHourInc;
	CSystemCfgDbInc      *m_pSystemCfgInc;

};
#endif