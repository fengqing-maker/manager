#ifndef BASE_BACK_GRID_MAGER_AW34I783
#define BASE_BACK_GRID_MAGER_AW34I783

#include "../../ui/CGridCtrl/comondef/comondef.h"
#include "../../ui/CGridCtrl/AbstractGridViewManager.h"
#include "Src/action/factory/actionServerFactory.h"

#define IDM_BACK_BASE                     (40)
#define IDM_USER_CLEARN_PWD               (IDM_BACK_BASE+1)         //�������
#define IDM_DELTE_OBJ                     (IDM_BACK_BASE+2)         //ɾ��
#define IDM_BACK_EDIT                    (IDM_BACK_BASE+3)          //�༭


#define USER_MANGER_MSG_BASE            (WM_USER + 200) //

class CBackBaseGridManger:public AbstractGridViewManager
{
public:
	CBackBaseGridManger(  CActionServerFactory *pSeverFactory );

    virtual void OnGridEndEidt( NMHDR* pNMHDR,LRESULT* pResult );
protected:
	//AbstractGridViewManager��д
	virtual  void        PreCreateGridView(); //��������ǰ
	virtual  void        AftCreateGridView(); //���������
	virtual  void        AftUpdateGridView();
	virtual CString      GetFixedItemText(int nRow, int nColumn); //���̶���ͷ����
	virtual GridItemType GetFlexItemType(int nRow,int nColumn);
    //
	virtual int  GetFixedColumnCount(){return RowIndex_Title;} //����0������ABCd��ͷ
	virtual int	 GetFixedRowCount(){ return 1+RowIndex_Max; };	/* ����һ�б��� */
	virtual int  GetFlexColumnCount();
	//CBaseGridManger���麯��
	virtual void CreateColumnMap();
	virtual void CreateRowItemMap(){};
	virtual int  GetFlexRowHeight(int rowIndex){ return 45;}

protected:
	CString GetOwnerNameStr( int userId );

	void        InsertUniqueColumn(GridColumnPtr columnInfo);
	void        InsertColumn( const CString &str, int width, int alignemnt, int type, int dataType );

protected:
	/** @brief ���<b>��</b>����map */
	GridColumnMap				m_gridColumnMap;
	CActionServerFactory        *m_pSeverFactory;
	CLoginActionInc              *m_pLoginSever;
	CBackStageActionInc          *m_pBackSever;
private:
	enum RowIndex{RowIndex_Title = 1,RowIndex_Max = 1};
};
#endif