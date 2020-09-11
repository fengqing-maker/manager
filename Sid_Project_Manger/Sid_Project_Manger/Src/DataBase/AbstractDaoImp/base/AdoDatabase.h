// AdoDatabase.h: interface for the CAdoDatabase class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ADODATABASE_H__48934634_A186_45FB_AD52_5692A8C5E26C__INCLUDED_)
#define AFX_ADODATABASE_H__48934634_A186_45FB_AD52_5692A8C5E26C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#pragma once

#include "icrsint.h"
#include <shlwapi.h>
#include <atlstr.h>
#include <ATLComTime.h> 

#pragma  warning(disable:4800)

#import "msadox.dll" named_guids rename_namespace("ADOCG") rename("EOS","AdoEOS") rename("EOF", "adoXEOF")\
	rename("DataTypeEnum","adoXDataTypeEnum") rename("FieldAttributeEnum", "adoFielAttributeEnum")
#import "msado15.dll" rename("EOF","adoEOF") rename("DataTypeEnum","adoDataTypeEnum")
using namespace ADOCG;
#import "MSJRO.DLL"
using namespace ADODB;


//using namespace ADOX;
//using namespace ADODB;
//using namespace  ADOCG;
//-----------------------------------------------------
// ��������
//-----------------------------------------------------
CString vartostr(const _variant_t &var);
class CAdoDatabase
{
private:
	bool m_bIsOpen;
	void Init();
public:
	bool Open(const CString & strConn,long nOption);
	//   Function   name   :   RunSqlScript   
	//   Description��:   ִ��SQL�ű�,   peckermen@163.com,   2003-09-15
	//   Return   type��:   BOOL   �ɹ�����TRUE 
	//   Argument��   :   LPCSTR   SqlScriptFile   SQL�ű��ļ��� 
	//   Argument��   :   char   separator   SQL   �ָ����,   ȱʡ';'     
    //   Argument��   :   char   Remark����SQL   ע�ͷ���,   ȱʡ'-' 
	static BOOL RunSqlScript(_ConnectionPtr conn,LPCSTR sqlScriptFile,char separator=';',char Remark='-');
	//---------- Members -------------------
	_ConnectionPtr m_lpConnection; 
	CString ConnectString;
	//-------------------------------------- 
	
	CAdoDatabase(void);
	CAdoDatabase(const char* lpConnStr);
	CAdoDatabase(const CString & szConnStr);  
	
	bool IsOpen();
	bool Open(const char* lpConnStr);
	bool Open(const CString & szConnStr);
	bool Open();
	bool OpenMSSQL(const char* lpServer, const char* lpUserID, const char* lpPassword, const char* lpDataBase);
	bool OpenOracle(const char* lpDataSource, const char* lpUserID, const char* lpPassword);
	bool OpenAccess(const char* lpDbFile, const char* lpPassword = "");
	bool OpenExcel(const char * lpFilePath);  //��excel�����ݿ�
	ADODB::_RecordsetPtr Execute(CString sql, long* recordsAffected = NULL); 
	
	
	//----------��ɲ���-------------------
	~CAdoDatabase(void);
	void Close();
	void Release();
	//------------------------------------- 
	
	void BeginTrans()   { m_lpConnection->BeginTrans();   }
	void CommitTrans()  { m_lpConnection->CommitTrans();  }
	void RollbackTrans(){ m_lpConnection->RollbackTrans();}  

}; 


class DbField : public _variant_t
{ 
public: 
	
	// -- Members ---------------
	ADODB::FieldPtr m_lpField;
	//--------------------------- 
	DbField(ADODB::FieldPtr& lpField) :_variant_t(lpField->Value)
	{
		m_lpField = lpField;        
	} 
	DbField()
	{
		
	} 
	void Attach(ADODB::FieldPtr& lpField)
	{
		m_lpField = lpField; 
		_variant_t::Attach(m_lpField->Value);  
	} 
	
	bool IsNull()
	{
		return (m_lpField->Value.vt == VT_NULL);
	} 
	
	CString GetName()
	{ 
	/*	CString s = (char *)m_lpField->GetName();  */ 
		CString s (_com_util::ConvertBSTRToString((_bstr_t) m_lpField->GetName()));
		return s;
	} 
	
	long GetActualSize()
	{
		return m_lpField->ActualSize;
	} 
	
	CString ToString()
	{
		return vartostr(m_lpField->Value);
	}
	
	~DbField()
	{
		
	} 
	
