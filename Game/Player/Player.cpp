#include "stdafx.h"
#include "Player.h"
#include "PlStateMachine.h"
#include "../Game.h"
#include "../GameEnd.h"


Player::Player() :m_stMa(this)
{
	//cmo�t�@�C���̓ǂݍ��݁B
	m_model.Init(L"Assets/modelData/ToonRTS_demo_Knight.cmo", enFbxUpAxisZ);
	//m_model_01.Init(L"Assets/modelData/plpath.cmo", enFbxUpAxisZ);
	//tka�t�@�C���̓ǂݍ��݁B
	//�ҋ@�A�j���[�V����
	m_animationClips[PlayerState::AnimState::idle].Load(L"Assets/animData/plidle.tka");
	m_animationClips[PlayerState::AnimState::idle].SetLoopFlag(true);

	//����A�j���[�V����
	m_animationClips[PlayerState::AnimState::run].Load(L"Assets/animData/plrun.tka");
	m_animationClips[PlayerState::AnimState::run].SetLoopFlag(true);

	//�U���A�j���[�V����
	m_animationClips[PlayerState::AnimState::attack].Load(L"Assets/animData/plattack.tka");
	m_animationClips[PlayerState::AnimState::attack].SetLoopFlag(false);

	//�A�j���[�V�����̏������B
	m_animation.Init(
		m_model,			//�A�j���[�V�����𗬂��X�L�����f���B
							//�A�j���[�V�����ƃX�L�����f���̊֘A�t���������B
		m_animationClips,	//�A�j���[�V�����N���b�v�̔z��B
		4					//�A�j���[�V�����N���b�v�̐��B
	);
	//m_scale *= 0.1f;
	//HP�̉摜�̓ǂݍ���
	m_hpSprite.Init(L"Assets/sprite/hp_gauge.dds", m_HpScaleX, m_HpScaleY);
	m_hpFrameSprite.Init(L"Assets/sprite/hp_frame.dds", m_HpScaleX, m_HpScaleY);
}
Player::~Player()
{
	//�e�����O����B
	g_graphicsEngine->GetShadowMap()->UnregistShadowCaster(&m_model);
	//�L�����N�^�[�R���g���[���[����������B
	m_charaCon.RemoveRigidBoby();
	//�����蔻��������B
	g_battleController->Deleteobjict(m_hit);
}
bool Player::Start()
{
	//�X�e�[�^�X�̐ݒ�
	//m_status.Lv = 1;
	m_hpFrame = 6;
	m_hp = 60;
	m_agi = 1000.0f; //400.0f;			//�f����
	//�|�W�V������������ɂ��Ă����B
	m_position.y = 200.0f;
	//�����̃|�W�V������ݒ肵�ă��X�|�[���n�_�ɂ���B
	m_respawnPosition = m_position;
	//�L�����N�^�[�R���g���[���[�̏������B
	m_charaCon.Init(40.0f, 70.0f, m_position);
	m_stMa.Start();
	m_rotation.SetRotationDeg(CVector3::AxisY(), 180.0f);
	//�����蔻������B
	m_hit = g_battleController->Create(
		&m_position, 150.0f,
		[&](float damage) {Damage(damage); },
		BattleHit::player
	);
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
	//�������ւ̈ړ����x�����B
	m_moveSpeed += cameraForward * lStick_y * m_agi;
	//�E�����ւ̈ړ����x�����Z�B
	m_moveSpeed += cameraRight * lStick_x * m_agi;
	//�L�����R�����g���Ĉړ�����B
	m_position = m_charaCon.Execute(1.0f / 60.0f, m_moveSpeed);
	//������ς���B
	if (fabsf(m_moveSpeed.x) > 0.1f
		|| fabsf(m_moveSpeed.z) > 0.1f) {
		auto angle = atan2f(m_moveSpeed.x, m_moveSpeed.z);
		m_rotation.SetRotation(CVector3::AxisY(), angle);
		//����A�j���[�V�����̍Đ����邽�߂ɃX�e�[�g�̕ύX
		m_stMa.Change(PlayerState::AnimState::run);
	}
	else {
		//�ҋ@�A�j���[�V�����̍Đ����邽�߂ɃX�e�[�g�̕ύX
		m_stMa.Change(PlayerState::AnimState::idle);
	}
}

