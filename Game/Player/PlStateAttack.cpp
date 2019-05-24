#include "stdafx.h"
#include "PlStateAttack.h"
#include "Player.h"

PlStateAttack::PlStateAttack(Player *pl, PlStateMachine *psm) :
	IPlState(pl, psm)
{
}


PlStateAttack::~PlStateAttack()
{
}

bool PlStateAttack::Start()
{
	return false;
}

void PlStateAttack::Update()
{

	m_moveSpeed = m_player->GetMoveSpeed();
	m_moveSpeed *= 0.0f;
	//�U���̓����蔻��̃|�W�V����
	//�v���C���[�̃|�W�V�����̏����O�̈ʒu
	m_player->SetAnimation(PlayerState::AnimationState::AnimAttack);
	m_attckPos.x = m_player->GetPosition().x + m_player->GetForward().x * 65.0f;
	m_attckPos.z = m_player->GetPosition().z + m_player->GetForward().z * 65.0f;
	m_attckPos.y = 100.0f;
	m_hitTiming++;
	//�U������Ă��炠�������^�C�~���O�ōU���������i�ȈՔŁj
	if (m_hitTiming == 15) {
		//�U�����q�b�g������B
		g_hitObject->HitTest(m_attckPos, m_player->GetStatus()->GetAtk(), Hit::enemy);
		m_hitTiming == 0;
	}
	if (m_player->GetAnimation().IsPlaying() == false) {
		m_atk = false;
		//�U���̊Ԋu��0�ɖ߂��B
		m_hitTiming = 0;
	}
	m_player->SetMoveSpeed(m_moveSpeed);
}
