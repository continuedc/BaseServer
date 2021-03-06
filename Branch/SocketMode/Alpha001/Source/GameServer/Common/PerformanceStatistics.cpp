//////////////////////////////////////////////////////////////////////////
// Copyright(c) 2011-2012, WuHan ChuangYou, All Rights Reserved
// Moudle:	PerformanceStatistics.cpp
// Author:	彭文奇(PengWenqi)
// Create: 	2012/12/29
//////////////////////////////////////////////////////////////////////////
#ifdef WIN32
#ifndef WINVER 
#define WINVER 0x0501
#endif  
#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0501
#endif
#pragma warning(disable:4996)
#endif

#pragma	warning(disable:4786)

#include "./PerformanceStatistics.h"
#include "./BaseFunc.h"

CPerformanceStatistics* CPerformanceStatistics::s_pInstance = NULL;
////////////////////////////////////////////////////////////////////////
// Description:  单例获取
// Arguments:
// Author: 彭文奇(Peng Wenqi)
// Return Value: CMemMonitor*
////////////////////////////////////////////////////////////////////////
CPerformanceStatistics* CPerformanceStatistics::GetInstance( void )
{
	if (NULL == s_pInstance)
	{
		s_pInstance = new CPerformanceStatistics;
	}
	
	return s_pInstance;	
}

////////////////////////////////////////////////////////////////////////
// Description:  单例析构
// Arguments:
// Author: 彭文奇(Peng Wenqi)
// Return Value: None
////////////////////////////////////////////////////////////////////////
void CPerformanceStatistics::DelInstance( void )
{
	SAFE_DELETE(s_pInstance);
}

//////////////////////////////////////////////////////////////////////////
CPerformanceStatistics::CPerformanceStatistics()
{
	for (int i = PERFORMANCE_STATISTICS_TYPE_BEGIN; i < PERFORMANCE_STATISTICS_TYPE_END; i++)
	{
		this->SetData((PERFORMANCE_STATISTICS_TYPE)i, 0i64);
	}
	::SafeCopy(m_szLogBuffer, "", sizeof(m_szLogBuffer));
	m_tmLogTimer.Startup(PERFORMANCE_STATISTICS_LOGSAVE_SECOND);
}

//////////////////////////////////////////////////////////////////////////
CPerformanceStatistics::~CPerformanceStatistics()
{
	
}

////////////////////////////////////////////////////////////////////////
// Description:  获取数据
// Arguments:
// Author: 彭文奇(Peng Wenqi)
// Return Value: None
////////////////////////////////////////////////////////////////////////
I64 CPerformanceStatistics::GetData(PERFORMANCE_STATISTICS_TYPE eType) const
{
	I64 i64Result = 0i64;
	if (eType >= PERFORMANCE_STATISTICS_TYPE_BEGIN && eType < PERFORMANCE_STATISTICS_TYPE_END)
	{
		i64Result = m_arrData[eType];
	}
	return i64Result;
}

////////////////////////////////////////////////////////////////////////
// Description:  原子操作, 设置数据
// Arguments:
// Author: 彭文奇(Peng Wenqi)
// Return Value: None
////////////////////////////////////////////////////////////////////////
void CPerformanceStatistics::SetData(PERFORMANCE_STATISTICS_TYPE eType, I64 i64NewData)
{
	CHECK(eType >= PERFORMANCE_STATISTICS_TYPE_BEGIN && eType < PERFORMANCE_STATISTICS_TYPE_END);
//	InterlockedExchange64(&(m_arrData[eType]), i64NewData);
	CMySingleLock xLock(&m_csAsync);
	m_arrData[eType] = i64NewData;
}

////////////////////////////////////////////////////////////////////////
// Description:  原子操作, 增加
// Arguments:
// Author: 彭文奇(Peng Wenqi)
// Return Value: None
////////////////////////////////////////////////////////////////////////
void CPerformanceStatistics::AddData(PERFORMANCE_STATISTICS_TYPE eType, I64 i64AddData)
{
	CHECK(eType >= PERFORMANCE_STATISTICS_TYPE_BEGIN && eType < PERFORMANCE_STATISTICS_TYPE_END);
//	InterlockedExchangeAdd64(&(m_arrData[eType]), i64AddData);
	CMySingleLock xLock(&m_csAsync);
	m_arrData[eType] += i64AddData;
}

