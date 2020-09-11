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
	boost::scoped_ptr<CLoginActionInc>     m_pLoginActionServer;
	boost::scoped_ptr<CConnectDbServerInc> m_pConectServer;
	boost::scoped_ptr<CTaskSeverInc>       m_pTaskSever;
	boost::scoped_ptr<CProjectSeverInc>    m_pPrjSever;
	boost::scoped_ptr<CScheduleActionInc>  m_pScheduleSever;
	boost::scoped_ptr<CChangeActionInc>    m_pChangeAction;
	boost::scoped_ptr<CDepartSeverInc>     m_pDepartAction;
	boost::scoped_ptr<CBackStageActionInc>     m_pBackAction;
	boost::scoped_ptr<CWorkHourSeverInc>       m_pWorkHourAction;
    boost::scoped_ptr<CSystemCfgSeverInc>       m_pSystemCfgAction;
}; 

#endif