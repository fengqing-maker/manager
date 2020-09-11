/**
*@file	�����XML�ļ�������
*@brief	����XML�ļ���������������ʵ������������XML�ļ�������ͨ�ù���
*@note	Ӧ��ͨ���̳����صķ�ʽʹ�ó���XML�ļ�������
*@author	�߷�
*/
#pragma once
#include "AbstractLeefModule.h"

#include "AbstractFileExportThread.h"

class AbstractFileExporter : public AbstractLeefModule
{
public:
	AbstractFileExporter(void);
	virtual ~AbstractFileExporter(void) = 0;
	/** @see AbstractLeefModule::GetModuleID() */
	virtual int GetModuleID() = 0;

	/**
	*@brief	ִ���ļ�����
	*@param	[in]	bMultiThread	�Ƿ�ʹ�ö��߳�ִ��
	*@return	��ʹ�ö��߳�ʱ����TRUE�������ɹ���FALSE������ʧ��
	*			ʹ�ö��߳�ʱ����TRUE�����߳������ɹ���FALSE�����߳�����ʧ��
	*@note	��ʹ�ö��߳�ʱ�ú���ֱ�ӵ���DoExportFile����ִ���ļ�������
	*		ʹ�ö��߳�ʱ�ú����������߳�
	*@see	
	*/
	BOOL ExportFile(BOOL bMultiThread);

	/**
	*@brief	�ļ�����ִ�к���
	*@return	TRUE�������ɹ���FALSE������ʧ�ܡ�
	*@note	�ú���Ӧ��ExportFile()���ã�����ļ�������ʵ�ʹ��̣�
	*		�����ļ�������Ӧ���ظú����������ļ�������ʵ�ʹ��̡�
	*@see	AbstractFileExporter::ExportFile()
	*/
	virtual BOOL DoExportFile() = 0;

protected:

	/** @brief �����������ļ����������������õ������߳�ָ�� */
	virtual Unique_ExportThread	GetFileExportThread() = 0;


	/** @brief �ļ��������߳�����ָ�� */
	Unique_ExportThread					m_exportThread;

private:

	/** @brief �����ļ��������߳� */
	BOOL StartFileExportThread();

};

