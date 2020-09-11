/******************************************************/
/**
*@file		AbstractFileManager.h
*@brief		����SCL�ļ������ඨ��
*@note		AbstractFileManager��������ʵ�����ļ��������
*@author	�߷�
*@date		[4/3/2014]
********************************************************/
#pragma once
#include "AbstractLeefModule.h"		/* ��Ҫ�����߳�������Ϣ������� */

#include "XMLModel.h"
#include "XMLModelParser.h"
#include "AbstractFileManageThread.h"
#include "AbstractFileCommonDef.h"


class XMLFileManageThread;
class AbstractFileManager : public AbstractLeefModule
{
public:
	AbstractFileManager(void);
	virtual ~AbstractFileManager(void) = 0;

	/**
	*@brief	�ж��ļ�����ģ���Ƿ����һ���ļ�
	*@note 	
	*@see 	AbstractFileManager::OpenFile()
	*/
	BOOL IsFileOpened();
	/**
	*@brief	�ж��ļ�����ģ���Ƿ��޸����ļ�
	*@note
	*/
	BOOL IsFileModified();


	/**
	*@brief	XML�ļ�ͨ�õ��½�����
	*@return		
	*@note
	*@see	
	*/
	BOOL NewFile(XmlFileCreateCtrl fileCraeteCtrl);
	/**
	*@brief XML�ļ�ͨ�õĴ򿪺���
	*@param [in]	�ļ�����
	*@return		TRUE���ɹ���FALSE��ʧ��
	*@note	���ļ�ʵ�ʹ��������߳��н��У�����ķ���ֵ����
	*		���߳����������ʵ�ʴ򿪽��ͨ�����߳���Ϣ��á�
	*@see	AbstractFileManager::doOpenFile()
	*/
	BOOL OpenFile(CString filePath);
	/**
	*@brief	XML�ļ�ͨ�õĹرպ���
	*@return		TRUE���رճɹ���FALSE���ر�ʧ��
	*note	�رյ�ǰ�ļ��������д򿪵��ļ���
	*/
	BOOL CloseFile();
	/**
	*@brief	XML�ļ�ͨ�õı��溯��
	*@return		TRUE������ɹ���FALSE������ʧ��
	*@note	��ԭ�ļ����Ʊ����ļ��������д򿪵��ļ���
	*/
	BOOL SaveFile();
	/**
	*@brief	XML�ļ�ͨ�����Ϊ����
	*@return		TRUE������ɹ���FALSE������ʧ��
	*@note	ͨ�����Ϊ�Ի���ʵ���ļ����Ϊ
	*/
	BOOL SaveAsFile(CString newFilePath);

	/**
	*@brief	��ȡ��ǰ�ļ�������������ļ�����
	*@note 	��ȡ���ļ����Ʋ�����·����Ϣ
	*@see 	AbstractFileManager::GurrentFileName()
	*/
	CString CurrentFileName();
	/**
	*@brief	��ȡ��ǰ�ļ�������������ļ�·������
	*@note 	��ȡ���ļ����ư������ļ�����Ϣ
	*@see 	AbstractFileManager::CurrentFilePath()
	*/
	CString CurrentFilePath();

	/**
	*@brief	��ȡ��ǰ�ļ������������<b>�ļ�ģ��</b>
	*@see	Weak_XMLModel
	*/
	Weak_XMLModel	CurrentFileModel();
	/**
	*@brief	��ȡ��ǰ�ļ������������<b>�ļ�ģ�͸��ڵ�ָ��</b>
	*@see	Weak_XMLElem
	*/
	Weak_XMLElem	CurrentFileRootElem();

	/**
	*@brief	����½��ļ��ľ������
	*@return		TRUE:�ɹ���FALSE��ʧ��	
	*@note	�˺�������½��ļ���ʵ�ʲ������������߳�����ɣ�
	*/
	virtual BOOL doNewFile();
	/**
	*@brief	��ɴ��ļ��ľ������
	*@return		TRUE:�ɹ���FALSE��ʧ��
	*@note	�˺�����ɴ��ļ�ʵ�ʲ������������߳�����ɣ�
	*/
	virtual BOOL doOpenFile();
	/**
	*@brief	��ɹر��ļ��ľ������
	*@return		TRUE:�ɹ���FALSE��ʧ��
	*@note	�˺�����ɹر��ļ�ʵ�ʲ������������߳�����ɣ�
	*/
	virtual BOOL doCloseFile();
protected:
	/** @brief �����Ըı��ļ���������Ϣ�����ʽ */
	virtual void OutputMsg(CString msg);
	/** @brief �����Ըı��ļ����������������ʽ */
	virtual void OutputErr(CString err);

	/**
	*@brief	�����������ļ���Ӧ��Schema�ļ�
	*@return		��XML�ļ�DOM�������Schemaģ��
	*@note	��Ӧ��Schema Object Model����XML�ļ��ڼ���DOMʱ�����
	*		�������ļ�������ָ��������Schema�ļ����򽫻��޸�ԭ�ļ���Schema��Ϣ��
	*		����ʹ��ԭ�ļ��е�Schema��Ϣ
	*@see	CXMLSOMFilePtr
	*/
	virtual CXMLSOMFilePtr	GetSchemaFile();
	/**
	*@brief �����������ļ�����������ʹ�õĽ�����
	*@return	�ļ�ģ�ͽ���������ָ��
	*@note	�����õĽ�����Ӧ�̳���Ĭ�Ͻ�����XMLModelParser
	*@see	XMLModelParser
	*/
	virtual Unique_XMLModelParser GetModelParser();
	/**
	*@brief �����������ļ�����������ʹ�õ����̶߳���
	*@return		�ļ��������̶߳��������ָ��
	*@note	�����õ��߳���Ӧ�̳���Ĭ���ļ������߳�AbstractFileManageThread
	*@see	AbstractFileManageThread
	*/
	virtual Unique_FileThread	GetFileThread();

/******************************file new operation***************************/

	/** @brief �ļ�������ʹ�õ�ģ�ͽ����� */
	Unique_XMLModelParser				m_xmlModelParser;
	/** @brief �ļ������̶߳��� */
	Unique_FileThread					m_xmlFileThread;


	/** @brief �������Ƿ���ļ���ʶ��TRUE���Ѵ��ļ���FALSE��δ���ļ� */
	BOOL								m_xmlFileOpened;
	/** @brief ��ǰ���������ڲ������ļ�������Ϣ��������·����Ϣ */
	CString								m_xmlFileName;
	/** @brief ��ǰ���������ڲ������ļ�·�����ƣ������ļ�����Ϣ */
	CString								m_xmlFilePath;
	/** @brief �ڲ�����XML�ļ���¼,[ key:�ļ�·�� value:�ļ�ģ�Ͷ��� ]	*/
	Shared_XMLModelMap					m_xmlModelMap;

	/** @brief �������ļ����ƽṹ */
	XmlFileCreateCtrl					m_xmlFileCreateCtrl;

private:


};

