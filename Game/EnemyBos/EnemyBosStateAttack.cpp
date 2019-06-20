#include "stdafx.h"
#include "EnemyBosStateAttack.h"
#include "EnemyBos.h"
#include "EnemyBosStateMachine.h"

EnemyBosStateAttack::EnemyBosStateAttack(EnemyBos* enbos, EnemyBosStateMachine*enstm)
	:IEnemyBosState(enbos, enstm)
{
}


EnemyBosStateAttack::~EnemyBosStateAttack()
{
}

bool EnemyBosStateAttack::Start()
{
	//m_enemy->SetAnimation(EnemyBosState::attack);
	return false;
}

void EnemyBosStateAttack::Update()
{
	m_AttackTiming++;
	//�U���̊Ԋu
	if (m_AttackTiming == m_atkAnimStart) {
		//�U���A�j���[�V�����̍Đ����邽�߂ɃX�e�[�g�̕ύX
		m_enemy->SetAnimation(EnemyBosState::AnimationState::AnimAttack);
		//m_ensm.Change(EnemyState::MoveState::attack);
		m_attackFlag = true;
	}
	//�U������Ă��炠�������^�C�~���O�ōU���������i�ȈՔŁj
	if (m_AttackTiming == m_atkHit) {
		//���������Ǝv����^�C�~���O��
		CVector3 hit = m_enemy->GetPosition();
		hit.y += 50.0f;
		hit += m_enemy->GetForward() * 50.0f;
		//�U�����q�b�g������B
		g_hitObject->HitTest(hit, 20.0f, m_enemy->GetStatus()->GetAtk(), Hit::player);
		//�U���̊Ԋu��0�ɖ߂��B
		m_AttackTiming = 0;
		m_isAtk = true;
	}
	if (m_isAtk) {
		m_AttackTiming = 0;
	}
	if (m_attackFlag == false) {
		m_enemy->SetAnimation(EnemyBosState::AnimationState::AnimIdle);
		m_isAtk = false;
	}
}
