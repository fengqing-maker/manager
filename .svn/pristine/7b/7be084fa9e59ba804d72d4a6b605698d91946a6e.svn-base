#pragma once

#include "baseBackGridManger.h"


class CWorkTimeTypeGridManger:public CBackBaseGridManger
{
public:
	CWorkTimeTypeGridManger(  CActionServerFactory *pSeverFactory );

	void OnGridRclick( NMHDR* pNMHDR,LRESULT* pResult );
	void UpdateGrid();

	void OnDelete();

protected:
	void        InsertName();
	void        InsertId();
protected:
	//重写
	virtual CString GetFlexItemText(int nRow, int nColumn);				/* 重载以设置可动表项 */
	virtual BOOL    SetFlexItemText(int nRow,int nColumn,CString text); //设置内容
	virtual BOOL    GetFlexItemEditable(int nRow,int nColumn);
	
	virtual int  GetFlexRowHeight(int rowIndex)
	{ 
		return 30;
	}
	virtual int     GetFlexRowCount()
	{
		return m_workTypeMap.size();
	};

	virtual void CreateColumnMap(); //创建列头
	virtual void CreateRowItemMap();
protected:
	BOOL        GetWorkTypeByRow( int nRow, TaskIdStringMapItor &itor  );
	BOOL       ValidateRow( int nRow, int nColumn, GridColumnMapItor &colItor,TaskIdStringMapItor &dataItor );
	enum       { workTimeTypeId, workTimeTypeName  };
protected:
	TaskIdStringMap       m_workTypeMap;
	CProjectSeverInc*     m_projectSever;     
	BOOL                  m_isNewRole;
};