    void operator=(_variant_t& value)   { m_lpField->put_Value(_variant_t(value)); }
	void operator=(int &value)     { m_lpField->put_Value(_variant_t((long)value)); }
	void operator=(const int &value)   { m_lpField->put_Value(_variant_t((long)value)); }
	void operator=(unsigned int &value)   { m_lpField->put_Value(_variant_t((long)value)); }
	void operator=(const unsigned int &value) { m_lpField->put_Value(_variant_t((long)value)); }
	void operator=(long &value)     { m_lpField->put_Value(_variant_t(value)); }
	void operator=(const long &value)   { m_lpField->put_Value(_variant_t(value)); }
	void operator=(const unsigned long &value) { m_lpField->put_Value(_variant_t((long)value)); }
	void operator=(unsigned long &value)  { m_lpField->put_Value(_variant_t((long)value)); }
	void operator=(float &value)    { m_lpField->put_Value(_variant_t(value)); }
	void operator=(const float &value)   { m_lpField->put_Value(_variant_t(value)); }
	void operator=(double &value)    { m_lpField->put_Value(_variant_t(value)); }
	void operator=(const double &value)   { m_lpField->put_Value(_variant_t(value)); }
	void operator=(char* value)     { m_lpField->put_Value(_variant_t(value)); }
	void operator=(const char* value)   { m_lpField->put_Value(_variant_t(value)); }
	void operator=(const unsigned char* puchar) { m_lpField->put_Value(_variant_t(puchar)); } 
	//�ַ�������,д������Ҳ����ֱ��д���ַ�������
	void operator=( const CString &value)
	{
		m_lpField->put_Value(_variant_t(value));
	} 
	
	//д������
	void operator=(COleDateTime &dt)
	{
		CString strDate = dt.Format(_T("%Y-%m-%d %H:%M:%S"));
		m_lpField->put_Value(_variant_t(strDate));
	} 
	void operator=(CTime &dt)
	{
		CString strDate = dt.Format(_T("%Y-%m-%d %H:%M:%S"));
		m_lpField->put_Value(_variant_t(strDate));
	}
	//���ֶ�׷�Ӷ���������
	bool AppendBinary(LPVOID lpData, UINT nBytes)
	{
		SAFEARRAY FAR *pSafeArray = NULL;
		SAFEARRAYBOUND rgsabound[1]; 
		try
		{
			rgsabound[0].lLbound = 0;
			rgsabound[0].cElements = nBytes;
			pSafeArray = SafeArrayCreate(VT_UI1, 1, rgsabound); 
			
			for (long i = 0; i < (long)nBytes; i++)
			{
				UCHAR &chData = ((UCHAR*)lpData)[i];
				HRESULT hr = SafeArrayPutElement(pSafeArray, &i, &chData);
				if (FAILED(hr)) return FALSE;
			} 
			
			_variant_t varChunk;
			varChunk.vt = VT_ARRAY | VT_UI1;
			varChunk.parray = pSafeArray; 
			
			return (m_lpField->AppendChunk(varChunk) == S_OK);
		}
		catch (...)
		{   
			return false;
		}
	} 
	
	//ȡ������������
	bool GetBinary(LPVOID lpData,UINT& nBytes)
	{
		ASSERT(m_lpField != NULL);
		ASSERT(lpData != NULL); 
		UINT reallength=m_lpField->ActualSize;
		if(reallength<0)
		{
			nBytes=0;
			return false;
		}
		if(nBytes<reallength)
		{
			return false;
		}
		memset(lpData,0,nBytes);
		nBytes=reallength;
		BYTE* pBuf=NULL;
		try
		{
			_variant_t varChunk=m_lpField->GetChunk(nBytes);
			if(varChunk.vt!=(VT_UI1|VT_ARRAY))
			{
				return false;
			}
			SafeArrayAccessData(varChunk.parray,(void **)&pBuf);
			memcpy(lpData,pBuf,nBytes);
			SafeArrayUnaccessData(varChunk.parray);
		}
		catch (_com_error & /*e*/)
		{
			return false;
		}
		return true;
	} 
	operator CTime() const
	{
		ASSERT(vt==VT_DATE);
		COleDateTime date(date);
		CTime re(date.GetYear(),date.GetMonth(),date.GetDay(),date.GetHour(),date.GetMinute(),date.GetSecond());
		return re;
	}
	operator CString() const
	{
		return vartostr(m_lpField->GetValue());
	}
}; 

#endif // !defined(AFX_ADODATABASE_H__48934634_A186_45FB_AD52_5692A8C5E26C__INCLUDED_)
