#include "stdafx.h"
#include "XMLDOMNode.h"

#include "XMLDOMFile.h"
//////////////////////////////////////////////////////////////////////////
#include <assert.h>

//////////////////////////////////////////////////////////////////////////
#define RETURN_NULL_PTR		CXMLDOMNodePtr pRetNode( new CXMLDOMNode( NULL ) );return pRetNode; 
#define RETURN_NULL_LIST	CXMLDOMNodesPtr pRetNodes( new CXMLDOMNodes( NULL ) );return pRetNodes; 
//////////////////////////////////////////////////////////////////////////
//Precompile processor _ For MFC
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
// Default constructor:
CXMLDOMNode::CXMLDOMNode( void )
{
	m_pXMLDOMNode = NULL;
}
// Copy constructor:
CXMLDOMNode::CXMLDOMNode( const CXMLDOMNode& srcXMLDOMNode )
{
	m_pXMLDOMNode = srcXMLDOMNode.m_pXMLDOMNode;
}
// Constructor with node smart pointer:
CXMLDOMNode::CXMLDOMNode( MSXML2::IXMLDOMNodePtr pXMLDOMNode )
{
	m_pXMLDOMNode = pXMLDOMNode;
}
// Overload operator " = " for CXMLDOMNode object:
CXMLDOMNode& CXMLDOMNode::operator= ( const CXMLDOMNode& srcXMLDOMNode )
{
	if( this == &srcXMLDOMNode ) { return *this; }
	m_pXMLDOMNode = srcXMLDOMNode.m_pXMLDOMNode;
	return *this;
}

// Overload operator "==" for CXMLDOMNode object:
bool CXMLDOMNode::operator== ( const CXMLDOMNode& srcXMLDOMNode )
{
	if (m_pXMLDOMNode != NULL && srcXMLDOMNode.m_pXMLDOMNode != NULL)
	{
		return ( m_pXMLDOMNode == srcXMLDOMNode.m_pXMLDOMNode );
	} 
	else if(m_pXMLDOMNode == NULL && srcXMLDOMNode.m_pXMLDOMNode == NULL)
	{
		return true;
	}
	else 
	{
		return false;
	}
}
// Overload operator "!=" for CXMLDOMNode object:
bool CXMLDOMNode::operator!= ( const CXMLDOMNode& srcXMLDOMNode )
{
	if (m_pXMLDOMNode != NULL && srcXMLDOMNode.m_pXMLDOMNode != NULL)
	{
		return ( m_pXMLDOMNode != srcXMLDOMNode.m_pXMLDOMNode );
	} 
	else if(m_pXMLDOMNode == NULL && srcXMLDOMNode.m_pXMLDOMNode == NULL)
	{
		return false;
	}
	else 
	{
		return true;
	}
}
// Friend operator "==" for CXMLDOMNodePtr:
bool operator==  ( const CXMLDOMNodePtr& pXMLDOMNode1, const CXMLDOMNodePtr& pXMLDOMNode2 )
{
	if (pXMLDOMNode1.get() != 0 && pXMLDOMNode2.get() != 0)
	{
		return ( *pXMLDOMNode1.get() == *pXMLDOMNode2.get() ); 
	}
	else if(pXMLDOMNode1.get() == 0 && pXMLDOMNode2.get() == 0)
	{
		return true;
	}
	else 
	{
		return false;
	}
}
// Friend operator "!=" for CXMLDOMNodePtr:
bool operator!=  ( const CXMLDOMNodePtr& pXMLDOMNode1, const CXMLDOMNodePtr& pXMLDOMNode2 )
{
	if (pXMLDOMNode1.get() != 0 && pXMLDOMNode2.get() != 0)
	{
		return ( *pXMLDOMNode1.get() != *pXMLDOMNode2.get() ); 
	}
	else if(pXMLDOMNode1.get() == 0 && pXMLDOMNode2.get() == 0)
	{
		return false;
	}
	else
	{
		return true;
	}
}

// Friend operator "!=" for Only compare with NULL.
bool operator!= (  const CXMLDOMNodePtr& pXMLDOMNode2, const TCHAR* pNULL )
{
	assert( NULL == pNULL);
	if (pXMLDOMNode2.get() != 0)
	{
		return( NULL != pXMLDOMNode2.get()->m_pXMLDOMNode );
	}
	else
	{
		return false;
	}
}
// Friend operator "!=" for Only compare with NULL.
bool operator!= ( const TCHAR* pNULL, const CXMLDOMNodePtr& pXMLDOMNode2 )
{
	assert( NULL == pNULL); 
	if (pXMLDOMNode2.get() != 0)
	{
		return(  NULL != pXMLDOMNode2.get()->m_pXMLDOMNode );
	}
	else
	{
		return true;
	}
}
// Friend operator "==" for Only compare with NULL.
bool operator== (  const CXMLDOMNodePtr& pXMLDOMNode2, const TCHAR* pNULL )
{
	assert( NULL == pNULL);
	if (pXMLDOMNode2.get() != 0)
	{
		return( NULL == pXMLDOMNode2.get()->m_pXMLDOMNode );
	}
	else
	{
		return true;
	}
}
// Friend operator "==" for Only compare with NULL.
bool operator== ( const TCHAR* pNULL, const CXMLDOMNodePtr& pXMLDOMNode2 )
{
	assert( NULL == pNULL);
	if (pXMLDOMNode2.get() != 0)
	{
		return( NULL == pXMLDOMNode2.get()->m_pXMLDOMNode );
	}
	else
	{
		return true;
	}
}

// Deconstructor:
CXMLDOMNode::~CXMLDOMNode()
{	
	if( NULL != m_pXMLDOMNode )
	{
		m_pXMLDOMNode.Release();
	}
	m_pXMLDOMNode = NULL;
}

BOOL CXMLDOMNode::HasChildNodes( void )
{
	if( NULL == m_pXMLDOMNode )
	{
		return FALSE;
	}
	BOOL bHasChildNodes = FALSE;
	try
	{
		MSXML2::IXMLDOMNodeListPtr pXMLDOMChildList = 
			m_pXMLDOMNode->GetchildNodes();
		ASSERT(pXMLDOMChildList != NULL);
		long childCount = pXMLDOMChildList->Getlength();
		for( long i = 0; i < childCount; i++ )
		{
			MSXML2::IXMLDOMNodePtr pXMLDOMChild = 
				pXMLDOMChildList->Getitem(i);
			MSXML2::DOMNodeType nodeType = 
				pXMLDOMChild->GetnodeType();
			if( nodeType == MSXML2::NODE_ELEMENT )
			{
				bHasChildNodes = TRUE;
				break;
			}
		}
	}
	catch (_com_error& e)
	{
		ShowComExceptionWithReason(_T("XML Node HasChildNodes() failed."),e);
		return FALSE;
	}
	catch (...)
	{
		ShowComExceptionUnknow(_T("XML Node HasChildNodes() failed."));
		return FALSE;
	}
	return bHasChildNodes;
}

/************************************************************************
*																		
*  NAME:			IsChildNodeExist                                                 
*  FUNCTION:		Judge specific child node of current node whether existing.		                                  
*  PARAMETER:															
*					Input:  szChildNodeName - Child Node name to be searched.  			    	
*					Output: void													
*  RETURN:	       	
*					true  - Exist
*					false - Not Exist.
*  CODER:			Songyongfeng													
*  DATE:			2007.11.30													   
*																								
************************************************************************/
bool CXMLDOMNode::IsChildNodeExist( const TCHAR* szChildNodeName )
{
	assert( NULL != m_pXMLDOMNode );
	if( NULL == m_pXMLDOMNode )
	{
		return false;
	}

	if( ! HasChildNodes() )
	{
		return false;
	}


	MSXML2::IXMLDOMNodePtr		pXMLDOMNode = NULL;
	MSXML2::IXMLDOMNodeListPtr	pXMLDOMNodeList = NULL;
	bool bIsExist = false;
	HRESULT hr ;
	try
	{
		/*HRESULT*/ hr = m_pXMLDOMNode->get_childNodes( &pXMLDOMNodeList );
		assert( SUCCEEDED(hr) );
		if( FAILED( hr ) )
		{
			pXMLDOMNodeList = NULL;
			return false;
		}

		//bool bIsExist = false;
		for( int i = 0; i < pXMLDOMNodeList->length; i++ )
		{
			pXMLDOMNode = pXMLDOMNodeList->item[i];
			TCHAR szCurrentChildNodeName[MAX_PATH];
			memset( szCurrentChildNodeName, 0, sizeof( szCurrentChildNodeName ) );
			_tcscpy( szCurrentChildNodeName, ( TCHAR* )( _bstr_t )pXMLDOMNode->GetnodeName() );
			assert( NULL != szCurrentChildNodeName );
			if( NULL == szCurrentChildNodeName )
			{
				if( NULL != pXMLDOMNode )
				{
					pXMLDOMNode.Release();
					pXMLDOMNode = NULL;
				}
				continue;
			}

#ifdef _DEBUG
			TCHAR szDebugString[MAX_PATH];
			memset( szDebugString, 0, sizeof( szDebugString ) );
			_stprintf( szDebugString, TEXT("\ncurrent node is: %s( =? %s )\n "), ( TCHAR* )( _bstr_t )pXMLDOMNode->GetnodeName(), szChildNodeName );
			OutputDebugString( szDebugString );
#endif	
			if( 0 == _tcscmp( szCurrentChildNodeName, szChildNodeName ) ) // Exist
			{
				bIsExist = true;
				if( NULL != pXMLDOMNode )
				{
					pXMLDOMNode.Release();
					pXMLDOMNode = NULL;
				}
				break;
			}
			if( NULL != pXMLDOMNode )
			{
				pXMLDOMNode.Release();
				pXMLDOMNode = NULL;
			}
		}

	}
	catch ( ... )
	{
#ifdef _DEBUG
		MessageBox( NULL, TEXT("UnKnown error occured!"), TEXT("Error"), MB_OK|MB_ICONSTOP );
#endif

		if( NULL != pXMLDOMNode )
		{
			pXMLDOMNode.Release();
			pXMLDOMNode = NULL;
		}
		if( NULL != pXMLDOMNodeList )
		{
			pXMLDOMNodeList.Release();
			pXMLDOMNodeList = NULL;
		}

		assert( false );
		return false;
	}

	if( NULL != pXMLDOMNode )
	{
		pXMLDOMNode.Release();
		pXMLDOMNode = NULL;
	}
	if( NULL != pXMLDOMNodeList )
	{
		pXMLDOMNodeList.Release();
		pXMLDOMNodeList = NULL;
	}

	return bIsExist;
}

/************************************************************************
*																		
*  NAME:			IsChildNodeExist                                                 
*  FUNCTION:		Judge specific child node of current node whether 
*					existing and return the pointer to the found node.		                                  
*  PARAMETER:															
*					Input:  szChildNodeName - Child Node name to be searched.  			    	
*					Output: pGetXMLDOMNode  - Get pointer to the found node.														
*  RETURN:	       	
*					true  - Exist
*					false - Not Exist.
*  CODER:			Songyongfeng													
*  DATE:			2007.12.11													   
*																								
************************************************************************/
bool CXMLDOMNode::IsChildNodeExist( const TCHAR* szChildNodeName, CXMLDOMNodePtr& pGetXMLDOMNode OUT )
{
	assert( NULL != m_pXMLDOMNode );
	if( NULL == m_pXMLDOMNode )
	{
		return false;
	}

	if( ! HasChildNodes() )
	{
		return false;
	}


	MSXML2::IXMLDOMNodePtr		pXMLDOMNode = NULL;
	MSXML2::IXMLDOMNodeListPtr	pXMLDOMNodeList = NULL;
	bool bIsExist = false;
	HRESULT hr;
	try
	{
		/*HRESULT */hr = m_pXMLDOMNode->get_childNodes( &pXMLDOMNodeList );
		assert( SUCCEEDED(hr) );
		if( FAILED( hr ) )
		{
			pXMLDOMNodeList = NULL;
			return false;
		}

		//bool bIsExist = false;
		for( int i = 0; i < pXMLDOMNodeList->length; i++ )
		{
			pXMLDOMNode = pXMLDOMNodeList->item[i];
			TCHAR szCurrentChildNodeName[MAX_PATH];
			memset( szCurrentChildNodeName, 0, sizeof( szCurrentChildNodeName ) );
			_tcscpy( szCurrentChildNodeName, ( TCHAR* )( _bstr_t )pXMLDOMNode->GetnodeName() );
			assert( NULL != szCurrentChildNodeName );
			if( NULL == szCurrentChildNodeName )
			{
				if( NULL != pXMLDOMNode )
				{
					pXMLDOMNode.Release();
					pXMLDOMNode = NULL;
				}
				continue;
			}

#ifdef _DEBUG
			TCHAR szDebugString[MAX_PATH];
			memset( szDebugString, 0, sizeof( szDebugString ) );
			_stprintf( szDebugString, TEXT("\ncurrent node is: %s( =? %s )\n "), ( TCHAR* )( _bstr_t )pXMLDOMNode->GetnodeName(), szChildNodeName );
			OutputDebugString( szDebugString );
#endif	
			// Exist.
			if( 0 == _tcscmp( szCurrentChildNodeName, szChildNodeName ) ) 
			{
				CXMLDOMNodePtr pTempXMLDOMNode( new CXMLDOMNode() );
				assert( NULL != pTempXMLDOMNode.get() );
				if( NULL != pTempXMLDOMNode.get() )
				{
					pTempXMLDOMNode->m_pXMLDOMNode = pXMLDOMNode;
				}
				pGetXMLDOMNode = pTempXMLDOMNode;


				if( NULL != pXMLDOMNode )
				{
					pXMLDOMNode.Release();
					pXMLDOMNode = NULL;
				}

				bIsExist = true;

				break;
			}
			if( NULL != pXMLDOMNode )
			{
				pXMLDOMNode.Release();
				pXMLDOMNode = NULL;
			}
		}

	}
	catch ( ... )
	{
#ifdef _DEBUG
		MessageBox( NULL, TEXT("UnKnown error occured!"), TEXT("Error"), MB_OK|MB_ICONSTOP );
#endif

		if( NULL != pXMLDOMNode )
		{
			pXMLDOMNode.Release();
			pXMLDOMNode = NULL;
		}
		if( NULL != pXMLDOMNodeList )
		{
			pXMLDOMNodeList.Release();
			pXMLDOMNodeList = NULL;
		}
		assert( false );
		return false;	
	}

	if( NULL != pXMLDOMNode )
	{
		pXMLDOMNode.Release();
		pXMLDOMNode = NULL;
	}
	if( NULL != pXMLDOMNodeList )
	{
		pXMLDOMNodeList.Release();
		pXMLDOMNodeList = NULL;
	}

	return bIsExist;
}


/************************************************************************
*																		
*  NAME:			IsChildNodeExist                                                 
*  FUNCTION:		Judge specific child node with specific value of 
*					current node whether existing.		                                  
*  PARAMETER:															
*					Input:  szChildNodeName  - Child node name to be searched. 
*							szChildNodeValue - Child Node Value to be searched.
*					Output: void													
*  RETURN:	       	
*					true  - Exist
*					false - Not Exist.
*  CODER:			Songyongfeng													
*  DATE:			2007.12.10													   
*																								
************************************************************************/
bool CXMLDOMNode::IsChildNodeExist( const TCHAR* szChildNodeName, const TCHAR* szChildNodeValue )
{
	assert( NULL != m_pXMLDOMNode );
	if( NULL == m_pXMLDOMNode )
	{
		return false;
	}

	// Have no child return.
	if( ! HasChildNodes() )
	{
		return false;
	}

	// Get child node list.
	MSXML2::IXMLDOMNodePtr		pXMLDOMNode = NULL;
	MSXML2::IXMLDOMNodeListPtr	pXMLDOMNodeList = NULL;
	bool bIsExist = false;
	HRESULT hr;
	try
	{
		/*HRESULT*/ hr = m_pXMLDOMNode->get_childNodes( &pXMLDOMNodeList );
		assert( SUCCEEDED(hr) );
		if( FAILED( hr ) )
		{
			pXMLDOMNodeList = NULL;
			return false;
		}

		// Travel the child node list and search for specific Node value.
		//bool bIsExist = false;
		for( int i = 0; i < pXMLDOMNodeList->length; i++ )
		{
			pXMLDOMNode = pXMLDOMNodeList->item[i] ;

			// Get node name.
			TCHAR szCurrentChildNodeName[MAX_PATH];
			memset( szCurrentChildNodeName, 0, sizeof( szCurrentChildNodeName ) );
			_tcscpy( szCurrentChildNodeName, ( TCHAR* )( _bstr_t )pXMLDOMNode->GetnodeName() );
			assert( NULL != szCurrentChildNodeName );
			if( NULL == szCurrentChildNodeName )
			{
				if( NULL != pXMLDOMNode )
				{
					pXMLDOMNode.Release();
					pXMLDOMNode = NULL;
				}
				continue;
			}

			// Get node value.
			TCHAR szCurrentChildNodeValue[MAX_PATH];
			memset( szCurrentChildNodeValue, 0, sizeof( szCurrentChildNodeValue ) );
			// 		VARIANT var;
			// 		VariantInit(&var);
			// 		pXMLDOMNode->get_nodeValue( &var );
			// 		_variant_t vt = pXMLDOMNode->GetnodeValue( );
			// 		_tcscpy( szCurrentChildNodeValue, _VariantToString( vt ) );

			BSTR bstr = NULL;
			HRESULT hr = pXMLDOMNode->get_text( &bstr );
			assert( SUCCEEDED(hr) );
			if( FAILED ( hr ) )
			{
				continue;
			}
			_tcscpy( szCurrentChildNodeValue, ( TCHAR* )( _bstr_t) bstr);//_VariantToString(var )
			if( NULL != bstr )
			{
				SysFreeString(bstr);
				bstr = NULL;
			}
			//VariantClear(&var);
			if( NULL == szCurrentChildNodeValue )
			{
				if( NULL != pXMLDOMNode )
				{
					pXMLDOMNode.Release();
					pXMLDOMNode = NULL;
				}
				continue;
			}

#ifdef _DEBUG
			TCHAR szDebugString[MAX_PATH];
			memset( szDebugString, 0, sizeof( szDebugString ) );
			_stprintf( szDebugString, TEXT("\ncurrent node is: %s( =? %s ) --node value is: %s( =? %s) \n " ), szCurrentChildNodeName, szChildNodeName, szCurrentChildNodeValue, szChildNodeValue );
			OutputDebugString( szDebugString );
#endif	
			// Specific node name Exist.
			if( 0 == _tcscmp( szCurrentChildNodeName, szChildNodeName ) ) 
			{
				// Judge whether Node value equal to the specific Node value.
				if( 0 == _tcscmp( szCurrentChildNodeValue, szChildNodeValue ) )
				{
#ifdef _DEBUG
					OutputDebugString( TEXT("\n<Exist>\n") );
#endif
					bIsExist = true;
					break;
				}

			} // End if( 0 == _tcscmp( szCurrentChildNodeName, szChildNodeName ) ) 

			if ( NULL != pXMLDOMNode )
			{
				pXMLDOMNode.Release();
				pXMLDOMNode = NULL;
			}
		} // End for( int i = 0; i < pXMLDOMNodeList->length; i++ )


	}
	catch ( ... )
	{
#ifdef _DEBUG
		MessageBox( NULL, TEXT("UnKnown error occured!"), TEXT("Error"), MB_OK|MB_ICONSTOP );
#endif

		if ( NULL != pXMLDOMNode )
		{
			pXMLDOMNode.Release();
			pXMLDOMNode = NULL;
		}
		if ( NULL != pXMLDOMNodeList )
		{
			pXMLDOMNodeList.Release();
			pXMLDOMNodeList = NULL;
		}
		assert( false );
		return false;	
	}


	if ( NULL != pXMLDOMNode )
	{
		pXMLDOMNode.Release();
		pXMLDOMNode = NULL;
	}
	if ( NULL != pXMLDOMNodeList )
	{
		pXMLDOMNodeList.Release();
		pXMLDOMNodeList = NULL;
	}

	return bIsExist;	
}

