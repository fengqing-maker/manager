#include "stdafx.h"
#include "CDaoFactory.h"
#include "../AbstractDaoImp/MySqlImp/DbSev.h"
#include "../UserDaoImp/CUserDaoImp.h"
#include "../ChangeDaoImp/changeDaoImp.h"
#include "../TaskDaoImp/CTaskDaoImp.h"
#include "../DepartmentDaoImp/DepartmentDaoImp.h"
#include "../Project/projectDbDaoImp.h"
#include "../system/WorkHourDb.h"
#include "../system/SystemConfigDbAdo.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CDaoFactory::CDaoFactory()
{
	m_pBaseDaoInc = new CMySqlBaseImp();
	ASSERT( NULL != m_pBaseDaoInc );
	m_pUserInc = NULL;
	m_pChangeInc = NULL;
	m_pDepartInc = NULL;
	m_pTaskDaoInc = NULL;
	m_pPrjDbInc = NULL;
	m_pWorkHourInc = NULL;
	m_pSystemCfgInc = NULL;
}

CDaoFactory::~CDaoFactory()
{
	if ( NULL != m_pUserInc )
	{
	    delete m_pUserInc;
		m_pUserInc = NULL;
	}
	if ( NULL != m_pBaseDaoInc )
	{
		delete m_pBaseDaoInc;
		m_pBaseDaoInc = NULL;
	}
	if ( NULL != m_pChangeInc )
	{
		delete m_pChangeInc;
		m_pChangeInc = NULL;
	}
	if ( NULL != m_pDepartInc )
	{
		delete m_pDepartInc;
		m_pDepartInc = NULL;
	}
	if ( NULL != m_pTaskDaoInc )
	{
		delete m_pTaskDaoInc;
		m_pTaskDaoInc = NULL;
	}
	if ( NULL != m_pPrjDbInc )
	{
		delete m_pPrjDbInc;
		m_pPrjDbInc = NULL;
	}
	if ( NULL != m_pWorkHourInc )
	{
		delete m_pWorkHourInc;
		m_pWorkHourInc = NULL;
	}
	if ( NULL != m_pSystemCfgInc )
	{
		delete m_pSystemCfgInc;
		m_pSystemCfgInc = NULL;
	}
}

CUserDaoInterface * CDaoFactory::GetUserDao()
{
	if ( NULL == m_pUserInc )
	{
		if ( NULL == m_pBaseDaoInc )
		{
			m_pBaseDaoInc = new CMySqlBaseImp();
		}
		m_pUserInc = new CUerDaoImp( m_pBaseDaoInc );
	}
	return m_pUserInc;
}
CChangeDaoInc * CDaoFactory::GetChangeDbDao()
{
	if ( NULL == m_pChangeInc )
	{
		if ( NULL == m_pBaseDaoInc )
		{
			m_pBaseDaoInc = new CMySqlBaseImp();
		}
		m_pChangeInc = new CChangeDaoImp( m_pBaseDaoInc );
	}
	return m_pChangeInc;
}
CDepartmentAdoInc * CDaoFactory::GetDepartDbDao()
{
	if ( NULL == m_pDepartInc )
	{
		if ( NULL == m_pBaseDaoInc )
		{
			m_pBaseDaoInc = new CMySqlBaseImp();
		}
		m_pDepartInc = new CDepartmentAdoImp( m_pBaseDaoInc );
	}
	return m_pDepartInc;
}
CTaskDaoInc * CDaoFactory::GetTaskDbDao()
{
	if ( NULL == m_pTaskDaoInc )
	{
		if ( NULL == m_pBaseDaoInc )
		{
			m_pBaseDaoInc = new CMySqlBaseImp();
		}
		m_pTaskDaoInc = new CTaskDaoImp( m_pBaseDaoInc );
	}
	return m_pTaskDaoInc;
}
CDaoSupportInterface* CDaoFactory::GetDaoSupportDao()
{
	return m_pBaseDaoInc;
}
CProjectDbDaoInc*      CDaoFactory::GetPrjDbDao()
{
	if ( NULL == m_pPrjDbInc )
	{
		if ( NULL == m_pBaseDaoInc )
		{
			m_pBaseDaoInc = new CMySqlBaseImp();
		}
		m_pPrjDbInc = new CProjectDbDaoImp( m_pBaseDaoInc );
	}
	return m_pPrjDbInc;
}
CWorkHourDbInc*    CDaoFactory::GetWorkHourDbDao()
{
	if ( NULL == m_pWorkHourInc )
	{
		if ( NULL == m_pBaseDaoInc )
		{
			m_pBaseDaoInc = new CMySqlBaseImp();
		}
		m_pWorkHourInc = new CWorkHourDbImp( m_pBaseDaoInc );
	}
	return m_pWorkHourInc;
}
CSystemCfgDbInc*   CDaoFactory::GetSystemCfgDbDao()
{
	if ( NULL == m_pSystemCfgInc )
	{
		if ( NULL == m_pBaseDaoInc )
		{
			m_pBaseDaoInc = new CMySqlBaseImp();
		}
		m_pSystemCfgInc = new CSystemDbAdoImp( m_pBaseDaoInc );
	}
	return m_pSystemCfgInc;
}