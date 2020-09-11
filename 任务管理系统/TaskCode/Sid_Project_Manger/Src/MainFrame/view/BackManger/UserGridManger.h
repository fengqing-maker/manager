#pragma once

#include "baseBackGridManger.h"



#define  UN_KNOWN_STR  _T("未知")

class CUserGridManger:public CBackBaseGridManger
{
public:
	CUserGridManger(  CActionServerFactory *pSeverFactory );

	void OnGridRclick( NMHDR* pNMHDR,LRESULT* pResult );
	void OnClearnPwd();
	void OnDeleteUser();
	void UpdateGridByDepart( int departId );
	void UpdateDb();

	const ListDepart& GetAllDepart() const{ return m_allDepart;};
protected:
	void   InsertDepart();
	void   InsertUserName();
	void   InsertUserLoginName();
	void   InsertUserRoleName();
	void   InsertUserStatus();
	void   InsertUserOnline();
	void   InsertUserExportSeq();

protected:
	//重写
	virtual CString GetFlexItemText(int nRow, int nColumn);				/* 重载以设置可动表项 */
	virtual BOOL    SetFlexItemText(int nRow,int nColumn,CString text); //设置内容
	virtual BOOL    GetFlexItemEditable(int nRow,int nColumn);
	virtual BOOL         InitFlexNumEditItem(CGridCellNumeric* pCell,int nRow,int nColumn);

	virtual BOOL    InitFlexComboItem(CGridCellCombo* pCell,int nRow,int nColumn);
	//行列设置
	virtual int     GetFlexRowHeight(int rowIndex){ return 30;}
	virtual int     GetFlexRowCount(){return m_showTaskMap.size();};

	virtual void CreateColumnMap(); //创建列头
	virtual void CreateRowItemMap();
protected:
	void          RefreshShowMap( int showDepart = -1 );
	CUser*        GetShowUserByRow( int nRow );
	CUser*        ValidateRow(int nRow, int nColumn, GridColumnMapItor &colItor  );

	enum { UserDepartName, UserName, UserLoginName, UserRoleName, 
		    UserStats, UserOnline, UserExportSeq };
protected:
	ListUser       m_allUser;
	ListDepart     m_allDepart;
	ListRole       m_allRole;
	IntToIntMap    m_showTaskMap;
	IntStringMap   m_workStatusMap;
	IntStringMap   m_onLineStatusMap;
	int            m_selDepart;
};
