#include "stdafx.h"
#include "EnStateMove.h"
#include "Enemy.h"
#include "EnStateMachine.h"

EnStateMove::EnStateMove(Enemy* en, EnStateMachine* enstm)
	:IEnState(en,enstm)
{
}


EnStateMove::~EnStateMove()
{
}

bool EnStateMove::Start()
{
	return false;
}

void EnStateMove::Update()
{
	m_enemy->SetAnimation(EnemyState::move);
	m_moveSpeed = m_enemy->GetMoveSpeed();
	m_rotation = m_enemy->GetRotation();
	m_moveSpeed.x *= 500.0f;
	m_moveSpeed.z *= 500.0f;
	//Œü‚«‚à•Ï‚¦‚éB
	if (fabsf(m_moveSpeed.x) > 0.1f || fabsf(m_moveSpeed.z) > 0.1f) {
		auto angle = atan2f(m_moveSpeed.x, m_moveSpeed.z);
		m_rotation.SetRotation(CVector3::AxisY(), angle);
	}
	m_enemy->SetMoveSpeed(m_moveSpeed);
	m_enemy->SetRotation(m_rotation);

}
