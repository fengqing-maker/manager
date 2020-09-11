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
		ret = _T("�����¼�������ظ�");
		break;
	case excSqlError:
		ret = _T("ִ��Sql���ʧ��");
		break;
	case validateUserNone:
		ret = _T("�û�������");
		break;
	case userPwdError:
		ret = _T("�������");
		break;
	case notfind:
		ret = _T("δ���ҵ���Ӧ��Ϣ");
		break;
	case mapError:
		ret = _T("ӳ�����");
		break;
	case userExcOk:
		ret = _T("ִ�гɹ�");
		break;
	case paraError:
		ret = _T("��������");
		break;
	case e_isOnline:
		ret = _T("�û��ѵ�¼");
	case  not_connect:
		ret = _T("���ݿ�δ����");
	case  connect_error:
		ret = _T("��������Դʧ��");
	}
	return ret;
}
CLoginActionInc*     CActionServerFactory::GetLoginServer()
{
	if ( NULL == m_pLoginActionServer.get() )
	{
		m_pLoginActionServer.reset( new CLoginActionImp( &m_dbObjFactory ) ) ;
	}
	return 	m_pLoginActionServer.get();
}
CConnectDbServerInc* CActionServerFactory::GetConnectSever()
{
	if ( NULL == m_pConectServer.get() )
	{
		m_pConectServer.reset( new CConectDbImp( m_dbObjFactory.GetDaoSupportDao() ));
	}
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