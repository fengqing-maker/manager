#pragma once
#include "../AbstractOutputManager.h"
#include "Src/Tools/SingletonTemplate/SingletonTemplate.h"

/** @brief ������Ϣ�ṹ���� */
enum updateViewType
{
	HandleMyTask, //������Ҫ��������
	PublishTask,  //��������������
	ReviewScheduleTask,   //�������ȵ�������
	ScoreTask,    //��������������
	ReviewChange, //����������������
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
	bool          isSendUpdateMsg; //�Ƿ��͹�ҳ�����������Ϣ
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

	/** @brief ���µ�ǰ����б����ڴ��ڱ��� */
	void UpdateOutputPaneCaption();

	/** @brief ���ظ��෽����������Ϣ�����ļ��� */
	virtual CString GetExportDirectoryName();
	/** @brief ���ظ��෽����������Ϣ����Ĭ���ļ����� */
	virtual CString GetExportDefaultName();

	void ShowUpdateTaskMsg( int taskMsgType,  int taskType, int id, const CString &extContent );

	void OnDblClick(NMHDR *pNMHDR, LRESULT *pResult); //����
	void OnClick(NMHDR *pNMHDR, LRESULT *pResult); //ѡ��

protected:
	SvrOutputManager(void);

	CString MakeOutputString( int taskMsgType,  int taskType, int id, const CString &extenStr);

};

