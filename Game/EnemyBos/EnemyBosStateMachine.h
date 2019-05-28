#pragma once
#include "EnemyBosStateMove.h"
#include "EnemyBosStateAttack.h"
#include "EnemyBosStateDamage.h"
#include "EnemyBosStateDeath.h"
#include "EnemyBosState.h"
class EnemyBos;
class EnemyBosStateMachine
{
public:
	EnemyBosStateMachine(EnemyBos* enbos);
	~EnemyBosStateMachine();
	bool Start();
	void Update();
	void Change(EnemyBosState::MoveState state);

	EnemyBosStateAttack* StateAttack()
	{
		return &m_enBosAttack;
	}
	EnemyBosStateDamage* StateDamage()
	{
		return &m_enBosDamage;
	}
	EnemyBosStateDeath* StateDeath()
	{
		return &m_enBosDeath;
	}
private:
	EnemyBosStateMove m_enBosMove;
	EnemyBosStateAttack m_enBosAttack;
	EnemyBosStateDamage m_enBosDamage;
	EnemyBosStateDeath m_enBosDeath;
	IEnemyBosState *m_ienbosState = nullptr;
	EnemyBosState::MoveState m_state = EnemyBosState::noState;
};
