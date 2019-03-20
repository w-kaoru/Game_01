#pragma once
#include "EnStateMove.h"
#include "EnStateIdle.h"
#include "EnStateAttack.h"
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
private:
	EnStateMove m_enMove;
	EnStateIdle m_enIdle;
	EnStateAttack m_enAttack;
	IEnState *m_iens = nullptr;
	EnemyState::MoveState m_state = EnemyState::noState;
};
