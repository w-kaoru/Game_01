#include "stdafx.h"
#include "EnemyBosStateMove.h"
#include "EnemyBos.h"
#include "EnemyBosStateMachine.h"

EnemyBosStateMove::EnemyBosStateMove(EnemyBos* enbos, EnemyBosStateMachine* enstm)
	:IEnemyBosState(enbos,enstm)
{
}


EnemyBosStateMove::~EnemyBosStateMove()
{
}

bool EnemyBosStateMove::Start()
{
	return false;
}

void EnemyBosStateMove::Update()
{
	m_enemy->SetAnimation(EnemyBosState::move);
	m_moveSpeed = m_enemy->GetMoveSpeed();
	m_rotation = m_enemy->GetRotation();
	m_moveSpeed.x *= m_enemy->GetAgi();
	m_moveSpeed.z *= m_enemy->GetAgi();
	//Œü‚«‚à•Ï‚¦‚éB
	if (fabsf(m_moveSpeed.x) > 0.1f || fabsf(m_moveSpeed.z) > 0.1f) {
		auto angle = atan2f(m_moveSpeed.x, m_moveSpeed.z);
		m_rotation.SetRotation(CVector3::AxisY(), angle);
	}
	m_enemy->SetMoveSpeed(m_moveSpeed);
	m_enemy->SetRotation(m_rotation);

}
