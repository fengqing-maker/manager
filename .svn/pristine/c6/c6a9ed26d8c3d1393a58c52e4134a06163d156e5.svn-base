#include "stdafx.h"
#include "XMLDOMNodes.h"
//////////////////////////////////////////////////////////////////////////
//
#include <assert.h>
#include <tchar.h>


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////////////////////////////////////////////////////////////
#define RETURN_NULL_PTR		CXMLDOMNodePtr pRetNode( new CXMLDOMNode( NULL ) );return pRetNode; 
#define RETURN_NULL_LIST	CXMLDOMNodesPtr pRetNodes( new CXMLDOMNodes( NULL ) );return pRetNodes; 
//////////////////////////////////////////////////////////////////////////
// Constructor&Deconstructor:
// Default constructor:
CXMLDOMNodes::CXMLDOMNodes( void )
{
	m_pXMLDOMNodeList = NULL;
}
// Copy constructor:
CXMLDOMNodes::CXMLDOMNodes( const CXMLDOMNodes& srcXMLDOMNodes )
{
	assert( NULL != srcXMLDOMNodes.m_pXMLDOMNodeList );
	m_pXMLDOMNodeList = srcXMLDOMNodes.m_pXMLDOMNodeList;
}
// Constructor with node smart pointer:
CXMLDOMNodes::CXMLDOMNodes( MSXML2::IXMLDOMNodeListPtr pXMLDOMNodeList )
{
	//assert( NULL != pXMLDOMNodeList );// allow it empty.
	m_pXMLDOMNodeList = pXMLDOMNodeList;
}

// Deconstructor:
CXMLDOMNodes::~CXMLDOMNodes()
{	
	if( NULL != m_pXMLDOMNodeList )
	{
		m_pXMLDOMNodeList.Release();
	}
	m_pXMLDOMNodeList = NULL;
}


// Overload operator:
// Overload operator " = " for CXMLDOMNodes object:
CXMLDOMNodes& CXMLDOMNodes::operator= ( const CXMLDOMNodes& srcXMLDOMNodes )
{
	if( *this == srcXMLDOMNodes ) { return *this; }
	if( NULL != m_pXMLDOMNodeList )
	{
		m_pXMLDOMNodeList.Release();
		m_pXMLDOMNodeList = NULL;
	}
	m_pXMLDOMNodeList = srcXMLDOMNodes.m_pXMLDOMNodeList;
	return *this;
}
// Overload operator " = " for CXMLDOMNodesPtr smart pointer:
CXMLDOMNodesPtr CXMLDOMNodes::operator= (  CXMLDOMNodesPtr psrcXMLDOMNodes )//const
{
	if( NULL != m_pXMLDOMNodeList )
	{
		m_pXMLDOMNodeList.Release();
		m_pXMLDOMNodeList = NULL;
	}
	m_pXMLDOMNodeList = psrcXMLDOMNodes->m_pXMLDOMNodeList;
	return psrcXMLDOMNodes;
}
// Overload operator "==" for CXMLDOMNodes object:
bool CXMLDOMNodes::operator== ( const CXMLDOMNodes& srcXMLDOMNodes )
{
	return ( m_pXMLDOMNodeList == srcXMLDOMNodes.m_pXMLDOMNodeList );
}
// Overload operator "!=" for CXMLDOMNodes object:
bool CXMLDOMNodes::operator!= ( const CXMLDOMNodes& srcXMLDOMNodes )
{
	return ( m_pXMLDOMNodeList != srcXMLDOMNodes.m_pXMLDOMNodeList );
}
// Overload operator "[]" for Geting Nodes item by index:
CXMLDOMNodePtr CXMLDOMNodes::operator[] ( int nIndex )
{
	return GetItem( nIndex );
}
// Overload operator "[]" for Geting Nodes item by Node Name:
CXMLDOMNodePtr CXMLDOMNodes::operator[] ( const TCHAR* pszNodeName )
{
	return GetItem( pszNodeName );
}


