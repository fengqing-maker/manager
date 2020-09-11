#ifndef PROJECT_VIEW_MANGER_H_2398
#define PROJECT_VIEW_MANGER_H_2398
/**
*@file	CProjectViewManger
*@brief	 项目管理页面
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
	//重写
	virtual CString GetFlexItemText(int nRow, int nColumn);				/* 重载以设置可动表项 */
	virtual BOOL    SetFlexItemText(int nRow,int nColumn,CString text); //设置内容
	virtual BOOL    GetFlexItemEditable(int nRow,int nColumn);
 
	virtual BOOL    InitFlexComboItem(CGridCellCombo* pCell,int nRow,int nColumn);

	virtual void CreateColumnMap(); //创建列头
	virtual void CreateRowItemMap();
	//行列设置
	virtual int     GetFlexRowCount(){return m_showTaskMap.size();};
protected:
	void             SetShowPrject( int prjLineId = -1  );
	CProject*        GetProjectByRow( int nRow );
	int              ValidateSelectColum( int colDataType );
	
protected:
	 IntToIntMap        m_showTaskMap;
	 ListProjectLine    m_allPrjLine; //产品线
	 BOOL               m_hasModiyPrj;
};

#endif