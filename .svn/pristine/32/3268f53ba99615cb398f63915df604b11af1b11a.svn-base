#ifndef PRJ_BASE_GRID_MAGER_AW34I785934
#define PRJ_BASE_GRID_MAGER_AW34I785934


#include "../baseGridManger.h"



class CPrjBaseGridManger:public CBaseGridManger
{
public:
	CPrjBaseGridManger(  CActionServerFactory *pSeverFactory );

	const ListProject& GetProjectList() const{ return m_allProject;};
protected:
	void    InsertPrjNameColumn();
	void    InsertPrjLongNameColumn();
	void    InsertPrjStepType();
	void    InsertPrjLine();
	void    InsertPrjStep();      //实际所属阶段
	void    InsertBegongToStep(); //应属阶段

	//void    InsertPrjDc();
	void    InsertPrjMember();
	void    UpdateProject();

	CProject* GetProjectByRow( int nRow, ListProject &lst, const IntToIntMap &showMap );
	void   GetProjectLineNameById( int lineId, CString &str );
    BOOL   GetProjectMemberString( int prjId, CString &str );

protected:
	enum PrjectTitleType
	{
		PrjName, PrjLongName, PrjLine, /*PrjDC,*/ PrjMember, PrjStepType, PrjNowStep, PrjBelongToStep
	};
protected:
    CProjectSeverInc   *m_pProjectSever;
	const CUser*        m_pLoginUser;
	ListProject        m_allProject;
private:

};
#endif