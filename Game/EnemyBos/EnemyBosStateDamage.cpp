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
	return false;
}

void EnemyBosStateDamage::Update()
{
}
