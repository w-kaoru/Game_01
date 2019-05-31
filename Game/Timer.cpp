#include "stdafx.h"
#include "Timer.h"


Timer::Timer()
{
}


Timer::~Timer()
{
}
void Timer::Start()
{
	::QueryPerformanceCounter((LARGE_INTEGER*)&m_start);
	m_Allelapsed = 0.0f;
	m_elapsed = 0.0f;
}

void Timer::Restart()
{
	::QueryPerformanceCounter((LARGE_INTEGER*)&m_start);
}
void Timer::Stop()
{
	::QueryPerformanceFrequency((LARGE_INTEGER*)&m_fleam);
	::QueryPerformanceCounter((LARGE_INTEGER*)&m_stop);
	m_elapsed = double(m_stop - m_start) / m_fleam;
	m_Allelapsed += m_elapsed;
}