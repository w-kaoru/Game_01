#include "stdafx.h"
#include "PlStateDamage.h"
#include "Player.h"

PlStateDamage::PlStateDamage(Player *pl, PlStateMachine *psm) :
	IPlState(pl, psm)
{
}

PlStateDamage::~PlStateDamage()
{
}

bool PlStateDamage::Start()
{
	m_player->SetAnimation(PlayerState::AnimationState::AnimDamage);
	return false;
}

void PlStateDamage::Update()
{
}
