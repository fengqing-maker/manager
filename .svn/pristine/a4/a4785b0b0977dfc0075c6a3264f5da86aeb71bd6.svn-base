#include "stdafx.h"
#include "CWatchDaoFactory.h"
#include "../AbstractDaoImp/MySqlImp/DbSev.h"
//#include "../UserDaoImp/CUserDaoImp.h"
//#include "../ChangeDaoImp/changeDaoImp.h"
#include "../TaskDaoImp/CTaskDaoImp.h"
//#include "../DepartmentDaoImp/DepartmentDaoImp.h"
//#include "../Project/projectDbDaoImp.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CWatchDaoFactory::CWatchDaoFactory()
{
	m_pBaseDaoInc.reset( new CMySqlBaseImp() );
}

CWatchDaoFactory::~CWatchDaoFactory()
{
}

//CUserDaoInterface * CWatchDaoFactory::GetUserDao()
//{
//	if ( NULL == m_pUserInc )
//	{
//		if ( NULL == m_pBaseDaoInc )
//		{
//			m_pBaseDaoInc = new CMySqlBaseImp();
//		}
//		m_pUserInc = new CUerDaoImp( m_pBaseDaoInc );
//	}
//	return m_pUserInc;
//}
//CChangeDaoInc * CWatchDaoFactory::GetChangeDbDao()
//{
//	if ( NULL == m_pChangeInc )
//	{
//		if ( NULL == m_pBaseDaoInc )
//		{
//			m_pBaseDaoInc = new CMySqlBaseImp();
//		}
//		m_pChangeInc = new CChangeDaoImp( m_pBaseDaoInc );
//	}
//	return m_pChangeInc;
//}
//CDepartmentAdoInc * CWatchDaoFactory::GetDepartDbDao()
//{
//	if ( NULL == m_pDepartInc )
//	{
//		if ( NULL == m_pBaseDaoInc )
//		{
//			m_pBaseDaoInc = new CMySqlBaseImp();
//		}
//		m_pDepartInc = new CDepartmentAdoImp( m_pBaseDaoInc );
//	}
//	return m_pDepartInc;
//}
CTaskDaoInc * CWatchDaoFactory::GetTaskDbDao()
{
	if ( NULL == m_pTaskDaoInc.get() )
	{
		if ( NULL == m_pBaseDaoInc.get() )
		{
			m_pBaseDaoInc.reset(new CMySqlBaseImp());
		}
		m_pTaskDaoInc.reset( new CTaskDaoImp( m_pBaseDaoInc.get() ) );
	}
	return m_pTaskDaoInc.get();
}
CDaoSupportInterface* CWatchDaoFactory::GetDaoSupportDao()
{
	return m_pBaseDaoInc.get();
}
//CProjectDbDaoInc*      CWatchDaoFactory::GetPrjDbDao()
//{
//	if ( NULL == m_pPrjDbInc )
//	{
//		if ( NULL == m_pBaseDaoInc )
//		{
//			m_pBaseDaoInc = new CMySqlBaseImp();
//		}
//		m_pPrjDbInc = new CProjectDbDaoImp( m_pBaseDaoInc );
//	}
//	return m_pPrjDbInc;
//}