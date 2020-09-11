#ifndef	__XMLDOMNODE_H__
#define	__XMLDOMNODE_H__
//////////////////////////////////////////////////////////////////////////
// 
#include <afx.h>
#include <memory>


#include "XMLCommonDef.h"


//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
//	Mask compile warning
#pragma warning( disable : 4996 )
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
// 
class CXMLDOMNode;
class CXMLDOMNodes;
// Smart pointer to CXMLDOMNode:
typedef std::tr1::shared_ptr< CXMLDOMNode > CXMLDOMNodePtr;
typedef std::tr1::shared_ptr< CXMLDOMNodes > CXMLDOMNodesPtr;



#define NULLSTRING TEXT('\0')

#ifndef _tstof 

#ifdef UNICODE
#pragma message( "Unicode have been defined!")
inline double _wtof( const TCHAR *szBuffer )
{
	int nLenWC = _tcslen( szBuffer );
 	int nLenMB =  WideCharToMultiByte( CP_ACP, 0, szBuffer, -1, NULL, 0, NULL, NULL );
	char* pMBString = new char[ nLenMB + 1 ];
	WideCharToMultiByte( CP_ACP, 0, szBuffer, nLenWC, pMBString, nLenMB, NULL, NULL );
	double dRet = atof( pMBString  );
	if( NULL != pMBString)
	{
		delete [] pMBString;
	}
 	return dRet;
}
#define _tstof _wtof

#else

#pragma message( "Unicode have not been defined, yet!")
#define _tstof	atof

#endif // #ifdef UNICODE

#endif // #ifndef _tstof 


#include "XMLDOMNodes.h"
#include "../XMLSchema/XMLSOMElement.h"
//////////////////////////////////////////////////////////////////////////
// Class definition:

class CXMLDOMNode
{
	friend class CXMLDOMNodes;
public:
	// Constructor&Deconstructor:
	CXMLDOMNode( void );
	CXMLDOMNode( const CXMLDOMNode& srcXMLDOMNode );
 	CXMLDOMNode( MSXML2::IXMLDOMNodePtr pXMLDOMNode );
	~CXMLDOMNode();

	// Overload operator:
	CXMLDOMNode& operator= ( const CXMLDOMNode& srcXMLDOMNode );

	bool operator== ( const CXMLDOMNode& srcXMLDOMNode );
	bool operator!= ( const CXMLDOMNode& srcXMLDOMNode );

	// Friend function for CXMLDOMNodePtr:
	friend bool operator== ( const CXMLDOMNodePtr& pXMLDOMNode1, const CXMLDOMNodePtr& pXMLDOMNode2 );
	friend bool operator!= ( const CXMLDOMNodePtr& pXMLDOMNode1, const CXMLDOMNodePtr& pXMLDOMNode2 );
	// Friend operator "!=" for Only compare with NULL.
	friend bool operator!= (  const CXMLDOMNodePtr& pXMLDOMNode2, const TCHAR* pNULL );
	friend bool operator!= ( const TCHAR* pNULL, const CXMLDOMNodePtr& pXMLDOMNode2 );
	// Friend operator "==" for Only compare with NULL.
	friend bool operator== (  const CXMLDOMNodePtr& pXMLDOMNode2, const TCHAR* pNULL );
	friend bool operator== ( const TCHAR* pNULL, const CXMLDOMNodePtr& pXMLDOMNode2 );

	/** @brief Judge current node whether has Child(ren) node(s). */ 
	BOOL HasChildNodes( void ); 
	// Judge specific child node of current node whether existing.
	bool IsChildNodeExist( const TCHAR* szChildNodeName );
	// Judge specific child node of current node whether existing and return the pointer to the found node.
	bool IsChildNodeExist( const TCHAR* szChildNodeName, CXMLDOMNodePtr& pGetXMLDOMNode OUT );
	// Judge specific child node with specific value of current node whether existing.
	bool IsChildNodeExist( const TCHAR* szChildNodeName, const TCHAR* szChildNodeValue );
	// Judge specific child node with specific STRING value of current node whether existing and return the pointer to the found node.
	bool IsChildNodeExist( const TCHAR* szChildNodeName, const TCHAR* szChildNodeValue, CXMLDOMNodePtr& pGetXMLDOMNode OUT );
	// Judge specific child node with specific INTEGER value of current node whether existing and return the pointer to the found node.
	bool IsChildNodeExist( const TCHAR* szChildNodeName, const int& nChildNodeValue, CXMLDOMNodePtr& pGetXMLDOMNode OUT );
	// Judge specific child node with STRING attribute value of current node whether existing.
	bool IsChildNodeExist( const TCHAR* szChildNodeName, const TCHAR* szAttributeName, const TCHAR* szAttributeValue );
	// Judge specific child node with STRING attribute value of current node whether existing and return the pointer to the found node.
	bool IsChildNodeExist( const TCHAR* szChildNodeName, const TCHAR* szAttributeName, const TCHAR* szAttributeValue, CXMLDOMNodePtr& pGetXMLDOMNode OUT );
	// Judge specific child node with DOUBULE attribute value of current node whether existing.
	bool IsChildNodeExist( const TCHAR* szChildNodeName, const TCHAR* szAttributeName, const double& dAttributeValue );
	// Judge specific child node with DOUBULE attribute value of current node whether existing and return the pointer to the found node.
	bool IsChildNodeExist( const TCHAR* szChildNodeName, const TCHAR* szAttributeName, const double& dAttributeValue, CXMLDOMNodePtr& pGetXMLDOMNode OUT );
	// Judge specific child node with INTEGER attribute value of current node whether existing.
	bool IsChildNodeExist( const TCHAR* szChildNodeName, const TCHAR* szAttributeName, const int& nAttributeValue );
	// Judge specific child node with INTEGER attribute value of current node whether existing and return the pointer to the found node.
	bool IsChildNodeExist( const TCHAR* szChildNodeName, const TCHAR* szAttributeName, const int& nAttributeValue, CXMLDOMNodePtr& pGetXMLDOMNode OUT );
	// Judge specific child node with BOOLEAN attribute value of current node whether existing.
	bool IsChildNodeExist( const TCHAR* szChildNodeName, const TCHAR* szAttributeName, const bool& bAttributeValue );
	// Judge specific child node with BOOLEAN attribute value of current node whether existing and return the pointer to the found node.
	bool IsChildNodeExist( const TCHAR* szChildNodeName, const TCHAR* szAttributeName, const bool& bAttributeValue, CXMLDOMNodePtr& pGetXMLDOMNode OUT );
	
