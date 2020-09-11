 // SAXXSDErrorHandler.cpp: derived class from SAXErrorHandlerImpl


#include "stdafx.h"
#include "SAXXSDErrorHandler.h"

SAXXSDErrorHandler::SAXXSDErrorHandler()
{
}

SAXXSDErrorHandler::~SAXXSDErrorHandler()
{
	//object destruction is handled by the Release() impl of parent class
}

//hides the new operator
SAXXSDErrorHandler * SAXXSDErrorHandler::CreateInstance()
{
	SAXXSDErrorHandler * pSAXXSDErrorHandler = NULL;
	pSAXXSDErrorHandler = new SAXXSDErrorHandler;
	ASSERT(pSAXXSDErrorHandler!=NULL);
	return pSAXXSDErrorHandler;
}

HRESULT STDMETHODCALLTYPE SAXXSDErrorHandler::error( 
			/* [in] */ ISAXLocator *pLocator,
            /* [in] */ wchar_t *pwchErrorMessage,
            /* [in] */ HRESULT hrErrorCode)
{
	int m = 0, n = 0;

	//HRESULT hr = pLocator->getLineNumber(&m);
	//hr = pLocator->getColumnNumber(&n);
    m = pLocator->getLineNumber();
	n = pLocator->getColumnNumber();
	wprintf(L"\n %ld \n line : %ld column : %ld\n  %s\n", hrErrorCode,m, n, pwchErrorMessage);
    printf("\n");
	// return S_OK to continue the parsing
	return S_OK;
}

HRESULT STDMETHODCALLTYPE SAXXSDErrorHandler::fatalError( 
		    /* [in] */ ISAXLocator *pLocator,
            /* [in] */ wchar_t *pwchErrorMessage,
            /* [in] */ HRESULT hrErrorCode)
{
	CString errorInfo;
	errorInfo.Format("\n fatal Error %ld \n line : %d column : %d\n  %s\n", hrErrorCode,pLocator->getLineNumber(), pLocator->getColumnNumber(), pwchErrorMessage);
    m_strErrorInfo += errorInfo;
	return hrErrorCode;
}

HRESULT STDMETHODCALLTYPE SAXXSDErrorHandler::ignorableWarning( 
			/* [in] */ ISAXLocator *pLocator,
            /* [in] */ wchar_t *pwchErrorMessage,
            /* [in] */ HRESULT hrErrorCode)
{
	CString errorInfo;
	errorInfo.Format("\n %ld \n line : %d column : %d\n  %s\n", hrErrorCode,pLocator->getLineNumber(), pLocator->getColumnNumber(), pwchErrorMessage);
   	m_strErrorInfo += errorInfo;
	return S_OK;
}

HRESULT __stdcall SAXXSDErrorHandler::raw_error (
/*[in]*/ struct ISAXLocator * pLocator,
	/*[in]*/ unsigned short * pwchErrorMessage,
	/*[in]*/ HRESULT hrErrorCode ) 
{
	CString temp;
    temp.Format("\n %ld \n line : %d column : %d\n  %s\n", hrErrorCode,pLocator->getLineNumber(), pLocator->getColumnNumber(), pwchErrorMessage);
    /*return S_OK;*/
	return hrErrorCode;
}

HRESULT __stdcall SAXXSDErrorHandler::raw_fatalError (
/*[in]*/ struct ISAXLocator * pLocator,
	/*[in]*/ unsigned short * pwchErrorMessage,
	/*[in]*/ HRESULT hrErrorCode )
{
	CString temp;
	temp.Format("\n %ld \n line : %d column : %d\n  %s\n", hrErrorCode,pLocator->getLineNumber(), pLocator->getColumnNumber(), pwchErrorMessage);
	/* return S_OK;*/
   return hrErrorCode;
}
HRESULT __stdcall SAXXSDErrorHandler::raw_ignorableWarning (
/*[in]*/ struct ISAXLocator * pLocator,
	/*[in]*/ unsigned short * pwchErrorMessage,
	/*[in]*/ HRESULT hrErrorCode ) 
{
	CString temp;
	temp.Format("\n %ld \n line : %d column : %d\n  %s\n", hrErrorCode,pLocator->getLineNumber(), pLocator->getColumnNumber(), pwchErrorMessage);
	/*return S_OK;*/
   return hrErrorCode;
}