////////////////////////////////////////////////////////////////////////
// Description:  原子操作, 设置最大值
// Arguments:
// Author: 彭文奇(Peng Wenqi)
// Return Value: None
////////////////////////////////////////////////////////////////////////
void CPerformanceStatistics::SetMax(PERFORMANCE_STATISTICS_TYPE eType, I64 i64NewData)
{
	CHECK(eType >= PERFORMANCE_STATISTICS_TYPE_BEGIN && eType < PERFORMANCE_STATISTICS_TYPE_END);
	if (i64NewData > m_arrData[eType])
	{
//		InterlockedExchange64(&(m_arrData[eType]), i64MaxData);
		CMySingleLock xLock(&m_csAsync);
		m_arrData[eType] = i64NewData;
	}
}

////////////////////////////////////////////////////////////////////////
// Description:  心跳
// Arguments:
// Author: 彭文奇(Peng Wenqi)
// Return Value: None
////////////////////////////////////////////////////////////////////////
void CPerformanceStatistics::OnTimer(void)
{
	DEBUG_TRY;
	::SafeCopy(m_szLogBuffer, "", sizeof(m_szLogBuffer));

	// 网络收发吞吐, 加锁记录
	{
		CMySingleLock xLock(&m_csAsync);
		m_arrData[PERFORMANCE_STATISTICS_TYPE_NET_CLIENT_SEND_5SECOND_MAX]	= max(m_arrData[PERFORMANCE_STATISTICS_TYPE_NET_CLIENT_SEND_5SECOND_MAX],	m_arrData[PERFORMANCE_STATISTICS_TYPE_NET_CLIENT_SEND_5SECOND]);
		m_arrData[PERFORMANCE_STATISTICS_TYPE_NET_CLIENT_RECV_5SECOND_MAX]	= max(m_arrData[PERFORMANCE_STATISTICS_TYPE_NET_CLIENT_RECV_5SECOND_MAX],	m_arrData[PERFORMANCE_STATISTICS_TYPE_NET_CLIENT_RECV_5SECOND]);
	}

	MEMORYSTATUSEX stMemStatus = {0};
	stMemStatus.dwLength = sizeof(stMemStatus);
	::GlobalMemoryStatusEx(&stMemStatus);
	
	sprintf(m_szLogBuffer,	"UserSet:\n"
							"    Total  Now[%I64d], Max[%I64d]\n"
							"    Choose[%I64d/%I64d] - Enter[%I64d/%I64d] - Online[%I64d/%I64d]\n"

							"Database:\n"
							"\t[Total] TimeSum[%I64d], TimeCount[%I64d], AverageMS[%I64d]\n"
							"\t[Async] TimeSum[%I64d], TimeCount[%I64d], AverageMS[%I64d]\n"
							"\t[Open]  TimeSum[%I64d], TimeCount[%I64d], AverageMS[%I64d]\n"

							"SocketThread:\n"
							"\t[MsgPort] RecvTimeSum[%I64d]\n"     
							"\t[OnTimer] TimeSum[%I64d], TimeCount[%I64d], AverageMS[%I64d]\n"
							"\t[MaxMS] OnProcess[%I64d]\n"

							"LoginThread:\n"
							"\t[MsgPort] RecvTimeSum[%I64d]\n"
							"\t[OnTimer] TimeSum[%I64d], TimeCount[%I64d], AverageMS[%I64d]\n"
							"\t[MaxMS] OnProcess[%I64d]\n"

							"GameThread:\n"
							"\t[MsgPort] RecvTimeSum[%I64d]\n"
							"\t[OnTimer] TimeSum[%I64d], TimeCount[%I64d], AverageMS[%I64d]\n"
							"\t[MaxMS] OnProcess[%I64d]\n"

							"RelationThread:\n"
							"\t[MsgPort] RecvTimeSum[%I64d]\n"
							"\t[OnTimer] TimeSum[%I64d], TimeCount[%I64d], AverageMS[%I64d]\n"
							"\t[MaxMS] OnProcess[%I64d]\n"

							"AI Thread:\n"
							"\t[MsgPort] RecvTimeSum[%I64d]\n"
							"\t[OnTimer] TimeSum[%I64d], TimeCount[%I64d], AverageMS[%I64d]\n"
							"\t[MaxMS] OnProcess[%I64d]"

							"Network:\n"
							"\tSend[%8I64d]KB/S, SendMax[%8I64d]KB/S\n"
							"\tRecv[%8I64d]KB/S, RecvMax[%8I64d]KB/S\n"
							 
							"Crash Count:[%8I64d]\n"
							"MemoryAvailVirtual: [%06I64u]MB.[%04I64u]KB \n"
							,
		m_arrData[PERFORMANCE_STATISTICS_TYPE_NOW_TOTAL_USER_AMOUNT], m_arrData[PERFORMANCE_STATISTICS_TYPE_MAX_TOTAL_USER_AMOUNT],
		m_arrData[PERFORMANCE_STATISTICS_TYPE_NOW_CHOOSE_USER_AMOUNT], m_arrData[PERFORMANCE_STATISTICS_TYPE_MAX_CHOOSE_USER_AMOUNT],
		m_arrData[PERFORMANCE_STATISTICS_TYPE_NOW_ENTER_USER_AMOUNT], m_arrData[PERFORMANCE_STATISTICS_TYPE_MAX_ENTER_USER_AMOUNT],
		m_arrData[PERFORMANCE_STATISTICS_TYPE_NOW_ONLINE_USER_AMOUNT], m_arrData[PERFORMANCE_STATISTICS_TYPE_MAX_ONLINE_USER_AMOUNT],

		m_arrData[PERFORMANCE_STATISTICS_TYPE_DATABASE_TIME_SUM], m_arrData[PERFORMANCE_STATISTICS_TYPE_DATABASE_TIME_COUNT], m_arrData[PERFORMANCE_STATISTICS_TYPE_DATABASE_TIME_COUNT] > 0 ? m_arrData[PERFORMANCE_STATISTICS_TYPE_DATABASE_TIME_SUM] / m_arrData[PERFORMANCE_STATISTICS_TYPE_DATABASE_TIME_COUNT] : 0,
		m_arrData[PERFORMANCE_STATISTICS_TYPE_DATABASE_TIME_SUM_ASYNC], m_arrData[PERFORMANCE_STATISTICS_TYPE_DATABASE_TIME_COUNT_ASYNC], m_arrData[PERFORMANCE_STATISTICS_TYPE_DATABASE_TIME_COUNT_ASYNC] > 0 ? m_arrData[PERFORMANCE_STATISTICS_TYPE_DATABASE_TIME_SUM_ASYNC] / m_arrData[PERFORMANCE_STATISTICS_TYPE_DATABASE_TIME_COUNT_ASYNC] : 0,
		m_arrData[PERFORMANCE_STATISTICS_TYPE_DATABASE_OPEN_TIME], m_arrData[PERFORMANCE_STATISTICS_TYPE_DATABASE_OPEN_COUNT], m_arrData[PERFORMANCE_STATISTICS_TYPE_DATABASE_OPEN_COUNT] > 0 ? m_arrData[PERFORMANCE_STATISTICS_TYPE_DATABASE_OPEN_TIME] / m_arrData[PERFORMANCE_STATISTICS_TYPE_DATABASE_OPEN_COUNT] : 0,

		m_arrData[PERFORMANCE_STATISTICS_TYPE_SOCKETTHREAD_MSGPORT_RECV_TIME_SUM],
		m_arrData[PERFORMANCE_STATISTICS_TYPE_SOCKETTHREAD_ONTIMER_TIME_SUM], m_arrData[PERFORMANCE_STATISTICS_TYPE_SOCKETTHREAD_ONTIMER_TIME_COUNT], m_arrData[PERFORMANCE_STATISTICS_TYPE_SOCKETTHREAD_ONTIMER_TIME_COUNT] > 0 ? m_arrData[PERFORMANCE_STATISTICS_TYPE_SOCKETTHREAD_ONTIMER_TIME_SUM] / m_arrData[PERFORMANCE_STATISTICS_TYPE_SOCKETTHREAD_ONTIMER_TIME_COUNT] : 0,
		m_arrData[PERFORMANCE_STATISTICS_TYPE_SOCKETTHREAD_ONPROCESS_TIME_MAX],

		m_arrData[PERFORMANCE_STATISTICS_TYPE_LOGINTHREAD_MSGPORT_RECV_TIME_SUM],
		m_arrData[PERFORMANCE_STATISTICS_TYPE_LOGINTHREAD_ONTIMER_TIME_SUM], m_arrData[PERFORMANCE_STATISTICS_TYPE_LOGINTHREAD_ONTIMER_TIME_COUNT], m_arrData[PERFORMANCE_STATISTICS_TYPE_LOGINTHREAD_ONTIMER_TIME_COUNT] > 0 ? m_arrData[PERFORMANCE_STATISTICS_TYPE_LOGINTHREAD_ONTIMER_TIME_SUM] / m_arrData[PERFORMANCE_STATISTICS_TYPE_LOGINTHREAD_ONTIMER_TIME_COUNT] : 0,
		m_arrData[PERFORMANCE_STATISTICS_TYPE_LOGINTHREAD_ONPROCESS_TIME_MAX],

		m_arrData[PERFORMANCE_STATISTICS_TYPE_GAMETHREAD_MSGPORT_RECV_TIME_SUM], 
		m_arrData[PERFORMANCE_STATISTICS_TYPE_GAMETHREAD_ONTIMER_TIME_SUM], m_arrData[PERFORMANCE_STATISTICS_TYPE_GAMETHREAD_ONTIMER_TIME_COUNT], m_arrData[PERFORMANCE_STATISTICS_TYPE_GAMETHREAD_ONTIMER_TIME_COUNT] > 0 ? m_arrData[PERFORMANCE_STATISTICS_TYPE_GAMETHREAD_ONTIMER_TIME_SUM] / m_arrData[PERFORMANCE_STATISTICS_TYPE_GAMETHREAD_ONTIMER_TIME_COUNT] : 0,
		m_arrData[PERFORMANCE_STATISTICS_TYPE_GAMETHREAD_ONPROCESS_TIME_MAX],

		m_arrData[PERFORMANCE_STATISTICS_TYPE_RELATIONTHREAD_MSGPORT_RECV_TIME_SUM], 
		m_arrData[PERFORMANCE_STATISTICS_TYPE_RELATIONTHREAD_ONTIMER_TIME_SUM], m_arrData[PERFORMANCE_STATISTICS_TYPE_RELATIONTHREAD_ONTIMER_TIME_COUNT], m_arrData[PERFORMANCE_STATISTICS_TYPE_RELATIONTHREAD_ONTIMER_TIME_COUNT] > 0 ? m_arrData[PERFORMANCE_STATISTICS_TYPE_RELATIONTHREAD_ONTIMER_TIME_SUM] / m_arrData[PERFORMANCE_STATISTICS_TYPE_RELATIONTHREAD_ONTIMER_TIME_COUNT] : 0,
		m_arrData[PERFORMANCE_STATISTICS_TYPE_RELATIONTHREAD_ONPROCESS_TIME_MAX],

		m_arrData[PERFORMANCE_STATISTICS_TYPE_AITHREAD_MSGPORT_RECV_TIME_SUM], 
		m_arrData[PERFORMANCE_STATISTICS_TYPE_AITHREAD_ONTIMER_TIME_SUM], m_arrData[PERFORMANCE_STATISTICS_TYPE_AITHREAD_ONTIMER_TIME_COUNT], m_arrData[PERFORMANCE_STATISTICS_TYPE_AITHREAD_ONTIMER_TIME_COUNT] > 0 ? m_arrData[PERFORMANCE_STATISTICS_TYPE_AITHREAD_ONTIMER_TIME_SUM] / m_arrData[PERFORMANCE_STATISTICS_TYPE_AITHREAD_ONTIMER_TIME_COUNT] : 0,
		m_arrData[PERFORMANCE_STATISTICS_TYPE_AITHREAD_ONPROCESS_TIME_MAX],

		m_arrData[PERFORMANCE_STATISTICS_TYPE_NET_CLIENT_SEND_5SECOND] / 1024 / PERFORMANCE_STATISTICS_ONTIMER_SECOND, m_arrData[PERFORMANCE_STATISTICS_TYPE_NET_CLIENT_SEND_5SECOND_MAX] / 1024 / PERFORMANCE_STATISTICS_ONTIMER_SECOND, 
		m_arrData[PERFORMANCE_STATISTICS_TYPE_NET_CLIENT_RECV_5SECOND] / 1024 / PERFORMANCE_STATISTICS_ONTIMER_SECOND, m_arrData[PERFORMANCE_STATISTICS_TYPE_NET_CLIENT_RECV_5SECOND_MAX] / 1024 / PERFORMANCE_STATISTICS_ONTIMER_SECOND, 
		m_arrData[PERFORMANCE_STATISTICS_TYPE_CRASH_COUNT], stMemStatus.ullAvailVirtual / 1024 / 1024, stMemStatus.ullAvailVirtual/ 1024 % 1024

		);

	// 网络五秒内吞吐, 加锁清零
	{
		CMySingleLock xLock(&m_csAsync);	
		m_arrData[PERFORMANCE_STATISTICS_TYPE_NET_CLIENT_SEND_5SECOND]	= 0i64;
		m_arrData[PERFORMANCE_STATISTICS_TYPE_NET_CLIENT_RECV_5SECOND]	= 0i64;
	}


	if (m_tmLogTimer.ToNextTime())
	{
		::MyLogSave("syslog/PerformanceStatistics", "==================================================================================================\n%s", m_szLogBuffer);
	}

	DEBUG_CATCH("CPerformanceStatistics::OnTimer")
}

