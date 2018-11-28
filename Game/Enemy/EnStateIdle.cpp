#include "stdafx.h"
#include "EnStateIdle.h"
#include "Enemy.h"
#include "EnStateMachine.h"

EnStateIdle::EnStateIdle(Enemy * en, EnStateMachine * estm)
	:IEnState(en, estm)
{
}

EnStateIdle::~EnStateIdle()
{
}

bool EnStateIdle::Start()
{
	m_moveSpeed = m_enemy->GetMoveSpeed();
	return false;
}

void EnStateIdle::Update()
{
	m_moveSpeed.x *= 0.0f;
	m_moveSpeed.z *= 0.0f;
	m_enemy->SetMoveSpeed(m_moveSpeed);
}
