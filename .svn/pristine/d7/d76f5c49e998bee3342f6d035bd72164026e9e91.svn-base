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
	/** @brief 获取内部保存的Schema文件指针 */
	MSXML2::IXMLDOMSchemaCollection2Ptr	GetInnerPtr();
	/**
	*@brief	打开Schema文件，并解析其中的元素约束规则
	*@param	[in]	filePath	Schema文件所在路径
	*@param	[in]	namespaceURI	Schema约束名称空间
	*@return		结果描述
	*/
	DesBool Open(CString filePath,CString namespaceURI);
	/**
	*@brief	关闭Schema文件
	*@return		结果描述
	*/
	DesBool Close();

	/** @brief 是否文件已打开 */
	BOOL IsFileOpened();
	/** @brief 获取文件名称(包括完整路径) */
	CString GetFileName();
	/** @brief 获取文件约束目标名称空间(targetNamespace) */
	CString GetFileNamespace();
protected:



private:
	/** @brief 是否存在指定文件 */
	BOOL IsFileExist( CString strFileName );

	///** 
	//*@brief 遍历整个schema约束，提前解析需要全局使用的约束信息
	//*@note	例如IdentityConstraint约束就需要在全局查询，
	//		因为其不是在自身节点下被定义的，通过节点约束查询无法获取。
	//*/
	//BOOL _processSchema(MSXML2::IXMLDOMSchemaCollection2Ptr pSchemaCollection,CString namespaceURI);

	///** @brief 处理元素信息 */
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

	CString									m_filePath;		/**< @brief Schema文件路径 */
	CString									m_namespaceURI;	/**< @brief Schema文件中的targetNamespace */
	MSXML2::IXMLDOMSchemaCollection2Ptr		m_pSchemaCollection;

	//ProcessedElementMap						m_processedElementMap;
	//ProcessedSimpleTypeMap					m_processedSimpleTypeMap;
	//ProcessedComplexTypeMap					m_processedComplexTypeMap;
	//ProcessedKeyMap							m_processedKeyMap;
	//ProcessedKeyrefMap						m_processedKeyrefMap;
};