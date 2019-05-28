#include "stdafx.h"
#include "PlStateDeath.h"
#include "Player.h"

#include "../Game.h"
#include "../GameEnd.h"

PlStateDeath::PlStateDeath(Player *pl, PlStateMachine *psm) :
	IPlState(pl, psm)
{
}


PlStateDeath::~PlStateDeath()
{
}

bool PlStateDeath::Start()
{
	return false;
}

void PlStateDeath::Update()
{
	m_player->SetAnimation(PlayerState::AnimationState::AnimDeath);
	if (m_player->GetAnimation().IsPlaying() == false) {
		g_gameObjM->DeleteGO(g_gameObjM->FindGO<Game>());
		g_gameObjM->NewGO<GameEnd>()->SetGameEnd(GameEnd::GameEndState::gameOver);
	}
}
