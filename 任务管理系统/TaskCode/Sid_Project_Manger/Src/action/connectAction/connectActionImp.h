#ifndef CONNECT_ACTION_IMP_H_345
#define CONNECT_ACTION_IMP_H_345

#include "../inc/connectDbseverInterface.h"

class CConectDbImp:public CConnectDbServerInc
{
public:
	CConectDbImp( CDaoSupportInterface *pDao );

	virtual  BOOL IsConect();
	virtual  BOOL Connect();
	virtual  void CloseConnect() ;
protected:
	DWORD         m_ip; //Êý¾Ý¿âIpµØÖ·
	CString       m_dbName;
};

#endif
