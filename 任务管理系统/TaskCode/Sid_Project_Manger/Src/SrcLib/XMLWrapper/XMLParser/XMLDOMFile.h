#ifndef __XMLDOMFILE_H__
#define __XMLDOMFILE_H__
//////////////////////////////////////////////////////////////////////////
#include <list>
#include <map>
#include "../XMLSchema/XMLSOMFile.h"
#include "XMLDOMNode.h"
#include "XMLCommonDef.h"

//////////////////////////////////////////////////////////////////////////


class CXMLDOMFile;
typedef std::tr1::shared_ptr<CXMLDOMFile>			CXMLDOMFilePtr;

//////////////////////////////////////////////////////////////////////////
/** @brief XSL转换参数列表定义 */
typedef std::map<CString,CString>		XSLParamMap;
typedef XSLParamMap::iterator			XSLParamMapItor;
typedef XSLParamMap::const_iterator		XSLParamMapConstItor;
/** @brief XSL转换控制结构 */
typedef struct xsltctrl
{
	CString xslFileName;				/**< @brief XSL转换样式表文件名称 */
	CString desFileName;				/**< @brief XSL转换目标文件名称 */
	XSLParamMap paraMap;				/**< @brief XSL转换参数列表 */

	xsltctrl()
	{
		xslFileName.Empty();
		desFileName.Empty();
		paraMap.clear();
	}
}XSLTCtrl;

/* 文件保存的编码格式定义 */
enum XCodecType
{
XCodecGB2312,
XCodecUTF8,
XCodecUTF16,
XCodecUnknown,
};
typedef struct xfileproperty 
{
	CString			filePath;	/**< @brief xml文件路径 */
	XCodecType		codeType;	/**< @brief xml文件编码方式 */
	BOOL			bOpened;	/**< @brief xml文件打开标识 */
	BOOL			bModified;	/**< @brief xml文件修改标识 */

	CXMLSOMFilePtr	schemaFile;	/**< @brief 引用schema文件指针 */

	xfileproperty()
	{
		filePath.Empty();
		codeType =	XCodecGB2312;
		bOpened	=	FALSE;
		bModified = FALSE;

		schemaFile = NULL;
	}
}XFileProperty;
typedef std::map<MSXML2::IXMLDOMDocument3Ptr,XFileProperty>	XFilePropertyMap;
typedef XFilePropertyMap::iterator							XFilePropertyMapItor;
typedef XFilePropertyMap::const_iterator					XFilePropertyMapConstItor;



class CXMLDOMFile
{
public:
	CXMLDOMFile();
	virtual ~CXMLDOMFile();

	/** @brief 是否存在指定文件 */
	BOOL IsFileExist( CString strFileName );
	/** @brief 是否已经打开XML文件 */
	BOOL IsFileOpened();

	/** 
	*@brief 在指定位置新建空白文件，若有打开的文件则对先前打开的文件保存后关闭，再进行新建 
	*@note	新建文件不包括打开文件，新建后文件立即被保存，若要对文件进行操作请使用Open()打开文件
	*/
	DesBool New( CString filePath,CString rootName,CString rootNamespace,CXMLSOMFilePtr schemaFile = NULL);
	/** @brief 为文件对象打开文件，若已经打开则对先前打开的文件保存后关闭，再进行打开 */
	DesBool Open( CString filePath,CXMLSOMFilePtr schemaFile = NULL);
	/** @brief 保存文件对象为指定名称文件，若指定名称为空则保存为默认文件名 */
	DesBool Save( CString saveAsFileName = _T(""));
	/** @brief 关闭文件对象，清空文件对象状态 */
	DesBool Close(BOOL bSaveOrNot);

	/** @brief 获取当前XML文件的根节点 */
	CXMLDOMNodePtr GetRoot( void );
	/** @brief 设置当前XML文件的根节点 */
	BOOL SetRoot(CXMLDOMNodePtr root);

	/** @brief 获取当前XML文件根节点使用的schema文件指针 */
	CXMLSOMFilePtr GetRootSchema();
	/** @brief 设置当前XML文件根节点使用的schema文件指针 */
	BOOL SetRootSchema(CXMLSOMFilePtr xmlSOMFile);

	/** 
	*@brief 获取使用XPath查询时使用的默认元素前缀
	*@note	使用XPath查询时对使用默认名称空间的元素应添加默认元素前缀
	*/
	CString GetDefaultElementPrefix();
	/** 
	*@brief 设置使用XPath查询时使用的默认元素前缀 
	*@note	使用XPath查询时对使用默认名称空间的元素应添加默认元素前缀
	*/
	BOOL	SetDefaultElementPrefix(CString defaultPrefix);

