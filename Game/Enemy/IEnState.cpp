#include "stdafx.h"
#include "IEnState.h"


IEnState::IEnState(Enemy* en, EnStateMachine* esm)
{
	m_enemy = en;
	m_stMa = esm;
}

IEnState::~IEnState()
{
}
