#ifndef XML_COMMON_DEF_H
#define XML_COMMON_DEF_H

#include <list>
//支持xml
/* MSXML库属于COM组件，在XP下可使用MSXML 2、3、4、6，在Win7下可使用MSXML 3、4、6，
   因此选择在这里加载MSXML 6.0以保持兼容性 */
#include <msxml6.h>/* 包含头文件是为了获得COM组件的接口函数声明与一些预定义数据声明 */
//#if (_WIN32_WINNT >= 0x0600)/* COM组件不是版本兼容的，因此对WINNT版本的判断是没用的 */
#import <msxml6.dll> /*raw_interfaces_only*//* 不使用原始指针函数接口，使用智能指针函数接口 */
//#else
//#import <msxml.dll> /*raw_interfaces_only*/
//#endif
//using namespace MSXML2;
/** @brief xml名称空间属性 */
#define STR_XML_XMLNS					_T("xmlns")
/** @brief xml schema文件位置属性 */
#define STR_XML_SCHEMALOCATION			_T("schemaLocation")

/** 
*@brief xml文件中元素未明确指定名称空间前缀时使用的默认前缀 
*		default value for "xxx" of xmlns:xxx="namespace for xml element"
*/
#define STR_XML_DEF_ELEM_PREFIX			_T("scl")
/** 
*@brief XML文件中元素实例属性未明确指定名称空间前时使用的默认前缀 
*		default value for "xxx" of xmlns:xxx="namespace for instance"
*/
#define STR_XML_DEF_INSTANCE_PREFIX		_T("xsi")

/** @brief XML实例文件元素名称空间 */
#define STR_XML_INSTANCE_NAMESPACE		_T("http://www.w3.org/2001/XMLSchema-instance")

#define STR_TOKEN_EQUAL					_T(" = ")
#define STR_TOKEN_SEPARATOR				_T(" , ")

typedef std::list<CString>						XMLStringList;
typedef XMLStringList::iterator					XMLStringListItor;
typedef XMLStringList::const_iterator			XMLStringListConstItor;

typedef std::list<MSXML2::IXMLDOMNodePtr>		XMLDOMNodeList;
typedef XMLDOMNodeList::iterator				XMLDOMNodeListItor;
typedef XMLDOMNodeList::const_iterator			XMLDOMNodeListConstItor;

typedef std::list<MSXML2::ISchemaItemPtr>		XMLSOMItemList;
typedef XMLSOMItemList::iterator				XMLSOMItemListItor;
typedef XMLSOMItemList::const_iterator			XMLSOMItemListConstItor;

/** @brief 带结果描述的布尔类型返回值 return with description */
typedef struct desbool 
{
	BOOL	result;			/**< @brief 操作结果 */
	CString description;	/**< @brief 操作结果描述 */
	desbool()
	{
		result = FALSE;
		description.Empty();
	}
}DesBool;





#define RETURN_DESBOOL_TRUE(desRet,desc)\
{\
	desRet.description = /*_T(*/desc/*)*/;\
	desRet.result = TRUE;\
	return desRet;\
}
#define RETURN_DESBOOL_FALSE(desRet,desc)\
{\
	desRet.description = /*_T(*/desc/*)*/;\
	desRet.result = FALSE;\
	return desRet;\
}

inline void TESTHR( HRESULT _hr ) 
{ 
	if ( FAILED( _hr) )
	{
		_com_issue_error(_hr);
	}
}

/** @brief SOM中属性使用情况约束 */
enum SOM_PARTICLE_TYPE
{
	SOM_PARTICLE_ELEM,
	SOM_PARTICLE_GROUP,
	SOM_PARTICLE_UNKNOWN,
};
enum SOM_ELEM_TYPE
{
	SOM_ELEM_SPECIFY,
	SOM_ELEM_ANY,
	SOM_ELEM_UNKNOWN,
};
enum SOM_ELEM_GROUP_TYPE
{
	SOM_ELEM_GROUP_ALL,
	SOM_ELEM_GROUP_CHOICE,
	SOM_ELEM_GROUP_SEQUENCE,
	SOM_ELEM_GROUP_UNKNOWN,
};

