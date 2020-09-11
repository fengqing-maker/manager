#pragma once
#include "Src/Tools/SingletonTemplate/SingletonTemplate.h"
#include "systemOutput/SvrOutputManager.h"

class ClientOutputManager : public SingletonS<ClientOutputManager>
{
	friend class SingletonS<ClientOutputManager>;
public:
	~ClientOutputManager(void);

	void	OutputMsg(CString msg);		
	void	OutputErr(CString err);		
	void    OutputUpdateMsg( int taskMsgType,  int taskType,int id, CString extText = _T("") );

protected:
	ClientOutputManager(void);

};

