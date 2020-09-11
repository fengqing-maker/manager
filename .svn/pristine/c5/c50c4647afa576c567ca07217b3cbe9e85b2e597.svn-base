#ifndef TASK_STAUS_1234598
#define TASK_STAUS_1234598

#include "TypeDefHead.h"

/*
 此表必须与数据库的status表一一对应
*/
enum EnumStaus
{
    newTasking = 1,     //新建中
	waitPublish = 2,     //审核中
	waitStart = 3,       //未开始
	ongoing = 4,         //进行中
	waitScore = 5,       //待评分
	task_close = 6,           //关闭
	task_suspend = 7,         //挂起
	midStop = 8,         //中止
	changeWait1 = 9,     //任务变更待一级评审
	changeWait2 = 10,    //待副总审核
	changeWait0 = 11,    //待项目点评变更
	finishReviewWait = 12,    //待项目审核完成
};


class CTaskStaus
{
public:
	bool            InsertStaus( int id, CString &strStaus );
	bool            GetStrStausById( int id, CString &strStatus ) const;
	void            Clear(){ m_idStausMap.clear();}
	const TaskIdStringMap&  GetTaskStringMap() const { return m_idStausMap;}
	int                     FindStatusIdByString( const CString &str ) const;
private:
	TaskIdStringMap m_idStausMap; //状态
};

#endif