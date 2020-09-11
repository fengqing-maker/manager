#pragma once
#include <map>

#include "../XMLParser/XMLCommonDef.h"


class SOMAttrType;
typedef std::tr1::shared_ptr<SOMAttrType>		SOMAttrTypePtr;
typedef std::list<SOMAttrTypePtr>				SOMAttrTypePtrList;
typedef SOMAttrTypePtrList::iterator			SOMAttrTypePtrListItor;
class SOMAttrType
{
public:
	SOMAttrType()
	{
		m_typeName.Empty();
		m_baseType = SOM_ATTR_VALUE_BASE_UNKNOWN;
		m_extensionType = SOM_ATTR_EXTENSION_UNKNOWN;

		m_valuePatternList.clear();
		m_valueLen = -1;
		m_valueMinLen = -1;
		m_valueMaxLen = -1;

		m_valueMinExclusive.Empty();
		m_valueMaxExclusive.Empty();
		m_valueMinInclusive.Empty();
		m_valueMaxInclusive.Empty();

		m_valueEnumList.clear();
	}
	~SOMAttrType()
	{
	}
	CString										m_typeName;
	SOM_ATTR_BASE_TYPE							m_baseType;
	SOM_ATTR_EXTENSION_TYPE						m_extensionType;

	XMLStringList								m_valuePatternList;
	XMLStringList								m_valueEnumList;

	int											m_valueLen;			/**< @brief 字符串长度精确值，大于等于0时有效 */
	int											m_valueMinLen;		/**< @brief 字符串长度下限，大于等于0时有效 */
	int											m_valueMaxLen;		/**< @brief 字符串长度上限，大于等于0时有效 */

	int											m_valueTotalDigits;		/**< @brief 允许数字位数 */
	int											m_valueFractionDigits;	/**< @brief 小数部分位数 */
	CString										m_valueMinExclusive;
	CString										m_valueMaxExclusive;
	CString										m_valueMinInclusive;
	CString										m_valueMaxInclusive;



};

class CXMLSOMAttribute;
typedef std::tr1::shared_ptr<CXMLSOMAttribute>	CXMLSOMAttributePtr;
//typedef std::map<CString,CXMLSOMAttributePtr>	CXMLSOMAttributePtrMap;
//typedef CXMLSOMAttributePtrMap::iterator		CXMLSOMAttributePtrMapItor;
class CXMLSOMAttribute
{
public:
	CXMLSOMAttribute(void);
	~CXMLSOMAttribute(void);

	BOOL InitConstraint(MSXML2::IXMLDOMElementPtr pXMLDOMElement,
		MSXML2::IXMLDOMAttributePtr pXMLDOMAttribute);

	XMLStringList GetAttributeValueRefList();

	XMLStringList GetAttributeValueEnumList();

private:
	BOOL _initWithElement(MSXML2::IXMLDOMElementPtr pXMLDOMElement);
	BOOL _initWithAttribute(MSXML2::IXMLDOMAttributePtr pXMLDOMAttribute);

	BOOL _processKeyref(MSXML2::IXMLDOMElementPtr pXMLDOMElement,
		MSXML2::ISchemaIdentityConstraintPtr pKeyref);

	BOOL _processAttribute(MSXML2::ISchemaAttributePtr pSchemaAttribute);
	BOOL _processSimpleType(MSXML2::ISchemaTypePtr pSchemaSimpleType);


	/** @brief 将IdentityConstraint中的XPath路径信息进行合并 */
	CString _joinIdentityConstraintXPath(MSXML2::ISchemaIdentityConstraintPtr pIdentity);
	/** @brief 判断指定的pXMLDOMNode接口是否包含在pXMLDOMNodeList接口列表中 */
	BOOL _isDOMNodeInDONNodeList(MSXML2::IXMLDOMNodePtr pXMLDOMNode,
					MSXML2::IXMLDOMNodeListPtr pXMLDOMNodeList);



	MSXML2::IXMLDOMElementPtr					m_pXMLDOMElement;
	MSXML2::IXMLDOMAttributePtr					m_pXMLDOMAttribute;

	MSXML2::ISchemaAttributePtr					m_pSchemaAttribute;
	
	XMLStringList								m_valueEnumList;
	XMLStringList								m_valueRefList;
};

