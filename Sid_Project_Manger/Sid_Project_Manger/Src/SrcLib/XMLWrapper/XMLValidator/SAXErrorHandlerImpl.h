// SAXErrorHandler.h: implementation class for ISAXErrorHandler interface

#if !defined(AFX_SAXERRORHANDLERIMPL_H__FBFF0150_5FE6_44B0_9E33_3E6A8B9B613F__INCLUDED_)
#define AFX_SAXERRORHANDLERIMPL_H__FBFF0150_5FE6_44B0_9E33_3E6A8B9B613F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
using namespace MSXML2;
class SAXErrorHandlerImpl : public ISAXErrorHandler  
{
public:
	SAXErrorHandlerImpl();
	virtual ~SAXErrorHandlerImpl();
    
	// This must be correctly implemented, if your handler must be a COM Object 
	// the current implementation is NOT thread-safe
	long __stdcall QueryInterface(const struct _GUID &riid,void ** ppvObject);
	unsigned long __stdcall AddRef(void);
		unsigned long __stdcall Release(void);

        virtual HRESULT STDMETHODCALLTYPE error( 
            /* [in] */ ISAXLocator *pLocator,
            /* [in] */ wchar_t *pwchErrorMessage,
            /* [in] */ HRESULT hrErrorCode);
        
        virtual HRESULT STDMETHODCALLTYPE fatalError( 
            /* [in] */ ISAXLocator *pLocator,
            /* [in] */ wchar_t *pwchErrorMessage,
            /* [in] */ HRESULT hrErrorCode);
        
        virtual HRESULT STDMETHODCALLTYPE ignorableWarning( 
            /* [in] */ ISAXLocator *pLocator,
            /* [in] */ wchar_t *pwchErrorMessage,
            /* [in] */ HRESULT hrErrorCode);
        
		virtual HRESULT __stdcall raw_error (
		/*[in]*/ struct ISAXLocator * pLocator,
			/*[in]*/ unsigned short * pwchErrorMessage,
			/*[in]*/ HRESULT hrErrorCode ) = 0;
		virtual HRESULT __stdcall raw_fatalError (
		/*[in]*/ struct ISAXLocator * pLocator,
			/*[in]*/ unsigned short * pwchErrorMessage,
			/*[in]*/ HRESULT hrErrorCode ) = 0;
		virtual HRESULT __stdcall raw_ignorableWarning (
		/*[in]*/ struct ISAXLocator * pLocator,
			/*[in]*/ unsigned short * pwchErrorMessage,
			/*[in]*/ HRESULT hrErrorCode ) = 0;
public:
	static CString m_strErrorInfo;
private:
	ULONG m_refCnt;
};

#endif // !defined(AFX_SAXERRORHANDLERIMPL_H__FBFF0150_5FE6_44B0_9E33_3E6A8B9B613F__INCLUDED_)
