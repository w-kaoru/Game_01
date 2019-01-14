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
}
