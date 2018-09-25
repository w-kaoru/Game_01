#include "stdafx.h"
#include "IPlState.h"


IPlState::IPlState(Player *pl, PlStateMachine *psm)
{
	m_player = pl;
	m_plstma = psm;
}


IPlState::~IPlState()
{
}
