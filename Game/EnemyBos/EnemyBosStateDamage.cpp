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
	//攻撃をくらったのでHPからくらった分を引く
	float hp = m_enemy->GetStatus()->GetHp();
	hp = (hp + m_enemy->GetStatus()->GetDef()) - m_damage;
	if (hp <= 0.0f) {
		hp = 0.0f;
	}
	m_moveSpeed *= 0.0f;
	m_enemy->GetStatus()->SetHp(hp);
	return false;
}

void EnemyBosStateDamage::Update()
{
}
