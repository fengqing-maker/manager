#ifndef CONNECT_DBSERVER_INTERFACE_H
#define CONNECT_DBSERVER_INTERFACE_H

#include "../../DataBase/inc/DaoSupportInterface.h"

class CConnectDbServerInc
{
public:
	CConnectDbServerInc( CDaoSupportInterface *pDao ){ m_pDaoBase = pDao;}
	virtual ~CConnectDbServerInc(){}

	virtual  BOOL IsConect() = 0;
	virtual  BOOL Connect() = 0;
	virtual  void CloseConnect() = 0;
protected:
	CDaoSupportInterface *m_pDaoBase;
};

#endif
