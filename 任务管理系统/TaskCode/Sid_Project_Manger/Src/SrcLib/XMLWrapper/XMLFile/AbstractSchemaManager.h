/**
*@file	AbstractSchemaManager.h
*@brief	�����Schema�ļ�������
*@note	����Schema�ļ����������ڼ̳������û��Զ����Schema�ļ���������
		����ֱ������ʵ������
		��Schema�ļ����������û����Schema�ļ��Ķ�ȡ����������ӽ������
		�в�ѯSchemaԼ��������
*@author	�߷�
*/
#pragma once
#include "AbstractLeefModule.h"

#include "XMLSOMFile.h"
#include "AbstractSchemaManageThread.h"

/**
*@brief	����Schema�ļ�������
*/
class AbstractSchemaManager : public AbstractLeefModule
{
public:
	AbstractSchemaManager(void);
	virtual ~AbstractSchemaManager(void);

	/** @brief ��ȡSchema�ļ��������б����SOM�ļ�ָ�� */
	CXMLSOMFilePtr	GetSOMFile();

	/** @brief �ж�Schema�ļ��Ƿ��Ѿ��� */
	BOOL IsFileOpened();


	/**
	*@brief	��ָ����Schema�ļ�������ָ�����ƿռ��SchemaԼ��
	*@param	[in]	filePath Schema�ļ�·��
	*@param	[in]	namespaceURI Schema�ļ�targetNamespace
	*@return	TRUE�����߳������ɹ���FALSE�����߳�����ʧ��
	*@note	Schema������ͨ�����̷߳�ʽ����Schema�ļ���Schema�ļ����ؽ��
	*		Ӧͨ���߳���Ϣȡ�á�
	*/
	BOOL	Open(CString filePath,CString namespaceURI);

	/**
	*@brief	�رյ�ǰ�򿪵�Schema�ļ����ͷ�����SchemaԼ��
	*@return	TRUE���رճɹ���FALSE���ر�ʧ��
	*@note	Schema�����������߳��н��йر�Schema�ļ��Ĳ���
	*/
	BOOL	Close();

	/** 
	*@brief Schema�ļ�����򿪹��̣�Ӧ�����߳��е��� 
	*@note	����Schema����������ͨ�����ظú�������һЩ���⹤�����������Ϣ���
	*/
	virtual BOOL	doOpen();
	/**
	*@brief Schema�ļ�����رչ��̣������߳��е���
	*@ntoe	����Schema����������ͨ�����ظú�������һЩ���⹤�����������Ϣ���
	*/
	virtual BOOL	doClose();
protected:

	/** @brief ����������Schema�ļ��������߳� */
	virtual Unique_SchemaThread	GetSchemaThread() = 0;


	/** @brief ���������ö�����Ϣ������� */
	virtual void OutputMsg(CString msg);
	/** @brief ���������ö������������� */
	virtual void OutputErr(CString err);


	/** @brief Schema�ļ����� */
	CString								m_filePath;
	/** @brief Schema�ļ�targetNamespace */
	CString								m_namespaceURI;
	/** @brief Schema�ļ�ָ�� */
	CXMLSOMFilePtr						m_schemaFile;
	/** @brief Schema�ļ������߳�ָ�� */
	Unique_SchemaThread					m_schemaThread;


};

