#ifndef PROJECT_VIEW_MANGER_H_2398
#define PROJECT_VIEW_MANGER_H_2398
/**
*@file	CProjectViewManger
*@brief	 ��Ŀ����ҳ��
*@note	
*		
*@author	lihuizhen
*/
#include "ProjectMangerBase.h"

//NM_CLICK
class CProjectViewManger:public CPrjBaseGridManger
{
public:
	CProjectViewManger( CActionServerFactory *pSeverFactory );

	const ListProjectLine& GetAllProjectLine() const { return m_allPrjLine;}

	void  ChangeSelProjectLine( int selIndex );
	void  RefreshProject( );

	void  RefreshProjectDataFromDb();

	//void OnGridSelChanged( NMHDR* pNMHDR,LRESULT* pResult );
	void OnGridRclick( NMHDR* pNMHDR,LRESULT* pResult );
	void OnGridClick( NMHDR* pNMHDR,LRESULT* pResult  );
	void OnChangeOwner(  );
	void OnChangePrjMember(  );
	void OnDeleteProject();
protected:
	//��д
	virtual CString GetFlexItemText(int nRow, int nColumn);				/* ���������ÿɶ����� */
	virtual BOOL    SetFlexItemText(int nRow,int nColumn,CString text); //��������
	virtual BOOL    GetFlexItemEditable(int nRow,int nColumn);
 
	virtual BOOL    InitFlexComboItem(CGridCellCombo* pCell,int nRow,int nColumn);

	virtual void CreateColumnMap(); //������ͷ
	virtual void CreateRowItemMap();
	//��������
	virtual int     GetFlexRowCount(){return m_showTaskMap.size();};
protected:
	void             SetShowPrject( int prjLineId = -1  );
	CProject*        GetProjectByRow( int nRow );
	int              ValidateSelectColum( int colDataType );
	
protected:
	 IntToIntMap        m_showTaskMap;
	 ListProjectLine    m_allPrjLine; //��Ʒ��
	 BOOL               m_hasModiyPrj;
};

#endif