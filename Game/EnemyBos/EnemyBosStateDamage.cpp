#include "stdafx.h"
#include "EnemyBosStateDamage.h"
#include "EnemyBos.h"
#include "EnemyBosStateMachine.h"

EnemyBosStateDamage::EnemyBosStateDamage(EnemyBos * enbos, EnemyBosStateMachine * estm)
	:IEnemyBosState(enbos, estm)
{
}

EnemyBosStateDamage::~EnemyBosStateDamage()
{
}

bool EnemyBosStateDamage::Start()
{
	m_enemy->SetAnimation(EnemyBosState::AnimationState::AnimDamage);
	m_isDamage = true;
	//UŒ‚‚ð‚­‚ç‚Á‚½‚Ì‚ÅHP‚©‚ç‚­‚ç‚Á‚½•ª‚ðˆø‚­
	float hp = m_enemy->GetStatus()->GetHp();
	float dmg = m_enemy->GetStatus()->GetDef() - m_damage;
	dmg *= -1.0f;
	dmg = max(0.01f, dmg);
	hp = hp - dmg;
	hp = min(hp, m_enemy->GetStatus()->GetHp());
	hp = max(0.0f, hp);
	m_moveSpeed *= 0.0f;
	m_enemy->GetStatus()->SetHp(hp);
	m_enemy->SetMoveSpeed(m_moveSpeed);
	return false;
}

void EnemyBosStateDamage::Update()
{
}
