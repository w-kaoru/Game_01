#include "stdafx.h"
#include "EnStateDamage.h"
#include "Enemy.h"
#include "EnStateMachine.h"

EnStateDamage::EnStateDamage(Enemy * en, EnStateMachine * estm)
	:IEnState(en, estm)
{
}

EnStateDamage::~EnStateDamage()
{
}

bool EnStateDamage::Start()
{
	m_enemy->SetAnimation(EnemyState::AnimationState::AnimDamage);
	m_moveSpeed *= 0.0f;
	//攻撃をくらったのでHPからくらった分を引く
	float hp = m_enemy->GetStatus()->GetHp();
	m_isDamage = true;
	//攻撃をくらったのでHPからくらった分を引く
	hp = (hp + m_enemy->GetStatus()->GetDef()) - m_damage;
	if (hp <= 0) {
		hp = 0.0f;
	}
	m_enemy->GetStatus()->SetHp(hp);
	m_enemy->SetMoveSpeed(m_moveSpeed);
	return false;
}

void EnStateDamage::Update()
{
}
