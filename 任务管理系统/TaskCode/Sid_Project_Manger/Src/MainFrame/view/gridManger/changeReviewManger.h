#ifndef CHANGE_REVIEW_MANGER_347892Q
#define CHANGE_REVIEW_MANGER_347892Q

#include "../baseGridManger.h"

class CChangeReviewManger:public CBaseGridManger
{
public:
	CChangeReviewManger( CActionServerFactory *pSeverFactory );
	void  UpdateDbData();
	void  UpdateShowAllChange();
	void  UpdateShowAllReviewChange();
	void  UpdateShowDepartChange();
	void  UpdateShowProjectChange();
	void  UpdateOwnerChange();

	//消息处理
	void OnClickGrid( NMHDR* pNMHDR,LRESULT* pResult );
	void OnGridSelChange( NMHDR* pNMHDR,LRESULT* pResult );
	void UpdateChangeContent( int changId );
	void SelectChangeLine( int changId );
protected:
	//重写
	virtual CString GetFlexItemText(int nRow, int nColumn);				/* 重载以设置可动表项 */
	virtual BOOL    SetFlexItemText(int nRow,int nColumn,CString text){return TRUE;}; //设置内容
	virtual BOOL    GetFlexItemEditable(int nRow,int nColumn);

	virtual void    CreateColumnMap(); //创建列头
	virtual void    CreateRowItemMap();
	//行列设置
	virtual int     GetFlexRowHeight(int rowIndex){ return 60;}
	virtual int     GetFlexRowCount(){return m_showTaskMap.size();};
protected:
	void            SetShowTask( const CString &taskType , BOOL isReview   ); //isReview是否需要评审
	int             GetTaskChangeByRow( int nRow, CTaskObj &obj, CTaskChange &change ); //负数失败
	void            InsertFirstReview();
	void            InsertSecondReview();
	int             FindChangeById( int changeId, ListChange::iterator &itor );
protected:
	ListChange           m_changeList; //所有要处理的任务
	IntToIntMap          m_showTaskMap;
};

#endif