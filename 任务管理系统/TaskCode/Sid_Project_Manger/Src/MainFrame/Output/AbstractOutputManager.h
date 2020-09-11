/**
*@file	AbstractOutputManager
*@brief	�������Ϣ����б������
*@note	����Ϊ�����࣬��������ʵ���������ڼ̳й����û���Ҫ����Ϣ�����б������
*@author	�߷�
*/
#pragma once

#include <list>
#include "../ui/COutputList/OutputList.h"

#define WM_OUTPUT_POPUP_MENUID_MIN			WM_USER+4000	/* �����ͼ�����˵���ID��Сֵ */
#define WM_OUTPUT_POPUP_MENUID_MAX			WM_USER+4100	/* �����ͼ�����˵���ID���ֵ */


typedef struct output_listcolumn		/* �б��ʽ */
{
	int index;			/* �к� */
	CString title;		/* ���� */
	DWORD alignment;	/* ���� */
	int widthRatio;		/* �п���� */
}Output_ListColumn;

typedef struct output_message			/* �ⲿʹ����Ϣ���� */
{
	int type;			/* ��Ϣ���ͱ�ʶ */
	CString content;	/* ��Ϣ���� */
}Output_Message;

/** @brief ˽����Ϣ���Ͷ��� */
typedef struct output_message_private
{
	int		imageIndex;	/**< @brief ͼ����� */
	CString index;		/**< @brief ��Ϣ��� */
	CString type;		/**< @brief ��Ϣ���� */
	CString content;	/**< @brief ��Ϣ���� */
	CString time;		/**< @brief ��Ϣ��ȡʱ�� */
	void    *pPara;      /**< @brief ������Ϣ */ 
	output_message_private()
	{
		imageIndex = -1;
		pPara = NULL;
	}
}Output_Message_Private;
typedef std::list<Output_Message_Private>					MessageList;
typedef std::list<Output_Message_Private>::iterator			MessageListItor;
typedef std::list<Output_Message_Private>::const_iterator	MessageListConstItor;


#define		WM_OUTPUT_UPDATE	WM_USER+1		/**< @brief �Զ�����Ϣ�����������б�ؼ�ˢ���б����� */


class AbstractOutputManager
{
public:
	AbstractOutputManager(void);
	virtual ~AbstractOutputManager(void) = 0;

	/** @brief ע�������Ϣ�б�ؼ� */
	BOOL RegisterAsOutputList(COutputListCtrl* outputList);

/****************************list operation************************************/
	/** @brief ��ʼ�������Ϣ�б�ؼ� */
	void InitOutputList();	
	/** @brief ��ʾ��ǰ����б����ڵĴ��� */
	//void ShowOutputPane();
	/** @brief ��ʾ�Ҽ������˵� */
	void ShowOutputContextMenu(CPoint point);
	/** @brief ��Ӧ�Ҽ������˵��� */
	void RespOutputContextMenu(UINT menuID);

	/** @brief ���������Ϣ�б�ؼ��п� */
	void ResetColumnWidth();

	/** @brief ˮƽ�۵������Ϣ�б�ؼ��ġ����ݡ��� */
	void CollapseContentColumnH();
	/** @brief ˮƽչ�������Ϣ�б�ؼ��ġ����ݡ��� */
	void ExpandContentColumnH();
	/** @brief ��ֱ�۵������Ϣ�б�ؼ��ġ����ݡ��� */
	void CollapseContentColumnV();
	/** @brief ��ֱ�۵������Ϣ�б�ؼ��ġ����ݡ��� */
	void ExpandContentColumnV();
	
	/** @brief ˢ���б����� */
	void UpdateContent();
	/** @brief ����б����� */
	void ClearContent();
	/** @brief �����б����� */
	void ExportContent();
	
/****************************message operation*********************************/
	/** @brief �������Ϣ��������Ϣ */
	void ShowMessage(CString msg);
	/** @brief ���������������Ϣ */
	void ShowError(CString err);
	/** @brief ��� ���Ρ���Ϣ��������Ϣ */
	void ShowParaMsg( CString msg, void *pPara );
protected:
	/** @brief �����������޸Ķ��Ҽ������˵�����Ӧ���� */
	virtual void ResponseContextMenu(UINT popmenuID);

