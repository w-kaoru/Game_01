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
	m_moveSpeed = m_enemy->GetMoveSpeed();
	m_rotation = m_enemy->GetRotation();

	//�U���A�j���[�V�������Đ�����ĂȂ���
	if (m_enemy->GetPlLen() > 160.0f && m_enemy->GetPlLen() < 1200.0f) {
		m_moveSpeed.x *= m_enemy->GetStatus()->GetAgi();
		m_moveSpeed.z *= m_enemy->GetStatus()->GetAgi();
		//�����A�j���[�V�����̍Đ����邽�߂ɃX�e�[�g�̕ύX
		m_enemy->SetAnimation(EnemyState::AnimationState::AnimMove);
		//�������ς���B
		if (fabsf(m_moveSpeed.x) > 0.1f || fabsf(m_moveSpeed.z) > 0.1f) {
			auto angle = atan2f(m_moveSpeed.x, m_moveSpeed.z);
			m_rotation.SetRotation(CVector3::AxisY(), angle);
		}
		m_enemy->SetRotation(m_rotation);
	}
	else {
		m_moveSpeed.x *= 0.0f;
		m_moveSpeed.z *= 0.0f;
		//�ҋ@�A�j���[�V�����̍Đ����邽�߂ɃX�e�[�g�̕ύX
	}
	if (m_enemy->GetPlLen() >= 1200.0f) {
		m_enemy->SetAnimation(EnemyState::AnimationState::AnimIdle);
	}
	m_enemy->SetMoveSpeed(m_moveSpeed);

}
