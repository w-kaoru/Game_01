#include "stdafx.h"
#include "PlStateAttack.h"
#include "Player.h"

PlStateAttack::PlStateAttack(Player *pl, PlStateMachine *psm) :
	IPlState(pl, psm)
{
}


PlStateAttack::~PlStateAttack()
{
}

bool PlStateAttack::Start()
{
	m_player->SetAnimation(PlayerState::attack);
	// /*
	CVector3 hit = m_player->GetPosition();
	hit.y += 25.0f;
	hit += m_player->GetForward() * 25.0f;
	g_battleController->Hit(hit, BattleHit::enemy);
	//*/
	return false;
}

void PlStateAttack::Update()
{
}
