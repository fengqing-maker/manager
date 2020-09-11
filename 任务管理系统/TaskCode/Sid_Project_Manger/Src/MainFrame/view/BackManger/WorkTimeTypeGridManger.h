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
	//��д
	virtual CString GetFlexItemText(int nRow, int nColumn);				/* ���������ÿɶ����� */
	virtual BOOL    SetFlexItemText(int nRow,int nColumn,CString text); //��������
	virtual BOOL    GetFlexItemEditable(int nRow,int nColumn);
	
	virtual int  GetFlexRowHeight(int rowIndex)
	{ 
		return 30;
	}
	virtual int     GetFlexRowCount()
	{
		return m_workTypeMap.size();
	};

	virtual void CreateColumnMap(); //������ͷ
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
