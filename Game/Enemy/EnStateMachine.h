#pragma once
#include "EnStateMove.h"
#include "EnStateAttack.h"
#include "EnStateDamage.h"
#include "EnemyState.h"

class Enemy;
class EnStateMachine
{
public:
	EnStateMachine(Enemy* en);
	~EnStateMachine();
	bool Start();
	void Update();
	void Change(EnemyState::MoveState state);

	EnStateAttack* StateAttack()
	{
		return &m_enAttack;
	}
	EnStateDamage* StateDamage()
	{
		return &m_enDamage;
	}
private:
	EnStateMove m_enMove;
	EnStateAttack m_enAttack;
	EnStateDamage m_enDamage;
	IEnState *m_iens = nullptr;
	EnemyState::MoveState m_state = EnemyState::noState;
};