/************************************************************************
*																		
*  NAME:			IsChildNodeExist                                                 
*  FUNCTION:		Judge specific child node with specific value of current
*					node whether existing return the pointer to the found node.		                                  
*  PARAMETER:															
*					Input:  szChildNodeName  - Child node name to be searched. 
*							szChildNodeValue - Child Node Value to be searched.
*					Output: pGetXMLDOMNode   - Get pointer to the found node.													
*  RETURN:	       	
*					true  - Exist
*					false - Not Exist.
*  CODER:			Songyongfeng													
*  DATE:			2007.12.11													   
*																								
************************************************************************/
bool CXMLDOMNode::IsChildNodeExist( const TCHAR* szChildNodeName, const TCHAR* szChildNodeValue, CXMLDOMNodePtr& pGetXMLDOMNode OUT )
{

	assert( NULL != m_pXMLDOMNode );
	if( NULL == m_pXMLDOMNode )
	{
		return false;
	}

	// Have no child return.
	if( ! HasChildNodes() )
	{
		return false;
	}

	// Get child node list.
	MSXML2::IXMLDOMNodePtr		pXMLDOMNode = NULL;
	MSXML2::IXMLDOMNodeListPtr	pXMLDOMNodeList = NULL;
	bool bIsExist = false;
	HRESULT hr;
	try
	{
		/*HRESULT*/ hr = m_pXMLDOMNode->get_childNodes( &pXMLDOMNodeList );
		assert( SUCCEEDED(hr) );
		if( FAILED( hr ) )
		{
			pXMLDOMNodeList = NULL;
			return false;
		}

		// Travel the child node list and search for specific Node value.
		//bool bIsExist = false;
		for( int i = 0; i < pXMLDOMNodeList->length; i++ )
		{
			pXMLDOMNode = pXMLDOMNodeList->item[i] ;

			// Get node name.
			TCHAR szCurrentChildNodeName[MAX_PATH];
			memset( szCurrentChildNodeName, 0, sizeof( szCurrentChildNodeName ) );
			_tcscpy( szCurrentChildNodeName, ( TCHAR* )( _bstr_t )pXMLDOMNode->GetnodeName() );
			assert( NULL != szCurrentChildNodeName );
			if( NULL == szCurrentChildNodeName )
			{
				if( NULL != pXMLDOMNode )
				{
					pXMLDOMNode.Release();
					pXMLDOMNode = NULL;
				}
				continue;
			}

			// Get node value.
			TCHAR szCurrentChildNodeValue[MAX_PATH];
			memset( szCurrentChildNodeValue, 0, sizeof( szCurrentChildNodeValue ) );
			BSTR bstr = NULL;
			HRESULT hr = pXMLDOMNode->get_text( &bstr );
			assert( SUCCEEDED(hr) );
			if( FAILED ( hr ) )
			{
				continue;
			}
			_tcscpy( szCurrentChildNodeValue, ( TCHAR* )( _bstr_t) bstr);
			if( NULL != bstr )
			{
				SysFreeString(bstr);
				bstr = NULL;
			}
			if( NULL == szCurrentChildNodeValue )
			{
				if( NULL != pXMLDOMNode )
				{
					pXMLDOMNode.Release();
					pXMLDOMNode = NULL;
				}
				continue;
			}

#ifdef _DEBUG
			TCHAR szDebugString[MAX_PATH];
			memset( szDebugString, 0, sizeof( szDebugString ) );
			_stprintf( szDebugString, TEXT( "\ncurrent node is: %s( =? %s ) --node value is: %s( =? %s) \n " ), szCurrentChildNodeName, szChildNodeName, szCurrentChildNodeValue, szChildNodeValue );
			OutputDebugString( szDebugString );
#endif	
			// Specific node name Exist.
			if( 0 == _tcscmp( szCurrentChildNodeName, szChildNodeName ) ) 
			{
				// Judge whether Node value equal to the specific Node value.
				if( 0 == _tcscmp( szCurrentChildNodeValue, szChildNodeValue ) )
				{
#ifdef _DEBUG
					OutputDebugString( TEXT("\n<Exist>\n") );
#endif
					//bIsExist = true;
					//CXMLDOMNodePtr pXMLDOMNode( new( CXMLDOMNode ) );
					//assert( NULL != pXMLDOMNode.get() );
					//pXMLDOMNode->m_pXMLDOMNode = m_pXMLDOMNode;
					if( NULL != pGetXMLDOMNode.get() )
					{
						pGetXMLDOMNode.reset();
					}

					CXMLDOMNodePtr pTempXMLDOMNode( new CXMLDOMNode() );
					assert( NULL != pTempXMLDOMNode.get() );
					if( NULL != pTempXMLDOMNode.get() )
					{
						pTempXMLDOMNode->m_pXMLDOMNode = pXMLDOMNode;
					}
					pGetXMLDOMNode = pTempXMLDOMNode;

					bIsExist = true;
					break;
				}

			} // End if( 0 == _tcscmp( szCurrentChildNodeName, szChildNodeName ) ) 

			if ( NULL != pXMLDOMNode )
			{
				pXMLDOMNode.Release();
				pXMLDOMNode = NULL;
			}
		} // End for( int i = 0; i < pXMLDOMNodeList->length; i++ )

	}
	catch ( ... )
	{
#ifdef _DEBUG
		MessageBox( NULL, TEXT("UnKnown error occured!"), TEXT("Error"), MB_OK|MB_ICONSTOP );
#endif

		if ( NULL != pXMLDOMNode )
		{
			pXMLDOMNode.Release();
			pXMLDOMNode = NULL;
		}
		if ( NULL != pXMLDOMNodeList )
		{
			pXMLDOMNodeList.Release();
			pXMLDOMNodeList = NULL;
		}

		assert( false );
		return false;	

	}

	if ( NULL != pXMLDOMNode )
	{
		pXMLDOMNode.Release();
		pXMLDOMNode = NULL;
	}
	if ( NULL != pXMLDOMNodeList )
	{
		pXMLDOMNodeList.Release();
		pXMLDOMNodeList = NULL;
	}

	return bIsExist;	
}


/************************************************************************
*																		
*  NAME:			IsChildNodeExist                                                 
*  FUNCTION:		Judge specific child node with specific INTEGER value of current
*					node whether existing return the pointer to the found node.		                                  
*  PARAMETER:															
*					Input:  szChildNodeName  - Child node name to be searched. 
*							nChildNodeValue  - Child Node Value to be searched.
*					Output: pGetXMLDOMNode   - Get pointer to the found node.													
*  RETURN:	       	
*					true  - Exist
*					false - Not Exist.
*  CODER:			Songyongfeng													
*  DATE:			2007.12.11													   
*																								
************************************************************************/
bool CXMLDOMNode::IsChildNodeExist( const TCHAR* szChildNodeName, const int& nChildNodeValue, CXMLDOMNodePtr& pGetXMLDOMNode OUT )
{
	assert( NULL != m_pXMLDOMNode );
	if( NULL == m_pXMLDOMNode )
	{
		return false;
	}

	const int nBase = 10; // Converting base value. 
	TCHAR szChildNodeValue[ MAX_PATH ];
	memset( szChildNodeValue, 0, sizeof( szChildNodeValue ) );
	_itot( nChildNodeValue, szChildNodeValue, nBase );
	assert( NULL != szChildNodeValue );
	if( NULL == szChildNodeValue )
	{
		return NULL;
	}

	return IsChildNodeExist(  szChildNodeName, szChildNodeValue, pGetXMLDOMNode  );

}


/************************************************************************
*																		
*  NAME:			IsChildNodeExist                                                 
*  FUNCTION:		Judge specific child node with STRING attribute value 
*					of current node whether existing.		                                  
*  PARAMETER:															
*					Input:  szChildNodeName  - Child node name to be searched. 
*							szAttributeName  - Attribute name of child node to be searched.
*							szAttributeValue - Attribute Value of Child Node to be searched.			    	
*					Output: void													
*  RETURN:	       	
*					true  - Exist
*					false - Not Exist.
*  CODER:			Songyongfeng													
*  DATE:			2007.11.30													   
*																								
************************************************************************/
bool CXMLDOMNode::IsChildNodeExist( const TCHAR* szChildNodeName, const TCHAR* szAttributeName, const TCHAR* szAttributeValue )
{
	assert( NULL != m_pXMLDOMNode );
	if( NULL == m_pXMLDOMNode )
	{
		return false;
	}

	// Have no child return.
	if( ! HasChildNodes() )
	{
		return false;
	}

	// Get child node list.
	MSXML2::IXMLDOMNodePtr		pXMLDOMNode = NULL;
	MSXML2::IXMLDOMNodeListPtr	pXMLDOMNodeList = NULL;
	bool bIsExist = false;
	HRESULT hr;
	try
	{
		/*HRESULT*/ hr = m_pXMLDOMNode->get_childNodes( &pXMLDOMNodeList );
		assert( SUCCEEDED(hr) );
		if( FAILED( hr ) )
		{
			pXMLDOMNodeList = NULL;
			return false;
		}

		// Travel the child node list and search for specific attribute value.
		//bool bIsExist = false;
		for( int i = 0; i < pXMLDOMNodeList->length; i++ )
		{
			pXMLDOMNode = pXMLDOMNodeList->item[i];

			// Get node name.
			TCHAR szCurrentChildNodeName[MAX_PATH];
			memset( szCurrentChildNodeName, 0, sizeof( szCurrentChildNodeName ) );
			_tcscpy( szCurrentChildNodeName, ( TCHAR* )( _bstr_t )pXMLDOMNode->GetnodeName() );
			assert( NULL != szCurrentChildNodeName );
			if( NULL == szCurrentChildNodeName )
			{
				if( NULL != pXMLDOMNode )
				{
					pXMLDOMNode.Release();
					pXMLDOMNode = NULL;
				}
				continue;
			}
#ifdef _DEBUG
			TCHAR szDebugString[MAX_PATH];
			memset( szDebugString, 0, sizeof( szDebugString ) );
			_stprintf( szDebugString, TEXT( "\ncurrent node is: %s( =? %s )\n " ), ( TCHAR* )( _bstr_t )pXMLDOMNode->GetnodeName(), szChildNodeName );
			OutputDebugString( szDebugString );
#endif	
			// Specific node name Exist.
			if( 0 == _tcscmp( szCurrentChildNodeName, szChildNodeName ) ) 
			{
				// Judge whether attribute value equal to the specific attribute value.
				//////////////////////////////////////////////////////////////////////////
				MSXML2::IXMLDOMElementPtr pXMLDOMElement = NULL;
				pXMLDOMElement = static_cast< MSXML2::IXMLDOMElementPtr >( pXMLDOMNode );
				TCHAR szCurrentChildAttributeValue[MAX_PATH];
				memset( szCurrentChildAttributeValue, 0, sizeof( szCurrentChildAttributeValue ) );
				_variant_t vt = pXMLDOMElement->getAttribute( _bstr_t( szAttributeName ) );
				_tcscpy( szCurrentChildAttributeValue, _VariantToString( vt ) );
				assert( NULL != szCurrentChildAttributeValue );
				if( NULL == szCurrentChildAttributeValue )
				{
					if( NULL != pXMLDOMNode )
					{
						pXMLDOMNode.Release();
						pXMLDOMNode = NULL;
					}
					continue;
				}
				// Attribute Exist.
				if( _tcslen( szCurrentChildAttributeValue ) > 0 )
				{
#ifdef _DEBUG
					TCHAR szDebugString[MAX_PATH];
					memset( szDebugString, 0, sizeof( szDebugString ) );
					_stprintf( szDebugString, TEXT( "\ncurrent attibute is: %s( =? %s )\n " ), szCurrentChildAttributeValue, szAttributeValue );
					OutputDebugString( szDebugString );
#endif			
					// Exist and equal.
					if( 0 == _tcscmp( szCurrentChildAttributeValue, szAttributeValue ) )
					{
						bIsExist = true;
						if( NULL != pXMLDOMElement )
						{
							pXMLDOMElement.Release();
							pXMLDOMElement = NULL;
						}
#ifdef _DEBUG
						OutputDebugString( TEXT( "\n<Exist>\n") );
#endif
						break;
					}
				}

				if( NULL != pXMLDOMElement )
				{
					pXMLDOMElement.Release();
					pXMLDOMElement = NULL;
				}
				//////////////////////////////////////////////////////////////////////////


			} // End if( 0 == _tcscmp( szCurrentChildNodeName, szChildNodeName ) ) 

			if ( NULL != pXMLDOMNode )
			{
				pXMLDOMNode.Release();
				pXMLDOMNode = NULL;
			}
		} // End for( int i = 0; i < pXMLDOMNodeList->length; i++ )

	}
	catch ( ... )
	{
#ifdef _DEBUG
		MessageBox( NULL, TEXT("UnKnown error occured!"), TEXT("Error"), MB_OK|MB_ICONSTOP );
#endif

		if ( NULL != pXMLDOMNode )
		{
			pXMLDOMNode.Release();
			pXMLDOMNode = NULL;
		}
		if ( NULL != pXMLDOMNodeList )
		{
			pXMLDOMNodeList.Release();
			pXMLDOMNodeList = NULL;
		}

		assert( false );
		return false;	
	}

	if ( NULL != pXMLDOMNode )
	{
		pXMLDOMNode.Release();
		pXMLDOMNode = NULL;
	}
	if ( NULL != pXMLDOMNodeList )
	{
		pXMLDOMNodeList.Release();
		pXMLDOMNodeList = NULL;
	}

	return bIsExist;
}


