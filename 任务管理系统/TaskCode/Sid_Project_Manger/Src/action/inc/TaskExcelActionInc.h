#ifndef  TASK_EXCEL_ACTION_HEARD_INC_34I9SDIRF
#define  TASK_EXCEL_ACTION_HEARD_INC_34I9SDIRF

#include "../factory/actionServerFactory.h"
#include "../../DataBase/inc/excel/CExcelTaskDaoInc.h"
#include "../../DataBase/ExcelDataDao/CExcelTaskDao.h"

#include <boost/scoped_ptr.hpp>

class CTaskExcelActionInc
{

public:

	CTaskExcelActionInc( CActionServerFactory *pFactory )
	{
		m_pFactory = pFactory; 
		m_pExcelTaskDao.reset( new CExcelTaskDao() );  
	}
	virtual ~CTaskExcelActionInc(){};
protected:
	CActionServerFactory  *                     m_pFactory;
	boost::scoped_ptr<CExcelTaskDaoInc> m_pExcelTaskDao; 
};



#endif



