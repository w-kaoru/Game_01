#include "stdafx.h"
#include "PlStateIdle.h"
#include "Player.h"


PlStateIdle::PlStateIdle(Player *pl, PlStateMachine *psm):
	IPlState(pl,psm)
{
}


PlStateIdle::~PlStateIdle()
{
}

bool PlStateIdle::Start()
{
	m_player->SetAnimation(PlayerState::idle);
	return false;
}

void PlStateIdle::Update()
{
	if (fabsf(m_player->GetmoveSpeed().x) > 0.1f 
		|| fabsf(m_player->GetmoveSpeed().z) > 0.1f) {
		m_plstma->Change(PlayerState::run);
	}

}
