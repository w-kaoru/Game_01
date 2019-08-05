#include "stdafx.h"
#include "EnemyBosStateAttack.h"
#include "EnemyBos.h"
#include "EnemyBosStateMachine.h"
#include <random>

EnemyBosStateAttack::EnemyBosStateAttack(EnemyBos* enbos, EnemyBosStateMachine*enstm)
	:IEnemyBosState(enbos, enstm)
{
}


EnemyBosStateAttack::~EnemyBosStateAttack()
{
}

bool EnemyBosStateAttack::Start()
{
	return false;
}

void EnemyBosStateAttack::Update()
{
	m_AttackTiming++;
	//�U���̊Ԋu
	if (m_AttackTiming == m_atkAnimStart) {
		//�U���A�j���[�V�����̍Đ����邽�߂ɃX�e�[�g�̕ύX
		switch (m_no)
		{
		case 0:
		case 1:
			m_enemy->SetAnimation(EnemyBosState::AnimationState::AnimAttack);
			break;
		case 2:
		case 3:
			m_enemy->SetAnimation(EnemyBosState::AnimationState::AnimAttackPunch);
			break;
		case 4:
			m_enemy->SetAnimation(EnemyBosState::AnimationState::AnimAttackStrong);
			break;
		}
		//m_ensm.Change(EnemyState::MoveState::attack);
		m_attackFlag = true;
	}
	if (m_AttackTiming == 37 && m_no == 4) {
		//�G�t�F�N�g�̍Đ��B
		m_playEffectHandle = g_graphicsEngine->GetEffekseerManager()->Play(
			m_enemy->Effect(),
			m_enemy->GetPosition().x,
			m_enemy->GetPosition().y + 100.0f,
			m_enemy->GetPosition().z
		);
		//�G�t�F�N�g�̊g�嗦�B
		g_graphicsEngine->GetEffekseerManager()->SetScale(
			m_playEffectHandle,
			30.0f, 30.0f, 30.0f
		);
	}
	//�U������Ă��炠�������^�C�~���O�ōU���������i�ȈՔŁj
	if (m_AttackTiming == m_atkHit) {
		//���������Ǝv����^�C�~���O��
		CVector3 hit = m_enemy->GetPosition();
		hit.y += 95.0f;
		hit += m_enemy->GetForward() * 50.0f;
		//�U�����q�b�g������B
		switch (m_no)
		{
		case 0:
		case 1:
			g_hitObject->HitTest(
				hit, 85.0f,
				m_enemy->GetStatus()->GetAtk(),
				Hit::player
			);
			break;
		case 2:
		case 3:
			g_hitObject->HitTest(
				hit, 90.0f,
				m_enemy->GetStatus()->GetAtk()*0.8f,
				Hit::player
			);
			break;
		case 4:
			g_hitObject->HitTest(
				m_enemy->GetPosition(), 170.0f,
				m_enemy->GetStatus()->GetAtk()*1.3f,
				Hit::player
			);
			break;
		}
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
		std::random_device rnd;     // �񌈒�I�ȗ���������𐶐�
		std::mt19937 mt(rnd());     //  �����Z���k�E�c�C�X�^��32�r�b�g�ŁA�����͏����V�[�h�l
		std::uniform_int_distribution<> rand(0, 4);        // [0, 4] �͈͂̈�l����
		m_no = rand(mt);
		switch (m_no)
		{
		case 0:
		case 1:
			m_atkHit = 35;
			break;
		case 2:
		case 3:
			m_atkHit = 30;
			break;
		case 4:
			m_atkHit = 55;
			break;
		}
	}
}
