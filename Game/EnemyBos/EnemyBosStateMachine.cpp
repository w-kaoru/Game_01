#include "stdafx.h"
#include "EnemyBosStateMachine.h"


EnemyBosStateMachine::EnemyBosStateMachine(EnemyBos * enbos)
	:m_enBosMove(enbos,this),m_enBosAttack(enbos,this),m_enBosDamage(enbos,this)
{
}

EnemyBosStateMachine::~EnemyBosStateMachine()
{
}

bool EnemyBosStateMachine::Start()
{
	Change(EnemyBosState::MoveState::move);
	return false;
}

void EnemyBosStateMachine::Update()
{
	m_ienbosState->Update();
}

void EnemyBosStateMachine::Change(EnemyBosState::MoveState state)
{
	if (state == m_state) {
		return;
	}
	switch (state)
	{
	case EnemyBosState::MoveState::move:
		m_ienbosState = &m_enBosMove;
		break;
	case EnemyBosState::MoveState::attack:
		m_ienbosState = &m_enBosAttack;
		break;
	case EnemyBosState::MoveState::damage:
		m_ienbosState = &m_enBosDamage;
	}
	m_ienbosState->Start();
}
