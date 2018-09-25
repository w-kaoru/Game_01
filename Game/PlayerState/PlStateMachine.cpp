#include "stdafx.h"
#include "PlStateMachine.h"

PlStateMachine::PlStateMachine(Player * pl) :
	m_plIdle(pl, this),m_plRun(pl,this)
{
}

PlStateMachine::~PlStateMachine()
{
}

bool PlStateMachine::Start()
{
	Change(PlayerState::AniMove::idle);
	return false;
}

void PlStateMachine::Update()
{
	m_ips->Update();
}

void PlStateMachine::Change(PlayerState::AniMove state)
{
	if (state == m_state) {
		return;
	}
	switch (state)
	{
	case PlayerState::AniMove::idle:
		m_ips = &m_plIdle;
		break;
	case PlayerState::AniMove::run:
		m_ips = &m_plRun;
		break;
	}
	m_ips->Start();
}