	/** @brief �����������޸��б����ݵ���ʱ�ļ������� */
	virtual CString GetExportDirectoryName();
	/** @brief �����������޸�Ĭ�ϵ����ļ�����(������·��) */
	virtual CString GetExportDefaultName();
	/** @brief ��ȡ��ǰ��Ϣ�б������Ϊindex(��0��ʼ)����Ϣ */
	Output_Message_Private* GetMessage(int index);
	

	COutputListCtrl*				m_outputList;	/* ����б���ָ�� */
private:

	/** @brief �����б�ʹ��ͼ�� */
	void	SetListIcon();
	/** @brief ��ȡ��ǰʱ����Ϊ��Ϣ���ʱ�� */
	CString CurrentTime();

	/**
	*@brief ��¼��ǰ�߳����ڴ������Ϣ 
	*@return	��¼�ɹ����ؼ�¼����Ϣ�б�����(��0��ʼ)�����򷵻�-1
	*/
	int RecordMessage(const Output_Message& msg, void *pPara = NULL);

	/** @brief ���б�ؼ�����ʾ��Ϣ���� */
	void DisplayMessage(Output_Message_Private );


	/** @brief ����б����ݵ���ΪXML�ļ��Ĺ��� */
	BOOL ExportContentToXML(CString filePath);
	/** @brief ��ȡ�б����ݵ���ʱĬ�ϱ����ļ����ƣ���������·����Ϣ�� */
	CString GetExportDefaultFileFullName();
	/** @brief ����б�������� */
	void  ClearnMessageList();



	/** @brief �б������� */
	enum columnIndex
	{
		indexIndex,			/**< @brief ����š����� */
		typeIndex,			/**< @brief �����͡����� */
		contentIndex,		/**< @brief �����ݡ����� */
		timeIndex,			/**< @brief ��ʱ�䡱���� */
	};
	/** @brief �п��壬ʹ�ñ������� */
	enum columWidthRatio
	{ 
		indexWidth = 5,		/**< @brief ����š��п���� */
		typeWidth = 15,		/**< @brief �����͡��п���� */
		contentWidth = 60,	/**< @brief �����ݡ��п���� */
		timeWidth = 30,		/**< @brief ��ʱ�䡱�п���� */
		totalWidth = 100,	/**< @brief ���п�� */
	};
	/** @brief ��Ϣ���Ͷ��� */
	enum messageType
	{	
		messageType_message = 0,		/**< @brief ����Ϣ������ */
		messageType_error = 1,			/**< @brief ���������� */
		mesdageType_unknown = 2,
		messageType_para,               /**< @brief  ���������Ϣ�����β� */
	};
	/** @brief ͼ����Ŷ��� */
	enum iconIndex 
	{
		iconIndex_message,	/**< @brief ����Ϣ��������Ϣͼ�� */
		iconIndex_error,	/**< @brief ������������Ϣͼ�� */
		iconIndex_unknow,	/**< @brief ��δ֪��������Ϣͼ�� */
	};
	/** @brief �����˵�ID���� */
	enum popupMenuID
	{
		popupMenuID_CollapseContentH = WM_OUTPUT_POPUP_MENUID_MIN,	/**< @brief �˵��ˮƽ�۵���ID */
		popupMenuID_ExpandContentH,									/**< @brief �˵��ˮƽչ����ID */
		popupMenuID_CollapseContentV,								/**< @brief �˵����ֱ�۵���ID */
		popupMenuID_ExpandContentV,									/**< @brief �˵����ֱչ����ID */
		popupMenuID_ClearContent,									/**< @brief �˵��������ݡ�ID */
		popupMenuID_ExportContent,									/**< @brief �˵���������ݡ�ID */
	};

	static Output_ListColumn	m_columnList[4];/* ����б�ĸ�ʽ���� */

	int							m_messageIndex;	/**< @brief ��ǰ��Ҫ����ʾ����Ϣ��� */
	MessageList					m_messageList;	/**< @brief �ڲ��������Ϣ�б� */

	CCriticalSection	       m_FillOutputCS;	/* ֹͣ��ʶͬ�� */
};

