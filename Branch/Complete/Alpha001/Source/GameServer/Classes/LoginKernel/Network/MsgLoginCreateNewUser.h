////////////////////////////////////////////////////////////////////////
// Copyright(c) 2011-2014, WuHan ChuangYou, All Rights Reserved
// Moudle: MsgLoginCreateNewUser.h
// Author: 彭文奇(Peng Wenqi)
// Created: 2014-11-4
////////////////////////////////////////////////////////////////////////
#ifndef _GAMESERVER_MSG_LOGIN_CREATE_NEW_USER_H_
#define _GAMESERVER_MSG_LOGIN_CREATE_NEW_USER_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "./NetMsg.h"

enum MSG_LOGIN_CREATE_NEW_USER_RESULT
{
	MSG_LOGIN_CREATE_NEW_USER_RESULT_SUCCESS		= 0,
	MSG_LOGIN_CREATE_NEW_USER_RESULT_FAIL			= 1,	// 通用失败
	MSG_LOGIN_CREATE_NEW_USER_RESULT_NAME_CHECK		= 2,	// 名字过不了服务端敏感字检查
	MSG_LOGIN_CREATE_NEW_USER_RESULT_NAME_REPEAT	= 3,	// 角色名重复
	MSG_LOGIN_CREATE_NEW_USER_RESULT_COUNT_FULL		= 4,	// 角色到达上限
};

class CMsgLoginCreateNewUser : public login_kernel::CNetMsg  
{
public:
	CMsgLoginCreateNewUser();
	virtual ~CMsgLoginCreateNewUser();
	virtual BOOL Create(char* pMsgBuf,DWORD dwSize);
	virtual void Process(void* pInfo);

private:
	typedef struct
	{
		MSG_NUM_4BIT	nResult;

		MSG_STR_32BIT	szName;
		MSG_NUM_4BIT	nCamp;
		MSG_NUM_4BIT	nFlagShip;
		MSG_NUM_4BIT	nUserID;
	}MSG_Info;

	MSG_Info* m_pInfo;
};


#endif // end of _GAMESERVER_MSG_LOGIN_CREATE_NEW_USER_H_