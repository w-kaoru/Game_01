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
	m_plstma->StateAttack()->SetHit(false);
	m_isDamage = true;
	//UŒ‚‚ð‚­‚ç‚Á‚½‚Ì‚ÅHP‚©‚ç‚­‚ç‚Á‚½•ª‚ðˆø‚­
	float hp = m_player->GetStatus()->GetHp();
	hp = (hp + m_player->GetStatus()->GetDef()) - m_damage;
	if (hp <= 0) {
		hp = 0.0f;
	}
	m_moveSpeed *= 0.0f;
	m_player->SetMoveSpeed(m_moveSpeed);
	m_player->GetStatus()->SetHp(hp);
	return false;
}

void PlStateDamage::Update()
{
}