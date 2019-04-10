#pragma once
#include "EnemyBosStateMove.h"
#include "EnemyBosStateIdle.h"
#include "EnemyBosStateAttack.h"
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
private:
	EnemyBosStateMove m_enBosMove;
	EnemyBosStateIdle m_enBosIdle;
	EnemyBosStateAttack m_enBosAttack;
	IEnemyBosState *m_ienbosState = nullptr;
	EnemyBosState::MoveState m_state = EnemyBosState::noState;
};
