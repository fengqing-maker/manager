#pragma once
#include "../AbstractOutputManager.h"
#include "Src/Tools/SingletonTemplate/SingletonTemplate.h"

/** @brief 任务信息结构参数 */
enum updateViewType
{
	HandleMyTask, //新增我要处理任务
	PublishTask,  //新增待发布任务
	ReviewScheduleTask,   //新增进度点评任务
	ScoreTask,    //新增待评分任务
	ReviewChange, //新增待评审变更任务
	WorkHourView,
};
enum updateMsgType
{
	task_msg_add,
	task_msg_update,
	task_msg_decrease,
	check_msg_workHour,
};
struct TaskMsgParam
{
	unsigned char msgType;
	bool          isSendUpdateMsg; //是否发送过页面更新数据消息
	short int     taskType; //taskMsgType
	int           taskId;
	TaskMsgParam()
	{
		isSendUpdateMsg = FALSE;
		taskType = -1;
		taskId = -1;
	}
};

class SvrOutputManager :public AbstractOutputManager,
	public SingletonD<SvrOutputManager>
{
	friend class SingletonD<SvrOutputManager>;
public:
	virtual ~SvrOutputManager(void);

	/** @brief 更新当前输出列表所在窗口标题 */
	void UpdateOutputPaneCaption();

	/** @brief 重载父类方法，更改信息导出文件夹 */
	virtual CString GetExportDirectoryName();
	/** @brief 重载父类方法，更改信息导出默认文件名称 */
	virtual CString GetExportDefaultName();

	void ShowUpdateTaskMsg( int taskMsgType,  int taskType, int id, const CString &extContent );

	void OnDblClick(NMHDR *pNMHDR, LRESULT *pResult); //更新
	void OnClick(NMHDR *pNMHDR, LRESULT *pResult); //选中

protected:
	SvrOutputManager(void);

	CString MakeOutputString( int taskMsgType,  int taskType, int id, const CString &extenStr);

};