	/** @brief Get Child count for current node. */
	int GetChildCount();
	/** @brief Get Child Pointer for current node by index */ 
	CXMLDOMNodePtr GetChildNode(int index);	
	/** @brief Get the first child pointer for current node by name */
	CXMLDOMNodePtr GetChildNode(CString childName);
	/** @brief Get the first child of current node. */ 
	CXMLDOMNodePtr GetFirstChildNode(); 
	/** @brief Get the last child of current node. */ 
	CXMLDOMNodePtr GetLastChildNode();
	/** @brief Get Children Pointers for current node by XPath. */
	CXMLDOMNodesPtr GetChildrenNodes();
	/** @brief Get Child Pointer for current node by XPath */
	CXMLDOMNodePtr GetFirstChildNodeByXPath(CString xpath);
	/** @brief Get Children pointers for current node by XPath */
	CXMLDOMNodesPtr GetChildrenNodesByXPath(CString xpath);

	/** @brief Get the parent for current node. */
	CXMLDOMNodePtr GetParentNode();
	
	/** @brief Get the number of current node's sibling. */
	int GetSiblingCount( void );
	// Get the first sibling node of current node.
	CXMLDOMNodePtr GetFirstSiblingNode( void );
	// Get the Next sibling node of current node.
	CXMLDOMNodePtr GetNextSiblingNode( void );
	// Get the Previous sibling node of current node.
	CXMLDOMNodePtr GetPrevSiblingNode( void );
	// Get the last sibling node of current node.
	CXMLDOMNodePtr GetLastSiblingNode( void );

	// Node Operation:
	/** @brief Get tag name for current node. */
	CString GetNodeName( void );
	/** @brief Get namespace for current node. */
	CString GetNodeNamespace( void );

	/** 
	*@brief Clone current node. ( bIsDeepClone indicates that whether to recursively clone all nodes 
	*		that are descendants of this node; ture: yes, false: no  )	
	*/
	BOOL CloneNode( CXMLDOMNodePtr& pGetClonedNodeRoot, BOOL bIsDeepClone = TRUE );
	/** @brief Insert node before current node. */
	BOOL InsertSiblingBefore(CXMLDOMNodePtr& pInNode );
	/** @brief Insert node before current node and Get pointer to inserted. */
	BOOL InsertSiblingBefore(CXMLDOMNodePtr& pInNode, CXMLDOMNodePtr& pGetInsertedNode OUT );
	/** @brief Insert node before current node with parent's namespace URI */
	CXMLDOMNodePtr InsertSiblingBefore(CString nodeName);		
	/** @brief Insert node before current node with sepcificed namespace URI */
	CXMLDOMNodePtr InsertSiblingBefore(CString nodeName,CString namespaceURI);
	/* 因为可能需要插入命名空间为空的特殊节点，所以不能将namespaceURI作为默认参数 */

	/** @brief Insert node after current node.	*/
	BOOL InsertSiblingAfter(CXMLDOMNodePtr& pInNode );
	/** @brief Insert node after current node and Get pointer to inserted.	*/
	BOOL InsertSiblingAfter(CXMLDOMNodePtr& pInNode, CXMLDOMNodePtr& pGetInsertedNode OUT );
	/** @brief Insert node after current node with parent's namespace URI */
	CXMLDOMNodePtr InsertSiblingAfter(CString nodeName);
	/** @brief Insert node before current node with sepcificed namespace URI */
	CXMLDOMNodePtr InsertSiblingAfter(CString nodeName,CString namespaceURI);
	/* 因为可能需要插入命名空间为空的特殊节点，所以不能将namespaceURI作为默认参数 */


