#pragma once

#include "baseBackGridManger.h"



#define  UN_KNOWN_STR  _T("δ֪")

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
	//��д
	virtual CString GetFlexItemText(int nRow, int nColumn);				/* ���������ÿɶ����� */
	virtual BOOL    SetFlexItemText(int nRow,int nColumn,CString text); //��������
	virtual BOOL    GetFlexItemEditable(int nRow,int nColumn);
	virtual int  GetFlexRowHeight(int rowIndex){ return 90;}

	virtual int     GetFlexRowCount(){return m_allProductLine.size();};

	virtual void CreateColumnMap(); //������ͷ
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
