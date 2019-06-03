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
	m_plstma->StateAttack()->SetHit(false);
	//UŒ‚‚ð‚­‚ç‚Á‚½‚Ì‚ÅHP‚©‚ç‚­‚ç‚Á‚½•ª‚ðˆø‚­
	float hp = m_player->GetStatus()->GetHp();
	m_player->SetAnimation(PlayerState::AnimationState::AnimDamage);
	hp = (hp + m_player->GetStatus()->GetDef()) - m_damage;
	hp = min(hp, m_player->GetStatus()->GetHp());
	hp = max(0.0f, hp);
	m_isDamage = true;
	m_moveSpeed *= 0.0f;
	m_player->SetMoveSpeed(m_moveSpeed);
	m_player->GetStatus()->SetHp(hp);
	return false;
}

void PlStateDamage::Update()
{
}