/************************************************************************
*																		
*  NAME:			IsChildNodeExist                                                 
*  FUNCTION:		Judge specific child node with STRING attribute value 
*					of current node whether existing and return the pointer
*					to the found node.		                                  
*  PARAMETER:															
*					Input:  szChildNodeName  - Child node name to be searched. 
*							szAttributeName  - Attribute name of child node to be searched.
*							szAttributeValue - Attribute Value of Child Node to be searched.			    	
*					Output: pGetXMLDOMNode   - Get pointer to the found node.														
*  RETURN:	       	
*					true  - Exist
*					false - Not Exist.
*  CODER:			Songyongfeng													
*  DATE:			2007.12.11													   
*																								
************************************************************************/
bool CXMLDOMNode::IsChildNodeExist( const TCHAR* szChildNodeName, const TCHAR* szAttributeName, const TCHAR* szAttributeValue, CXMLDOMNodePtr& pGetXMLDOMNode OUT )
{
	assert( NULL != m_pXMLDOMNode );
	if( NULL == m_pXMLDOMNode )
	{
		return false;
	}

	// Have no child return.
	if( ! HasChildNodes() )
	{
		return false;
	}

	// Get child node list.
	MSXML2::IXMLDOMNodePtr		pXMLDOMNode = NULL;
	MSXML2::IXMLDOMNodeListPtr	pXMLDOMNodeList = NULL;
	bool bIsExist = false;
	HRESULT hr;
	try
	{
		/*HRESULT*/ hr = m_pXMLDOMNode->get_childNodes( &pXMLDOMNodeList );
		assert( SUCCEEDED(hr) );
		if( FAILED( hr ) )
		{
			pXMLDOMNodeList = NULL;
			return false;
		}

		// Travel the child node list and search for specific attribute value.
		//bool bIsExist = false;
		for( int i = 0; i < pXMLDOMNodeList->length; i++ )
		{
			pXMLDOMNode = pXMLDOMNodeList->item[i];

			// Get node name.
			TCHAR szCurrentChildNodeName[MAX_PATH];
			memset( szCurrentChildNodeName, 0, sizeof( szCurrentChildNodeName ) );
			_tcscpy( szCurrentChildNodeName, ( TCHAR* )( _bstr_t )pXMLDOMNode->GetnodeName() );
			assert( NULL != szCurrentChildNodeName );
			if( NULL == szCurrentChildNodeName )
			{
				if( NULL != pXMLDOMNode )
				{
					pXMLDOMNode.Release();
					pXMLDOMNode = NULL;
				}
				continue;
			}
#ifdef _DEBUG
			TCHAR szDebugString[MAX_PATH];
			memset( szDebugString, 0, sizeof( szDebugString ) );
			_stprintf( szDebugString, TEXT( "\ncurrent node is: %s( =? %s )\n " ), ( TCHAR* )( _bstr_t )pXMLDOMNode->GetnodeName(), szChildNodeName );
			OutputDebugString( szDebugString );
#endif	
			// Specific node name Exist.
			if( 0 == _tcscmp( szCurrentChildNodeName, szChildNodeName ) ) 
			{
				// Judge whether attribute value equal to the specific attribute value.
				//////////////////////////////////////////////////////////////////////////
				MSXML2::IXMLDOMElementPtr pXMLDOMElement = NULL;
				pXMLDOMElement = static_cast< MSXML2::IXMLDOMElementPtr >( pXMLDOMNode );
				TCHAR szCurrentChildAttributeValue[MAX_PATH];
				memset( szCurrentChildAttributeValue, 0, sizeof( szCurrentChildAttributeValue ) );
				_variant_t vt = pXMLDOMElement->getAttribute( _bstr_t( szAttributeName ) );
				_tcscpy( szCurrentChildAttributeValue, _VariantToString( vt ) );
				assert( NULL != szCurrentChildAttributeValue );
				if( NULL == szCurrentChildAttributeValue )
				{
					if( NULL != pXMLDOMNode )
					{
						pXMLDOMNode.Release();
						pXMLDOMNode = NULL;
					}
					continue;
				}
				// Attribute Exist.
				if( _tcslen( szCurrentChildAttributeValue ) > 0 )
				{
#ifdef _DEBUG
					TCHAR szDebugString[MAX_PATH];
					memset( szDebugString, 0, sizeof( szDebugString ) );
					_stprintf( szDebugString, TEXT( "\ncurrent attibute is: %s( =? %s )\n " ), szCurrentChildAttributeValue, szAttributeValue );
					OutputDebugString( szDebugString );
#endif			
					// Exist and equal.
					if( 0 == _tcscmp( szCurrentChildAttributeValue, szAttributeValue ) )
					{
#ifdef _DEBUG
						OutputDebugString( TEXT( "\n<Exist>\n" ) );
#endif	
						CXMLDOMNodePtr pTempXMLDOMNode( new CXMLDOMNode() );
						assert( NULL != pTempXMLDOMNode.get() );
						if( NULL != pTempXMLDOMNode.get() )
						{
							pTempXMLDOMNode->m_pXMLDOMNode = pXMLDOMNode;
						}
						pGetXMLDOMNode = pTempXMLDOMNode;

						if( NULL != pXMLDOMElement )
						{
							pXMLDOMElement.Release();
							pXMLDOMElement = NULL;
						}

						bIsExist = true;

						break;
					}
				}

				if( NULL != pXMLDOMElement )
				{
					pXMLDOMElement.Release();
					pXMLDOMElement = NULL;
				}
				//////////////////////////////////////////////////////////////////////////


			} // End if( 0 == _tcscmp( szCurrentChildNodeName, szChildNodeName ) ) 

			if ( NULL != pXMLDOMNode )
			{
				pXMLDOMNode.Release();
				pXMLDOMNode = NULL;
			}
		} // End for( int i = 0; i < pXMLDOMNodeList->length; i++ )

	}
	catch ( ... )
	{
#ifdef _DEBUG
		MessageBox( NULL, TEXT("UnKnown error occured!"), TEXT("Error"), MB_OK|MB_ICONSTOP );
#endif

		if ( NULL != pXMLDOMNode )
		{
			pXMLDOMNode.Release();
			pXMLDOMNode = NULL;
		}
		if ( NULL != pXMLDOMNodeList )
		{
			pXMLDOMNodeList.Release();
			pXMLDOMNodeList = NULL;
		}

		assert( false );
		return false;	
	}


	if ( NULL != pXMLDOMNode )
	{
		pXMLDOMNode.Release();
		pXMLDOMNode = NULL;
	}
	if ( NULL != pXMLDOMNodeList )
	{
		pXMLDOMNodeList.Release();
		pXMLDOMNodeList = NULL;
	}

	return bIsExist;
}


/************************************************************************
*																		
*  NAME:			IsChildNodeExist                                                 
*  FUNCTION:		Judge specific child node with DOUBULE attribute value 
*					of current node whether existing.		                                  
*  PARAMETER:															
*					Input:  szChildNodeName  - Child node name to be searched. 
*							szAttributeName  - Attribute name of child node to be searched.
*							dAttributeValue  - Attribute Value of Child Node to be searched.				    	
*					Output: void													
*  RETURN:	       	
*					true  - Exist
*					false - Not Exist.
*  CODER:			Songyongfeng													
*  DATE:			2007.11.30													   
*																								
************************************************************************/
bool CXMLDOMNode::IsChildNodeExist( const TCHAR* szChildNodeName, const TCHAR* szAttributeName, const double& dAttributeValue )
{

	assert( NULL != szChildNodeName );
	if( NULL == szChildNodeName )
	{
		return false;	
	}
	assert( NULL != szAttributeName );
	if( NULL == szAttributeName )
	{
		return false;	
	}
	assert( NULL != m_pXMLDOMNode );
	if( NULL == m_pXMLDOMNode )
	{
		return false;
	}

	TCHAR szAttributeValue[ MAX_PATH ];
	memset( szAttributeValue, 0, sizeof( szAttributeValue ) );
	_tcscpy( szAttributeValue, _DoubleToString( dAttributeValue ) );
	assert( NULL != szAttributeValue );
	if( NULL == szAttributeValue )
	{
		return false;
	}
	return IsChildNodeExist( szChildNodeName, szAttributeName, szAttributeValue );
}

/************************************************************************
*																		
*  NAME:			IsChildNodeExist                                                 
*  FUNCTION:		Judge specific child node with DOUBULE attribute value 
*					of current node whether existing and return the pointer
*					to the found node		       		                                  
*  PARAMETER:															
*					Input:  szChildNodeName  - Child node name to be searched. 
*							szAttributeName  - Attribute name of child node to be searched.
*							dAttributeValue  - Attribute Value of Child Node to be searched.				    	
*					Output: pGetXMLDOMNode   - Get pointer to the found node.													
*  RETURN:	       	
*					true  - Exist
*					false - Not Exist.
*  CODER:			Songyongfeng													
*  DATE:			2007.12.11													   
*																								
************************************************************************/
bool CXMLDOMNode::IsChildNodeExist( const TCHAR* szChildNodeName, const TCHAR* szAttributeName, const double& dAttributeValue, CXMLDOMNodePtr& pGetXMLDOMNode OUT )
{
	assert( NULL != szChildNodeName );
	if( NULL == szChildNodeName )
	{
		return false;	
	}
	assert( NULL != szAttributeName );
	if( NULL == szAttributeName )
	{
		return false;	
	}
	assert( NULL != m_pXMLDOMNode );
	if( NULL == m_pXMLDOMNode )
	{
		return false;
	}

	TCHAR szAttributeValue[ MAX_PATH ];
	memset( szAttributeValue, 0, sizeof( szAttributeValue ) );
	_tcscpy( szAttributeValue, _DoubleToString( dAttributeValue ) );
	assert( NULL != szAttributeValue );
	if( NULL == szAttributeValue )
	{
		return false;
	}
	return IsChildNodeExist( szChildNodeName, szAttributeName, szAttributeValue, pGetXMLDOMNode );
}


/************************************************************************
*																		
*  NAME:			IsChildNodeExist                                                 
*  FUNCTION:		Judge specific child node with INTEGER attribute value 
*					of current node whether existing.		                                  
*  PARAMETER:															
*					Input:  szChildNodeName  - Child node name to be searched. 
*							szAttributeName  - Attribute name of child node to be searched.
*							nAttributeValue  - Attribute Value of Child Node to be searched.				    	
*					Output: void													
*  RETURN:	       	
*					true  - Exist
*					false - Not Exist.
*  CODER:			Songyongfeng													
*  DATE:			2007.11.30													   
*																								
************************************************************************/
bool CXMLDOMNode::IsChildNodeExist( const TCHAR* szChildNodeName, const TCHAR* szAttributeName, const int& nAttributeValue )
{
	assert( NULL != szChildNodeName );
	if( NULL == szChildNodeName )
	{
		return false;	
	}
	assert( NULL != szAttributeName );
	if( NULL == szAttributeName )
	{
		return false;	
	}
	assert( NULL != m_pXMLDOMNode );
	if( NULL == m_pXMLDOMNode )
	{
		return false;
	}

	const int nBase = 10; // Converting base value. 
	TCHAR szAttributeValue[ MAX_PATH ];
	memset( szAttributeValue, 0, sizeof( szAttributeValue ) );
	_itot( nAttributeValue, szAttributeValue, nBase );
	assert( NULL != szAttributeValue );
	if( NULL == szAttributeValue )
	{
		return false;
	}
	return IsChildNodeExist( szChildNodeName, szAttributeName, szAttributeValue );
}


/************************************************************************
*																		
*  NAME:			IsChildNodeExist                                                 
*  FUNCTION:		Judge specific child node with INTEGER attribute value 
*					of current node whether existing and return the pointer
*					to the found node		       		  		                                  
*  PARAMETER:															
*					Input:  szChildNodeName  - Child node name to be searched. 
*							szAttributeName  - Attribute name of child node to be searched.
*							nAttributeValue  - Attribute Value of Child Node to be searched.				    	
*					Output: pGetXMLDOMNode   - Get pointer to the found node.													
*  RETURN:	       	
*					true  - Exist
*					false - Not Exist.
*  CODER:			Songyongfeng													
*  DATE:			2007.12.11													   
*																								
************************************************************************/
bool CXMLDOMNode::IsChildNodeExist( const TCHAR* szChildNodeName, const TCHAR* szAttributeName, const int& nAttributeValue, CXMLDOMNodePtr& pGetXMLDOMNode OUT )
{
	assert( NULL != szChildNodeName );
	if( NULL == szChildNodeName )
	{
		return false;	
	}
	assert( NULL != szAttributeName );
	if( NULL == szAttributeName )
	{
		return false;	
	}
	assert( NULL != m_pXMLDOMNode );
	if( NULL == m_pXMLDOMNode )
	{
		return false;
	}

	const int nBase = 10; // Converting base value. 
	TCHAR szAttributeValue[ MAX_PATH ];
	memset( szAttributeValue, 0, sizeof( szAttributeValue ) );
	_itot( nAttributeValue, szAttributeValue, nBase );
	assert( NULL != szAttributeValue );
	if( NULL == szAttributeValue )
	{
		return false;
	}
	return IsChildNodeExist( szChildNodeName, szAttributeName, szAttributeValue, pGetXMLDOMNode );
}


/************************************************************************
*																		
*  NAME:			IsChildNodeExist                                                 
*  FUNCTION:		Judge specific child node with BOOLEAN attribute value 
*					of current node whether existing.		                                  
*  PARAMETER:															
*					Input:  szChildNodeName  - Child node name to be searched. 
*							szAttributeName  - Attribute name of child node to be searched.
*							bAttributeValue  - Attribute Value of Child Node to be searched.	 			    	
*					Output: void													
*  RETURN:	       	
*					true  - Exist
*					false - Not Exist.
*  CODER:			Songyongfeng													
*  DATE:			2007.11.30													   
*																								
************************************************************************/
bool CXMLDOMNode::IsChildNodeExist( const TCHAR* szChildNodeName, const TCHAR* szAttributeName, const bool& bAttributeValue )
{
	assert( NULL != szChildNodeName );
	if( NULL == szChildNodeName )
	{
		return false;	
	}
	assert( NULL != szAttributeName );
	if( NULL == szAttributeName )
	{
		return false;	
	}
	assert( NULL != m_pXMLDOMNode );
	if( NULL == m_pXMLDOMNode )
	{
		return false;
	}

	return IsChildNodeExist( szChildNodeName, szAttributeName, ( int )bAttributeValue );
}

/************************************************************************
*																		
*  NAME:			IsChildNodeExist                                                 
*  FUNCTION:		Judge specific child node with BOOLEAN attribute value 
*					of current node whether existing and return the pointer
*					to the found node.		       		  		                                  
*  PARAMETER:															
*					Input:  szChildNodeName  - Child node name to be searched. 
*							szAttributeName  - Attribute name of child node to be searched.
*							bAttributeValue  - Attribute Value of Child Node to be searched.	 			    	
*					Output: pGetXMLDOMNode   - Get pointer to the found node.													
*  RETURN:	       	
*					true  - Exist
*					false - Not Exist.
*  CODER:			Songyongfeng													
*  DATE:			2007.12.11													   
*																								
************************************************************************/
bool CXMLDOMNode::IsChildNodeExist( const TCHAR* szChildNodeName, const TCHAR* szAttributeName, const bool& bAttributeValue, CXMLDOMNodePtr& pGetXMLDOMNode OUT )
{
	assert( NULL != szChildNodeName );
	if( NULL == szChildNodeName )
	{
		return false;	
	}
	assert( NULL != szAttributeName );
	if( NULL == szAttributeName )
	{
		return false;	
	}
	assert( NULL != m_pXMLDOMNode );
	if( NULL == m_pXMLDOMNode )
	{
		return false;
	}

	return IsChildNodeExist( szChildNodeName, szAttributeName, ( int )bAttributeValue, pGetXMLDOMNode );
}

