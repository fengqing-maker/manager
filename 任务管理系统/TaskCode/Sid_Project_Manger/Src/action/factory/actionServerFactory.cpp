#include "stdafx.h"
#include "actionServerFactory.h"
#include "../connectAction/connectActionImp.h"
#include "../UserAction/LoginActionImp.h"
#include "../TaskAction/taskSeverImp.h"
#include "../ProjectAction/projectSever.h"
#include "../ScheduleAction/ScheduleAction.h"
#include "../changeAction/changeActionImp.h"
#include "../departAction/departSeverImp.h"
#include "../backStageAction/BackStageActionImp.h"
#include "../workHourAction/WorkHourSeverImp.h"
#include "../SystemConfig/SystemConfigImp.h"

CActionServerFactory::CActionServerFactory()
{
}

CActionServerFactory::~CActionServerFactory()
{
}
CString   CActionServerFactory::GetErrorString(int erroInfo)
{
	CString ret;
	switch( erroInfo )
	{
	case insertNameRepeat:
		ret = _T("插入记录，名称重复");
		break;
	case excSqlError:
		ret = _T("执行Sql语句失败");
		break;
	case validateUserNone:
		ret = _T("用户不存在");
		break;
	case userPwdError:
		ret = _T("密码错误");
		break;
	case notfind:
		ret = _T("未查找到对应信息");
		break;
	case mapError:
		ret = _T("映射错误");
		break;
	case userExcOk:
		ret = _T("执行成功");
		break;
	case paraError:
		ret = _T("参数出错");
		break;
	case e_isOnline:
		ret = _T("用户已登录");
	case  not_connect:
		ret = _T("数据库未连接");
	case  connect_error:
		ret = _T("连接数据源失败");
	}
	return ret;
}
CLoginActionInc*     CActionServerFactory::GetLoginServer()
{
	//判断智能指针是否成功
	if ( NULL == m_pLoginActionServer.get() )
	{
		//使用智能指针、基类、子类进行操作，然后生成一个子类的指针
		//m_dbObjFactory是用来获取其他类的指针，获取的是各个类的基类指针
		m_pLoginActionServer.reset( new CLoginActionImp( &m_dbObjFactory ) ) ;	
	}
	return 	m_pLoginActionServer.get();
}
CConnectDbServerInc* CActionServerFactory::GetConnectSever()
{
	//如果m_pConnectServer不存在
	if ( NULL == m_pConectServer.get() )
	{
		//一个管理类的智能指针重新添加一个数据库连接类进行管理，但是后面这个我没看懂
		m_pConectServer.reset( new CConectDbImp( m_dbObjFactory.GetDaoSupportDao() ));
	}
	//然后返回这个智能指针
	return 	m_pConectServer.get();
}
CTaskSeverInc*        CActionServerFactory::GetTaskSever()
{
	if ( NULL == m_pTaskSever.get() )
	{
		m_pTaskSever.reset( new CTaskSeverImp( &m_dbObjFactory ));
	}
	return 	m_pTaskSever.get();
}
CProjectSeverInc*       CActionServerFactory::GetProjectSever()
{
	if ( NULL == m_pPrjSever.get() )
	{
		m_pPrjSever.reset( new CProjectSever( &m_dbObjFactory ) ) ;
	}
	return 	m_pPrjSever.get() ;
}
CScheduleActionInc*    CActionServerFactory::GetScheduleSever()
{
	if ( NULL == m_pScheduleSever.get() )
	{
		m_pScheduleSever.reset( new CSCheduleAction( &m_dbObjFactory ) ) ;
	}
	return 	 m_pScheduleSever.get();
}
CChangeActionInc*     CActionServerFactory::GetChangeSever()
{
	if ( NULL == m_pChangeAction.get() )
	{
		m_pChangeAction.reset( new ChangeActionImp( &m_dbObjFactory ) ) ;
	}
	return 	 m_pChangeAction.get();
}
CDepartSeverInc*        CActionServerFactory::GetDepartSever()
{
	if ( NULL == m_pDepartAction.get() )
	{
		m_pDepartAction.reset( new CDepartSeverImp( &m_dbObjFactory ) ) ;
	}
	return 	 m_pDepartAction.get();
}
CBackStageActionInc*        CActionServerFactory::GetBackStageSver()
{
	if ( NULL == m_pBackAction.get() )
	{
		m_pBackAction.reset( new CBackStageActionImp( &m_dbObjFactory ) ) ;
	}
	return 	 m_pBackAction.get();
}
CWorkHourSeverInc*       CActionServerFactory::GetWorkHourSever()
{
	if ( NULL == m_pWorkHourAction.get() )
	{
		m_pWorkHourAction.reset( new CWorkHourSverImp( &m_dbObjFactory ) ) ;
	}
	return 	 m_pWorkHourAction.get();

}
CSystemCfgSeverInc*       CActionServerFactory::GetSystemCfgSever()
{
	if ( NULL == m_pSystemCfgAction.get() )
	{
		m_pSystemCfgAction.reset( new CSystemCfgSeverImp( &m_dbObjFactory ) ) ;
	}
	return 	 m_pSystemCfgAction.get();

}