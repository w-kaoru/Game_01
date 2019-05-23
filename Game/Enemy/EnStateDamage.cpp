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
	return false;
}

void EnStateDamage::Update()
{
}