bool operator==( const CXMLDOMNodesPtr& pXMLDOMNodes1, const CXMLDOMNodesPtr& pXMLDOMNodes2 )
{
	//return ( *pXMLDOMNodes1.get() == *pXMLDOMNodes2.get() ); 
	int nCount1 = pXMLDOMNodes1->GetCount();
	int nCount2 = pXMLDOMNodes2->GetCount();
	if ( nCount1 != nCount2 )
	{
		return false;
	}

	bool bEqual = true;
	MSXML2::IXMLDOMNodePtr p1 = NULL;
	MSXML2::IXMLDOMNodePtr p2 = NULL;
	for( int i=0; i<pXMLDOMNodes1->GetCount(); i++ )
	{
		p1 = pXMLDOMNodes1.get()->m_pXMLDOMNodeList->item[i];
		p2 = pXMLDOMNodes2.get()->m_pXMLDOMNodeList->item[i];
		if( p1  != p2 )
		{
			bEqual = false;
			break;
		}
	}

	if( p1 != NULL )
	{
		p1.Release();
		p1 = NULL;
	}
	if( p2 != NULL )
	{
		p2.Release();
		p2 = NULL;
	}
	return bEqual;
}
bool operator!=( const CXMLDOMNodesPtr& pXMLDOMNodes1, const CXMLDOMNodesPtr& pXMLDOMNodes2 )
{
	//return ( *pXMLDOMNodes1.get() != *pXMLDOMNodes2.get() ); 
	int nCount1 = pXMLDOMNodes1->GetCount();
	int nCount2 = pXMLDOMNodes2->GetCount();
	if ( nCount1 != nCount2 )
	{
		return true;
	}

	bool bNotEqual = false;
	MSXML2::IXMLDOMNodePtr p1 = NULL;
	MSXML2::IXMLDOMNodePtr p2 = NULL;
	for( int i=0; i<pXMLDOMNodes1->GetCount(); i++ )
	{
		p1 = pXMLDOMNodes1.get()->m_pXMLDOMNodeList->item[i];
		p2 = pXMLDOMNodes2.get()->m_pXMLDOMNodeList->item[i];
		if( p1  != p2 )
		{
			bNotEqual = true;
			break;
		}
	}

	if( p1 != NULL )
	{
		p1.Release();
		p1 = NULL;
	}
	if( p2 != NULL )
	{
		p2.Release();
		p2 = NULL;
	}
	return bNotEqual;
	
}
bool operator!=( const TCHAR* pNULL, const CXMLDOMNodesPtr& pXMLDOMNodes2 )
{
	assert( NULL == pNULL );
	return( NULL != pXMLDOMNodes2.get()->m_pXMLDOMNodeList );
}
bool operator!=( const CXMLDOMNodesPtr& pXMLDOMNodes1, const TCHAR* pNULL )
{
	assert( NULL == pNULL );
	return( NULL != pXMLDOMNodes1.get()->m_pXMLDOMNodeList );
}
bool operator==( const TCHAR* pNULL, const CXMLDOMNodesPtr& pXMLDOMNodes2 )
{
	assert( NULL == pNULL );
	return( NULL == pXMLDOMNodes2.get()->m_pXMLDOMNodeList );
}
bool operator==( const CXMLDOMNodesPtr& pXMLDOMNodes1, const TCHAR* pNULL )
{
	assert( NULL == pNULL );
	return( NULL == pXMLDOMNodes1.get()->m_pXMLDOMNodeList );
}

int CXMLDOMNodes::GetCount( void )
{

	assert(  NULL != m_pXMLDOMNodeList );
	if( NULL == m_pXMLDOMNodeList )
	{
		return 0;
	}
	
	int elemCount = 0;/* 元素节点数目 */
	int textCount = 0;/* 文本节点数目 */
	MSXML2::DOMNodeType NodeType;
	MSXML2::IXMLDOMNodePtr pXMLDOMNode = NULL;
	try
	{
		long len = m_pXMLDOMNodeList->Getlength();
		for( int i = 0; i < len; i++ )
		{
			pXMLDOMNode = m_pXMLDOMNodeList->Getitem(i);

			NodeType = pXMLDOMNode->GetnodeType();

			// Only count element node.
			if( NodeType == MSXML2::NODE_ELEMENT )
			{
#ifdef _DEBUG
				CString elemName = (LPCTSTR)(_bstr_t)pXMLDOMNode->GetnodeName();
#endif
				elemCount ++;
			}
			if (NodeType == MSXML2::NODE_TEXT)
			{
#ifdef _DEBUG
				CString textValue = (LPCTSTR)(_bstr_t)pXMLDOMNode->GetnodeValue();
#endif
				textCount ++;
			}
		}
	}
	catch (_com_error& e)
	{
		ShowComExecptionWithReason(_T("XML Nodes GetCount() failed."),e);
		return 0;
	}
	catch ( ... )
	{
		ShowComExceptionUnknow(_T("XML Nodes GetCount() failed."));
		return 0;
	}
	
	return elemCount;
}

