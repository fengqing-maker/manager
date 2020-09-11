// SAXXSDErrorHandler.h: derived class from SAXErrorHandlerImpl

#if !defined(AFX_SAXXSDERRORHANDLER_H__BD78BDE9_488F_4C50_B770_CF6B2D50B845__INCLUDED_)
#define AFX_SAXXSDERRORHANDLER_H__BD78BDE9_488F_4C50_B770_CF6B2D50B845__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "SAXErrorHandlerImpl.h"

class SAXXSDErrorHandler : public SAXErrorHandlerImpl 
{
public:
	SAXXSDErrorHandler();
	virtual ~SAXXSDErrorHandler();
	static SAXXSDErrorHandler * CreateInstance();


    virtual HRESULT STDMETHODCALLTYPE error( 
            /* [in] */ ISAXLocator *pLocator,
            /* [in] */ wchar_t *pwchErrorMessage,
            /* [in] */ HRESULT hrErrorCode);

      virtual HRESULT STDMETHODCALLTYPE fatalError( 
            /* [in] */ ISAXLocator *pLocator,
            /* [in] */ wchar_t *pwchErrorMessage,
            /* [in] */ HRESULT hrErrorCode) ;

     virtual HRESULT STDMETHODCALLTYPE ignorableWarning( 
            /* [in] */ ISAXLocator *pLocator,
            /* [in] */ wchar_t *pwchErrorMessage,
            /* [in] */ HRESULT hrErrorCode) ;

	 virtual HRESULT __stdcall raw_error (
	 /*[in]*/ struct ISAXLocator * pLocator,
		 /*[in]*/ unsigned short * pwchErrorMessage,
		 /*[in]*/ HRESULT hrErrorCode );
	 virtual HRESULT __stdcall raw_fatalError (
	 /*[in]*/ struct ISAXLocator * pLocator,
		 /*[in]*/ unsigned short * pwchErrorMessage,
		 /*[in]*/ HRESULT hrErrorCode );
	 virtual HRESULT __stdcall raw_ignorableWarning (
	 /*[in]*/ struct ISAXLocator * pLocator,
		 /*[in]*/ unsigned short * pwchErrorMessage,
		 /*[in]*/ HRESULT hrErrorCode );
};



#endif // !defined(AFX_SAXXSDERRORHANDLER_H__BD78BDE9_488F_4C50_B770_CF6B2D50B845__INCLUDED_)

        
       