	/** @brief 向当前节点的子节点列表末尾追加一个子节点( usually cooperate with CloneNode in use. )	*/
	BOOL AppendChildNode(CXMLDOMNodePtr& pInNode );
	/** @brief 向当前节点的子节点列表末尾追加一个子节点( usually cooperate with CloneNode in use. ) */
	BOOL AppendChildNode(CXMLDOMNodePtr& pInNode, CXMLDOMNodePtr& pGetAppendedNode OUT );
	/** @brief 向当前节点的子节点列表末尾追加一个子节点( usually cooperate with CloneNode in use. ) */
	CXMLDOMNodePtr AppendChildNode(CString nodeName);
	/** @brief 向当前节点的子节点列表末尾追加一个子节点( usually cooperate with CloneNode in use. ) */
	CXMLDOMNodePtr AppendChildNode(CString nodeName,CString namespaceURI);

	/** @brief 向当前节点的子节点列表末尾追加一个子节点( usually cooperate with CloneNode in use. ) */
	CXMLDOMNodePtr AppendChildNodeWithValue(CString nodeName,CString nodeValue);
	/** @brief 向当前节点的子节点列表末尾追加一个子节点( usually cooperate with CloneNode in use. ) */
	CXMLDOMNodePtr AppendChildNodeWithAttr(CString nodeName,CString nodeValue,CString attrName,CString attrValue);
	
	/** @brief  Delete current node and its child node(s) if having any. */
	BOOL DeleteCurrentNode();
	/** @brief  Delete current node's all of the children nodes but Reserve current node. */
	BOOL DeleteChildrenNodes();
	
	/** @brief validate current node against schema */
	BOOL ValidateCurrentNode(XMLStringList& errorInfoList);

	// Attribute Operation:	
	/** @brief Get Attribute count of current node. */
	int GetAttributeCount( void );
	/** @brief Get Attribute name of current node by Index and return attribute name. */
	CString GetAttributeName( int nIndex);

	/** @brief The specificied attribute name or STRING value for current node Exist? */
	BOOL IsAttributeExist(CString attrName);
	/** @brief Insert attribute item by attribute name */
	BOOL AddAttributeNode(CString attrName);
	/** @brief Insert attribute item by qualified attribute name */
	BOOL AddAttributeNode(CString attrName,CString namespaceURI);
	/** @brief Insert attribute item with specificed value and qualified attribute name */
	BOOL AddAttributeNode(CString attrName,CString namespaceURI,CString attrValue);
	/** @brief Delete attribute item by attribute name. */
	BOOL RemoveAttributeNode( CString attrName);

	/** @brief Get STRING attribute value of current node by Attribute index.  */
	CString GetAttributeValue(int index);
	/** @brief Get STRING attribute value of current node by Attribute name.  */
	CString GetAttributeValue(CString attrName);

	/** @brief Set STRING attribute value for current node by attribute index. */
	BOOL SetAttributeValue(int index,CString attrValue);
	/** @brief Set STRING attribute value for current node by attribute name. */
	BOOL SetAttributeValue(CString attrName, CString attrValue);

	/** @brief Validate attribute against schema by attribute index. */
	BOOL	ValidateAttribute(int index,XMLStringList& errorInfoList);
	/** @brief Validate attribute against schema by attribute name. */
	BOOL	ValidateAttribute(CString attrName,XMLStringList& errorInfoList);
	

	// Get&Set Node Value Operation:
	/** @brief  Get STRING value from current node. */
	CString GetNodeValue( void );
	/** @brief Set STRING value for current node. */ 
	BOOL SetNodeValue( CString newValue );



	/** @brief 获取元素节点对应Schema约束 */
	CXMLSOMElementPtr	GetElementConstraint();
	/** @brief 获取指定名称的属性节点对应Schema约束 */
	CXMLSOMAttributePtr	GetAttributeConstraint(CString attrName);


	// return MSXML2::IXMLDOMNodePtr inner pointer
	MSXML2::IXMLDOMNodePtr GetInnerPtr(){return m_pXMLDOMNode;}

private:
	// Get value of current node.
	CString _GetValue( void );
	// Set value for current node.
	BOOL _SetValue( CString value );
	//Delete value for current node.
	BOOL _DeleteValue( void );

	// Convert double to string:
	TCHAR* _DoubleToString( const double& dToBeConvert );
	// Convert _variant_t to string:
	TCHAR* _VariantToString( const _variant_t& var );

	/** @brief 设置文档修改标识 */
	void _SetDocumentModified();

	/** @brief 获取元素节点对应Schema Object Model节点约束信息 */
	CXMLSOMElementPtr _getSchemaElementConstraint();
	/** @brief 获取属性节点对应Schema Object Model节点约束信息 */
	CXMLSOMAttributePtr	_getSchemaAttributeConstraint(CString attrName);



	MSXML2::IXMLDOMNodePtr   m_pXMLDOMNode;

	CXMLSOMElementPtr		m_pXMLSOMElem;
	CXMLSOMAttributePtr		m_pXMLSOMAttr;
};

#endif // End of #ifndef	__XMLDOMNODE_H__
