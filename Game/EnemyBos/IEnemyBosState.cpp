#include "stdafx.h"
#include "IEnemyBosState.h"


IEnemyBosState::IEnemyBosState(EnemyBos* en, EnemyBosStateMachine* esm)
{
	m_enemy = en;
	m_stMa = esm;
}

IEnemyBosState::~IEnemyBosState()
{
}
