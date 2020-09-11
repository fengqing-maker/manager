/**
*@file	AbstractOutputManager
*@brief	抽象的消息输出列表管理器
*@note	该类为抽象类，不能用于实例化，用于继承构建用户需要的消息数据列表管理器
*@author	高峰
*/
#pragma once

#include <list>
#include "../ui/COutputList/OutputList.h"

#define WM_OUTPUT_POPUP_MENUID_MIN			WM_USER+4000	/* 输出视图弹出菜单项ID最小值 */
#define WM_OUTPUT_POPUP_MENUID_MAX			WM_USER+4100	/* 输出视图弹出菜单项ID最大值 */


typedef struct output_listcolumn		/* 列表格式 */
{
	int index;			/* 列号 */
	CString title;		/* 标题 */
	DWORD alignment;	/* 对齐 */
	int widthRatio;		/* 列宽比例 */
}Output_ListColumn;

typedef struct output_message			/* 外部使用消息定义 */
{
	int type;			/* 消息类型标识 */
	CString content;	/* 消息内容 */
}Output_Message;

/** @brief 私有消息类型定义 */
typedef struct output_message_private
{
	int		imageIndex;	/**< @brief 图标序号 */
	CString index;		/**< @brief 消息序号 */
	CString type;		/**< @brief 消息类型 */
	CString content;	/**< @brief 消息内容 */
	CString time;		/**< @brief 消息获取时间 */
	void    *pPara;      /**< @brief 其他信息 */ 
	output_message_private()
	{
		imageIndex = -1;
		pPara = NULL;
	}
}Output_Message_Private;
typedef std::list<Output_Message_Private>					MessageList;
typedef std::list<Output_Message_Private>::iterator			MessageListItor;
typedef std::list<Output_Message_Private>::const_iterator	MessageListConstItor;


#define		WM_OUTPUT_UPDATE	WM_USER+1		/**< @brief 自定义消息，用于驱动列表控件刷新列表内容 */


class AbstractOutputManager
{
public:
	AbstractOutputManager(void);
	virtual ~AbstractOutputManager(void) = 0;

	/** @brief 注册输出信息列表控件 */
	BOOL RegisterAsOutputList(COutputListCtrl* outputList);

/****************************list operation************************************/
	/** @brief 初始化输出信息列表控件 */
	void InitOutputList();	
	/** @brief 显示当前输出列表所在的窗口 */
	//void ShowOutputPane();
	/** @brief 显示右键弹出菜单 */
	void ShowOutputContextMenu(CPoint point);
	/** @brief 响应右键弹出菜单项 */
	void RespOutputContextMenu(UINT menuID);

	/** @brief 重置输出信息列表控件列宽 */
	void ResetColumnWidth();

	/** @brief 水平折叠输出信息列表控件的“内容”列 */
	void CollapseContentColumnH();
	/** @brief 水平展开输出信息列表控件的“内容”列 */
	void ExpandContentColumnH();
	/** @brief 垂直折叠输出信息列表控件的“内容”列 */
	void CollapseContentColumnV();
	/** @brief 垂直折叠输出信息列表控件的“内容”列 */
	void ExpandContentColumnV();
	
	/** @brief 刷新列表内容 */
	void UpdateContent();
	/** @brief 清除列表内容 */
	void ClearContent();
	/** @brief 导出列表内容 */
	void ExportContent();
	
/****************************message operation*********************************/
	/** @brief 输出“消息”类型信息 */
	void ShowMessage(CString msg);
	/** @brief 输出“错误”类型信息 */
	void ShowError(CString err);
	/** @brief 输出 带参“消息”类型信息 */
	void ShowParaMsg( CString msg, void *pPara );
protected:
	/** @brief 可以重载以修改对右键弹出菜单的响应方法 */
	virtual void ResponseContextMenu(UINT popmenuID);

	/** @brief 可以重载以修改列表内容导出时文件夹名称 */
	virtual CString GetExportDirectoryName();
	/** @brief 可以重载以修改默认导出文件名称(不包含路径) */
	virtual CString GetExportDefaultName();
	/** @brief 获取当前消息列表中序号为index(从0开始)的消息 */
	Output_Message_Private* GetMessage(int index);
	

	COutputListCtrl*				m_outputList;	/* 输出列表窗口指针 */
private:

	/** @brief 设置列表使用图标 */
	void	SetListIcon();
	/** @brief 获取当前时间作为消息获得时间 */
	CString CurrentTime();

	/**
	*@brief 记录当前线程正在处理的消息 
	*@return	记录成功返回记录至消息列表的序号(从0开始)，否则返回-1
	*/
	int RecordMessage(const Output_Message& msg, void *pPara = NULL);

	/** @brief 在列表控件中显示消息内容 */
	void DisplayMessage(Output_Message_Private );


	/** @brief 完成列表内容导出为XML文件的过程 */
	BOOL ExportContentToXML(CString filePath);
	/** @brief 获取列表内容导出时默认保存文件名称（包括完整路径信息） */
	CString GetExportDefaultFileFullName();
	/** @brief 清空列表相关数据 */
	void  ClearnMessageList();



	/** @brief 列表列序定义 */
	enum columnIndex
	{
		indexIndex,			/**< @brief “序号”列序 */
		typeIndex,			/**< @brief “类型”列序 */
		contentIndex,		/**< @brief “内容”列序 */
		timeIndex,			/**< @brief “时间”列序 */
	};
	/** @brief 列宽定义，使用比例定义 */
	enum columWidthRatio
	{ 
		indexWidth = 5,		/**< @brief “序号”列宽比例 */
		typeWidth = 15,		/**< @brief “类型”列宽比例 */
		contentWidth = 60,	/**< @brief “内容”列宽比例 */
		timeWidth = 30,		/**< @brief “时间”列宽比例 */
		totalWidth = 100,	/**< @brief 总列宽和 */
	};
	/** @brief 信息类型定义 */
	enum messageType
	{	
		messageType_message = 0,		/**< @brief “消息”类型 */
		messageType_error = 1,			/**< @brief “错误”类型 */
		mesdageType_unknown = 2,
		messageType_para,               /**< @brief  任务操作信息，带形参 */
	};
	/** @brief 图标序号定义 */
	enum iconIndex 
	{
		iconIndex_message,	/**< @brief “消息”类型信息图标 */
		iconIndex_error,	/**< @brief “错误”类型信息图标 */
		iconIndex_unknow,	/**< @brief “未知”类型信息图标 */
	};
	/** @brief 弹出菜单ID定义 */
	enum popupMenuID
	{
		popupMenuID_CollapseContentH = WM_OUTPUT_POPUP_MENUID_MIN,	/**< @brief 菜单项“水平折叠”ID */
		popupMenuID_ExpandContentH,									/**< @brief 菜单项“水平展开”ID */
		popupMenuID_CollapseContentV,								/**< @brief 菜单项“垂直折叠”ID */
		popupMenuID_ExpandContentV,									/**< @brief 菜单项“垂直展开”ID */
		popupMenuID_ClearContent,									/**< @brief 菜单项“清除内容”ID */
		popupMenuID_ExportContent,									/**< @brief 菜单项“导出内容”ID */
	};

	static Output_ListColumn	m_columnList[4];/* 输出列表的格式定义 */

	int							m_messageIndex;	/**< @brief 当前需要被显示的消息序号 */
	MessageList					m_messageList;	/**< @brief 内部保存的消息列表 */

	CCriticalSection	       m_FillOutputCS;	/* 停止标识同步 */
};

