#pragma once

#include "baseBackGridManger.h"


class CRolePowerGridManger:public CBackBaseGridManger
{
public:
	CRolePowerGridManger(  CActionServerFactory *pSeverFactory );

	void OnGridRclick( NMHDR* pNMHDR,LRESULT* pResult );
	void UpdateGrid();

	void OnDeleteRole();
	void OnChangePowers(  );

protected:
	void        InsertName();
	void        InsertPowerList();
	void        InsertDc();


protected:
	//重写
	virtual CString GetFlexItemText(int nRow, int nColumn);				/* 重载以设置可动表项 */
	virtual BOOL    SetFlexItemText(int nRow,int nColumn,CString text); //设置内容
	virtual BOOL    GetFlexItemEditable(int nRow,int nColumn);
	virtual int  GetFlexRowHeight(int rowIndex){ return 75;}

	virtual int     GetFlexRowCount(){return m_allRole.size();};

	virtual void CreateColumnMap(); //创建列头
	virtual void CreateRowItemMap();
protected:
	CRole*       GetRoleByRow( int nRow );
    void         GetPowerListStr( int roleId, CString &text );
	CRole*       ValidateRow( int nRow, int nColumn,  GridColumnMapItor &colItor  );
	int            ValidateSelectColum( int colDataType );
	enum { RoleName, RoleDec, RolePowers  };
protected:
	ListRole              m_allRole;
	CProjectSeverInc*     m_projectSever;     
	BOOL                  m_isNewRole;
};
