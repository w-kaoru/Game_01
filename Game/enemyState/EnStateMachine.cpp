#include "stdafx.h"
#include "EnStateMachine.h"


EnStateMachine::EnStateMachine(Enemy * en)
	:m_enMove(en,this),m_enIdle(en,this)
{
}

EnStateMachine::~EnStateMachine()
{
}

bool EnStateMachine::Start()
{
	Change(EnemyState::MoveState::idle);
	return false;
}

void EnStateMachine::Update()
{
	m_iens->Update();
}

void EnStateMachine::Change(EnemyState::MoveState state)
{
	if (state == m_state) {
		return;
	}
	switch (state)
	{
	case EnemyState::MoveState::move:
		m_iens = &m_enMove;
		break;
	case EnemyState::MoveState::idle:
		m_iens = &m_enIdle;
		break;
	}
	m_iens->Start();
}
