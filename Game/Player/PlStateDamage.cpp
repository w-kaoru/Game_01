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
	m_isDamage = true;
	//�U������������̂�HP���炭�������������
	float hp = m_player->GetStatus()->GetHp();
	hp = (hp + m_player->GetStatus()->GetDef()) - m_damage;
	hp = min(hp, m_player->GetStatus()->GetHp());
	hp = max(0.0f, hp);
	m_moveSpeed *= 0.0f;
	m_player->SetMoveSpeed(m_moveSpeed);
	m_player->GetStatus()->SetHp(hp);
	return false;
}

void PlStateDamage::Update()
{
	m_player->SetAnimation(PlayerState::AnimationState::AnimDamage);
}