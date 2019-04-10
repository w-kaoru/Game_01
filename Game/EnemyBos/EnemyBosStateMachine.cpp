#include "stdafx.h"
#include "EnemyBosStateMachine.h"


EnemyBosStateMachine::EnemyBosStateMachine(EnemyBos * enbos)
	:m_enBosMove(enbos,this),m_enBosIdle(enbos,this),m_enBosAttack(enbos,this)
{
}

EnemyBosStateMachine::~EnemyBosStateMachine()
{
}

bool EnemyBosStateMachine::Start()
{
	Change(EnemyBosState::MoveState::idle);
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
	case EnemyBosState::MoveState::idle:
		m_ienbosState = &m_enBosIdle;
		break;
	case EnemyBosState::MoveState::attack:
		m_ienbosState = &m_enBosAttack;
		break;
	}
	m_ienbosState->Start();
}
