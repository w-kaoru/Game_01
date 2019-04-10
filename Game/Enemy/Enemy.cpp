#include "stdafx.h"
#include "Enemy.h"
#include "Player\Player.h"


Enemy::Enemy():m_ensm(this)
{
}


Enemy::~Enemy()
{
	g_graphicsEngine->GetShadowMap()->UnregistShadowCaster(&m_model);
	m_charaCon.RemoveRigidBoby();
	g_battleController->Deleteobjict(m_hit);
}

bool Enemy::Start()
{
	//m_model.Init(L"Assets/modelData/enemy.cmo");

	m_hp = 1.0f;
	m_agi = 400.0f;
	if (m_selectModel == 1) {
		m_model.Init(L"Assets/modelData/enemy_01.cmo");
	}
	if (m_selectModel == 2) {
		m_model.Init(L"Assets/modelData/enemy_01.cmo");
	}

	//tka�t�@�C���̓ǂݍ��݁B
	m_animationClips[EnemyState::idle].Load(L"Assets/animData/enidle.tka");
	m_animationClips[EnemyState::idle].SetLoopFlag(true);

	m_animationClips[EnemyState::move].Load(L"Assets/animData/enwalk.tka");
	m_animationClips[EnemyState::move].SetLoopFlag(true);

	m_animationClips[EnemyState::attack].Load(L"Assets/animData/enattack.tka");
	m_animationClips[EnemyState::attack].SetLoopFlag(false);
	//�A�j���[�V�����̏������B
	m_animation.Init(
		m_model,			//�A�j���[�V�����𗬂��X�L�����f���B
							//����ŃA�j���[�V�����ƃX�L�����f�����֘A�t�������B
		m_animationClips,	//�A�j���[�V�����N���b�v�̔z��B
		4					//�A�j���[�V�����N���b�v�̐��B
	);
	m_hpSprite.Init(L"Assets/sprite/hp_gauge.dds", 100.0f, 10.0f);
	m_position.y = 200.0f;
	//�L�����N�^�[�R���g���[���[�̏������B
	m_charaCon.Init(40.0f, 70.0f, m_position);
	m_ensm.Start();
	m_scale *= 2.0f;
	//���������H
	m_hit = g_battleController->Create(
		&m_position, 150.0f,
		[&](float damage) {Damage(damage); },
		BattleHit::enemy
	);
	return true;
}

void Enemy::PlLen()
{
	//�G�l�~�[����v���C���[�ɐL�т�x�N�g�������߂�B
	m_moveSpeed = m_player->GetPosition() - m_position;
	//���K�����s���O�ɁA�v���C���[�܂ł̋��������߂Ă����B
	m_toPlayerLen = m_moveSpeed.Length();
	//���K���I
	m_moveSpeed.Normalize();
}

void Enemy::Search()
{
	PlLen();
	m_forward = CVector3::AxisZ();
	m_rotation.Multiply(m_forward);
	//enemyForward��m_moveSpeed�Ƃ̓��ς��v�Z����B
	float d = m_forward.Dot(m_moveSpeed);
	//���ς̌��ʂ�acos�֐��ɓn���āAenemyForward��m_moveSpeed�̂Ȃ��p�����߂�B
	float angle = acos(d);
	//����p����
	//fabsf�͐�Βl�����߂�֐��I
	//�p�x�̓}�C�i�X�����݂��邩��A��Βl�ɂ���B
	// /*
	if (fabsf(angle) < CMath::DegToRad(60.0f)) {
		if (m_toPlayerLen <= 150.0) {
			Attack();
		}
	}
	// */
	if (attackFlag == true) {
		if (m_animation.IsPlaying() == false) {
			attackFlag = false;
		}
	}
	else
	{
		if (m_toPlayerLen > 150.0f && m_toPlayerLen < 600.0f) {
			m_ensm.Change(EnemyState::MoveState::move);
		}
		else {
			m_ensm.Change(EnemyState::MoveState::idle);
		}
	}
}
 // /*
void Enemy::Attack()
{
	m_AttackTiming++;
	if (m_AttackTiming == 70.0f) {
		m_ensm.Change(EnemyState::MoveState::attack);
		attackFlag = true;
		m_AttackTiming = 0;
	}
}
//*/

void Enemy::Damage(float damage)
{
	m_hp -= damage;
}

void Enemy::HP_Gauge()
{
	auto pos = m_position;
	pos.y += 200.0f;
	CVector3 hp_angle;
	hp_angle = g_camera3D.GetPosition() - m_position;
	hp_angle.y = 0;
	hp_angle.Normalize();
	float angle = acos(hp_angle.Dot(m_Sprite_Front));
	angle = CMath::RadToDeg(angle);
	CVector3 hp_axis;
	hp_axis.Cross(m_Sprite_Front, hp_angle);
	if (hp_axis.y > 0) {
		m_Sprite_angle.SetRotationDeg(CVector3::AxisY(), angle);
	}
	else {
		m_Sprite_angle.SetRotationDeg(CVector3::AxisY()*-1, angle);
	}
	m_hpSprite.Update(pos, m_Sprite_angle, { m_hp, 1.0f, 1.0f });
	m_hpSprite.Draw(
		g_camera3D.GetViewMatrix(),
		g_camera3D.GetProjectionMatrix()
	);
}

void Enemy::Update()
{
	m_damageTiming = 0;
	//Move();
	Search();
	m_ensm.Update();
	//�d�͉����x
	m_moveSpeed.y -= 9800.0f * (1.0f / 60.0f);
	//�L�����R�����g���Ĉړ�����B
	m_position = m_charaCon.Execute(1.0f / 60.0f, m_moveSpeed);

	//�V���h�E�L���X�^�[��o�^�B
	g_graphicsEngine->GetShadowMap()->RegistShadowCaster(&m_model);
	//m_model.SetShadowReciever(true);
	//���[���h�s������߂�B
	m_model.UpdateWorldMatrix(m_position, m_rotation, m_scale);
	//�A�j���[�V�����𗬂��B
	m_animation.Update(1.0f / 30.0f);
	if (m_hp <= 0.01f) {
		g_gameObjM->DeleteGameObject(this);
	}
}

void Enemy::Draw()
{
	m_model.Draw(
		enRenderMode_Normal,
		g_camera3D.GetViewMatrix(),
		g_camera3D.GetProjectionMatrix()
	);
}

void Enemy::PostDraw()
{
	HP_Gauge();
}
