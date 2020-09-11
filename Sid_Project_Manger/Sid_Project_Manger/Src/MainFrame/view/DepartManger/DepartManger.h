#ifndef PROJECT_VIEW_MANGER_H_2398
#define PROJECT_VIEW_MANGER_H_2398
/**
*@file	CDepartViewManger
*@brief	 部门管理页面
*@note	
*		
*@author	lihuizhen
*/
#include "DepartMangerBase.h"

//NM_CLICK
class CDepartViewManger:public CDepartBaseGridManger
{
public:
	CDepartViewManger( CActionServerFactory *pSeverFactory );

	void  RefreshDepartment( );


	//void OnGridSelChanged( NMHDR* pNMHDR,LRESULT* pResult );
	void OnGridRclick( NMHDR* pNMHDR,LRESULT* pResult );
	void OnGridClick( NMHDR* pNMHDR,LRESULT* pResult  );
	void OnChangeOwner(  );
	void OnChangeDepartBakManger(  );
	void OnDeleteDepart();
protected:
	//重写
	virtual CString GetFlexItemText(int nRow, int nColumn);				/* 重载以设置可动表项 */
	virtual BOOL    SetFlexItemText(int nRow,int nColumn,CString text); //设置内容
	virtual BOOL    GetFlexItemEditable(int nRow,int nColumn);
 
	virtual BOOL    InitFlexComboItem(CGridCellCombo* pCell,int nRow,int nColumn);
	virtual BOOL    InitFlexCheckItem(CGridCellCheck* pCell,int nRow,int nColumn);

	virtual void CreateColumnMap(); //创建列头
	virtual void CreateRowItemMap();
	//行列设置
	virtual int     GetFlexRowCount(){return m_allDepart.size();};
protected:
	CDepartment*        GetDepartByRow( int nRow );
	int                ValidateSelectColum( int colDataType );
	
protected:
	 BOOL               m_hasModiyPrj;
};

#endif