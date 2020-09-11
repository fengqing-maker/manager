#ifndef SID_IEDCONFIG_INCLUDE_MYXMLERROREHANDLER_HEADER_47865
#define SID_IEDCONFIG_INCLUDE_MYXMLERROREHANDLER_HEADER_47865

#include <xercesc/sax/ErrorHandler.hpp>
#include <xercesc/sax/SAXParseException.hpp>

XERCES_CPP_NAMESPACE_USE
#include <list>
using namespace std;
class  CDOMTreeErrorReporter:public ErrorHandler    
{
public:
	CDOMTreeErrorReporter(list<CString> *outPutStrList);
	~CDOMTreeErrorReporter();
	virtual void warning(const SAXParseException& toCatch);
	virtual void resetErrors(){};

	virtual void error(const SAXParseException& toCatch);
	virtual void fatalError(const SAXParseException& toCatch);
	list<CString> *m_outPutStrList;
	bool    m_bError;
protected:
private:
};

#endif