////////////////////////////////////////////////////////////////////////
// Description:  性能统计数据类(构造)
// Arguments:
// Author: 彭文奇(Peng Wenqi)
// Return Value: None
////////////////////////////////////////////////////////////////////////
CPerformanceStatisticsData::CPerformanceStatisticsData(PERFORMANCE_STATISTICS_TYPE eType, I64 i64Data)
	: m_i64Data(i64Data)
{
	IF_NOT (eType > PERFORMANCE_STATISTICS_TYPE_BEGIN && eType < PERFORMANCE_STATISTICS_TYPE_END)
	{
		m_eType = PERFORMANCE_STATISTICS_TYPE_BEGIN;
		STACK_TRACE_ONCE;
		return;
	}
	m_eType = eType;
}

////////////////////////////////////////////////////////////////////////
// Description:  性能统计数据类(析构)
// Arguments:
// Author: 彭文奇(Peng Wenqi)
// Return Value: None
////////////////////////////////////////////////////////////////////////
CPerformanceStatisticsData::~CPerformanceStatisticsData()
{
	if (m_eType > PERFORMANCE_STATISTICS_TYPE_BEGIN && m_eType < PERFORMANCE_STATISTICS_TYPE_END)
	{
		CPerformanceStatistics::GetInstance()->SetData(m_eType, m_i64Data);
	}
}

