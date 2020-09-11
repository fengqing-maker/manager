#ifndef	__XMLDOMNODES_H__
#define	__XMLDOMNODES_H__
//////////////////////////////////////////////////////////////////////////
// Header file:
#include "XMLDOMNode.h"
#include "XMLCommonDef.h"
//////////////////////////////////////////////////////////////////////////
   

//////////////////////////////////////////////////////////////////////////
// Class definition:

class CXMLDOMNodes
{
	friend class CXMLDOMNode;

public:
	// Constructor&Deconstructor:
	CXMLDOMNodes( void );
	CXMLDOMNodes( const CXMLDOMNodes& srcXMLDOMNodes );
	CXMLDOMNodes( MSXML2::IXMLDOMNodeListPtr pXMLDOMNodeList );
	~CXMLDOMNodes();

	// Overload operator:
	CXMLDOMNodes& operator= ( const CXMLDOMNodes& srcXMLDOMNodes );
	CXMLDOMNodesPtr operator= ( CXMLDOMNodesPtr psrcXMLDOMNodes );// const
	bool operator== ( const CXMLDOMNodes& srcXMLDOMNodes );
	bool operator!= ( const CXMLDOMNodes& srcXMLDOMNodes );
	CXMLDOMNodePtr operator[] ( int nIndex );
	CXMLDOMNodePtr operator[] ( const TCHAR* pszNodeName );

	// Friend function for operator:
	friend bool operator!=( const CXMLDOMNodesPtr& pXMLDOMNodes1, const CXMLDOMNodesPtr& pXMLDOMNodes2 );
	friend bool operator==( const CXMLDOMNodesPtr& pXMLDOMNodes1, const CXMLDOMNodesPtr& pXMLDOMNodes2 );
	
	friend bool operator!=( const TCHAR* pNULL, const CXMLDOMNodesPtr& pXMLDOMNodes2 );
	friend bool operator!=( const CXMLDOMNodesPtr& pXMLDOMNodes1, const TCHAR* pNULL );

	friend bool operator==( const TCHAR* pNULL, const CXMLDOMNodesPtr& pXMLDOMNodes2 );
	friend bool operator==( const CXMLDOMNodesPtr& pXMLDOMNodes1, const TCHAR* pNULL );

	// Get total item count:
	int GetCount( void );
	
	// Get node Pointer to specific item by item index:
	CXMLDOMNodePtr GetItem( int nIndex );
	// Get node Pointer to specific item by Item name:
	CXMLDOMNodePtr GetItem(CString nodeName );

protected:	
	MSXML2::IXMLDOMNodeListPtr   m_pXMLDOMNodeList;


private:
	/** @brief 显示DOM文档操作过程中抛出的未知COM异常信息 */
	CString ShowComExceptionUnknow(CString info);
	/** @brief 显示DOM文档操作过程中抛出的COM异常信息 */
	CString ShowComExecptionWithReason(CString info,_com_error& e);


};

#endif // End of #ifndef	__XMLDOMNODES_H__
