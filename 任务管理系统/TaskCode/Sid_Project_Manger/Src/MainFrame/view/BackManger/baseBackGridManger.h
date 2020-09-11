#ifndef BASE_BACK_GRID_MAGER_AW34I783
#define BASE_BACK_GRID_MAGER_AW34I783

#include "../../ui/CGridCtrl/comondef/comondef.h"
#include "../../ui/CGridCtrl/AbstractGridViewManager.h"
#include "Src/action/factory/actionServerFactory.h"

#define IDM_BACK_BASE                     (40)
#define IDM_USER_CLEARN_PWD               (IDM_BACK_BASE+1)         //清空密码
#define IDM_DELTE_OBJ                     (IDM_BACK_BASE+2)         //删除
#define IDM_BACK_EDIT                    (IDM_BACK_BASE+3)          //编辑


#define USER_MANGER_MSG_BASE            (WM_USER + 200) //

class CBackBaseGridManger:public AbstractGridViewManager
{
public:
	CBackBaseGridManger(  CActionServerFactory *pSeverFactory );

    virtual void OnGridEndEidt( NMHDR* pNMHDR,LRESULT* pResult );
protected:
	//AbstractGridViewManager重写
	virtual  void        PreCreateGridView(); //创建网格前
	virtual  void        AftCreateGridView(); //创建网格后
	virtual  void        AftUpdateGridView();
	virtual CString      GetFixedItemText(int nRow, int nColumn); //填充固定列头内容
	virtual GridItemType GetFlexItemType(int nRow,int nColumn);
    //
	virtual int  GetFixedColumnCount(){return RowIndex_Title;} //返回0，就无ABCd列头
	virtual int	 GetFixedRowCount(){ return 1+RowIndex_Max; };	/* 增加一行标题 */
	virtual int  GetFlexColumnCount();
	//CBaseGridManger的虚函数
	virtual void CreateColumnMap();
	virtual void CreateRowItemMap(){};
	virtual int  GetFlexRowHeight(int rowIndex){ return 45;}

protected:
	CString GetOwnerNameStr( int userId );

	void        InsertUniqueColumn(GridColumnPtr columnInfo);
	void        InsertColumn( const CString &str, int width, int alignemnt, int type, int dataType );

protected:
	/** @brief 表格<b>列</b>定义map */
	GridColumnMap				m_gridColumnMap;
	CActionServerFactory        *m_pSeverFactory;
	CLoginActionInc              *m_pLoginSever;
	CBackStageActionInc          *m_pBackSever;
private:
	enum RowIndex{RowIndex_Title = 1,RowIndex_Max = 1};
};
#endif