int CXMLDOMNode::GetChildCount()
{
	CXMLDOMNodesPtr childNodes = this->GetChildrenNodes();
	return childNodes->GetCount();
}
CXMLDOMNodePtr CXMLDOMNode::GetChildNode(int index)
{
	if (m_pXMLDOMNode == NULL || index < 0)
	{
		return NULL;
	}
	MSXML2::IXMLDOMNodePtr pIXMLDOMRetChild = NULL;
	try
	{
		MSXML2::IXMLDOMNodeListPtr pIXMLDOMChildList = 
			m_pXMLDOMNode->GetchildNodes();
		long childCount = pIXMLDOMChildList->Getlength();
		int curIndex = 0;
		for (long i = 0; i < childCount ; i++)
		{
			MSXML2::IXMLDOMNodePtr pIXMLDOMChild = pIXMLDOMChildList->Getitem(i);
			MSXML2::DOMNodeType childType = 
				pIXMLDOMChild->GetnodeType();
			if (childType == MSXML2::NODE_ELEMENT)
			{
				if (curIndex == index)
				{
					pIXMLDOMRetChild = pIXMLDOMChild;
				}
				curIndex++;
			}
		}
		if (pIXMLDOMRetChild == NULL)
		{
			return NULL;
		}
	}
	catch (_com_error& e)
	{
		ShowComExceptionWithReason(_T("XML Node GetChildNode() failed."),e);
		return NULL;
	}
	catch (...)
	{
		ShowComExceptionUnknow(_T("XML Node GetChildNode() failed."));
		return NULL;
	}
	CXMLDOMNodePtr pRetChild(new CXMLDOMNode(pIXMLDOMRetChild));

	return pRetChild;
}
CXMLDOMNodePtr CXMLDOMNode::GetChildNode(CString childName)
{
	if (m_pXMLDOMNode == NULL || childName.IsEmpty())
	{
		return NULL;
	}
	MSXML2::IXMLDOMNodePtr pIXMLDOMRetChild = NULL;
	try
	{
		MSXML2::IXMLDOMNodeListPtr pIXMLDOMChildList = 
			m_pXMLDOMNode->GetchildNodes();
		long childCount = pIXMLDOMChildList->Getlength();
		for (long i = 0; i < childCount ; i++)
		{
			MSXML2::IXMLDOMNodePtr pIXMLDOMChild = pIXMLDOMChildList->Getitem(i);
			MSXML2::DOMNodeType childType = 
				pIXMLDOMChild->GetnodeType();
			if (childType == MSXML2::NODE_ELEMENT)
			{
				CString nodeName = (LPCTSTR)(_bstr_t)pIXMLDOMChild->GetnodeName();
				if (nodeName == childName)
				{
					pIXMLDOMRetChild = pIXMLDOMChild;
					break;
				}
			}
		}
		if (pIXMLDOMRetChild == NULL)
		{
			return NULL;
		}
	}
	catch (_com_error& e)
	{
		ShowComExceptionWithReason(_T("XML Node GetChildNode() failed."),e);
		return NULL;
	}
	catch (...)
	{
		ShowComExceptionUnknow(_T("XML Node GetChildNode() failed."));
		return NULL;
	}
	CXMLDOMNodePtr pRetChild(new CXMLDOMNode(pIXMLDOMRetChild));

	return pRetChild;
}
CXMLDOMNodePtr CXMLDOMNode::GetFirstChildNode()
{
	if( NULL == m_pXMLDOMNode )
	{
		return NULL;	
	}

	MSXML2::IXMLDOMNodePtr pIXMLDOMFirstChild = NULL;
	try
	{
		MSXML2::IXMLDOMNodeListPtr pIXMLDOMChildList = 
			m_pXMLDOMNode->GetchildNodes();
		long childCount = pIXMLDOMChildList->Getlength();
		for (long i = 0; i < childCount ; i++)
		{
			MSXML2::IXMLDOMNodePtr pIXMLDOMChild = 
				pIXMLDOMChildList->Getitem(i);
			MSXML2::DOMNodeType childType = 
				pIXMLDOMChild->GetnodeType();
			if (childType == MSXML2::NODE_ELEMENT)
			{
				pIXMLDOMFirstChild = pIXMLDOMChild;
				break;
			}
		}
	}
	catch (_com_error& e)
	{
		ShowComExceptionWithReason(_T("XML Node GetFirstChildNode() failed."),e);
		return NULL;
	}
	catch (...)
	{
		ShowComExceptionUnknow(_T("XML Node GetFirstChildNode() failed."));
		return NULL;
	}

	CXMLDOMNodePtr pRetFirstChild( new CXMLDOMNode( pIXMLDOMFirstChild ) );

	return pRetFirstChild;
}
CXMLDOMNodePtr CXMLDOMNode::GetLastChildNode()
{
	if( NULL == m_pXMLDOMNode )
	{
		return NULL;	
	}

	MSXML2::IXMLDOMNodePtr pIXMLDOMLastChild = NULL;
	try
	{
		MSXML2::IXMLDOMNodeListPtr pIXMLDOMChildList = 
			m_pXMLDOMNode->GetchildNodes();
		long childCount = pIXMLDOMChildList->Getlength();
		for (long i = 0; i < childCount ; i++)
		{
			MSXML2::IXMLDOMNodePtr pIXMLDOMChild = 
				pIXMLDOMChildList->Getitem(i);
			MSXML2::DOMNodeType childType = 
				pIXMLDOMChild->GetnodeType();
			if (childType == MSXML2::NODE_ELEMENT)
			{
				pIXMLDOMLastChild = pIXMLDOMChild;
			}
		}
	}
	catch (_com_error& e)
	{
		ShowComExceptionWithReason(_T("XML Node GetLastChildNode() failed."),e);
		return NULL;
	}
	catch (...)
	{
		ShowComExceptionUnknow(_T("XML Node GetLastChildNode() failed."));
		return NULL;
	}

	CXMLDOMNodePtr pRetLastChild( new CXMLDOMNode( pIXMLDOMLastChild ) );

	return pRetLastChild;

}
CXMLDOMNodesPtr CXMLDOMNode::GetChildrenNodes()
{
	if( NULL == m_pXMLDOMNode )
	{
		return NULL;
	}
	CXMLDOMNodesPtr pRetXMLDOMNodes;
	MSXML2::IXMLDOMNodeListPtr pXMLDOMNodeList = NULL;
	try
	{
		pXMLDOMNodeList = m_pXMLDOMNode->GetchildNodes();
		pRetXMLDOMNodes.reset( new CXMLDOMNodes( pXMLDOMNodeList ) );
	}
	catch(_com_error& e)
	{
		ShowComExceptionWithReason(_T("XML Node GetChildrenNodes() failed."),e);
		RETURN_NULL_LIST;
	}
	catch ( ... )
	{
		ShowComExceptionUnknow(_T("XML Node GetChildrenNodes() failed."));
		RETURN_NULL_LIST;
	}
	return pRetXMLDOMNodes;
}
CXMLDOMNodePtr CXMLDOMNode::GetFirstChildNodeByXPath(CString xpath)
{	
	if( NULL == m_pXMLDOMNode || xpath.IsEmpty())
	{
		return NULL;
	}
	MSXML2::IXMLDOMNodePtr pXMLDOMChild = NULL;
	try
	{
		pXMLDOMChild = m_pXMLDOMNode->selectSingleNode((_bstr_t)(LPCTSTR)xpath);
		if (pXMLDOMChild == NULL)
		{
			return NULL;
		}
	}
	catch (_com_error& e)
	{
		ShowComExceptionWithReason(_T("XML Node GetFirstChildNodeByXPath() failed."),e);
		return NULL;
	}
	catch (...)
	{
		ShowComExceptionUnknow(_T("XML Node GetFirstChildNodeByXPath() failed."));
		return NULL;
	}

	CXMLDOMNodePtr pNewChild( new CXMLDOMNode(pXMLDOMChild));

	return pNewChild;
}
CXMLDOMNodesPtr CXMLDOMNode::GetChildrenNodesByXPath(CString xpath)
{
	if (m_pXMLDOMNode == NULL || xpath.IsEmpty())
	{
		return NULL;
	}
	MSXML2::IXMLDOMNodeListPtr pXMLDOMChildList = NULL;
	try
	{
		pXMLDOMChildList = m_pXMLDOMNode->selectNodes((_bstr_t)(LPCTSTR)xpath);
		if (pXMLDOMChildList == NULL)
		{
			return NULL;
		}
	}
	catch (_com_error& e)
	{
		ShowComExceptionWithReason(_T("XML Node GetChildrenNodesByXPath() failed."),e);
		return FALSE;
	}
	catch (...)
	{
		ShowComExceptionUnknow(_T("XML Node GetChildrenNodesByXPath() failed."));
		return FALSE;
	}

	CXMLDOMNodesPtr pNewNodes(new CXMLDOMNodes(pXMLDOMChildList));

	return pNewNodes;
}

CXMLDOMNodePtr CXMLDOMNode::GetParentNode( void )
{
	if( NULL  == m_pXMLDOMNode )
	{
		return NULL;
	}
	MSXML2::IXMLDOMNodePtr pIXMLDOMParent = NULL;
	try
	{
		pIXMLDOMParent = m_pXMLDOMNode->GetparentNode();
		if( NULL == pIXMLDOMParent )
		{
			return NULL;
		}

	}
	catch (_com_error& e)
	{
		ShowComExceptionWithReason(_T("XML Node GetParentNode() failed."),e);
		return NULL;
	}
	catch (...)
	{
		ShowComExceptionUnknow(_T("XML Node GetParentNode() failed."));
		return NULL;
	}

	CXMLDOMNodePtr pRetParent( new CXMLDOMNode( pIXMLDOMParent ) );

	return pRetParent;
}

int CXMLDOMNode::GetSiblingCount()
{
	if( NULL == m_pXMLDOMNode )
	{
		return 0;
	}
	MSXML2::IXMLDOMNodePtr	pIXMLDOMParent = NULL;
	MSXML2::IXMLDOMNodePtr	pICurNode = NULL; // work pointer.
	MSXML2::DOMNodeType		NodeType;
	int						nCount = 0; // sibling count.
	try
	{
		pIXMLDOMParent = m_pXMLDOMNode->GetparentNode();
		if( NULL != pIXMLDOMParent )
		{
			pICurNode = pIXMLDOMParent->GetfirstChild();
			while (pICurNode != NULL)
			{
				NodeType = pICurNode->GetnodeType();
				if (NodeType == MSXML2::NODE_ELEMENT)
				{
					nCount++;
				}
				pICurNode = pICurNode->GetnextSibling();
			}
		}
		else
		{
			/* 文档根节点的父节点未NULL */
			nCount = 1;
		}
	}
	catch (_com_error& e)
	{
		ShowComExceptionWithReason(_T("XML Node GetSiblingCount() failed."),e);
		return 0;
	}
	catch (...)
	{
		ShowComExceptionUnknow(_T("XML Node GetSiblingCount() failed."));
		return 0;
	}
	return nCount;
}
/************************************************************************
*																		
*  NAME:			GetFirstSiblingNode                                                 
*  FUNCTION:		Get the first sibling node of current node.		                                  
*  PARAMETER:															
*					Input:  void.  			    	
*					Output: void													
*  RETURN:	       	
*					Success - Pointer to first sibling node.
*					Fail	- NULL.
*  CODER:			Songyongfeng													
*  DATE:			2007.12.17													   
*																								
************************************************************************/
CXMLDOMNodePtr CXMLDOMNode::GetFirstSiblingNode( void )
{
	assert( NULL != m_pXMLDOMNode );
	if( NULL == m_pXMLDOMNode )
	{
		RETURN_NULL_PTR;	
	}

	MSXML2::IXMLDOMNodePtr pIXMLDOMParent = NULL;
	CXMLDOMNodePtr pFirstSibling;
	try
	{
		pIXMLDOMParent = m_pXMLDOMNode->GetparentNode();
		assert( NULL != pIXMLDOMParent );
		if( NULL == pIXMLDOMParent )
		{
			RETURN_NULL_PTR;	
		}

		CXMLDOMNodePtr pParent( new CXMLDOMNode( pIXMLDOMParent ) );
		assert( NULL != pParent.get() );
		if( NULL == pParent.get() )
		{
			RETURN_NULL_PTR;	
		}

		/*CXMLDOMNodePtr*/ pFirstSibling = pParent->GetFirstChildNode();
		assert( NULL != pFirstSibling.get() );
		if( NULL == pFirstSibling.get() )
		{
			RETURN_NULL_PTR;	
		}
	}
	catch ( ... )
	{
#ifdef _DEBUG
		MessageBox( NULL, TEXT("UnKnown error occured!"), TEXT("Error"), MB_OK|MB_ICONSTOP );
#endif
		if( NULL != pIXMLDOMParent )
		{
			pIXMLDOMParent.Release();
			pIXMLDOMParent = NULL;
		}

		assert( false );
		RETURN_NULL_PTR;	
	}

	if( NULL != pIXMLDOMParent )
	{
		pIXMLDOMParent.Release();
		pIXMLDOMParent = NULL;
	}

	return pFirstSibling;

}


/************************************************************************
*																		
*  NAME:			GetNextSiblingNode                                                 
*  FUNCTION:		Get the Next sibling node of current node.		                                  
*  PARAMETER:															
*					Input:  void.  			    	
*					Output: void													
*  RETURN:	       	
*					Success - Pointer to first child node.
*					Fail	- NULL.
*  CODER:			Songyongfeng													
*  DATE:			2007.12.11													   
*																								
************************************************************************/
CXMLDOMNodePtr CXMLDOMNode::GetNextSiblingNode( void )
{
	assert( NULL != m_pXMLDOMNode );
	if( NULL == m_pXMLDOMNode )
	{
		RETURN_NULL_PTR;	
	}

	MSXML2::IXMLDOMNodePtr pIXMLDOMNextSibling = NULL;
	MSXML2::DOMNodeType NodeType;
	HRESULT hr;
	try
	{
		pIXMLDOMNextSibling = m_pXMLDOMNode->GetnextSibling();
		/*assert( NULL != pIXMLDOMNextSibling );*/
		if( NULL == pIXMLDOMNextSibling )
		{
			RETURN_NULL_PTR;	
		}

		// filter the node.
		//MSXML2::DOMNodeType NodeType;
		hr = pIXMLDOMNextSibling->get_nodeType( &NodeType );
		assert( SUCCEEDED( hr ) );
		if( FAILED( hr ) )
		{
			RETURN_NULL_PTR;	
		}
		while( NodeType != MSXML2::NODE_ELEMENT )
		{
			pIXMLDOMNextSibling = pIXMLDOMNextSibling->GetnextSibling(); // m_pXMLDOMNode
			hr = pIXMLDOMNextSibling->get_nodeType( &NodeType );
			assert( SUCCEEDED( hr ) );
			if( FAILED( hr ) )
			{
				RETURN_NULL_PTR;	
			}
		}
	}
	catch ( ... )
	{
#ifdef _DEBUG
		MessageBox( NULL, TEXT("UnKnown error occured!"), TEXT("Error"), MB_OK|MB_ICONSTOP );
#endif

		if( NULL != pIXMLDOMNextSibling )
		{
			pIXMLDOMNextSibling.Release();
			pIXMLDOMNextSibling = NULL;
		}
		assert( false );
		RETURN_NULL_PTR;	
	}


	CXMLDOMNodePtr pRetNextSibling( new CXMLDOMNode( pIXMLDOMNextSibling ) );
	assert( NULL != pRetNextSibling.get() );
	if( NULL == pRetNextSibling.get() )
	{
		RETURN_NULL_PTR;	
	}

	if( NULL != pIXMLDOMNextSibling )
	{
		pIXMLDOMNextSibling.Release();
		pIXMLDOMNextSibling = NULL;
	}

	return pRetNextSibling;
}


/************************************************************************
*																		
*  NAME:			GetPrevSiblingNode                                                 
*  FUNCTION:		Get the Previous sibling node of current node.		                                  
*  PARAMETER:															
*					Input:  void.  			    	
*					Output: void													
*  RETURN:	       	
*					Success - Pointer to first child node.
*					Fail	- NULL.
*  CODER:			Songyongfeng													
*  DATE:			2007.12.11													   
*																								
************************************************************************/
CXMLDOMNodePtr CXMLDOMNode::GetPrevSiblingNode( void )
{
	assert( NULL != m_pXMLDOMNode );
	if( NULL == m_pXMLDOMNode )
	{
		RETURN_NULL_PTR;	
	}


	MSXML2::IXMLDOMNodePtr pIXMLDOMPrevSibling = NULL;
	MSXML2::DOMNodeType NodeType;
	HRESULT hr;
	try
	{
		pIXMLDOMPrevSibling = m_pXMLDOMNode->GetpreviousSibling();
		assert( NULL != pIXMLDOMPrevSibling );
		if( NULL == pIXMLDOMPrevSibling )
		{
			RETURN_NULL_PTR;	
		}

		// filter the node.
		//MSXML2::DOMNodeType NodeType;
		/*HRESULT*/ hr = pIXMLDOMPrevSibling->get_nodeType( &NodeType );
		assert( SUCCEEDED( hr ) );
		if( FAILED( hr ) )
		{
			RETURN_NULL_PTR;	
		}
		while( NodeType != MSXML2::NODE_ELEMENT )
		{
			pIXMLDOMPrevSibling = pIXMLDOMPrevSibling->GetpreviousSibling(); // m_pXMLDOMNode
			hr = pIXMLDOMPrevSibling->get_nodeType( &NodeType );
			assert( SUCCEEDED( hr ) );
			if( FAILED( hr ) )
			{
				RETURN_NULL_PTR;	
			}
		}
	}
	catch ( ... )
	{
#ifdef _DEBUG
		MessageBox( NULL, TEXT("UnKnown error occured!"), TEXT("Error"), MB_OK|MB_ICONSTOP );
#endif

		if( NULL != pIXMLDOMPrevSibling )
		{
			pIXMLDOMPrevSibling.Release();
			pIXMLDOMPrevSibling = NULL;
		}
		assert( false );
		RETURN_NULL_PTR;	
	}


	CXMLDOMNodePtr pRetPrevSibling( new CXMLDOMNode( pIXMLDOMPrevSibling ) );
	assert( NULL != pRetPrevSibling.get() );
	if( NULL == pRetPrevSibling.get() )
	{
		RETURN_NULL_PTR;	
	}

	if( NULL != pIXMLDOMPrevSibling )
	{
		pIXMLDOMPrevSibling.Release();
		pIXMLDOMPrevSibling = NULL;
	}

	return pRetPrevSibling;
}


/************************************************************************
*																		
*  NAME:			GetLastSiblingNode                                                 
*  FUNCTION:		Get the last sibling node of current node.		                                  
*  PARAMETER:															
*					Input:  void.  			    	
*					Output: void													
*  RETURN:	       	
*					Success - Pointer to Last sibling node.
*					Fail	- NULL.
*  CODER:			Songyongfeng													
*  DATE:			2007.12.17													   
*																								
************************************************************************/
CXMLDOMNodePtr CXMLDOMNode::GetLastSiblingNode( void )
{
	assert( NULL != m_pXMLDOMNode );
	if( NULL == m_pXMLDOMNode )
	{
		RETURN_NULL_PTR;	
	}

	MSXML2::IXMLDOMNodePtr pIXMLDOMParent = NULL;
	CXMLDOMNodePtr pLastSibling;
	try
	{
		pIXMLDOMParent = m_pXMLDOMNode->GetparentNode();
		assert( NULL != pIXMLDOMParent );
		if( NULL == pIXMLDOMParent )
		{
			RETURN_NULL_PTR;	
		}

		CXMLDOMNodePtr pParent( new CXMLDOMNode( pIXMLDOMParent ) );
		assert( NULL != pParent.get() );
		if( NULL == pParent.get() )
		{
			RETURN_NULL_PTR;	
		}

		/*CXMLDOMNodePtr*/ pLastSibling = pParent->GetLastChildNode();
		assert( NULL != pLastSibling.get() );
		if( NULL == pLastSibling.get() )
		{
			RETURN_NULL_PTR;	
		}

	}
	catch ( ... )
	{
#ifdef _DEBUG
		MessageBox( NULL, TEXT("UnKnown error occured!"), TEXT("Error"), MB_OK|MB_ICONSTOP );
#endif

		if( NULL != pIXMLDOMParent )
		{
			pIXMLDOMParent.Release();
			pIXMLDOMParent = NULL;
		}
		assert( false );
		RETURN_NULL_PTR;	
	}


	if( NULL != pIXMLDOMParent )
	{
		pIXMLDOMParent.Release();
		pIXMLDOMParent = NULL;
	}


	return pLastSibling;
}

CString CXMLDOMNode::GetNodeName( void )
{
	CString nodeName;
	if( NULL == m_pXMLDOMNode )
	{
		return nodeName;
	}
	try
	{
		nodeName = (LPCTSTR)(_bstr_t)m_pXMLDOMNode->GetnodeName();
	}
	catch (_com_error& e)
	{
		ShowComExceptionWithReason(_T("XML Node GetNodeName() failed."),e);
		return NULL;
	}
	catch (...)
	{
		ShowComExceptionUnknow(_T("XML Node GetNodeName() failed."));
		return NULL;
	}

	return nodeName;
}
CString CXMLDOMNode::GetNodeNamespace( void )
{
	CString nodeNamespace;
	if( NULL == m_pXMLDOMNode )
	{
		return nodeNamespace;
	}
	try
	{
		nodeNamespace = (LPCTSTR)(_bstr_t)m_pXMLDOMNode->GetnamespaceURI();
	}
	catch (_com_error& e)
	{
		ShowComExceptionWithReason(_T("XML Node GetNodeNamespace() failed."),e);
		return NULL;
	}
	catch (...)
	{
		ShowComExceptionUnknow(_T("XML Node GetNodeNamespace() failed."));
		return NULL;
	}

	return nodeNamespace;
}

