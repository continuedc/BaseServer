////////////////////////////////////////////////////////////////////////
// Copyright(c) 2011-2014, WuHan ChuangYou, All Rights Reserved
// Moudle: MsgLoginKickReason.h
// Author: ������(Peng Wenqi)
// Created: 2014-11-4
////////////////////////////////////////////////////////////////////////
#ifndef _GAMESERVER_MSG_LOGIN_KICK_REASON_H_
#define _GAMESERVER_MSG_LOGIN_KICK_REASON_H_


#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "NetMsg.h"

class CMsgLoginKickReason : public login_kernel::CNetMsg  
{
public:
	CMsgLoginKickReason();
	virtual ~CMsgLoginKickReason();
	virtual BOOL Create(char* pMsgBuf,DWORD dwSize);
	virtual void Process(void* pInfo){};

	bool CreateInfo(int nReason, int nParam = 0);
private:
	typedef struct
	{
		MSG_NUM_4BIT	nReason;
		MSG_NUM_4BIT	nParam;
	}MSG_Info;
	
	MSG_Info* m_pInfo;
};

#endif	// end of _GAMESERVER_MSG_LOGIN_KICK_REASON_H_