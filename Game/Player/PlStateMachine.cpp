#include "stdafx.h"
#include "PlStateMachine.h"

PlStateMachine::PlStateMachine(Player * pl) :
	m_plMove(pl,this),m_plAttack(pl,this),m_plDamage(pl,this),m_plDeath(pl,this)
{
}

PlStateMachine::~PlStateMachine()
{
}

bool PlStateMachine::Start()
{
	Change(PlayerState::MoveState::Move);
	return false;
}

void PlStateMachine::Update()
{
	m_ips->Update();
}

void PlStateMachine::Change(PlayerState::MoveState state)
{
	if (state == m_state) {
		return;
	}
	switch (state)
	{
	case PlayerState::MoveState::Move:
		m_ips = &m_plMove;
		break;
	case PlayerState::MoveState::Attack:
		m_ips = &m_plAttack;
		break;
	case PlayerState::MoveState::Damage:
		m_ips = &m_plDamage;
		break;
	case PlayerState::MoveState::Death:
		m_ips = &m_plDeath;
	}
	m_ips->Start();
}