BOOL CXMLDOMNode::CloneNode( CXMLDOMNodePtr& pGetClonedNodeRoot, BOOL bIsDeepClone /*= TRUE */)
{
	if( NULL == m_pXMLDOMNode )
	{
		return false;
	}

	VARIANT_BOOL vbDeepClone;
	if( bIsDeepClone )
	{
		vbDeepClone = VARIANT_TRUE;	
	}
	else
	{
		vbDeepClone = VARIANT_FALSE;
	}

	MSXML2::IXMLDOMNodePtr pIXMLDOMCloneNode = NULL;
	try
	{
		pIXMLDOMCloneNode = m_pXMLDOMNode->cloneNode( ( VARIANT_BOOL)vbDeepClone );
		assert( NULL != pIXMLDOMCloneNode  );
		ASSERT(pIXMLDOMCloneNode != m_pXMLDOMNode);
		if( NULL == pIXMLDOMCloneNode  )
		{
			return FALSE;
		}

	}
	catch (_com_error& e)
	{
		ShowComExceptionWithReason(_T("XML Node CloneNode() failed."),e);
		return FALSE;
	}
	catch (...)
	{
		ShowComExceptionUnknow(_T("XML Node CloneNode() failed."));
		return FALSE;
	}

	CXMLDOMNodePtr pTempNode( new CXMLDOMNode( pIXMLDOMCloneNode ) );
	pGetClonedNodeRoot = pTempNode;

	return TRUE;
}

BOOL CXMLDOMNode::InsertSiblingBefore(CXMLDOMNodePtr& pInNode )
{
	if( NULL == m_pXMLDOMNode )
	{
		return FALSE;
	}
	if(pInNode == NULL)
	{
		return FALSE;
	}

	MSXML2::IXMLDOMNodePtr pIXMLDOMParent = NULL;
	MSXML2::IXMLDOMNodePtr pIXMLDOMNewNode = NULL;
	try
	{
		// Get the parent of current node's.
		pIXMLDOMParent = m_pXMLDOMNode->GetparentNode();
		if( NULL == pIXMLDOMParent )
		{
			return FALSE;
		}
		// Insert pInNode before current node.
		pIXMLDOMNewNode =  pIXMLDOMParent->insertBefore( pInNode->m_pXMLDOMNode,( _variant_t)( IDispatch*) m_pXMLDOMNode );
		ASSERT(pIXMLDOMNewNode == pInNode->m_pXMLDOMNode);
		if( NULL == pIXMLDOMNewNode )
		{
			return FALSE;
		}
	}
	catch (_com_error& e)
	{
		ShowComExceptionWithReason(_T("XML Node InsertBefore() failed."),e);
		return FALSE;
	}
	catch (...)
	{
		ShowComExceptionUnknow(_T("XML Node InsertBefore() failed."));
		return FALSE;
	}
	_SetDocumentModified();
	return TRUE;

}
BOOL CXMLDOMNode::InsertSiblingBefore(CXMLDOMNodePtr& pInNode, CXMLDOMNodePtr& pGetInsertedNode)
{
	if( NULL == m_pXMLDOMNode )
	{
		return FALSE;
	}
	if( NULL == pInNode)
	{
		return FALSE;
	}

	MSXML2::IXMLDOMNodePtr pIXMLDOMParent = NULL;
	MSXML2::IXMLDOMNodePtr pIXMLDOMNewNode = NULL;
	try
	{
		// Get the parent of current node's.
		pIXMLDOMParent = m_pXMLDOMNode->GetparentNode();
		if( NULL == pIXMLDOMParent )
		{
			return FALSE;
		}

		// Insert pInNode before current node.
		pIXMLDOMNewNode =  pIXMLDOMParent->insertBefore( pInNode->m_pXMLDOMNode , ( _variant_t)( IDispatch*) m_pXMLDOMNode ); 
		ASSERT(pIXMLDOMNewNode == pInNode->m_pXMLDOMNode);
		if( NULL == pIXMLDOMNewNode )
		{
			return FALSE;
		}
	}
	catch (_com_error& e)
	{
		ShowComExceptionWithReason(_T("XML Node InsertBefore() failed."),e);
		return FALSE;
	}
	catch (...)
	{
		ShowComExceptionUnknow(_T("XML Node InsertBefore() failed."));
		return FALSE;
	}

	// Create new node.
	CXMLDOMNodePtr pTempNode( new CXMLDOMNode( pIXMLDOMNewNode ) );
	// Return the pointer to new node.
	pGetInsertedNode = pTempNode;
	_SetDocumentModified();
	return TRUE;
}
CXMLDOMNodePtr CXMLDOMNode::InsertSiblingBefore(CString nodeName)
{
	if (nodeName.IsEmpty() || m_pXMLDOMNode == NULL)
	{
		return NULL;
	}
	CXMLDOMNodePtr newNode;
	try
	{
		MSXML2::IXMLDOMDocument3Ptr pIXMLDOMDocument = m_pXMLDOMNode->GetownerDocument();
		MSXML2::IXMLDOMNodePtr pIXMLDOMParent = m_pXMLDOMNode->GetparentNode();
		if (pIXMLDOMParent == NULL || pIXMLDOMDocument == NULL)
		{
			return NULL;
		}
		CString namespaceURI = (LPCTSTR)(_bstr_t)pIXMLDOMParent->GetnamespaceURI();
		MSXML2::IXMLDOMNodePtr pIXMLDOMNewNode = pIXMLDOMDocument->createNode
			(MSXML2::NODE_ELEMENT,(_bstr_t)(LPCTSTR)nodeName,(_bstr_t)(LPCTSTR)namespaceURI);
		if (pIXMLDOMNewNode == NULL)
		{
			return NULL;
		}
		MSXML2::IXMLDOMNodePtr pIXMLDOMTempNode = pIXMLDOMNewNode;
		pIXMLDOMParent->insertBefore(pIXMLDOMNewNode,(_variant_t)(IDispatch*)m_pXMLDOMNode);
		ASSERT(pIXMLDOMTempNode == pIXMLDOMNewNode);
		if (pIXMLDOMNewNode != NULL)
		{
			newNode.reset(new CXMLDOMNode(pIXMLDOMNewNode));
		}
	}
	catch (_com_error& e)
	{
		ShowComExceptionWithReason(_T("XML Node InsertSiblingBefore() failed."),e);
		return NULL;
	}
	catch (...)
	{
		ShowComExceptionUnknow(_T("XML Node InsertSiblingBefore() failed."));
		return NULL;
	}
	if (newNode != NULL)
	{
		_SetDocumentModified();
	}
	return newNode;
}
CXMLDOMNodePtr CXMLDOMNode::InsertSiblingBefore(CString nodeName,CString namespaceURI)
{
	CXMLDOMNodePtr newNode;
	if (nodeName.IsEmpty() || m_pXMLDOMNode == NULL)
	{
		return newNode;
	}
	try
	{
		MSXML2::IXMLDOMDocument3Ptr pIXMLDOMDocument = m_pXMLDOMNode->GetownerDocument();
		MSXML2::IXMLDOMNodePtr pIXMLDOMParent = m_pXMLDOMNode->GetparentNode();
		if (pIXMLDOMParent == NULL || pIXMLDOMDocument == NULL)
		{
			return NULL;
		}
		MSXML2::IXMLDOMNodePtr pIXMLDOMNewNode = pIXMLDOMDocument->createNode
			(MSXML2::NODE_ELEMENT,(_bstr_t)(LPCTSTR)nodeName,(_bstr_t)(LPCTSTR)namespaceURI);
		if (pIXMLDOMNewNode == NULL)
		{
			return NULL;
		}
		MSXML2::IXMLDOMNodePtr pIXMLDOMTempNode = pIXMLDOMNewNode;
		pIXMLDOMParent->insertBefore(pIXMLDOMNewNode,(_variant_t)(IDispatch*)m_pXMLDOMNode);
		ASSERT(pIXMLDOMTempNode == pIXMLDOMNewNode);
		if (pIXMLDOMNewNode != NULL)
		{
			newNode.reset(new CXMLDOMNode(pIXMLDOMNewNode));
		}
	}
	catch (_com_error& e)
	{
		ShowComExceptionWithReason(_T("XML Node InsertSiblingBefore() failed."),e);
		return NULL;
	}
	catch (...)
	{
		ShowComExceptionUnknow(_T("XML Node InsertSiblingBefore() failed."));
		return NULL;
	}
	if (newNode != NULL)
	{
		_SetDocumentModified();
	}
	return newNode;
}

BOOL CXMLDOMNode::InsertSiblingAfter(CXMLDOMNodePtr& pInNode )
{
	if( NULL == m_pXMLDOMNode )
	{
		return FALSE;
	}
	if( NULL == pInNode)
	{
		return FALSE;
	}

	MSXML2::IXMLDOMNodePtr pIXMLDOMParent = NULL;
	MSXML2::IXMLDOMNodePtr pIXMLDOMNewNode = NULL;
	MSXML2::IXMLDOMNodePtr pIXMLDOMNextSibling = NULL;
	try
	{
		// Get the parent of current node's.
		pIXMLDOMParent = m_pXMLDOMNode->GetparentNode();
		if( NULL == pIXMLDOMParent )
		{
			return FALSE;
		}
		pIXMLDOMNextSibling = m_pXMLDOMNode->GetnextSibling();
		if (pIXMLDOMNextSibling == NULL)
		{
			/* 当前节点后没有兄弟节点，则用appendNode */
			pIXMLDOMNewNode = pIXMLDOMParent->appendChild(pInNode->m_pXMLDOMNode);
		} 
		else
		{
			/* 当前节点后有兄弟节点,则用insertBefore */
			pIXMLDOMNewNode = pIXMLDOMParent->insertBefore(pInNode->m_pXMLDOMNode,(_variant_t)(IDispatch*)pIXMLDOMNextSibling);
		}
		ASSERT(pIXMLDOMNewNode == pInNode->m_pXMLDOMNode);
		if (pIXMLDOMNewNode == NULL)
		{
			return FALSE;
		}
	}
	catch (_com_error& e)
	{
		ShowComExceptionWithReason(_T("XML Node InsertSiblingAfter() failed."),e);
		return FALSE;
	}
	catch (...)
	{
		ShowComExceptionUnknow(_T("XML Node InsertSiblingAfter() failed."));
		return FALSE;
	}
	_SetDocumentModified();
	return TRUE;
}
BOOL CXMLDOMNode::InsertSiblingAfter(CXMLDOMNodePtr& pInNode, CXMLDOMNodePtr& pGetInsertedNode)
{
	if( NULL == m_pXMLDOMNode )
	{
		return FALSE;
	}
	if( NULL == pInNode)
	{
		return FALSE;
	}

	MSXML2::IXMLDOMNodePtr pIXMLDOMParent = NULL;
	MSXML2::IXMLDOMNodePtr pIXMLDOMNewNode = NULL;
	MSXML2::IXMLDOMNodePtr pIXMLDOMNextSibling = NULL;
	try
	{
		// Get the parent of current node's.
		pIXMLDOMParent = m_pXMLDOMNode->GetparentNode();
		if( NULL == pIXMLDOMParent )
		{
			return FALSE;
		}
		pIXMLDOMNextSibling = m_pXMLDOMNode->GetnextSibling();
		if (pIXMLDOMNextSibling == NULL)
		{
			/* 当前节点后没有兄弟节点，用appendChild */
			pIXMLDOMNewNode = pIXMLDOMParent->appendChild(pInNode->m_pXMLDOMNode);
		} 
		else
		{
			/* 当前节点后有兄弟节点，用insertBefore */
			pIXMLDOMNewNode = pIXMLDOMParent->insertBefore(pInNode->m_pXMLDOMNode,(_variant_t)(IDispatch*)pIXMLDOMNextSibling);
		}
		ASSERT(pIXMLDOMNewNode == pInNode->m_pXMLDOMNode);
		if( NULL == pIXMLDOMNewNode )
		{
			return FALSE;
		}

	}
	catch (_com_error& e)
	{
		ShowComExceptionWithReason(_T("XML Node InsertSiblingAfter() failed."),e);
		return FALSE;
	}
	catch (...)
	{
		ShowComExceptionUnknow(_T("XML Node InsertSiblingAfter() failed."));
		return FALSE;
	}

	// Create new node.
	CXMLDOMNodePtr pTempNode( new CXMLDOMNode( pIXMLDOMNewNode ) );
	// Return the pointer to new node.
	pGetInsertedNode = pTempNode;
	_SetDocumentModified();
	return TRUE;
}
CXMLDOMNodePtr CXMLDOMNode::InsertSiblingAfter(CString nodeName)
{
	if (nodeName.IsEmpty() || m_pXMLDOMNode == NULL)
	{
		return NULL;
	}
	CXMLDOMNodePtr newNode;
	try
	{
		MSXML2::IXMLDOMDocument3Ptr pIXMLDOMDocument = m_pXMLDOMNode->GetownerDocument();
		MSXML2::IXMLDOMNodePtr pIXMLDOMParent = m_pXMLDOMNode->GetparentNode();
		if (pIXMLDOMParent == NULL || pIXMLDOMDocument == NULL)
		{
			return NULL;
		}
		CString namespaceURI = (LPCTSTR)(_bstr_t)pIXMLDOMParent->GetnamespaceURI();
		MSXML2::IXMLDOMNodePtr pIXMLDOMNewNode = pIXMLDOMDocument->createNode
			(MSXML2::NODE_ELEMENT,(_bstr_t)(LPCTSTR)nodeName,(_bstr_t)(LPCTSTR)namespaceURI);
		if (pIXMLDOMNewNode == NULL)
		{
			return NULL;
		}
		MSXML2::IXMLDOMNodePtr pIXMLDOMNextSibling = m_pXMLDOMNode->GetnextSibling();
		MSXML2::IXMLDOMNodePtr pIXMLDOMTempNode = pIXMLDOMNewNode;
		if (pIXMLDOMNextSibling == NULL)
		{
			pIXMLDOMNewNode = pIXMLDOMParent->appendChild(pIXMLDOMNewNode);
		} 
		else
		{
			pIXMLDOMNewNode = pIXMLDOMParent->insertBefore(pIXMLDOMNewNode,(_variant_t)(IDispatch*)pIXMLDOMNextSibling);
		}
		ASSERT(pIXMLDOMTempNode == pIXMLDOMNewNode);
		if (pIXMLDOMNewNode != NULL)
		{
			newNode.reset(new CXMLDOMNode(pIXMLDOMNewNode));
		}
	}
	catch (_com_error& e)
	{
		ShowComExceptionWithReason(_T("XML Node InsertSiblingAfter() failed."),e);
		return NULL;
	}
	catch (...)
	{
		ShowComExceptionUnknow(_T("XML Node InsertSiblingAfter() failed."));
		return NULL;
	}
	if (newNode != NULL)
	{
		_SetDocumentModified();
	}
	return newNode;
}
CXMLDOMNodePtr CXMLDOMNode::InsertSiblingAfter(CString nodeName,CString namespaceURI)
{
	CXMLDOMNodePtr newNode;
	if (nodeName.IsEmpty() || m_pXMLDOMNode == NULL)
	{
		return newNode;
	}
	try
	{
		MSXML2::IXMLDOMDocument3Ptr pIXMLDOMDocument = m_pXMLDOMNode->GetownerDocument();
		MSXML2::IXMLDOMNodePtr pIXMLDOMParent = m_pXMLDOMNode->GetparentNode();
		if (pIXMLDOMParent == NULL || pIXMLDOMDocument == NULL)
		{
			return NULL;
		}
		MSXML2::IXMLDOMNodePtr pIXMLDOMNewNode = pIXMLDOMDocument->createNode
			(MSXML2::NODE_ELEMENT,(_bstr_t)(LPCTSTR)nodeName,(_bstr_t)(LPCTSTR)namespaceURI);
		if (pIXMLDOMNewNode == NULL)
		{
			return NULL;
		}
		MSXML2::IXMLDOMNodePtr pIXMLDOMNextSibling = m_pXMLDOMNode->GetnextSibling();
		MSXML2::IXMLDOMNodePtr pIXMLDOMTempNode = pIXMLDOMNewNode;
		if (pIXMLDOMNextSibling == NULL)
		{
			pIXMLDOMNewNode = pIXMLDOMParent->appendChild(pIXMLDOMNewNode);
		} 
		else
		{
			pIXMLDOMNewNode = pIXMLDOMParent->insertBefore(pIXMLDOMNewNode,(_variant_t)(IDispatch*)pIXMLDOMNextSibling);
		}
		ASSERT(pIXMLDOMTempNode == pIXMLDOMNewNode);
		if (pIXMLDOMNewNode != NULL)
		{
			newNode.reset(new CXMLDOMNode(pIXMLDOMNewNode));
		}
	}
	catch (_com_error& e)
	{
		ShowComExceptionWithReason(_T("XML Node InsertSiblingAfter() failed."),e);
		return NULL;
	}
	catch (...)
	{
		ShowComExceptionUnknow(_T("XML Node InsertSiblingAfter() failed."));
		return NULL;
	}
	if (newNode != NULL)
	{
		_SetDocumentModified();
	}
	return newNode;
}

