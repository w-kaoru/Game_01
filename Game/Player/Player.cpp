#include "stdafx.h"
#include "Player.h"
#include "PlStateMachine.h"


Player::Player() :m_stMa(this)
{
	//cmo�t�@�C���̓ǂݍ��݁B
	m_model.Init(L"Assets/modelData/ToonRTS_demo_Knight.cmo", enFbxUpAxisZ);

	//tka�t�@�C���̓ǂݍ��݁B
	m_animationClips[PlayerState::AnimState::idle].Load(L"Assets/animData/plidle.tka");
	m_animationClips[PlayerState::AnimState::idle].SetLoopFlag(true);

	m_animationClips[PlayerState::AnimState::run].Load(L"Assets/animData/plrun.tka");
	m_animationClips[PlayerState::AnimState::run].SetLoopFlag(true);

	//m_animationClips[PlayerState::AnimState::jump].Load(L"Assets/animData/jump.tka");
	//m_animationClips[PlayerState::AnimState::jump].SetLoopFlag(false);

	m_animationClips[PlayerState::AnimState::attack].Load(L"Assets/animData/plattack.tka");
	m_animationClips[PlayerState::AnimState::attack].SetLoopFlag(false);

	//�A�j���[�V�����̏������B
	m_animation.Init(
		m_model,			//�A�j���[�V�����𗬂��X�L�����f���B
							//����ŃA�j���[�V�����ƃX�L�����f�����֘A�t�������B
		m_animationClips,	//�A�j���[�V�����N���b�v�̔z��B
		4					//�A�j���[�V�����N���b�v�̐��B
	);
	//m_scale *= 0.1f;
	m_hpSprite.Init(L"Assets/sprite/hp_gauge.dds", 100.0f, 10.0f);
	m_hpFrameSprite.Init(L"Assets/sprite/hp_frame.dds", 100.0f, 10.0f);
}
Player::~Player()
{
	g_graphicsEngine->GetShadowMap()->UnregistShadowCaster(&m_model);
	m_charaCon.RemoveRigidBoby();
}
bool Player::Start()
{
	//�X�e�[�^�X�ݒ�
	//m_status.Lv = 1;
	m_maxHp =  6.0f;		//�̗�
	m_hpFrame = m_hp;
	m_hp = m_maxHp;
	m_agi = 1000.0f; //400.0f;			//�f����
	m_position.y = 200.0f;
	m_respawnPosition = m_position;
	//�L�����N�^�[�R���g���[���[�̏������B
	m_charaCon.Init(10.0f, 50.0f, m_position);
	m_stMa.Start();
	/*m_battle->SetHP(m_hp);
	m_battle->SetATK(m_atk);
	m_battle->SetDEF(m_def);*/
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
	m_moveSpeed += cameraForward * lStick_y * m_agi;	//�������ւ̈ړ����x�����B
	m_moveSpeed += cameraRight * lStick_x * m_agi;		//�E�����ւ̈ړ����x�����Z�B
	//m_moveSpeed.y -= 980.0f * (1.0f / 60.0f);
	//�L�����R�����g���Ĉړ�����B
	m_position = m_charaCon.Execute(1.0f / 60.0f, m_moveSpeed);
	//�������ς���B
	if (fabsf(m_moveSpeed.x) > 0.1f
		|| fabsf(m_moveSpeed.z) > 0.1f) {
		auto angle = atan2f(m_moveSpeed.x, m_moveSpeed.z);
		m_rotation.SetRotation(CVector3::AxisY(), angle);
		m_stMa.Change(PlayerState::AnimState::run);
	}
	else {
		m_stMa.Change(PlayerState::AnimState::idle);
	}

	/*if (m_charaCon.IsJump() == true && m_charaCon.IsOnGround() == false) {
		m_moveSpeed.y += 450.0f;
	}*/
}

//�U��
void Player::Attack()
{
	m_stMa.Change(PlayerState::AnimState::attack);
	 /*
	m_forward = CVector3::AxisZ();
	m_rotation.Multiply(m_forward);
	//m_forward��m_moveSpeed�Ƃ̓��ς��v�Z����B
	float d = m_forward.Dot(m_moveSpeed);
	//���ς̌��ʂ�acos�֐��ɓn���āAenemyForward��m_moveSpeed�̂Ȃ��p�����߂�B
	float angle = acos(d);
	//����p����
	//fabsf�͐�Βl�����߂�֐��I
	//�p�x�̓}�C�i�X�����݂��邩��A��Βl�ɂ���B
	CVector3 hit = m_position;
	hit.y += 25.0f;
	hit += m_forward * 25.0f;
	g_battleController->Hit(hit, BattleHit::enemy);
	if (fabsf(angle) < CMath::DegToRad(60.0f)) {
		g_battleController->Hit(hit, BattleHit::enemy);
	}
	// */
}

void Player::HP_Gauge()
{
	m_hpFrameSprite.Update(
		{ -640.0f,360.0f, 0.0f },
		CQuaternion::Identity(),
		{ m_hpFrame, 1.5f, 1.0f },
		{0.0f,1.0f}
	);
	m_hpSprite.Update(
		{ -640.0f,360.0f, 0.0f },
		CQuaternion::Identity(),
		{ m_hp, 1.5f, 1.0f },
		{ 0.0f,1.0f }
	);
	m_hpFrameSprite.Draw(
		g_camera2D.GetViewMatrix(),
		g_camera2D.GetProjectionMatrix()
	);
	m_hpSprite.Draw(
		g_camera2D.GetViewMatrix(),
		g_camera2D.GetProjectionMatrix()
	);
}

void Player::Damage()
{
	m_damageTiming++;
	if (m_damageTiming == 50) {
		m_hp -= 0.3f;
	}
}

void Player::Update()
{
	m_damageTiming = 0.0f;
	m_stMa.Update();
	
	//���������H
	m_hit = g_battleController->Create(
		&m_position, 300.0f,
		[&]() {Damage(); },
		BattleHit::player
	);

	if (g_pad[0].IsTrigger(enButtonA) && atk == false) {
		atk = true;
		Attack();
	}
	if (atk == true) {
		if (m_animation.IsPlaying() == false) {
			atk = false;
		}
	}
	else {
		Move();
	}
	//*
	//*/
	//�V���h�E�L���X�^�[��o�^�B
	g_graphicsEngine->GetShadowMap()->RegistShadowCaster(&m_model);
	//m_model.SetShadowReciever(true);
	if (m_hp <= 0.0f) {
		m_hp = m_maxHp;
		m_position = m_respawnPosition;
	}
	//���[���h�s��̍X�V�B
	m_moveSpeed.y -= 980.0f * (1.0f / 60.0f);
	m_model.UpdateWorldMatrix(m_position, m_rotation, m_scale);
	//�A�j���[�V�����𗬂��B
	m_animation.Update(1.0f / 30.0f);

	m_rotMatrix.MakeRotationFromQuaternion(m_rotation);
	m_forward.x = m_rotMatrix.m[2][0];
	m_forward.y = m_rotMatrix.m[2][1];
	m_forward.z = m_rotMatrix.m[2][2];
	m_forward.Normalize();
}

void Player::Draw()
{
	m_model.Draw(
		enRenderMode_Normal,
		g_camera3D.GetViewMatrix(), 
		g_camera3D.GetProjectionMatrix()
	);
}

void Player::PostDraw()
{
	HP_Gauge();
}