////////////////////////////////////////////////////////////////////////
// Description:  性能统计计时类(构造)
// Arguments:
// Author: 彭文奇(Peng Wenqi)
// Return Value: None
////////////////////////////////////////////////////////////////////////
CPerformanceStatisticsMS::CPerformanceStatisticsMS(PERFORMANCE_STATISTICS_TYPE eType, bool bMax/* = false*/)
	: m_eType(PERFORMANCE_STATISTICS_TYPE_BEGIN)
	, m_i64Start(0i64)
	, m_bMax(bMax)
{
	IF_NOT (eType > PERFORMANCE_STATISTICS_TYPE_BEGIN && eType < PERFORMANCE_STATISTICS_TYPE_END)
	{
		STACK_TRACE_ONCE;
		return;
	}
	m_eType = eType;
	m_i64Start = ::GetUtcMillisecond();
}

////////////////////////////////////////////////////////////////////////
// Description:  性能统计计时类(析构)
// Arguments:
// Author: 彭文奇(Peng Wenqi)
// Return Value: None
////////////////////////////////////////////////////////////////////////
CPerformanceStatisticsMS::~CPerformanceStatisticsMS()
{
	if (m_eType > PERFORMANCE_STATISTICS_TYPE_BEGIN && m_eType < PERFORMANCE_STATISTICS_TYPE_END)
	{
		I64 i64End = ::GetUtcMillisecond();
		if (m_bMax)
		{
			CPerformanceStatistics::GetInstance()->SetMax(m_eType, i64End - m_i64Start);
		}
		else
		{
			CPerformanceStatistics::GetInstance()->AddData(m_eType, i64End - m_i64Start);
		}
	}
}

