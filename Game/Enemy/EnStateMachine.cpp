#include "stdafx.h"
#include "EnStateMachine.h"


EnStateMachine::EnStateMachine(Enemy * en)
	:m_enMove(en,this),m_enAttack(en,this),m_enDamage(en,this)
{
}

EnStateMachine::~EnStateMachine()
{
}
void EnStateMachine::Destroy()
{
	m_iens->Destroy();
}

bool EnStateMachine::Start()
{
	Change(EnemyState::MoveState::move);
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
	case EnemyState::MoveState::attack:
		m_iens = &m_enAttack;
		break;
	case EnemyState::MoveState::damage:
		m_iens = &m_enDamage;
		break;
	}
	m_iens->Start();
}
