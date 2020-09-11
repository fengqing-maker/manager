#ifndef XML_COMMON_DEF_H
#define XML_COMMON_DEF_H

#include <list>
//֧��xml
/* MSXML������COM�������XP�¿�ʹ��MSXML 2��3��4��6����Win7�¿�ʹ��MSXML 3��4��6��
   ���ѡ�����������MSXML 6.0�Ա��ּ����� */
#include <msxml6.h>/* ����ͷ�ļ���Ϊ�˻��COM����Ľӿں���������һЩԤ������������ */
//#if (_WIN32_WINNT >= 0x0600)/* COM������ǰ汾���ݵģ���˶�WINNT�汾���ж���û�õ� */
#import <msxml6.dll> /*raw_interfaces_only*//* ��ʹ��ԭʼָ�뺯���ӿڣ�ʹ������ָ�뺯���ӿ� */
//#else
//#import <msxml.dll> /*raw_interfaces_only*/
//#endif
//using namespace MSXML2;
/** @brief xml���ƿռ����� */
#define STR_XML_XMLNS					_T("xmlns")
/** @brief xml schema�ļ�λ������ */
#define STR_XML_SCHEMALOCATION			_T("schemaLocation")

/** 
*@brief xml�ļ���Ԫ��δ��ȷָ�����ƿռ�ǰ׺ʱʹ�õ�Ĭ��ǰ׺ 
*		default value for "xxx" of xmlns:xxx="namespace for xml element"
*/
#define STR_XML_DEF_ELEM_PREFIX			_T("scl")
/** 
*@brief XML�ļ���Ԫ��ʵ������δ��ȷָ�����ƿռ�ǰʱʹ�õ�Ĭ��ǰ׺ 
*		default value for "xxx" of xmlns:xxx="namespace for instance"
*/
#define STR_XML_DEF_INSTANCE_PREFIX		_T("xsi")

/** @brief XMLʵ���ļ�Ԫ�����ƿռ� */
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

/** @brief ����������Ĳ������ͷ���ֵ return with description */
typedef struct desbool 
{
	BOOL	result;			/**< @brief ������� */
	CString description;	/**< @brief ����������� */
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

/** @brief SOM������ʹ�����Լ�� */
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
	SOM_ATTR_BASE_STRING,		/**< @brief �ַ������ͣ�ͨ��������ʽԼ�� */
	SOM_ATTR_BASE_DECIMAL,		/**< @brief �������ͣ�ͨ����ȷλ�������ֵ����СֵԼ�� */
	
	SOM_ATTR_VALUE_BASE_UNKNOWN,
};
enum SOM_ATTR_EXTENSION_TYPE
{
	SOM_ATTR_EXTENSION_NONE,		/**< @brief ��ö�١����б� */
	SOM_ATTR_EXTENSION_LIST,		/**< @brief �б����� */
	SOM_ATTR_EXTENSION_ENUM,		/**< @brief ö������ */

	SOM_ATTR_EXTENSION_UNKNOWN,		
};
enum SOM_ATTR_USE_TYPE
{
	SOM_ATTR_USE_OPTIONAL,
	SOM_ATTR_USE_PROHIBITED,
	SOM_ATTR_USE_REQUIRED,
	SOM_ATTR_USE_UNKNOWN,
};


/** @brief ��xpath·���л�ȡ�������� */
CString GetAttributeNameFromXPath(CString xpath);

/** @brief �ϲ��ַ����б� */
CString JoinXMLStringList(const XMLStringList& stringList);


/** @brief ���ڵ�������ڵ�ǰ׺����QName����������� */
CString JoinQName(CString prefix,CString name);


/** @brief ��xmlns������ǰ׺��Ϣ�������(xmlns:xxx��xxx���Բ�����) */
CString JoinXmlnsAttrName(CString prefix);
/** @brief �ӽڵ��xmlns:xxx����ȡ��ǰ���ƿռ��Ӧ��ǰ׺��ǰ׺����Ϊ�� */
BOOL	SplitXmlnsAttrName(CString attrName,CString& prefix);


/** @brief ���ڵ��schemaĿ�����ƿռ���schema�ļ�λ���������(xxx:schemaLocation="schema���ƿռ� schema�ļ�λ��") */
CString JoinSchemaLocation(CString schemaNamespace,CString schemaPath);
/** @brief ���ڵ��schemaLocation����ֵ���ΪschemaĿ�����ƿռ��schema�ļ�λ����Ϣ */
BOOL	SplitSchemaLocation(CString schemaLocation,CString& schemaNamespace,CString& schemaPath);



/*******************************************************************************/
/** @brief Ϊ�ڵ��ȡ��schema�е�Լ������ */
MSXML2::ISchemaItemPtr	_getDeclaration(MSXML2::IXMLDOMNodePtr pXMLDOMNode);
/** @brief �ж����Խڵ��Ƿ��ܻ�ȡԼ����Ϣ */
BOOL _isAttributeHasNoDeclaration(MSXML2::IXMLDOMNodePtr pXMLDOMNode);

/** @brief ��ʾ�ļ���֤������Ϣ */
XMLStringList ShowFileValidateError(CString fileName,MSXML2::IXMLDOMParseError2Ptr pParseError);
/** @brief ��ʾԪ����֤������Ϣ */
XMLStringList ShowElemmentValidateError(CString elemName,MSXML2::IXMLDOMParseError2Ptr pParseError);
/** @brief ��ʾ������֤������Ϣ */
XMLStringList ShowAttributeValidateError(CString elemName,CString attrName,MSXML2::IXMLDOMParseError2Ptr pParseError);


/** @brief ��ʾDOM�ĵ������������׳���δ֪COM�쳣��Ϣ */
CString ShowComExceptionUnknow(CString info);
/** @brief ��ʾDOM�ĵ������������׳���COM�쳣��Ϣ */
CString ShowComExceptionWithReason(CString info,_com_error& e);


#endif