	/** @brief 获得当前XML文件路径名 */
	CString GetXMLFilePath();
	/** @brief 设置当前XML文件路径名，下次保存生效 */
	void SetXMLFilePath(CString filePath);
	/** @brief 获得当前XML文件编码方式 */
	XCodecType GetXMLFileCodeType();
	/** @brief 设置当前XML文件编码方式，下次保存生效 */
	void SetXMLFileCodeType(XCodecType codeType);
	/** @brief 获得当前XML文件修改标识 */
	BOOL GetXMLFileModified();
	/** @brief 设置当前XML文件修改标识 */
	void SetXMLFileModified(BOOL bModified);
	/** @brief 获取当前XML文件打开状态 */
	BOOL GetXMLFileOpenState();
	/** @brief 设置当前XML文件打开状态 */
	void SetXMLFileOpenState(BOOL bOpened);
	/** @brief 获取当前XML文件依赖的Schema文件指针 */
	CXMLSOMFilePtr GetXMLFileSchemaFile();
	/** @brief 设置当前XML文件依赖的Schema文件指针 */
	void SetXMLFileSchemaFile(CXMLSOMFilePtr pSchemaFile);

	/** 
	*@brief 使用XPath查找首次出现的元素节点 
	*@note	对XPath中使用默认名称空间的元素应添加默认名称空间对应的前缀
	*		只可以查找元素节点，不要用于查找属性、文本等其他类型节点。
	*/
	CXMLDOMNodePtr SelectSingleNode(CString xpath); 
	/** 
	*@brief 使用XPath查找符合要求的所有元素节点
	*@note	对XPath中使用默认名称空间的元素应添加默认名称空间对应的前缀
	*		只可以查找元素节点，不要用于查找属性、文本等其他类型节点。
	*/
	CXMLDOMNodesPtr SelectNodes( CString xpath );

	/** @brief 使用XSLT转换XML文件 */
	DesBool TransformByXSL(XSLTCtrl& transCtrl);


private:
	/** @brief 初始化XPath查询 */
	BOOL InitXPathQuery(MSXML2::IXMLDOMDocument3Ptr pXMLDoc);
	/** @brief 初始化XSLT脚本设置 */
	BOOL InitXSLTScript(MSXML2::IXMLDOMDocument3Ptr pXMLDoc);
	/** @brief 初始化多错误返回 */
	BOOL InitMultiErrorMessages(MSXML2::IXMLDOMDocument3Ptr pXMLDoc);
	/** @brief 用XSLT对XML文件进行格式化 */
	BOOL NormalizeFormat(CString filePath,XCodecType codeType);

	void AddToFilePropertyMap(MSXML2::IXMLDOMDocument3Ptr domDoc);
	void RemoveFromFilePropertyMap(MSXML2::IXMLDOMDocument3Ptr domDoc);


	/** @brief 显示DOM文档操作过程中的解析错误信息 */
	CString ShowParseErrorWithReason(CString info,CString filePath,MSXML2::IXMLDOMDocument3Ptr domDoc);


	MSXML2::IXMLDOMDocument3Ptr	m_pXMLDOMDocument;	//	The pointer to XML Dom Document.


/*Static */
public:
	static CString GetXMLFilePath(MSXML2::IXMLDOMDocument3Ptr pDomDoc);
	static void SetXMLFilePath(MSXML2::IXMLDOMDocument3Ptr pDomDoc,CString filePath);

	static XCodecType GetXMLFileCodeType(MSXML2::IXMLDOMDocument3Ptr pDomDoc);
	static void SetXMLFileCodeType(MSXML2::IXMLDOMDocument3Ptr pDomDoc,XCodecType codeType);

	static BOOL GetXMLFileModified(MSXML2::IXMLDOMDocument3Ptr pDomDoc);
	static void SetXMLFileModified(MSXML2::IXMLDOMDocument3Ptr pDomDoc,BOOL bModified);

	static BOOL GetXMLFileOpenState(MSXML2::IXMLDOMDocument3Ptr pDomDoc);
	static void SetXMLFileOpenState(MSXML2::IXMLDOMDocument3Ptr pDomDoc,BOOL bOpened);

	static CXMLSOMFilePtr GetXMLFileSchemaFile(MSXML2::IXMLDOMDocument3Ptr pDomDoc);
	static void SetXMLFileSchemaFile(MSXML2::IXMLDOMDocument3Ptr pDomDoc,CXMLSOMFilePtr pSchemaFile);


private:
	static XFilePropertyMap		m_filePropertyMap;
};

#endif	//	End of #ifndef __XMLDOMFILE_H__