//�U��
void Player::Attack()
{
	if (g_pad[0].IsTrigger(enButtonA) && m_atkAnim == false) {
		//A�{�^����������čU�����[�V���������ĂȂ����B
		m_atkAnim = true;
		//�U���A�j���[�V�����̍Đ����邽�߂ɃX�e�[�g�̕ύX
		m_stMa.Change(PlayerState::AnimState::attack);
	}
	if (m_atkAnim == true) {
		m_hitTiming++;
		//�U������Ă��炠�������^�C�~���O�ōU���������i�ȈՔŁj
		if (m_hitTiming == 12) {
			//�U�����q�b�g������B
			g_battleController->Hit(m_attckPos, 1.0f, BattleHit::enemy);
		}
		if (m_animation.IsPlaying() == false) {
			m_atkAnim = false;
			//�U���̊Ԋu��0�ɖ߂��B
			m_hitTiming = 0;
		}
	}
	else {

		//�U�����Ă��Ȃ����Ɉړ��Ȃǂ̏����B
		Move();
	}
}

//HP��\������X�v���C�g�̂��߂̊֌W�B
void Player::HP_Gauge()
{
	//�X�v���C�g�̍X�V
	m_hpFrameSprite.Update(
		{ -640.0f,350.0f, 0.0f },
		CQuaternion::Identity(),
		{ m_hpFrame, 1.5f, 1.0f },
		{ 0.0f,1.0f }
	);
	//�X�v���C�g�̍X�V
	m_hpSprite.Update(
		{ -640.0f,350.0f, 0.0f },
		CQuaternion::Identity(),
		{ m_hp / 10, 1.5f, 1.0f },
		{ 0.0f,1.0f }
	);
	//�X�v���C�g���Q�����ŕ\��������B
	m_hpFrameSprite.Draw(
		g_camera2D.GetViewMatrix(),
		g_camera2D.GetProjectionMatrix()
	);
	//�X�v���C�g���Q�����ŕ\��������B
	m_hpSprite.Draw(
		g_camera2D.GetViewMatrix(),
		g_camera2D.GetProjectionMatrix()
	);
}

//�_���[�W
void Player::Damage(float Enatk)
{
	//�U������������̂�HP���炭�������������
	m_hp -= Enatk;
}

void Player::Update()
{
	//�X�e�[�g�}�V���̍X�V�B
	m_stMa.Update();
	//m_model.SetShadowReciever(true);

	//���S�̔���
	if (m_hp <= 0.0f) {
		//if (ki <= 3) {
		//	//HP�������̒l�ɖ߂��B
		//	m_hp = 60;
		//	//���X�|�[���n�_�֓]��
		//	m_position = m_respawnPosition;
		//	//�L�����R���Ƀ|�W�V������ݒ�B
		//	m_charaCon.SetPosition(m_position);
		//	ki++;
		//}
		g_gameObjM->DeleteGO(g_gameObjM->FindGO<Game>());
		g_gameObjM->NewGO<GameEnd>()->SetGameEnd(GameEnd::GameEndState::gameOver);
	}
	//�U��
	Attack();
	//�O�����̎擾�B
	m_rotMatrix.MakeRotationFromQuaternion(m_rotation);
	m_forward.x = m_rotMatrix.m[2][0];
	m_forward.y = m_rotMatrix.m[2][1];
	m_forward.z = m_rotMatrix.m[2][2];
	m_forward.Normalize();
	//�U���̓����蔻��̃|�W�V����
	//�v���C���[�̃|�W�V�����̏����O�̈ʒu
	m_attckPos.x = m_position.x + m_forward.x * 60.0f;
	m_attckPos.z = m_position.z + m_forward.z * 60.0f;
	m_attckPos.y = 100.0f;
	//�d��
	m_moveSpeed.y -= 980.0f * (1.0f / 60.0f);
	//���[���h�s��̍X�V�B
	m_model.UpdateWorldMatrix(m_position, m_rotation, m_scale);
	//m_model_01.UpdateWorldMatrix(m_attckPos, CQuaternion::Identity(), CVector3::One());
	//�A�j���[�V�����𗬂��B
	m_animation.Update(1.0f / 30.0f);
	//�V���h�E�L���X�^�[��o�^�B
	g_graphicsEngine->GetShadowMap()->RegistShadowCaster(&m_model);

}

void Player::Draw()
{
	//���f����`��B
	m_model.Draw(
		enRenderMode_Normal,
		g_camera3D.GetViewMatrix(), 
		g_camera3D.GetProjectionMatrix()
	);
	/*m_model_01.Draw(
		enRenderMode_Normal,
		g_camera3D.GetViewMatrix(),
		g_camera3D.GetProjectionMatrix()
	);*/
}

void Player::PostDraw()
{
	//�X�v���C�g�̕`��B
	HP_Gauge();
}
