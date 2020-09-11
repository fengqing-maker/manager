#ifndef  ACTION_SERVER_FACTORY_H_384U
#define  ACTION_SERVER_FACTORY_H_384U

#include "../../DataBase/factory/CDaoFactory.h"
#include "../inc/loginActionInterfac.h"
#include "../inc/connectDbseverInterface.h"
#include "../inc/taskSeverInterface.h"
#include "../inc/projectSeverInterface.h"
#include "../inc/scheduleAcionInterface.h"
#include "../inc/changeActionInterface.h"
#include "../inc/departSeverInterface.h"
#include "../inc/BackStageActionInc.h"
#include "../inc/WorkHourActionInc.h"
#include "../inc/SystemConfigInc.h"

#include <boost/scoped_ptr.hpp>

class CActionServerFactory
{
public:
	friend class CTaskExcelAction;

	CActionServerFactory();
    ~CActionServerFactory();
    static CString              GetErrorString(int) ;
	CLoginActionInc*            GetLoginServer();
	CConnectDbServerInc*        GetConnectSever();
    CTaskSeverInc*              GetTaskSever();
    CProjectSeverInc*           GetProjectSever();
	CScheduleActionInc*         GetScheduleSever();
	CChangeActionInc*           GetChangeSever();
	CDepartSeverInc*            GetDepartSever();
	CBackStageActionInc*        GetBackStageSver();
	CWorkHourSeverInc*          GetWorkHourSever();
	CSystemCfgSeverInc*         GetSystemCfgSever();

protected:
	CDaoFactory&  GetDaoFactory(){ return m_dbObjFactory;};
private:
	CDaoFactory         m_dbObjFactory;					
	boost::scoped_ptr<CLoginActionInc>     m_pLoginActionServer;			//好像是获取数据库的信息的基类
	boost::scoped_ptr<CConnectDbServerInc> m_pConectServer;					//好像是连接数据库的基类
	boost::scoped_ptr<CTaskSeverInc>       m_pTaskSever;					//关于任务服务的基类
	boost::scoped_ptr<CProjectSeverInc>    m_pPrjSever;						//关于项目操作的基类
	boost::scoped_ptr<CScheduleActionInc>  m_pScheduleSever;				//
	boost::scoped_ptr<CChangeActionInc>    m_pChangeAction;					//
	boost::scoped_ptr<CDepartSeverInc>     m_pDepartAction;					//关于部门管理的基类
	boost::scoped_ptr<CBackStageActionInc>     m_pBackAction;				//关于后台管理的基类
	boost::scoped_ptr<CWorkHourSeverInc>       m_pWorkHourAction;			//关于任务工时的基类
    boost::scoped_ptr<CSystemCfgSeverInc>       m_pSystemCfgAction;			//关于系统管理员的基类
}; 

#endif