BOOL CXMLDOMNode::AppendChildNode(CXMLDOMNodePtr& pInNode )
{
	if( NULL == m_pXMLDOMNode 
		||NULL == pInNode)
	{
		return FALSE;
	}
	BOOL result = FALSE;
	try
	{
		ASSERT(MSXML2::NODE_ELEMENT == m_pXMLDOMNode->GetnodeType());
		MSXML2::IXMLDOMNodePtr pIXMLDOMNewNode = pInNode->m_pXMLDOMNode;
		if (pIXMLDOMNewNode != NULL)
		{
			MSXML2::IXMLDOMNodePtr pIXMLDOMTempNode = pIXMLDOMNewNode;
			pIXMLDOMNewNode = m_pXMLDOMNode->appendChild(pIXMLDOMNewNode);
			ASSERT(pIXMLDOMNewNode == pIXMLDOMTempNode);
			result = TRUE;
		}
	}
	catch (_com_error& e)
	{
		ShowComExceptionWithReason(_T("XML Node AppendNode() failed."),e);
		return FALSE;
	}
	catch (...)
	{
		ShowComExceptionUnknow(_T("XML Node AppendNode() failed."));
		return FALSE;
	}
	if (result == TRUE)
	{
		_SetDocumentModified();
	}
	return result;
}
BOOL CXMLDOMNode::AppendChildNode(CXMLDOMNodePtr& pInNode, CXMLDOMNodePtr& pGetAppendedNode OUT )
{
	if( NULL == m_pXMLDOMNode
		||NULL == pInNode )
	{
		return FALSE;
	}
	BOOL result = FALSE;
	try
	{
		ASSERT(MSXML2::NODE_ELEMENT == m_pXMLDOMNode->GetnodeType());
		MSXML2::IXMLDOMNodePtr pIXMLDOMNewNode = pInNode->m_pXMLDOMNode;
		if (pIXMLDOMNewNode != NULL)
		{
			MSXML2::IXMLDOMNodePtr pIXMLDOMTempNode = pIXMLDOMNewNode;
			pIXMLDOMNewNode = m_pXMLDOMNode->appendChild(pIXMLDOMNewNode);
			ASSERT(pIXMLDOMNewNode == pIXMLDOMTempNode);
			// Return current node pointer.
			CXMLDOMNodePtr pTemp( new CXMLDOMNode(pIXMLDOMNewNode) );
			pGetAppendedNode = pTemp;
			result = TRUE;
		}
	}
	catch (_com_error& e)
	{
		ShowComExceptionWithReason(_T("XML Node AppendNode() failed."),e);
		return FALSE;
	}
	catch (...)
	{
		ShowComExceptionUnknow(_T("XML Node AppendNode() failed."));
		return FALSE;
	}
	if (result == TRUE)
	{
		_SetDocumentModified();
	}
	return result;
}
CXMLDOMNodePtr CXMLDOMNode::AppendChildNode(CString nodeName)
{
	if (m_pXMLDOMNode == NULL
		||nodeName.IsEmpty())
	{
		return NULL;
	}
	BOOL result = FALSE;
	CXMLDOMNodePtr resultNode;
	try
	{
		ASSERT(MSXML2::NODE_ELEMENT == m_pXMLDOMNode->GetnodeType());
		MSXML2::IXMLDOMDocument3Ptr pIXMLDOMDocument = m_pXMLDOMNode->GetownerDocument();
		if(pIXMLDOMDocument == NULL)
			return NULL;
		CString namespaceURI = (LPCTSTR)(_bstr_t)m_pXMLDOMNode->GetnamespaceURI();
		MSXML2::IXMLDOMNodePtr pIXMLDOMNewNode = pIXMLDOMDocument->createNode(MSXML2::NODE_ELEMENT,
			(_bstr_t)(LPCTSTR)nodeName,(_bstr_t)(LPCTSTR)namespaceURI);
		if(pIXMLDOMNewNode == NULL)
			return NULL;
		MSXML2::IXMLDOMNodePtr pIXMLDOMTempNode = pIXMLDOMNewNode;
		pIXMLDOMNewNode = m_pXMLDOMNode->appendChild(pIXMLDOMNewNode);
		ASSERT(pIXMLDOMTempNode == pIXMLDOMNewNode);
		if (pIXMLDOMNewNode != NULL)
		{
			resultNode.reset(new CXMLDOMNode(pIXMLDOMNewNode));
			result = TRUE;
		}
	}
	catch (_com_error& e)
	{
		ShowComExceptionWithReason(_T("XML Node AppendNode() failed."),e);
		return NULL;
	}
	catch (...)
	{
		ShowComExceptionUnknow(_T("XML Node AppendNode() failed."));
		return NULL;
	}
	if (result == TRUE)
	{
		_SetDocumentModified();
	}
	return resultNode;
}
CXMLDOMNodePtr CXMLDOMNode::AppendChildNode(CString nodeName,CString namespaceURI)
{
	if (m_pXMLDOMNode == NULL
		||nodeName.IsEmpty())
	{
		return NULL;
	}
	BOOL result = FALSE;
	CXMLDOMNodePtr resultNode;
	try
	{
		ASSERT(MSXML2::NODE_ELEMENT == m_pXMLDOMNode->GetnodeType());
		MSXML2::IXMLDOMElementPtr pIXMLDOMElement = m_pXMLDOMNode;
		MSXML2::IXMLDOMDocument3Ptr pIXMLDOMDocument = pIXMLDOMElement->GetownerDocument();
		if(pIXMLDOMDocument == NULL)
			return NULL;
		MSXML2::IXMLDOMNodePtr pIXMLDOMNewNode = pIXMLDOMDocument->createNode(MSXML2::NODE_ELEMENT,
			(_bstr_t)(LPCTSTR)nodeName,(_bstr_t)(LPCTSTR)namespaceURI);
		if(pIXMLDOMNewNode == NULL)
			return NULL;
		MSXML2::IXMLDOMNodePtr pIXMLDOMTempNode = pIXMLDOMNewNode;
		pIXMLDOMNewNode = pIXMLDOMElement->appendChild(pIXMLDOMNewNode);
		ASSERT(pIXMLDOMTempNode == pIXMLDOMNewNode);
		if (pIXMLDOMNewNode != NULL)
		{
			resultNode.reset(new CXMLDOMNode(pIXMLDOMNewNode));
			result = TRUE;
		}
	}
	catch (_com_error& e)
	{
		ShowComExceptionWithReason(_T("XML Node AppendNode() failed."),e);
		return NULL;
	}
	catch (...)
	{
		ShowComExceptionUnknow(_T("XML Node AppendNode() failed."));
		return NULL;
	}
	if (result == TRUE)
	{
		_SetDocumentModified();
	}
	return resultNode;
}

CXMLDOMNodePtr CXMLDOMNode::AppendChildNodeWithValue(CString nodeName,CString nodeValue)
{
	CXMLDOMNodePtr pNewNode = AppendChildNode(nodeName);
	if (pNewNode != NULL)
	{
		if (!nodeValue.IsEmpty())
		{
			pNewNode->SetNodeValue(nodeValue);
		}
	}
	return pNewNode;
}
CXMLDOMNodePtr CXMLDOMNode::AppendChildNodeWithAttr(CString nodeName,CString nodeValue,CString attrName,CString attrValue)
{
	CXMLDOMNodePtr pNewNode = AppendChildNode(nodeName);
	if (pNewNode != NULL)
	{
		if (!nodeValue.IsEmpty())
		{
			pNewNode->SetNodeValue(nodeValue);
		}
		if (!attrName.IsEmpty())
		{
			if(pNewNode->AddAttributeNode(attrName))
			{
				pNewNode->SetAttributeValue(attrName,attrValue);
			}
		}
	}
	return pNewNode;
}

BOOL  CXMLDOMNode::DeleteCurrentNode( void )
{
	if( NULL == m_pXMLDOMNode )
	{
		return FALSE;
	}
	BOOL result = FALSE;
	try
	{
		ASSERT(MSXML2::NODE_ELEMENT == m_pXMLDOMNode->GetnodeType());
		MSXML2::IXMLDOMNodePtr pIXMLDOMParent = m_pXMLDOMNode->GetparentNode();
		if (pIXMLDOMParent == NULL)
		{
			return FALSE;
		}

		MSXML2::IXMLDOMNodePtr pIXMLDOMTempNode = pIXMLDOMParent->removeChild( m_pXMLDOMNode );
		ASSERT(pIXMLDOMTempNode == m_pXMLDOMNode);
		if(pIXMLDOMTempNode == NULL)
		{
			return FALSE;
		}
		result = TRUE;
	}
	catch (_com_error& e)
	{
		ShowComExceptionWithReason(_T("XML Node DeleteCurrentNode() failed."),e);
		return FALSE;
	}
	catch (...)
	{
		ShowComExceptionUnknow(_T("XML Node DeleteCurrentNode() failed."));
		return FALSE;
	}
	if (result == TRUE)
	{
		_SetDocumentModified();
	}
	return result;
}
BOOL CXMLDOMNode:: DeleteChildrenNodes( void )
{
	if( NULL == m_pXMLDOMNode )
	{
		return FALSE;
	}
	BOOL result = FALSE;
	try
	{
		MSXML2::IXMLDOMNodeListPtr pIXMLDOMChildNodeList = 
			m_pXMLDOMNode->GetchildNodes();
		if(pIXMLDOMChildNodeList == NULL)
		{
			return FALSE;
		}
		//错误的删除方法，因为随着节点被删除子节点列表已经没有原先数量的子节点，
		//导致Getitem(i)取子节点失败
		//long nCountChildrenNodes = pIXMLDOMChildNodeList->Getlength();
		//for( long i = 0; i < nCountChildrenNodes ; i++ )
		//{
		//	MSXML2::IXMLDOMNodePtr pIXMLDOMChildNode =
		//		pIXMLDOMChildNodeList->Getitem(i);
		//	MSXML2::IXMLDOMNodePtr pIXMLDOMTempNode = pIXMLDOMChildNode;
		//	pIXMLDOMChildNode = m_pXMLDOMNode->removeChild(pIXMLDOMChildNode);
		//	ASSERT(pIXMLDOMTempNode == pIXMLDOMChildNode);
		//}
		MSXML2::IXMLDOMNodePtr pIXMLDOMChildNode = NULL;
		do 
		{
			pIXMLDOMChildNode = pIXMLDOMChildNodeList->nextNode();
			if (pIXMLDOMChildNode != NULL)
			{
					MSXML2::IXMLDOMNodePtr pIXMLDOMTempNode = pIXMLDOMChildNode;
					pIXMLDOMChildNode = m_pXMLDOMNode->removeChild(pIXMLDOMChildNode);
					ASSERT(pIXMLDOMTempNode == pIXMLDOMChildNode);
			}
		} while (pIXMLDOMChildNode != NULL);
#ifdef _DEBUG
		pIXMLDOMChildNodeList = m_pXMLDOMNode->GetchildNodes();
		int childCount = pIXMLDOMChildNodeList->Getlength();
#endif
		result = TRUE;
	}
	catch (_com_error& e)
	{
		ShowComExceptionWithReason(_T("XML Node DeleteChildrenNodes() failed."),e);
		return FALSE;
	}
	catch (...)
	{
		ShowComExceptionUnknow(_T("XML Node DeleteChildrenNodes() failed."));
		return FALSE;
	}
	if (result == TRUE)
	{
		_SetDocumentModified();
	}
	return result;
}

BOOL CXMLDOMNode::ValidateCurrentNode(XMLStringList& errorInfoList)
{
	errorInfoList.clear();
	if (m_pXMLDOMNode == NULL)
	{
		errorInfoList.push_back(_T("Element pointer of Document Object Model is NULL."));
		return FALSE;
	}
	try
	{
		MSXML2::DOMNodeType nodeType = m_pXMLDOMNode->GetnodeType();
		ASSERT(nodeType == MSXML2::NODE_ELEMENT);

		MSXML2::IXMLDOMElementPtr pXMLDOMElement = m_pXMLDOMNode;
		CString elemName = (LPCTSTR)(_bstr_t)pXMLDOMElement->GetnodeName();
		MSXML2::IXMLDOMDocument3Ptr pXMLDOMDocument = m_pXMLDOMNode->GetownerDocument();
		MSXML2::IXMLDOMParseErrorPtr pParseError = pXMLDOMDocument->validateNode(pXMLDOMElement);
		if (pParseError->GeterrorCode() == 0)
		{
			CString msg;
			msg.Format(_T("Element \"%s\" is valid."),elemName);
			errorInfoList.push_back(msg);
			return TRUE;
		}
		else
		{
			errorInfoList = ShowElemmentValidateError(elemName,pParseError);
			return FALSE;
		}
	}
	catch (_com_error& e)
	{
		CString desc = ShowComExceptionWithReason(_T("XML Node ValidateAttribute() failed."),e);
		errorInfoList.push_back(desc);
		return FALSE;
	}
	catch (...)
	{
		CString desc = ShowComExceptionUnknow(_T("XML Node ValidateAttribute() failed."));
		errorInfoList.push_back(desc);
		return FALSE;
	}
}

/************************node attribute operation******************************/

int CXMLDOMNode::GetAttributeCount( void )
{
	if( NULL == m_pXMLDOMNode )
	{
		return 0;
	}
	int nCount = 0;

	try
	{
		MSXML2::IXMLDOMNamedNodeMapPtr pAttrMap = m_pXMLDOMNode->Getattributes();
#ifdef _DEBUG
		CString nodeName = (LPCTSTR)(_bstr_t)m_pXMLDOMNode->GetnodeName();
#endif
		if (pAttrMap != NULL)
		{
			MSXML2::IXMLDOMAttributePtr pAttribute;
			while(pAttribute = pAttrMap->nextNode())
			{
#ifdef _DEBUG
				CString attrName = (LPCTSTR)(_bstr_t)pAttribute->Getname();
				CString attrValue = (LPCTSTR)(_bstr_t)pAttribute->Getvalue();
#endif
				if(VARIANT_TRUE == pAttribute->Getspecified())
					nCount++;
			}
		}
	}
	catch (_com_error& e)
	{
		ShowComExceptionWithReason(_T("XML Node GetAttributeCount() failed."),e);
		return 0;
	}
	catch (...)
	{
		ShowComExceptionUnknow(_T("XML Node GetAttributeCount() failed."));
		return 0;
	}
	return nCount;
}

CString CXMLDOMNode::GetAttributeName( int nIndex)
{
	CString attrName;
	if( NULL == m_pXMLDOMNode )
	{
		return attrName;
	}

	MSXML2::IXMLDOMNamedNodeMapPtr	pXMLDOMNamedNodeMap = NULL;
	MSXML2::IXMLDOMAttributePtr		pXMLDOMAttribute = NULL;
	try
	{
		pXMLDOMNamedNodeMap = m_pXMLDOMNode->Getattributes();
		int curIndex = 0;
		while (pXMLDOMAttribute = pXMLDOMNamedNodeMap->nextNode())
		{
			if (VARIANT_TRUE == pXMLDOMAttribute->Getspecified()
				&& curIndex == nIndex)
			{
				attrName = (LPCTSTR)pXMLDOMAttribute->Getname();
				break;
			}
			else if (VARIANT_TRUE == pXMLDOMAttribute->Getspecified()
				&& curIndex != nIndex)
			{
				curIndex++;
			}
			
		}
	}
	catch (_com_error& e)
	{
		ShowComExceptionWithReason(_T("XML Node GetAttributeName() failed."),e);
		return attrName;
	}
	catch (...)
	{
		ShowComExceptionUnknow(_T("XML Node GetAttributeName() failed."));
		return attrName;
	}
	return attrName;
}

