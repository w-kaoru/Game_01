#include "stdafx.h"
#include "PlStateMove.h"
#include "Player.h"

PlStateMove::PlStateMove(Player *pl, PlStateMachine *psm)
	:IPlState(pl,psm)
{
}


PlStateMove::~PlStateMove()
{
}

bool PlStateMove::Start()
{
	return false;
}

void PlStateMove::Update()
{
	m_moveSpeed = m_player->GetMoveSpeed();
	m_rotation = m_player->GetRotation();
	//�X�e�B�b�N�̓��͗ʂ��󂯎��B
	float lStick_x = g_pad[0].GetLStickXF();
	float lStick_y = g_pad[0].GetLStickYF();
	//�J�����̑O�������ƉE�������擾�B
	CVector3 cameraForward = g_camera3D.GetForward();
	CVector3 cameraRight = g_camera3D.GetRight();
	//XZ���ʂł̑O�������A�E�����ɕϊ�����B
	cameraForward.y = 0.0f;
	cameraForward.Normalize();
	cameraRight.y = 0.0f;
	cameraRight.Normalize();
	//XZ�����̈ړ����x���N���A�B
	m_moveSpeed.x = 0.0f;
	m_moveSpeed.z = 0.0f;
	//�������ւ̈ړ����x�����B
	m_moveSpeed += cameraForward * lStick_y * m_player->GetStatus()->GetAgi();
	//�E�����ւ̈ړ����x�����Z�B
	m_moveSpeed += cameraRight * lStick_x * m_player->GetStatus()->GetAgi();
	//������ς���B
	if (fabsf(m_moveSpeed.x) > 0.1f
		|| fabsf(m_moveSpeed.z) > 0.1f) {
		auto angle = atan2f(m_moveSpeed.x, m_moveSpeed.z);
		m_rotation.SetRotation(CVector3::AxisY(), angle);
		//����A�j���[�V�����̍Đ����邽�߂ɃX�e�[�g�̕ύX
		m_player->SetAnimation(PlayerState::AnimationState::AnimRun);
	}
	else {
		//�ҋ@�A�j���[�V�����̍Đ����邽�߂ɃX�e�[�g�̕ύX
		m_player->SetAnimation(PlayerState::AnimationState::AnimIdle);
	}
	m_player->SetMoveSpeed(m_moveSpeed);
	m_player->SetRotation(m_rotation);
}
