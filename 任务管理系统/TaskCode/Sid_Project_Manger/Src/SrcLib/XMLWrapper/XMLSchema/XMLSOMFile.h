#pragma once

#include "../XMLParser/XMLDOMNode.h"
#include "../XMLParser/XMLCommonDef.h"

class CXMLSOMFile;
typedef std::tr1::shared_ptr<CXMLSOMFile>			CXMLSOMFilePtr;

typedef std::map<MSXML2::ISchemaElementPtr,CString>			ProcessedElementMap;
typedef ProcessedElementMap::iterator						ProcessedElementMapItor;

typedef std::map<MSXML2::ISchemaTypePtr,CString>			ProcessedSimpleTypeMap;
typedef ProcessedSimpleTypeMap::iterator					ProcessedSimpleTypeMapItor;

typedef std::map<MSXML2::ISchemaComplexTypePtr,CString>		ProcessedComplexTypeMap;
typedef ProcessedComplexTypeMap::iterator					ProcessedComplexTypeMapItor;

typedef std::map<MSXML2::ISchemaIdentityConstraintPtr,CString>		ProcessedKeyMap;
typedef ProcessedKeyMap::iterator									ProcessedKeyMapItor;

typedef std::map<MSXML2::ISchemaIdentityConstraintPtr,CString>		ProcessedKeyrefMap;
typedef ProcessedKeyrefMap::iterator								ProcessedKeyrefMapItor;

class CXMLSOMFile
{
public:
	CXMLSOMFile(void);
	~CXMLSOMFile(void);
	/** @brief ��ȡ�ڲ������Schema�ļ�ָ�� */
	MSXML2::IXMLDOMSchemaCollection2Ptr	GetInnerPtr();
	/**
	*@brief	��Schema�ļ������������е�Ԫ��Լ������
	*@param	[in]	filePath	Schema�ļ�����·��
	*@param	[in]	namespaceURI	SchemaԼ�����ƿռ�
	*@return		�������
	*/
	DesBool Open(CString filePath,CString namespaceURI);
	/**
	*@brief	�ر�Schema�ļ�
	*@return		�������
	*/
	DesBool Close();

	/** @brief �Ƿ��ļ��Ѵ� */
	BOOL IsFileOpened();
	/** @brief ��ȡ�ļ�����(��������·��) */
	CString GetFileName();
	/** @brief ��ȡ�ļ�Լ��Ŀ�����ƿռ�(targetNamespace) */
	CString GetFileNamespace();
protected:



private:
	/** @brief �Ƿ����ָ���ļ� */
	BOOL IsFileExist( CString strFileName );

	///** 
	//*@brief ��������schemaԼ������ǰ������Ҫȫ��ʹ�õ�Լ����Ϣ
	//*@note	����IdentityConstraintԼ������Ҫ��ȫ�ֲ�ѯ��
	//		��Ϊ�䲻��������ڵ��±�����ģ�ͨ���ڵ�Լ����ѯ�޷���ȡ��
	//*/
	//BOOL _processSchema(MSXML2::IXMLDOMSchemaCollection2Ptr pSchemaCollection,CString namespaceURI);

	///** @brief ����Ԫ����Ϣ */
	//BOOL _processElement(MSXML2::ISchemaElementPtr pSchemaElement);

	//BOOL _processType(MSXML2::ISchemaTypePtr pSchemaType);
	//BOOL _processSimpleType(MSXML2::ISchemaTypePtr pSchemaSimpleType);
	//BOOL _processComplexType(MSXML2::ISchemaComplexTypePtr pSchemaComplexType);

	//BOOL _processModelGroup(MSXML2::ISchemaModelGroupPtr pSchemaModelGroup);

	//BOOL _processKeyConstraint(MSXML2::ISchemaIdentityConstraintPtr pSchemaIdentityConstraint);
	//BOOL _processKeyrefConstraint(MSXML2::ISchemaIdentityConstraintPtr pSchemaReferenceConstraint);

	//BOOL _addProcessedElement(MSXML2::ISchemaElementPtr pSchemaElement);
	//BOOL _isProcessedElement(MSXML2::ISchemaElementPtr pSchemaElement);

	//BOOL _addProcessedSimpleType(MSXML2::ISchemaTypePtr pSchemaSimpleType);
	//BOOL _isProcessedSimpleType(MSXML2::ISchemaTypePtr pSchemaSimpleType);

	//BOOL _addProcessedComplexType(MSXML2::ISchemaComplexTypePtr pSchemaComplexType);
	//BOOL _isProcessedComplexType(MSXML2::ISchemaComplexTypePtr pSchemaComplexType);

	//BOOL _isProcessedKey(MSXML2::ISchemaIdentityConstraintPtr pSchemaKey);
	//BOOL _addProcessedKey(MSXML2::ISchemaIdentityConstraintPtr pSchemaKey);

	//BOOL _isProcessedKeyref(MSXML2::ISchemaIdentityConstraintPtr pSchemaKeyref);
	//BOOL _addProcessedKeyref(MSXML2::ISchemaIdentityConstraintPtr pSchemaKeyref);

	CString									m_filePath;		/**< @brief Schema�ļ�·�� */
	CString									m_namespaceURI;	/**< @brief Schema�ļ��е�targetNamespace */
	MSXML2::IXMLDOMSchemaCollection2Ptr		m_pSchemaCollection;

	//ProcessedElementMap						m_processedElementMap;
	//ProcessedSimpleTypeMap					m_processedSimpleTypeMap;
	//ProcessedComplexTypeMap					m_processedComplexTypeMap;
	//ProcessedKeyMap							m_processedKeyMap;
	//ProcessedKeyrefMap						m_processedKeyrefMap;
};