BOOL CXMLDOMNode::IsAttributeExist(CString attrName)
{
	if( NULL == m_pXMLDOMNode || attrName.IsEmpty())
	{
		return FALSE;
	}

	BOOL bIsExist = FALSE;
	try
	{
#ifdef _DEBUG
		ASSERT(MSXML2::NODE_ELEMENT == m_pXMLDOMNode->GetnodeType());
		CString elemName = (LPCTSTR)(_bstr_t)m_pXMLDOMNode->GetnodeName();
#endif
		MSXML2::IXMLDOMElementPtr pXMLDOMElement = m_pXMLDOMNode;
		MSXML2::IXMLDOMAttributePtr pXMLDOMAttribute = pXMLDOMElement->getAttributeNode((_bstr_t)(LPCTSTR)attrName);
		if (pXMLDOMAttribute != NULL && VARIANT_TRUE == pXMLDOMAttribute->Getspecified())
		{
			bIsExist = TRUE;
		}
		else
		{
			bIsExist = FALSE;
		}
	}
	catch (_com_error& e)
	{
		ShowComExceptionWithReason(_T("XML Node IsAttributeExist() failed."),e);
		return FALSE;
	}
	catch (...)
	{
		ShowComExceptionUnknow(_T("XML Node IsAttributeExist() failed."));
		return FALSE;
	}

	return bIsExist;

}
BOOL CXMLDOMNode::AddAttributeNode(CString attrName)
{
	if (NULL == m_pXMLDOMNode)
	{
		return FALSE;
	}
	BOOL result = FALSE;
	try
	{
#ifdef _DEBUG
		ASSERT(MSXML2::NODE_ELEMENT == m_pXMLDOMNode->GetnodeType());
		CString elemName = m_pXMLDOMNode->GetnodeName();
#endif
		MSXML2::IXMLDOMElementPtr pXMLDOMElement = m_pXMLDOMNode;
		MSXML2::IXMLDOMDocument3Ptr pXMLDOMDocument = pXMLDOMElement->GetownerDocument();
		CString namespaceURI = _T("");
		if (pXMLDOMElement != NULL && pXMLDOMDocument != NULL)
		{
			MSXML2::IXMLDOMAttributePtr pXMLDOMAttribute = 
				pXMLDOMElement->getAttributeNode((_bstr_t)(LPCTSTR)attrName);
			if (pXMLDOMAttribute == NULL || VARIANT_FALSE == pXMLDOMAttribute->Getspecified())
			{
				/* 属性不存在，创建属性节点 */
				MSXML2::IXMLDOMAttributePtr pNewAttribute = NULL;
				if (pXMLDOMAttribute == NULL)
				{
					pNewAttribute = pXMLDOMDocument->createNode(MSXML2::NODE_ATTRIBUTE,
						(_bstr_t)(LPCTSTR)attrName,(_bstr_t)(LPCTSTR)namespaceURI);
				}
				else
				{
					CString defAttrValue = (LPCTSTR)(_bstr_t)pXMLDOMAttribute->GetnodeValue();
					pNewAttribute = pXMLDOMDocument->createNode(MSXML2::NODE_ATTRIBUTE,
						(_bstr_t)(LPCTSTR)attrName,(_bstr_t)(LPCTSTR)namespaceURI);
					pNewAttribute->PutnodeValue((_bstr_t)(LPCTSTR)defAttrValue);
				}
				if (pNewAttribute != NULL)
				{
					MSXML2::IXMLDOMAttributePtr pOldAttribute = 
						pXMLDOMElement->setAttributeNode(pNewAttribute);
					result = TRUE;
				}
				else
				{
					result = FALSE;
				}
			}
			else
			{
				/* 属性已经存在 */
				result = TRUE;
			}
		}
	}
	catch (_com_error& e)
	{
		ShowComExceptionWithReason(_T("XML Node AddAttributeNode() failed."),e);
		return FALSE;
	}
	catch (...)
	{
		ShowComExceptionUnknow(_T("XML Node AddAttributeNode() failed."));
		return FALSE;
	}
	if (result)
	{
		_SetDocumentModified();		/* 标记文档已被修改 */
	}
	return result;
}
BOOL CXMLDOMNode::AddAttributeNode(CString attrName,CString namespaceURI)
{
	if (NULL == m_pXMLDOMNode)
	{
		return FALSE;
	}
	BOOL result = FALSE;
	try
	{
#ifdef _DEBUG
		ASSERT(MSXML2::NODE_ELEMENT == m_pXMLDOMNode->GetnodeType());
		CString elemName = m_pXMLDOMNode->GetnodeName();
#endif
		MSXML2::IXMLDOMElementPtr pXMLDOMElement = m_pXMLDOMNode;
		MSXML2::IXMLDOMDocument3Ptr pXMLDOMDocument = pXMLDOMElement->GetownerDocument();
		if (pXMLDOMElement != NULL && pXMLDOMDocument != NULL)
		{
			MSXML2::IXMLDOMAttributePtr pXMLDOMAttribute = 
				pXMLDOMElement->getAttributeNode((_bstr_t)(LPCTSTR)attrName);
			if (pXMLDOMAttribute == NULL || VARIANT_FALSE == pXMLDOMAttribute->Getspecified())
			{
				/* 属性不存在，创建属性节点 */
				MSXML2::IXMLDOMAttributePtr pNewAttribute = NULL;
				if (pXMLDOMAttribute == NULL)
				{
					pNewAttribute = pXMLDOMDocument->createNode(MSXML2::NODE_ATTRIBUTE,
						(_bstr_t)(LPCTSTR)attrName,(_bstr_t)(LPCTSTR)namespaceURI);
				}
				else
				{
					CString defAttrValue = (LPCTSTR)(_bstr_t)pXMLDOMAttribute->GetnodeValue();
					pNewAttribute = pXMLDOMDocument->createNode(MSXML2::NODE_ATTRIBUTE,
						(_bstr_t)(LPCTSTR)attrName,(_bstr_t)(LPCTSTR)namespaceURI);
					pNewAttribute->PutnodeValue((_bstr_t)(LPCTSTR)defAttrValue);
				}
				if (pNewAttribute != NULL)
				{
					MSXML2::IXMLDOMAttributePtr pOldAttribute = 
						pXMLDOMElement->setAttributeNode(pNewAttribute);
					result = TRUE;
				}
				else
				{
					result = FALSE;
				}
			}
			else
			{
				/* 属性已经存在 */
				result = TRUE;
			}
		}
	}
	catch (_com_error& e)
	{
		ShowComExceptionWithReason(_T("XML Node AddAttributeNode() failed."),e);
		return FALSE;
	}
	catch (...)
	{
		ShowComExceptionUnknow(_T("XML Node AddAttributeNode() failed."));
		return FALSE;
	}
	if (result)
	{
		_SetDocumentModified();		/* 标记文档已被修改 */
	}
	return result;
}
BOOL CXMLDOMNode::AddAttributeNode(CString attrName,CString namespaceURI,CString attrValue)
{
	if (NULL == m_pXMLDOMNode)
	{
		return FALSE;
	}
	BOOL result = FALSE;
	try
	{
#ifdef _DEBUG
		ASSERT(MSXML2::NODE_ELEMENT == m_pXMLDOMNode->GetnodeType());
		CString elemName = m_pXMLDOMNode->GetnodeName();
#endif
		MSXML2::IXMLDOMElementPtr pXMLDOMElement = m_pXMLDOMNode;
		MSXML2::IXMLDOMDocument3Ptr pXMLDOMDocument = pXMLDOMElement->GetownerDocument();
		if (pXMLDOMElement != NULL && pXMLDOMDocument != NULL)
		{
			MSXML2::IXMLDOMAttributePtr pXMLDOMAttribute = 
				pXMLDOMElement->getAttributeNode((_bstr_t)(LPCTSTR)attrName);
			if (pXMLDOMAttribute == NULL || VARIANT_FALSE == pXMLDOMAttribute->Getspecified())
			{
				/* 属性不存在，创建属性节点 */
				MSXML2::IXMLDOMAttributePtr pNewAttribute = NULL;
				if (pXMLDOMAttribute == NULL)
				{
					pNewAttribute = pXMLDOMDocument->createNode(MSXML2::NODE_ATTRIBUTE,
						(_bstr_t)(LPCTSTR)attrName,(_bstr_t)(LPCTSTR)namespaceURI);
				}
				else
				{
					pNewAttribute = pXMLDOMDocument->createNode(MSXML2::NODE_ATTRIBUTE,
						(_bstr_t)(LPCTSTR)attrName,(_bstr_t)(LPCTSTR)namespaceURI);
				}
				pNewAttribute->PutnodeValue((_bstr_t)(LPCTSTR)attrValue);
				if (pNewAttribute != NULL)
				{
					MSXML2::IXMLDOMAttributePtr pOldAttribute = 
						pXMLDOMElement->setAttributeNode(pNewAttribute);
					result = TRUE;
				}
				else
				{
					result = FALSE;
				}
			}
			else
			{
				/* 属性已经存在 */
				result = TRUE;
			}
		}
	}
	catch (_com_error& e)
	{
		ShowComExceptionWithReason(_T("XML Node AddAttributeNode() failed."),e);
		return FALSE;
	}
	catch (...)
	{
		ShowComExceptionUnknow(_T("XML Node AddAttributeNode() failed."));
		return FALSE;
	}
	if (result)
	{
		_SetDocumentModified();		/* 标记文档已被修改 */
	}
	return result;
}
BOOL CXMLDOMNode::RemoveAttributeNode( CString attrName )
{
	if( NULL == m_pXMLDOMNode )
	{
		return FALSE;
	}
	BOOL result = FALSE;
	try
	{
#ifdef _DEBUG
		ASSERT(MSXML2::NODE_ELEMENT == m_pXMLDOMNode->GetnodeType());
		CString elemName = m_pXMLDOMNode->GetnodeName();
#endif
		MSXML2::IXMLDOMElementPtr pXMLDOMElement = m_pXMLDOMNode;
		if (pXMLDOMElement != NULL)
		{
			MSXML2::IXMLDOMAttributePtr pXMLDOMAttribute = 
				pXMLDOMElement->getAttributeNode((_bstr_t)(LPCTSTR)attrName);
			if (pXMLDOMAttribute != NULL 
				&& VARIANT_TRUE == pXMLDOMAttribute->Getspecified())
			{
#ifdef _DEBUG
				CString namespaceURI = (LPCTSTR)(_bstr_t)pXMLDOMAttribute->GetnamespaceURI();
#endif
				/* 属性存在，删除属性节点 */
				MSXML2::IXMLDOMAttributePtr pRemovedAttribute = 
					pXMLDOMElement->removeAttributeNode(pXMLDOMAttribute);
				result = TRUE;
			}
			else
			{
				/* 属性不存在 */
				result = TRUE;
			}
		}
	}
	catch (_com_error& e)
	{
		ShowComExceptionWithReason(_T("XML Node DelAttributeNode() failed."),e);
		return FALSE;
	}
	catch (...)
	{
		ShowComExceptionUnknow(_T("XML Node DelAttributeNode() failed."));
		return FALSE;
	}
	if (result == TRUE)
	{
		_SetDocumentModified();		/* 标记文档已被修改 */
	}
	return result;
}

CString CXMLDOMNode::GetAttributeValue(int index)
{
	CString attrValue;
	if( NULL == m_pXMLDOMNode )
	{
		return attrValue;
	}
	
	try
	{
#ifdef _DEBUG
		MSXML2::DOMNodeType nodeType = m_pXMLDOMNode->GetnodeType();
		ASSERT(nodeType == MSXML2::NODE_ELEMENT);
		CString nodeName = (LPCTSTR)m_pXMLDOMNode->GetnodeName();
#endif
		MSXML2::IXMLDOMNamedNodeMapPtr pIXMLDOMNamedNodeMap = m_pXMLDOMNode->Getattributes();
		MSXML2::IXMLDOMAttributePtr pIXMLDOMAttribute = NULL;
		int curIndex = 0;
		while (pIXMLDOMAttribute = pIXMLDOMNamedNodeMap->nextNode())
		{
			if (VARIANT_TRUE == pIXMLDOMAttribute->Getspecified()
				&& curIndex == index)
			{
				attrValue = (LPCTSTR)(_bstr_t)pIXMLDOMAttribute->Getvalue();
				break;
			}
			else if (VARIANT_TRUE == pIXMLDOMAttribute->Getspecified()
				&& curIndex != index)
			{
				curIndex++;
			}
		}
	}
	catch (_com_error& e)
	{
		ShowComExceptionWithReason(_T("XML Node GetAttributeValue() failed."),e);
		return attrValue;
	}
	catch (...)
	{
		ShowComExceptionUnknow(_T("XML Node GetAttributeValue() failed."));
		return attrValue;
	}

	return attrValue;
}
CString CXMLDOMNode::GetAttributeValue(CString attrName )
{
	CString attrValue;
	if(attrName.IsEmpty())
	{
		return attrValue;
	}
	if( NULL == m_pXMLDOMNode )
	{
		return attrValue;
	}
	try
	{
		//attrName = _T("certificate");
#ifdef _DEBUG
		MSXML2::DOMNodeType nodeType = m_pXMLDOMNode->GetnodeType();
		ASSERT(nodeType == MSXML2::NODE_ELEMENT);
		CString nodeName = (LPCTSTR)m_pXMLDOMNode->GetnodeName();
#endif
		MSXML2::IXMLDOMElementPtr pIXMLDOMElement = m_pXMLDOMNode;
		MSXML2::IXMLDOMAttributePtr pIXMLDOMAttribute = pIXMLDOMElement->
			getAttributeNode((_bstr_t)(LPCTSTR)attrName);
		if (pIXMLDOMAttribute != NULL && VARIANT_TRUE == pIXMLDOMAttribute->Getspecified())
		{
			attrValue = (LPCTSTR)(_bstr_t)pIXMLDOMAttribute->Getvalue();
		}
		//else if (pIXMLDOMAttribute != NULL && VARIANT_FALSE == pIXMLDOMAttribute->Getspecified())
		//{
		//	attrValue = (LPCTSTR)(_bstr_t)pIXMLDOMAttribute->Getvalue();
		//}
	}
	catch (_com_error& e)
	{
		ShowComExceptionWithReason(_T("XML Node GetAttributeValue() failed."),e);
		return attrValue;
	}
	catch (...)
	{
		ShowComExceptionUnknow(_T("XML Node GetAttributeValue() failed."));
		return attrValue;
	}
	return attrValue;
} 

BOOL CXMLDOMNode::SetAttributeValue(int index,CString attrValue)
{
	if (NULL == m_pXMLDOMNode || index < 0)
	{
		return FALSE;
	}
	BOOL result = FALSE;
	try
	{
#ifdef _DEBUG
		MSXML2::DOMNodeType nodeType = m_pXMLDOMNode->GetnodeType();
		ASSERT(nodeType == MSXML2::NODE_ELEMENT);
		CString nodeName = (LPCTSTR)m_pXMLDOMNode->GetnodeName();
#endif
		MSXML2::IXMLDOMElementPtr pXMLDOMElement = m_pXMLDOMNode;
		MSXML2::IXMLDOMNamedNodeMapPtr pXMLDOMAttributeMap = 
			pXMLDOMElement->Getattributes();
		long attrCount = pXMLDOMAttributeMap->Getlength();
		long curIndex = 0;
		MSXML2::IXMLDOMAttributePtr pXMLDOMAttribute = NULL;
		while ((pXMLDOMAttribute = pXMLDOMAttributeMap->nextNode()) != NULL)
		{
			if (VARIANT_TRUE == pXMLDOMAttribute->Getspecified())
			{
				if (curIndex == index)
				{
					pXMLDOMAttribute->Putvalue((_bstr_t)(LPCTSTR)attrValue);
					result = TRUE;
					break;
				}
				curIndex++;
			}
		}
	}
	catch (_com_error& e)
	{
		ShowComExceptionWithReason(_T("XML Node SetAttributeValue() failed."),e);
		return FALSE;
	}
	catch (...)
	{
		ShowComExceptionUnknow(_T("XML Node SetAttributeValue() failed."));
		return FALSE;
	}
	if (result = TRUE)
	{
		_SetDocumentModified();	/* 记录文档已修改 */
	}
	return result;
}
BOOL CXMLDOMNode::SetAttributeValue(CString attrName, CString attrValue)
{
	if( NULL == m_pXMLDOMNode || attrName.IsEmpty())
	{
		return FALSE;
	}
	BOOL result = FALSE;
	try
	{
#ifdef _DEBUG
		MSXML2::DOMNodeType nodeType = m_pXMLDOMNode->GetnodeType();
		ASSERT(nodeType == MSXML2::NODE_ELEMENT);
		CString nodeName = (LPCTSTR)m_pXMLDOMNode->GetnodeName();
#endif
		MSXML2::IXMLDOMElementPtr pXMLDOMElement = m_pXMLDOMNode;
		MSXML2::IXMLDOMAttributePtr pXMLDOMAttribute = 
			pXMLDOMElement->getAttributeNode((_bstr_t)(LPCTSTR)attrName);
		if (pXMLDOMAttribute != NULL 
			&& VARIANT_TRUE == pXMLDOMAttribute->Getspecified())
		{
			pXMLDOMAttribute->Putvalue((_bstr_t)(LPCTSTR)attrValue);
			result = TRUE;
		}
	}
	catch (_com_error& e)
	{
		ShowComExceptionWithReason(_T("XML Node SetAttributeValue() failed."),e);
		return FALSE;
	}
	catch (...)
	{
		ShowComExceptionUnknow(_T("XML Node SetAttributeValue() failed."));
		return FALSE;
	}
	if (result == TRUE)
	{
		_SetDocumentModified();		/* 记录文档已修改 */
	}
	return result;
}

BOOL CXMLDOMNode::ValidateAttribute(int index,XMLStringList& errorInfoList)
{
	errorInfoList.clear();
	if (m_pXMLDOMNode == NULL)
	{
		errorInfoList.push_back(_T("Element pointer of Document Object Model is NULL."));
		return FALSE;
	}
	try
	{
		MSXML2::DOMNodeType nodeType = m_pXMLDOMNode->GetnodeType();
		ASSERT(nodeType == MSXML2::NODE_ELEMENT);

		MSXML2::IXMLDOMElementPtr pXMLDOMElement = m_pXMLDOMNode;
		CString elemName = (LPCTSTR)(_bstr_t)pXMLDOMElement->GetnodeName();
		MSXML2::IXMLDOMDocument3Ptr pXMLDOMDocument = m_pXMLDOMNode->GetownerDocument();
		MSXML2::IXMLDOMAttributePtr pXMLDOMAttribute = NULL;
		MSXML2::IXMLDOMNamedNodeMapPtr pXMLDOMAttributeMap = 
			pXMLDOMElement->Getattributes();
		long attrCount = pXMLDOMAttributeMap->Getlength();
		int curIndex = 0;
		while ( (pXMLDOMAttribute = pXMLDOMAttributeMap->nextNode()) != NULL )
		{
			if (pXMLDOMAttribute->Getspecified() == VARIANT_TRUE)
			{
				if (curIndex == index)
				{
					break;
				}
				curIndex++;
			}
		}
		if (pXMLDOMAttribute == NULL || VARIANT_FALSE == pXMLDOMAttribute->Getspecified())
		{
			CString msg;
			msg.Format(_T("The element \"%s\" does not contain the attribute index \"%d\"."),
				elemName,index);
			errorInfoList.push_back(msg);
			return FALSE;
		}
		else
		{
			CString attrName = (LPCTSTR)(_bstr_t)pXMLDOMAttribute->GetnodeName();
			MSXML2::IXMLDOMParseError2Ptr pParseError = pXMLDOMDocument->validateNode(pXMLDOMAttribute);
			if (pParseError == NULL)
			{
				errorInfoList.push_back(_T("The attribute \"%s\" of element \"%s\" is valid."));
				return TRUE;
			}
			else
			{
				errorInfoList = ShowAttributeValidateError(elemName,attrName,pParseError);
				return FALSE;
			}
		}
	}
	catch (_com_error& e)
	{
		CString desc = ShowComExceptionWithReason(_T("XML Node ValidateAttribute() failed."),e);
		errorInfoList.push_back(desc);
		return FALSE;
	}
	catch (...)
	{
		CString desc = ShowComExceptionUnknow(_T("XML Node ValidateAttribute() failed."));
		errorInfoList.push_back(desc);
		return FALSE;
	}
}
BOOL CXMLDOMNode::ValidateAttribute(CString attrName,XMLStringList& errorInfoList)
{
	errorInfoList.clear();
	if (m_pXMLDOMNode == NULL)
	{
		errorInfoList.push_back(_T("Element pointer of Document Object Model is NULL."));
		return FALSE;
	}
	try
	{
		MSXML2::DOMNodeType nodeType = m_pXMLDOMNode->GetnodeType();
		ASSERT(nodeType == MSXML2::NODE_ELEMENT);

		MSXML2::IXMLDOMElementPtr pXMLDOMElement = m_pXMLDOMNode;
		CString elemName = (LPCTSTR)(_bstr_t)pXMLDOMElement->GetnodeName();
		MSXML2::IXMLDOMDocument3Ptr pXMLDOMDocument = m_pXMLDOMNode->GetownerDocument();
		MSXML2::IXMLDOMAttributePtr pXMLDOMAttribute = 
			pXMLDOMElement->getAttributeNode((_bstr_t)(LPCTSTR)attrName);
		if (pXMLDOMAttribute == NULL || VARIANT_FALSE == pXMLDOMAttribute->Getspecified())
		{
			CString msg;
			msg.Format(_T("The element \"%s\" does not contain the attribute \"%s\"."),
				elemName,attrName);
			errorInfoList.push_back(msg);
			return FALSE;
		}
		else
		{
			MSXML2::IXMLDOMParseErrorPtr pParseError = pXMLDOMDocument->validateNode(pXMLDOMAttribute);
			if (pParseError->GeterrorCode() == 0)
			{
				CString msg;
				msg.Format(_T("The attribute \"%s\" of element \"%s\" is valid."),
					attrName,elemName);
				errorInfoList.push_back(msg);
				return TRUE;
			}
			else
			{
				errorInfoList = ShowAttributeValidateError(elemName,attrName,pParseError);
				return FALSE;
			}
		}
	}
	catch (_com_error& e)
	{
		CString desc = ShowComExceptionWithReason(_T("XML Node ValidateAttribute() failed."),e);
		errorInfoList.push_back(desc);
		return FALSE;
	}
	catch (...)
	{
		CString desc = ShowComExceptionUnknow(_T("XML Node ValidateAttribute() failed."));
		errorInfoList.push_back(desc);
		return FALSE;
	}
}
/************************node value operation**********************************/
CString CXMLDOMNode::GetNodeValue( void )
{
	CString value = _GetValue();
	return value;
}
BOOL CXMLDOMNode::SetNodeValue( CString newValue )
{
	BOOL bSetSuccess = FALSE;
	if(newValue.IsEmpty())
	{
		bSetSuccess = _DeleteValue();
	}
	else
	{
		bSetSuccess = _SetValue(newValue);
	}
	return bSetSuccess;
}


