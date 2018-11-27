#include "stdafx.h"
#include "Player.h"
#include "PlStateMachine.h"


Player::Player() :m_stMa(this)
{
	//cmo�t�@�C���̓ǂݍ��݁B
	m_model.Init(L"Assets/modelData/ToonRTS_demo_Knight.cmo", enFbxUpAxisZ);

	//tka�t�@�C���̓ǂݍ��݁B
	m_animationClips[PlayerState::idle].Load(L"Assets/animData/plidle.tka");
	m_animationClips[PlayerState::idle].SetLoopFlag(true);

	m_animationClips[PlayerState::run].Load(L"Assets/animData/plrun.tka");
	m_animationClips[PlayerState::run].SetLoopFlag(true);

	//m_animationClips[PlayerState::jump].Load(L"Assets/animData/jump.tka");
	//m_animationClips[PlayerState::jump].SetLoopFlag(false);

	m_animationClips[PlayerState::attack].Load(L"Assets/animData/plattack.tka");
	m_animationClips[PlayerState::attack].SetLoopFlag(false);

	//�A�j���[�V�����̏������B
	m_animation.Init(
		m_model,			//�A�j���[�V�����𗬂��X�L�����f���B
							//����ŃA�j���[�V�����ƃX�L�����f�����֘A�t�������B
		m_animationClips,	//�A�j���[�V�����N���b�v�̔z��B
		4					//�A�j���[�V�����N���b�v�̐��B
	);
	//m_scale *= 4;
}
Player::~Player()
{
	m_charaCon.RemoveRigidBoby();
}
bool Player::Start()
{
	m_position.y = 200.0f;
	//�L�����N�^�[�R���g���[���[�̏������B
	m_charaCon.Init(10.0f, 50.0f, m_position);
	m_stMa.Start();
	return true;
}
//�ړ�
void Player::Move()
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
	if (g_pad[0].IsTrigger(enButtonA) == true
		&& m_charaCon.IsOnGround() == true
		) {
		PlayerState::AniMove::jump;
	}
	if (m_charaCon.IsJump() == true && m_charaCon.IsOnGround() == false) {
		m_moveSpeed.y += 450.0f;
	}
}


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
	Move();
	m_stMa.Update();
	//���[���h�s��̍X�V�B
	m_model.UpdateWorldMatrix(m_position, m_rotation, m_scale);
	//�A�j���[�V�����𗬂��B
	m_animation.Update(1.0f / 30.0f);
}

void Player::Draw()
{
	m_model.Draw(
		g_camera3D.GetViewMatrix(), 
		g_camera3D.GetProjectionMatrix()
	);
}