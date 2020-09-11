#include "stdafx.h"
#include "actionWatchServerFactory.h"
#include "../connectAction/connectActionImp.h"
//#include "../UserAction/LoginActionImp.h"
#include "../TaskAction/taskSeverImp.h"
#include "../ProjectAction/projectSever.h"
//#include "../ScheduleAction/ScheduleAction.h"
//#include "../changeAction/changeActionImp.h"
//#include "../departAction/departSeverImp.h"
//#include "../backStageAction/BackStageActionImp.h"

CActionWatchServerFactory::CActionWatchServerFactory()
{
}

CActionWatchServerFactory::~CActionWatchServerFactory()
{
}
CString   CActionWatchServerFactory::GetErrorString(int erroInfo)
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
//CLoginActionInc*     CActionWatchServerFactory::GetLoginServer()
//{
//    if ( NULL == m_pLoginActionServer.get() )
//    {
//		m_pLoginActionServer.reset( new CLoginActionImp( &m_dbObjFactory ) ) ;
//	}
//	return 	m_pLoginActionServer.get();
//}
CConnectDbServerInc* CActionWatchServerFactory::GetConnectSever()
{
	if ( NULL == m_pConectServer.get() )
	{
		m_pConectServer.reset( new CConectDbImp( m_dbObjFactory.GetDaoSupportDao() ));
	}
	return 	m_pConectServer.get();
}
 CTaskSeverInc*        CActionWatchServerFactory::GetTaskSever()
{
	if ( NULL == m_pTaskSever.get() )
	{
		m_pTaskSever.reset( new CTaskSeverImp( &m_dbObjFactory ) );
	}
	return 	m_pTaskSever.get();
}
 CProjectSeverInc*       CActionWatchServerFactory::GetProjectSever()
 {
	 if ( NULL == m_pPrjSever.get() )
	 {
		 m_pPrjSever.reset( new CProjectSever( &m_dbObjFactory ) ) ;
	 }
	 return 	m_pPrjSever.get() ;
 }
 //CScheduleActionInc*    CActionWatchServerFactory::GetScheduleSever()
 //{
	// if ( NULL == m_pScheduleSever.get() )
	// {
	//	 m_pScheduleSever.reset( new CSCheduleAction( &m_dbObjFactory ) ) ;
	// }
	// return 	 m_pScheduleSever.get();
 //}
// CChangeActionInc*     CActionWatchServerFactory::GetChangeSever()
// {
//	 if ( NULL == m_pChangeAction.get() )
//	 {
//		 m_pChangeAction.reset( new ChangeActionImp( &m_dbObjFactory ) ) ;
//	 }
//	 return 	 m_pChangeAction.get();
// }
//CDepartSeverInc*        CActionWatchServerFactory::GetDepartSever()
//{
//	if ( NULL == m_pDepartAction.get() )
//	{
//		m_pDepartAction.reset( new CDepartSeverImp( &m_dbObjFactory ) ) ;
//	}
//	return 	 m_pDepartAction.get();
//}
//CBackStageActionInc*        CActionWatchServerFactory::GetBackStageSver()
//{
//	if ( NULL == m_pBackAction.get() )
//	{
//		m_pBackAction.reset( new CBackStageActionImp( &m_dbObjFactory ) ) ;
//	}
//	return 	 m_pBackAction.get();
//}