CXMLSOMElementPtr CXMLDOMNode::GetElementConstraint()
{
	if (m_pXMLDOMNode == 0)
	{
		return NULL;
	}

	CXMLSOMElementPtr pXMLSOMElem = NULL;

	try
	{
		MSXML2::IXMLDOMDocument3Ptr pXMLDOMDocument = m_pXMLDOMNode->GetownerDocument();
		if (CXMLDOMFile::GetXMLFileSchemaFile(pXMLDOMDocument) != 0)
		{
			pXMLSOMElem = _getSchemaElementConstraint();
		}
	}
	catch (_com_error& e)
	{
		ShowComExceptionWithReason(_T("XML Node GetElementConstraint() failed."),e);
		return NULL;
	}
	catch (...)
	{
		ShowComExceptionUnknow(_T("XML Node GetElementConstraint() failed."));
		return NULL;
	}

	return pXMLSOMElem;
}
CXMLSOMAttributePtr CXMLDOMNode::GetAttributeConstraint(CString attrName)
{
	if (m_pXMLDOMNode == 0 || attrName.IsEmpty())
	{
		return NULL;
	}

	CXMLSOMAttributePtr pXMLSOMAttr = NULL;

	try
	{
		MSXML2::IXMLDOMDocument3Ptr pXMLDOMDocument = m_pXMLDOMNode->GetownerDocument();
		if (CXMLDOMFile::GetXMLFileSchemaFile(pXMLDOMDocument) != 0)
		{
			pXMLSOMAttr = _getSchemaAttributeConstraint(attrName);
		}
	}
	catch (_com_error& e)
	{
		ShowComExceptionWithReason(_T("XML Node GetAttributeConstraint() failed."),e);
		return NULL;
	}
	catch (...)
	{
		ShowComExceptionUnknow(_T("XML Node GetAttributeConstraint() failed."));
		return NULL;
	}

	return pXMLSOMAttr;
}
/******************************private function***************************/
CString CXMLDOMNode::_GetValue( void ) 
{
	CString value;
	if( NULL == m_pXMLDOMNode )
	{
		return value;
	}
	HRESULT hr;
	try
	{
#ifdef _DEBUG
		CString elemName = (LPCTSTR)(_bstr_t)m_pXMLDOMNode->GetnodeName();
#endif
		MSXML2::IXMLDOMElementPtr pXMLDOMElement = m_pXMLDOMNode;
		hr = pXMLDOMElement->normalize();
		TESTHR(hr);
		MSXML2::IXMLDOMNodeListPtr pXMLDOMNodeList = pXMLDOMElement->GetchildNodes();
		long len = pXMLDOMNodeList->Getlength();
		for (long i = 0; i < len ; i++)
		{
			MSXML2::IXMLDOMNodePtr pXMLDOMNode = pXMLDOMNodeList->Getitem(i);
			ASSERT(pXMLDOMNode != NULL);
			if(MSXML2::NODE_TEXT == pXMLDOMNode->GetnodeType())
			{
				value = (LPCTSTR)(_bstr_t)pXMLDOMNode->GetnodeValue();
				break;
			}
		}
	}
	catch (_com_error& e)
	{
		ShowComExceptionWithReason(_T("XML Node _GetValue() failed."),e);
		return value;
	}
	catch( ... )
	{
		ShowComExceptionUnknow(_T("XML Node _GetValue() failed."));
		return value;
	}

	return value;

}
BOOL CXMLDOMNode::_SetValue( CString value )
{
	if( NULL == m_pXMLDOMNode )
	{
		ASSERT(0);
		return FALSE;
	}
	BOOL result = FALSE;
	HRESULT hr;
	try
	{
		BOOL bTextExist = FALSE;
		MSXML2::IXMLDOMElementPtr pXMLDOMElement = m_pXMLDOMNode;
		hr = pXMLDOMElement->normalize();
		TESTHR(hr);

		MSXML2::IXMLDOMNodeListPtr pXMLDOMNodeList = pXMLDOMElement->GetchildNodes();
		long len = pXMLDOMNodeList->Getlength();
		for (long i = 0 ; i < len; i++)
		{
			MSXML2::IXMLDOMNodePtr pXMLDOMNode = pXMLDOMNodeList->Getitem(i);
			ASSERT(pXMLDOMNode != NULL);
			if (MSXML2::NODE_TEXT == pXMLDOMNode->GetnodeType())
			{
				pXMLDOMNode->PutnodeValue((_bstr_t)(LPCTSTR)value);
				bTextExist = TRUE;
				result = TRUE;
				break;
			}
		}
		if (bTextExist == FALSE)
		{
			MSXML2::IXMLDOMDocument3Ptr pXMLDOMDocument = m_pXMLDOMNode->GetownerDocument();
			MSXML2::IXMLDOMTextPtr pXMLDOMText = pXMLDOMDocument->createTextNode((_bstr_t)(LPCTSTR)value);
			MSXML2::IXMLDOMNodePtr pXMLDOMResult = m_pXMLDOMNode->insertBefore(pXMLDOMText,
				( _variant_t)( IDispatch*)m_pXMLDOMNode->GetfirstChild());
			ASSERT(pXMLDOMResult != NULL);
			result = TRUE;
		}
		hr = pXMLDOMElement->normalize();
		TESTHR(hr);
	}
	catch (_com_error& e)
	{
		ShowComExceptionWithReason(_T("XML Node _SetValue() failed."),e);
		return FALSE;
	}
	catch (...)
	{
		ShowComExceptionUnknow(_T("XML Node _SetValue() failed."));
		return FALSE;
	}
	if (result == TRUE)
	{
		_SetDocumentModified();
	}
	return result;
}
BOOL CXMLDOMNode::_DeleteValue( void )
{
	if( NULL == m_pXMLDOMNode )
	{
		ASSERT(0);
		return FALSE;
	}
	BOOL result = FALSE;
	HRESULT hr;
	try
	{
		BOOL bTextExist = FALSE;
		MSXML2::IXMLDOMElementPtr pXMLDOMElement = m_pXMLDOMNode;
		hr = pXMLDOMElement->normalize();
		TESTHR(hr);

		MSXML2::IXMLDOMNodeListPtr pXMLDOMNodeList = pXMLDOMElement->GetchildNodes();
		long len = pXMLDOMNodeList->Getlength();
		for (long i = 0 ; i < len; i++)
		{
			MSXML2::IXMLDOMNodePtr pXMLDOMNode = pXMLDOMNodeList->Getitem(i);
			ASSERT(pXMLDOMNode != NULL);
			if (MSXML2::NODE_TEXT == pXMLDOMNode->GetnodeType())
			{
				pXMLDOMElement->removeChild(pXMLDOMNode);
				bTextExist = TRUE;
				result = TRUE;
				break;
			}
		}
		if (bTextExist == FALSE)
		{
			result = TRUE;
		}
		hr = pXMLDOMElement->normalize();
		TESTHR(hr);
	}
	catch (_com_error& e)
	{
		ShowComExceptionWithReason(_T("XML Node _SetValue() failed."),e);
		return FALSE;
	}
	catch (...)
	{
		ShowComExceptionUnknow(_T("XML Node _SetValue() failed."));
		return FALSE;
	}
	if (result == TRUE)
	{
		_SetDocumentModified();
	}
	return result;
}

/************************************************************************
*																		
*  NAME:			_DoubleToString ( private function)                                                 
*  FUNCTION:		Convert double to string.( Maximum bit = 16 ( Excluding point )) 		                                  
*  PARAMETER:															
*					Input:	dToBeConvert - double value to be converted.	
*					Output: void.													
*  RETURN:	       	
*					Success - return String having been converted.
*					Fail	- return NULL;
*  CODER:			Songyongfeng													
*  DATE:			2007.11.27													   
*																								
************************************************************************/
TCHAR* CXMLDOMNode::_DoubleToString( const double& dToBeConvert )
{
#ifndef _CVTBUFSIZE
	const int _CVTBUFSIZE = MAX_PATH;
#endif
	static TCHAR* pRetString = NULL; // Return pointer to string.
	const int nNumDigits = 16; // The count of digits after converting.
	static TCHAR szBuffer[_CVTBUFSIZE]; // String buffer for converting.
	char  szTarget[_CVTBUFSIZE];
	memset( szBuffer, 0, sizeof( szBuffer ) );
	memset( szTarget, 0, sizeof( szTarget ) );
#ifdef _UNICODE
	OutputDebugString( TEXT( "\n_UNICODE have been defined!\n" ) );
	// Convert Wide to multi:
	/*int nLen1 = *///WideCharToMultiByte( CP_ACP, 0, szBuffer, -1, NULL, 0, NULL, NULL );
	_gcvt( dToBeConvert, nNumDigits, szTarget );

	// Convert multi to wide:
	int nLen = 0;
	nLen = MultiByteToWideChar( CP_ACP, 0, szTarget, -1, NULL, NULL );
	TCHAR* szWideTarget = new TCHAR[ nLen + 1 ];
	assert( NULL != szWideTarget );
	if( NULL == szWideTarget )
	{
		return NULL;
	}
	memset( szWideTarget, 0, sizeof( szWideTarget ) );
	MultiByteToWideChar( CP_ACP, 0, szTarget, -1, szWideTarget, nLen );

	_tcscpy( szBuffer, szWideTarget );

	if( NULL != szWideTarget )
	{
		delete[] szWideTarget;
	}	

#else // NO _UNICODE
	_gcvt( dToBeConvert, nNumDigits, szBuffer );
#endif

	assert( NULL != szBuffer );
	if( NULL == szBuffer )
	{
		return NULL;
	}

#ifdef _DEBUG
	// For test:
	TCHAR szDebugString[ MAX_PATH ];
	memset( szDebugString, 0, sizeof( szDebugString ) );
	_stprintf( szDebugString,TEXT( "\nBuffer: '%s' (total: %d chars)\n" ), szBuffer, _tcslen( szBuffer ) );
	OutputDebugString( szDebugString );
	//_tprintf(szDebugString);
	// End for test
#endif

	pRetString = szBuffer;
	return pRetString;

}


/************************************************************************
*																		
*  NAME:			_VariantToString ( private function)                                                 
*  FUNCTION:		Convert _variant_t to string. 		                                  
*  PARAMETER:															
*					Input:	var - _variant_t value to be converted.	
*					Output: void.													
*  RETURN:	       	
*					Success - return String having been converted.
*					Fail	- return TEXT("");
*  CODER:			Songyongfeng													
*  DATE:			2007.11.30													   
*																								
************************************************************************/
TCHAR* CXMLDOMNode::_VariantToString( const _variant_t& var )
{
	TCHAR szBuffer[MAX_PATH];   
	memset( szBuffer, 0, sizeof( szBuffer ) );

	switch( var.vt )   
	{   
	case   VT_EMPTY:   
	case   VT_NULL:   
	case   VT_DISPATCH:   
	case   VT_VARIANT:   
	case   VT_UI4:   
	case   VT_I1:   
	case   VT_UI2:   
	case   VT_SAFEARRAY:   
	case   VT_UNKNOWN:   
	case   VT_UINT:   
	case   VT_VOID:   
	case   VT_I8:   
	case   VT_HRESULT:   
	case   VT_ERROR:   
	case   VT_INT:   
	case   VT_PTR:   
	case   VT_UI8:   
	case   VT_CARRAY:   
	case   VT_USERDEFINED:   
	case   VT_LPWSTR:   
	case   VT_FILETIME:   
	case   VT_BLOB:   
	case   VT_LPSTR:   
	case   VT_STREAM:   
	case   VT_STORAGE:   
	case   VT_STREAMED_OBJECT:   
	case   VT_STORED_OBJECT:   
	case   VT_BLOB_OBJECT:   
	case   VT_CF:   
	case   VT_CLSID: 
		_tcscpy( szBuffer, TEXT("") );
		break;   
	case   VT_I2:   
		_stprintf( szBuffer, TEXT("%hd"), V_I2( &var ) );   
		break;   
	case   VT_I4:   
		_stprintf( szBuffer, TEXT("%d"), V_I4( &var ) );   
		break;   
	case   VT_R4:   
		_stprintf( szBuffer, TEXT("%e"), ( double )V_R4( &var ) );   
		break;   
	case   VT_R8:   
		_stprintf( szBuffer, TEXT("%e"), V_R8( &var ) );   
		break;  

		//////////////////////////////////////////////////////////////////////////
		// 		//For MFC
		// 	case   VT_CY:   
		// 		_stprintf( szBuffer, TEXT("%s"),COleCurrency(var).Format() );   
		// 		break;   
		// 	case   VT_DATE:   
		// 		_stprintf( szBuffer, TEXT("%s"),COleDateTime(var).Format( TEXT("%m-%d-%y") ) );   
		// 		break;   
		//////////////////////////////////////////////////////////////////////////
	case   VT_BSTR:   
		_tcscpy( szBuffer,( TCHAR* )(_bstr_t)V_BSTR( &var ) );//
		break;   
	case   VT_BOOL:   
		_stprintf( szBuffer, TEXT("%s"), ( V_BOOL(&var) ? TEXT("true") : TEXT("false") ) ); 
		break;
	case   VT_UI1:   
		_stprintf( szBuffer, TEXT("0x%02hX"), ( unsigned short )V_UI1( &var ) );   
		break;   
	default:   
		_tcscpy( szBuffer, TEXT("") );   
		break;
	}

	TCHAR* pszRetString=NULL;
	pszRetString = szBuffer;

	return   pszRetString; 

}

void CXMLDOMNode::_SetDocumentModified()
{
	if (m_pXMLDOMNode == 0)
	{
		ASSERT(0);
		return ;
	}
	try
	{
		MSXML2::IXMLDOMDocument3Ptr pXMLDOMDocument = m_pXMLDOMNode->GetownerDocument();
		ASSERT(pXMLDOMDocument != NULL);
		CXMLDOMFile::SetXMLFileModified(pXMLDOMDocument,TRUE);
	}
	catch (_com_error& e)
	{
		ShowComExceptionWithReason(_T("XML Node _SetDocumentModified() failed."),e);
		return ;
	}
	catch (...)
	{
		ShowComExceptionUnknow(_T("XML Node _SetDocumentModified() failed."));
		return ;
	}
	return; 
}

CXMLSOMElementPtr CXMLDOMNode::_getSchemaElementConstraint()
{
	try
	{
		m_pXMLSOMElem.reset(new CXMLSOMElement());
		MSXML2::IXMLDOMElementPtr pXMLDOMElement = m_pXMLDOMNode;
		if(!m_pXMLSOMElem->InitConstraint(pXMLDOMElement))
		{
			/* 解析失败 */
			m_pXMLSOMElem.reset();
		}
		return m_pXMLSOMElem;
	}
	catch (_com_error& e)
	{
		ShowComExceptionWithReason(_T("XML DOM _getSchemaElementConstraint() failed."),e);
		return NULL;
	}
	catch (...)
	{
		ShowComExceptionUnknow(_T("XML DOM _getSchemaElementConstraint() failed."));
		return NULL;
	}
}
CXMLSOMAttributePtr CXMLDOMNode::_getSchemaAttributeConstraint(CString attrName)
{
	try
	{
		CXMLSOMAttributePtr pXMLSOMAttr(new CXMLSOMAttribute());
		MSXML2::IXMLDOMElementPtr pXMLDOMElement = m_pXMLDOMNode;
		MSXML2::IXMLDOMAttributePtr pXMLDOMAttribute = 
			pXMLDOMElement->getAttributeNode((_bstr_t)(LPCTSTR)attrName);
		if(!pXMLSOMAttr->InitConstraint(pXMLDOMElement,pXMLDOMAttribute))
		{
			/* 解析失败 */
			pXMLSOMAttr.reset();
		}
		return pXMLSOMAttr;
	}
	catch (_com_error& e)
	{
		ShowComExceptionWithReason(_T("XML DOM _getSchemaAttributeConstraint() failed."),e);
		return NULL;
	}
	catch (...)
	{
		ShowComExceptionUnknow(_T("XML DOM _getSchemaAttributeConstraint() failed."));
		return NULL;
	}
}

