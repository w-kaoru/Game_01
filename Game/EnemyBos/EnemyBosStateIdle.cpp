#include "stdafx.h"
#include "EnemyBosStateIdle.h"
#include "EnemyBos.h"
#include "EnemyBosStateMachine.h"

EnemyBosStateIdle::EnemyBosStateIdle(EnemyBos * enbos, EnemyBosStateMachine * estm)
	:IEnemyBosState(enbos, estm)
{
}

EnemyBosStateIdle::~EnemyBosStateIdle()
{
}

bool EnemyBosStateIdle::Start()
{
	return false;
}

void EnemyBosStateIdle::Update()
{
	m_enemy->SetAnimation(EnemyBosState::idle);
	m_moveSpeed = m_enemy->GetMoveSpeed();
	m_moveSpeed.x *= 0.0f;
	m_moveSpeed.z *= 0.0f;
	m_enemy->SetMoveSpeed(m_moveSpeed);
}
