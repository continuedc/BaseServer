#pragma	warning(disable:4786)
#include "SessionList.h"
#include "Session.h"
MYHEAP_IMPLEMENTATION(SessionList, s_heap);

SessionList::SessionList()
{
	InitializeCriticalSection( &m_cs );
}

SessionList::~SessionList()
{
	Clear();
	DeleteCriticalSection( &m_cs );
}

VOID SessionList::Clear()
{
	SESSION_LIST_ITER it;

	Session *pSession;

	Lock();
	for( it = begin(); it != end(); ++it )
	{
		pSession = *it;
		delete pSession;
	}
	Unlock();

	clear();
}
