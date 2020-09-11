// SAXErrorHandler.cpp: : implementation class for ISAXErrorHandler interface

#include "stdafx.h"
#include "SAXErrorHandlerImpl.h"
#include <stdio.h>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CString SAXErrorHandlerImpl::m_strErrorInfo;
SAXErrorHandlerImpl::SAXErrorHandlerImpl()
{
	m_refCnt=0;
}

SAXErrorHandlerImpl::~SAXErrorHandlerImpl()
{
}

HRESULT STDMETHODCALLTYPE SAXErrorHandlerImpl::error( 
            /* [in] */ ISAXLocator *pLocator,
            /* [in] */ wchar_t *pwchErrorMessage,
            /* [in] */ HRESULT hrErrorCode)
{
	return S_OK;
}
        
HRESULT STDMETHODCALLTYPE SAXErrorHandlerImpl::fatalError( 
		    /* [in] */ ISAXLocator *pLocator,
            /* [in] */ wchar_t *pwchErrorMessage,
            /* [in] */ HRESULT hrErrorCode)
{
	return hrErrorCode;
}
        
HRESULT STDMETHODCALLTYPE SAXErrorHandlerImpl::ignorableWarning( 
			/* [in] */ ISAXLocator *pLocator,
            /* [in] */ wchar_t *pwchErrorMessage,
            /* [in] */ HRESULT hrErrorCode)
{
	return S_OK;
}

long __stdcall SAXErrorHandlerImpl::QueryInterface(const struct _GUID &riid,void ** ppvObject)
{
	*ppvObject = NULL;
	if (riid == IID_IUnknown ||riid == __uuidof(ISAXErrorHandler))
	{
		*ppvObject = static_cast<ISAXErrorHandler *>(this);
	}
	
	if (*ppvObject)
	{
		AddRef();
		return S_OK;
	}	
	else return E_NOINTERFACE;
}

unsigned long __stdcall SAXErrorHandlerImpl::AddRef()
{
	return ++m_refCnt; // NOT thread-safe
}


unsigned long __stdcall SAXErrorHandlerImpl::Release()
{
   --m_refCnt; // NOT thread-safe
   if (m_refCnt == 0) {
      delete this;
      return 0; // Can't return the member of a deleted object.
   }
   else return m_refCnt;
}

