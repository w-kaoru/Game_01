#include "stdafx.h"
#include "PlStateMachine.h"

PlStateMachine::PlStateMachine(Player * pl) :
	m_plIdle(pl, this),m_plRun(pl,this),m_plAttack(pl,this)
{
}

PlStateMachine::~PlStateMachine()
{
}

bool PlStateMachine::Start()
{
	Change(PlayerState::AnimState::idle);
	return false;
}

void PlStateMachine::Update()
{
	m_ips->Update();
}

void PlStateMachine::Change(PlayerState::AnimState state)
{
	if (state == m_state) {
		return;
	}
	switch (state)
	{
	case PlayerState::AnimState::idle:
		m_ips = &m_plIdle;
		break;
	case PlayerState::AnimState::run:
		m_ips = &m_plRun;
		break;
	case PlayerState::AnimState::attack:
		m_ips = &m_plAttack;
		break;
	}
	m_ips->Start();
}