////////////////////////////////////////////////////////////////////////
// Description:  性能统计计次类(构造)
// Arguments:
// Author: 彭文奇(Peng Wenqi)
// Return Value: None
////////////////////////////////////////////////////////////////////////
CPerformanceStatisticsCount::CPerformanceStatisticsCount(PERFORMANCE_STATISTICS_TYPE eType, I64 i64AddCount/* = 1*/, bool bMax/* = false*/)
	: m_i64AddCount(i64AddCount)
	, m_bMax(bMax)
{
	IF_NOT (eType > PERFORMANCE_STATISTICS_TYPE_BEGIN && eType < PERFORMANCE_STATISTICS_TYPE_END)
	{
		m_eType = PERFORMANCE_STATISTICS_TYPE_BEGIN;
		STACK_TRACE_ONCE;
		return;
	}
	m_eType = eType;
	m_i64AddCount = max(0i64, i64AddCount);
}

////////////////////////////////////////////////////////////////////////
// Description:  性能统计计次类(析构)
// Arguments:
// Author: 彭文奇(Peng Wenqi)
// Return Value: None
////////////////////////////////////////////////////////////////////////
CPerformanceStatisticsCount::~CPerformanceStatisticsCount()
{
	if (m_eType > PERFORMANCE_STATISTICS_TYPE_BEGIN && m_eType < PERFORMANCE_STATISTICS_TYPE_END)
	{
		if (m_bMax)
		{
			CPerformanceStatistics::GetInstance()->SetMax(m_eType, m_i64AddCount);
		}
		else
		{
			CPerformanceStatistics::GetInstance()->AddData(m_eType, m_i64AddCount);
		}
	}
}
