#pragma once

#include "baseBackGridManger.h"



#define  UN_KNOWN_STR  _T("未知")

class CProductLineGridManger:public CBackBaseGridManger
{
public:
	CProductLineGridManger(  CActionServerFactory *pSeverFactory );

	void OnGridRclick( NMHDR* pNMHDR,LRESULT* pResult );
	void UpdateGrid();

	void OnDeleteProductLine();
	void OnChangeOwner(  );

protected:
	void        InsertName();
    void        InsertProjectList();
	void        InsertDc();
	void        InsertOwernColum();

protected:
	//重写
	virtual CString GetFlexItemText(int nRow, int nColumn);				/* 重载以设置可动表项 */
	virtual BOOL    SetFlexItemText(int nRow,int nColumn,CString text); //设置内容
	virtual BOOL    GetFlexItemEditable(int nRow,int nColumn);
	virtual int  GetFlexRowHeight(int rowIndex){ return 90;}

	virtual int     GetFlexRowCount(){return m_allProductLine.size();};

	virtual void CreateColumnMap(); //创建列头
	virtual void CreateRowItemMap();
protected:
	void           GetProjectStrOfLine( int lineId, CString &text );
	CProductLine*  GetProductLineByRow( int nRow );
	CProductLine*  ValidateRow( int nRow, int nColumn,GridColumnMapItor &colItor  );
	int            ValidateSelectColum( int colDataType );
	enum { ProductlineName, ProductProject,COLUM_DC, COLUM_OWNER  };
protected:
	ListProjectLine       m_allProductLine;
	CProjectSeverInc*     m_projectSever;      
};
