#include "stdafx.h"
#include "Player.h"
#include "PlayerState\PlStateMachine.h"


Player::Player():m_stMa(this)
{
	//cmo�t�@�C���̓ǂݍ��݁B
	m_model.Init(L"Assets/modelData/Thethief_H.cmo", enFbxUpAxisZ);

	//tka�t�@�C���̓ǂݍ��݁B
		m_animationClips[PlayerState::idle].Load(L"Assets/animData/idle.tka");
		m_animationClips[PlayerState::idle].SetLoopFlag(true);

		m_animationClips[PlayerState::run].Load(L"Assets/animData/run.tka");
		m_animationClips[PlayerState::run].SetLoopFlag(true);
		
		m_animationClips[PlayerState::jump].Load(L"Assets/animData/jump.tka");
		m_animationClips[PlayerState::jump].SetLoopFlag(false);
		
		m_animationClips[PlayerState::attack].Load(L"Assets/animData/attack.tka");
		m_animationClips[PlayerState::attack].SetLoopFlag(false);

	//�A�j���[�V�����̏������B
	m_animation.Init(
		m_model,			//�A�j���[�V�����𗬂��X�L�����f���B
							//����ŃA�j���[�V�����ƃX�L�����f�����֘A�t�������B
		m_animationClips,	//�A�j���[�V�����N���b�v�̔z��B
		4					//�A�j���[�V�����N���b�v�̐��B
	);

	m_position.y = 100.0f;
	m_position.x = 500.0f;
	m_position.x = 300.0f;
	m_scale *= 4;
	//�L�����N�^�[�R���g���[���[�̏������B
	m_charaCon.Init(10.0f, 50.0f, m_position);
	m_stMa.Start();
}


Player::~Player()
{
	m_charaCon.RemoveRigidBoby();
}
//�ړ�
void Player::Run()
{
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
	m_moveSpeed += cameraForward * lStick_y * 400.0f;	//�������ւ̈ړ����x�����B
	m_moveSpeed += cameraRight * lStick_x * 400.0f;		//�E�����ւ̈ړ����x�����Z�B
	m_moveSpeed.y -= 980.0f * (1.0f / 60.0f);
	//�L�����R�����g���Ĉړ�����B
	m_position = m_charaCon.Execute(1.0f / 60.0f, m_moveSpeed);
	//�������ς���B
	if (fabsf(m_moveSpeed.x) > 0.1f
		|| fabsf(m_moveSpeed.z) > 0.1f) {
		auto angle = atan2f(m_moveSpeed.x, m_moveSpeed.z);
		m_rotation.SetRotation(CVector3::AxisY(), angle);
	}
	/*else {
		animove = idle;
	}
	if (g_pad[0].IsTrigger(enButtonA) == true
		&& m_charaCon.IsOnGround() == true
		) {
		animove = jump;
	}
	if (g_pad[0].IsTrigger(enButtonX) == true
		&& m_charaCon.IsOnGround() == true
		) {
		animove = attack;
	}*/
}
//�ҋ@
//void Player::Idle()
//{
//	//�X�e�B�b�N�̓��͗ʂ��󂯎��B
//	float lStick_x = g_pad[0].GetLStickXF();
//	float lStick_y = g_pad[0].GetLStickYF();
//	//XZ�����̈ړ����x���N���A�B
//	m_moveSpeed.x = 0.0f;
//	m_moveSpeed.z = 0.0f;
//	m_moveSpeed.z += lStick_y * 400.0f;	//�������ւ̈ړ����x�����B
//	m_moveSpeed.x += lStick_x * 400.0f;		//�E�����ւ̈ړ����x�����Z�B
//	m_moveSpeed.y -= 980.0f * (1.0f / 60.0f);
//	//�L�����R�����g���Ĉړ�����B
//	m_position = m_charaCon.Execute(1.0f / 60.0f, m_moveSpeed);
//	if (fabsf(m_moveSpeed.x) > 0.1f 
//		|| fabsf(m_moveSpeed.z) > 0.1f
//		) {
//		animove = run;
//	}
//	if (g_pad[0].IsTrigger(enButtonA) == true
//		&& m_charaCon.IsOnGround() == true
//		) {
//		animove = jump;
//	}
//	if (g_pad[0].IsTrigger(enButtonX) == true
//		&& m_charaCon.IsOnGround() == true
//		) {
//		animove = attack;
//	}
//}

//�W�����v
//void Player::Jump()
//{
//	if (m_charaCon.IsOnGround() == false) {
//		m_moveSpeed.y += 450.0f;
//		//�L�����R�����g���Ĉړ�����B
//		m_position = m_charaCon.Execute(1.0f / 60.0f, m_moveSpeed);
//
//	}
//	else
//	{
//		if (m_animation.IsPlaying() == false) {
//			if (fabsf(m_moveSpeed.x) > 0.1f
//				|| fabsf(m_moveSpeed.z) > 0.1f
//				) {
//				animove = run;
//			}
//			else {
//
//				animove = idle;
//			}
//		}
//	}
//}
//�U��
//void Player::Attack()
//{
//	if (m_animation.IsPlaying() == false) {
//		if (fabsf(m_moveSpeed.x) > 0.1f
//			|| fabsf(m_moveSpeed.z) > 0.1f
//			) {
//			animove = run;
//		}
//		else {
//
//			animove = idle;
//		}
//	}
//}

void Player::Update()
{	
	Run();
	m_stMa.Update();

	/*switch (animove)
	{
	case idle:
		Idle();
		m_animation.Play(idle, 0.2f);
		break;
	case run:
		Run();
		m_animation.Play(run, 0.2f);
		break;
	case jump:
		Jump();
		m_animation.Play(jump, 0.2f);
		break;
	case attack:
		Attack();
		m_animation.Play(attack, 0.2f);
		break;
	}*/
	//���[���h�s��̍X�V�B
	m_model.UpdateWorldMatrix(m_position, m_rotation, m_scale);
	//�A�j���[�V�����𗬂��B
	m_animation.Update(1.0f / 60.0f);
}

void Player::Draw()
{
	m_model.Draw(
		g_camera3D.GetViewMatrix(), 
		g_camera3D.GetProjectionMatrix()
	);
}
