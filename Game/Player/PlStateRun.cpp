#include "stdafx.h"
#include "PlStateRun.h"
#include "Player.h"

PlStateRun::PlStateRun(Player *pl, PlStateMachine *psm)
	:IPlState(pl,psm)
{
}


PlStateRun::~PlStateRun()
{
}

bool PlStateRun::Start()
{
	m_player->SetAnimation(PlayerState::run);
	return false;
}

void PlStateRun::Update()
{
	if (fabsf(m_player->GetmoveSpeed().x) < 0.1f 
		&& fabsf(m_player->GetmoveSpeed().z) < 0.1f) {
		m_plstma->Change(PlayerState::idle);
	}
}