CXMLDOMNodePtr CXMLDOMNodes::GetItem( int nIndex )
{
	if( NULL == m_pXMLDOMNodeList )
	{
		RETURN_NULL_PTR;
	}
	int nGetCount = GetCount();
	if( nIndex < 0  ||  nIndex >= nGetCount ) 
	{
		RETURN_NULL_PTR;
	}
	
	int elemCount = 0;
	MSXML2::DOMNodeType NodeType;
	MSXML2::IXMLDOMNodePtr pXMLDOMNodesItem = NULL;
	CXMLDOMNodePtr pXMLDOMNode;
	
	try
	{
		long len = m_pXMLDOMNodeList->length;
		for( int i = 0; i < len; i++)
		{
			pXMLDOMNodesItem = m_pXMLDOMNodeList->Getitem(i);

			NodeType = pXMLDOMNodesItem->GetnodeType();
			if( NodeType == MSXML2::NODE_ELEMENT )
			{
				if( elemCount == nIndex )
				{
					pXMLDOMNode.reset(new CXMLDOMNode(pXMLDOMNodesItem));
					break;
				}

				elemCount ++;
			}
		}
	}
	catch (_com_error& e)
	{
		ShowComExecptionWithReason(_T("XML Nodes GetItem() failed."),e);
		RETURN_NULL_PTR;
	}
	catch ( ... )
	{
		ShowComExceptionUnknow(_T("XML Nodes GetItem() failed."));
		RETURN_NULL_PTR;
	}
	
	return pXMLDOMNode;
}
CXMLDOMNodePtr CXMLDOMNodes::GetItem(CString nodeName )
{
	if( NULL == m_pXMLDOMNodeList )
	{
		RETURN_NULL_PTR;
	}
	if( nodeName.IsEmpty() )
	{
		RETURN_NULL_PTR;
	}
	
	MSXML2::DOMNodeType NodeType;
	MSXML2::IXMLDOMNodePtr pXMLDOMNodesItem = NULL;
	
	CXMLDOMNodePtr pXMLDOMNode;
	try
	{
		long len = m_pXMLDOMNodeList->Getlength();
		for( int i = 0; i < len; i++ )
		{
			pXMLDOMNodesItem = m_pXMLDOMNodeList->Getitem(i);
			NodeType = pXMLDOMNodesItem->GetnodeType();
			if (NodeType == MSXML2::NODE_ELEMENT)
			{
				CString itemName = (LPCTSTR)(_bstr_t)pXMLDOMNodesItem->GetnodeName();
				if (itemName == nodeName)
				{
					pXMLDOMNode.reset(new CXMLDOMNode(pXMLDOMNodesItem));
					break;
				}
			}
		}
	}
	catch (_com_error& e)
	{
		ShowComExecptionWithReason(_T("XML Nodes GetItem() failed."),e);
		RETURN_NULL_PTR;
	}
	catch ( ... )
	{
		ShowComExceptionUnknow(_T("XML Nodes GetItem() failed."));
		RETURN_NULL_PTR;
	}
	return pXMLDOMNode;
}

CString CXMLDOMNodes::ShowComExceptionUnknow(CString info)
{
	CString errInfo = info+_T("\n");
	errInfo+=_T("Unknown Exception!");

	HWND hWnd = AfxGetApp()->m_pMainWnd->m_hWnd;
	::MessageBox(hWnd,errInfo,_T("Exception!"),MB_OK|MB_ICONERROR|MB_TOPMOST);
	return errInfo;
}
CString CXMLDOMNodes::ShowComExecptionWithReason(CString info,_com_error& e)
{
	CString errInfo = info+_T("\n");
	CString Code;
	CString CodeMeaning;
	CString Source;
	CString Description;

	Code.Format(_T("Exception Code:%08lx\n"),e.Error());
	CString text = e.ErrorMessage();
	CodeMeaning.Format(_T("Exception Code meaning:%s\n"),text);
	text = (LPCTSTR)(e.Source());
	Source.Format(_T("Exception Source:%s\n"),text);
	text = (LPCTSTR)(e.Description());
	Description.Format(_T("Exception Description:%s\n"),text);

	errInfo+=Code;
	errInfo+=CodeMeaning;
	errInfo+=Source;
	errInfo+=Description;

	HWND hWnd = AfxGetApp()->m_pMainWnd->m_hWnd;
	::MessageBox(hWnd,errInfo,_T("Exception!"),MB_OK|MB_ICONERROR|MB_TOPMOST);
	return errInfo;
}