enum SOM_ATTR_TYPE
{
	SOM_ATTR_SPECIFY,
	SOM_ATTR_ANY,
	SOM_ATTR_UNKNOWN,
};
enum SOM_ATTR_BASE_TYPE
{
	SOM_ATTR_BASE_STRING,		/**< @brief 字符串类型，通过正则表达式约束 */
	SOM_ATTR_BASE_DECIMAL,		/**< @brief 数字类型，通过精确位数、最大值、最小值约束 */
	
	SOM_ATTR_VALUE_BASE_UNKNOWN,
};
enum SOM_ATTR_EXTENSION_TYPE
{
	SOM_ATTR_EXTENSION_NONE,		/**< @brief 非枚举、非列表 */
	SOM_ATTR_EXTENSION_LIST,		/**< @brief 列表类型 */
	SOM_ATTR_EXTENSION_ENUM,		/**< @brief 枚举类型 */

	SOM_ATTR_EXTENSION_UNKNOWN,		
};
enum SOM_ATTR_USE_TYPE
{
	SOM_ATTR_USE_OPTIONAL,
	SOM_ATTR_USE_PROHIBITED,
	SOM_ATTR_USE_REQUIRED,
	SOM_ATTR_USE_UNKNOWN,
};


/** @brief 从xpath路径中获取属性名称 */
CString GetAttributeNameFromXPath(CString xpath);

/** @brief 合并字符串列表 */
CString JoinXMLStringList(const XMLStringList& stringList);


/** @brief 将节点名称与节点前缀按照QName规则组合起来 */
CString JoinQName(CString prefix,CString name);


/** @brief 将xmlns属性与前缀信息组合起来(xmlns:xxx中xxx可以不存在) */
CString JoinXmlnsAttrName(CString prefix);
/** @brief 从节点的xmlns:xxx中提取当前名称空间对应的前缀，前缀可能为空 */
BOOL	SplitXmlnsAttrName(CString attrName,CString& prefix);


/** @brief 将节点的schema目标名称空间与schema文件位置组合自来(xxx:schemaLocation="schema名称空间 schema文件位置") */
CString JoinSchemaLocation(CString schemaNamespace,CString schemaPath);
/** @brief 将节点的schemaLocation属性值拆分为schema目标名称空间和schema文件位置信息 */
BOOL	SplitSchemaLocation(CString schemaLocation,CString& schemaNamespace,CString& schemaPath);



/*******************************************************************************/
/** @brief 为节点获取在schema中的约束定义 */
MSXML2::ISchemaItemPtr	_getDeclaration(MSXML2::IXMLDOMNodePtr pXMLDOMNode);
/** @brief 判断属性节点是否不能获取约束信息 */
BOOL _isAttributeHasNoDeclaration(MSXML2::IXMLDOMNodePtr pXMLDOMNode);

/** @brief 显示文件验证错误信息 */
XMLStringList ShowFileValidateError(CString fileName,MSXML2::IXMLDOMParseError2Ptr pParseError);
/** @brief 显示元素验证错误信息 */
XMLStringList ShowElemmentValidateError(CString elemName,MSXML2::IXMLDOMParseError2Ptr pParseError);
/** @brief 显示属性验证错误信息 */
XMLStringList ShowAttributeValidateError(CString elemName,CString attrName,MSXML2::IXMLDOMParseError2Ptr pParseError);


/** @brief 显示DOM文档操作过程中抛出的未知COM异常信息 */
CString ShowComExceptionUnknow(CString info);
/** @brief 显示DOM文档操作过程中抛出的COM异常信息 */
CString ShowComExceptionWithReason(CString info,_com_error& e);


#endif