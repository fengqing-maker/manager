#ifndef  SYSTEM_CFG_INC_DB_IW47U3847
#define   SYSTEM_CFG_INC_DB_IW47U3847
/*
 ϵͳ���ñ����������������ձ���Ŀ���̽׶εȱ���Ϣ
*/
#include "DaoSupportInterface.h"
#include "../../Model/ProjectStep.h"
#include <list> 
typedef std::list<CProjectStep>          ListProjectStep;
typedef ListProjectStep::iterator        ListProjectStepItor;
typedef ListProjectStep::const_iterator  ListProjectStepConstItor;

class CSystemCfgDbInc
{
public:
	CSystemCfgDbInc( CDaoSupportInterface* pDao ){ m_pDaoSupportInc = pDao;}
	virtual ~CSystemCfgDbInc(){};
	virtual BOOL InsertProjectStep(  CProjectStep &step ) = 0; //����
	virtual BOOL UpdateProjectStep( const CProjectStep &step ) = 0;
	virtual BOOL GetProjectStepList( const CString &type, ListProjectStep &obj ) = 0;
	virtual BOOL GetProjectStepType( ListString &lst ) = 0; //��ȡ��Ŀ��������
	virtual BOOL GetProjectStep( const CString stepName, CProjectStep &step ) = 0;
	virtual BOOL GetProjectStep(const CString& type, const CString stepName, CProjectStep &step ) = 0;
	virtual BOOL GetProjectStep( int stepId, CProjectStep &step ) = 0;


	//����
	virtual BOOL IsHoliday( const COleDateTime &date ,BOOL &reslut ) = 0;
	virtual BOOL AddHoliday( const COleDateTime &date ) = 0;
	virtual BOOL DeleteHoliday( const COleDateTime &date ) =0;
protected:
	CDaoSupportInterface* m_pDaoSupportInc;

};
#endif