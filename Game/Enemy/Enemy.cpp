#include "stdafx.h"
#include "Enemy.h"
#include "Player\Player.h"
#include "../Game.h"


Enemy::Enemy():m_ensm(this)
{
}


Enemy::~Enemy()
{
	//�e�����O����B
	g_graphicsEngine->GetShadowMap()->UnregistShadowCaster(&m_model);
	//�L�����N�^�[�R���g���[���[����������B
	m_charaCon.RemoveRigidBoby();
	//�����蔻��������B
	g_hitObject->Delete(m_hit);

	m_effect->Release();
}

void Enemy::Destroy()
{
}

bool Enemy::Start()
{
	//m_model.Init(L"Assets/modelData/enemy.cmo");

	//m_spd = 500.0f;
	switch (m_type)
	{
	case type_skeleton:
		m_model.Init(L"Assets/modelData/enemy_01.cmo");
		//tka�t�@�C���̓ǂݍ��݁B
		//�ҋ@�A�j���[�V����
		m_animationClips[EnemyState::AnimationState::AnimIdle].Load(L"Assets/animData/enemy/enidle.tka");
		m_animationClips[EnemyState::AnimationState::AnimIdle].SetLoopFlag(true);
		//�����A�j���[�V����
		m_animationClips[EnemyState::AnimationState::AnimMove].Load(L"Assets/animData/enemy/enwalk.tka");
		m_animationClips[EnemyState::AnimationState::AnimMove].SetLoopFlag(true);
		//�U���A�j���[�V����
		m_animationClips[EnemyState::AnimationState::AnimAttack].Load(L"Assets/animData/enemy/enattack.tka");
		m_animationClips[EnemyState::AnimationState::AnimAttack].SetLoopFlag(false);
		//�_���[�W�A�j���[�V����
		m_animationClips[EnemyState::AnimationState::AnimDamage].Load(L"Assets/animData/enemy/endamage.tka");
		m_animationClips[EnemyState::AnimationState::AnimDamage].SetLoopFlag(false);
		//�G�l�~�[�̑傫���B
		m_scale *= 2.0f;
		//�U���𓖂Ă�^�C�~���O
		m_atkHit = 25;
		break;
	case type_troll:
		m_model.Init(L"Assets/modelData/TrollGiant.cmo");
		//tka�t�@�C���̓ǂݍ��݁B
		//�ҋ@�A�j���[�V����
		m_animationClips[EnemyState::AnimationState::AnimIdle].Load(L"Assets/animData/troll_anim/troll_idle.tka");
		m_animationClips[EnemyState::AnimationState::AnimIdle].SetLoopFlag(true);
		//�����A�j���[�V����
		m_animationClips[EnemyState::AnimationState::AnimMove].Load(L"Assets/animData/troll_anim/troll_walk.tka");
		m_animationClips[EnemyState::AnimationState::AnimMove].SetLoopFlag(true);
		//�U���A�j���[�V����
		m_animationClips[EnemyState::AnimationState::AnimAttack].Load(L"Assets/animData/troll_anim/troll_attack.tka");
		m_animationClips[EnemyState::AnimationState::AnimAttack].SetLoopFlag(false);
		//�_���[�W�A�j���[�V����
		m_animationClips[EnemyState::AnimationState::AnimDamage].Load(L"Assets/animData/troll_anim/troll_damage.tka");
		m_animationClips[EnemyState::AnimationState::AnimDamage].SetLoopFlag(false);
		m_scale *= 90.0f;
		//�U���𓖂Ă�^�C�~���O
		m_atkHit = 23;
		break;
	}
	//�A�j���[�V�����̏������B
	m_animation.Init(
		m_model,								//�A�j���[�V�����𗬂��X�L�����f���B
												//����ŃA�j���[�V�����ƃX�L�����f�����֘A�t�������B
		m_animationClips,						//�A�j���[�V�����N���b�v�̔z��B
		EnemyState::AnimationState::AnimNum		//�A�j���[�V�����N���b�v�̐��B
	);
	//hp�o�[�̃X�v���C�g�B
	m_hpSprite.Init(L"Assets/sprite/hp_gauge.dds", 40.0f, 10.0f);
	m_shieldSprite.Init(L"Assets/sprite/shield.dds", 20.0f, 20.0f);
	m_position.y = 200.0f;
	//�L�����N�^�[�R���g���[���[�̏������B
	m_charaCon.Init(40.0f, 70.0f, m_position);
	//�G�l�~�[�̃X�e�[�g�}�V���̃X�^�[�g�֐����ĂԁB
	//�X�e�[�g�}�V���̏������B
	m_ensm.Start();
	//�����蔻��̍쐬�B
	m_hit = g_hitObject->Create(
		&m_position, 150.0f,
		[&](float damage) {Damage(damage); },
		Hit::enemy
	);
	//SE
	m_se_damade.Init(L"Assets/sound/se_damage.wav");
	m_se_damade.SetVolume(1.0f);	
	m_effect = Effekseer::Effect::Create(
		g_graphicsEngine->GetEffekseerManager(),
		(const EFK_CHAR*)L"Assets/effect/death.efk"
	);
	return true;
}

//�v���C���[�Ɍ������x�N�g���ƒ����̎擾�B
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
	//�O�����̎擾�B
	m_forward = CVector3::AxisZ();
	m_rotation.Multiply(m_forward);
	//fabsf�͐�Βl�����߂�֐��I
	//�p�x�̓}�C�i�X�����݂��邩��A��Βl�ɂ���B
	if (m_toPlayerLen <= 160.0) {
		//�v���C���[�Ƃ̋��������ȉ���
		//�U���X�e�[�g����ׁ[�B
		m_ensm.Change(EnemyState::MoveState::attack);
		m_ensm.StateAttack()->SetAttack(m_atkHit, m_atkAnimStart);
	}
	if(m_toPlayerLen > 160.0
		&&m_ensm.StateAttack()->GetAtkFlag() == false)
	{
		m_ensm.Change(EnemyState::MoveState::move);
		m_ensm.StateAttack()->SetTiming(0);
	}
	if (m_ensm.StateAttack()->GetAtkFlag() == true) {
		//�U���A�j���[�V�������I��������B
		if (m_animation.IsPlaying() == false) {
			m_ensm.StateAttack()->SetAtkFlag(false);
			m_ensm.StateAttack()->SetTiming(0);
		}
	}
}
//�_���[�W
void Enemy::Damage(float damage)
{
	m_se_damade.Play(false);
	if (!m_damageCut) {
		m_ensm.StateDamage()->SetDamage(damage);
		m_ensm.Change(EnemyState::MoveState::damage);
	}
	else {
		//�U������������̂�HP���炭�������������
		float hp = m_status.GetHp();
		float Damage = damage - m_status.GetDef();
		Damage = max(0.0f, Damage);
		hp -= (Damage / 3.0f);
		hp = min(hp, m_status.GetHp());
		hp = max(0.0f, hp);
		m_status.SetHp(hp);
	}
	m_damageCutCounter++;
}

void Enemy::DamageCut()
{
	if (m_damageCutCounter >= 2 && m_damageCutSpan >= m_damageCutValue) {
		m_damageCut = true;
	}
	if (m_damageCut) {
		m_damageCutSpan -= 1.5f * (1.0f / 60.0f);
		m_damageCutSpan = max(0.0f, m_damageCutSpan);
		if (m_damageCutSpan <= 0.0f) {
			m_damageCut = false;
		}
	}
	else {
		m_damageCutSpan += 1.0f * (1.0f / 60.0f);
		m_damageCutSpan = min(m_damageCutValue, m_damageCutSpan);
	}
}

//HP��\������X�v���C�g�̂��߂̊֌W�B
void Enemy::HP_Gauge()
{
	//�|�W�V�����𓪂̏�t�߂ɂ���B
	auto pos = m_position;
	auto pos_s = m_position;
	pos.y += 200.0f;
	pos_s.y += 220.0f;
	//�J�����̃|�W�V���������̃x�N�g�����擾�B
	CVector3 hp_angle;
	hp_angle = g_camera3D.GetPosition() - m_position;
	hp_angle.y = 0;
	hp_angle.Normalize();
	//HP�̉摜���J�����Ɍ������đO�������悤�ɂ���B
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
	m_hpGauge = (m_status.GetHp() / m_status.GetMaxHp()) * m_spriteScale;

	//HP�X�v���C�g�̍X�V
	m_hpSprite.Update(pos, m_Sprite_angle, { m_hpGauge, 1.0f, 1.0f });
	//HP�X�v���C�g�̕\��
	m_hpSprite.Draw(
		g_camera3D.GetViewMatrix(),
		g_camera3D.GetProjectionMatrix()
	);
	if (m_damageCut) {
		//HP�X�v���C�g�̍X�V
		m_shieldSprite.Update(pos_s, m_Sprite_angle, { 1.0f, 1.0f, 1.0f });
		//HP�X�v���C�g�̕\��
		m_shieldSprite.Draw(
			g_camera3D.GetViewMatrix(),
			g_camera3D.GetProjectionMatrix()
		);
	}
}

void Enemy::Update()
{
	if (m_isDeath == false) {
		if (m_ensm.StateDamage()->GetDamageFlag() == false) {
			Search();
		}
		else {
			if (m_animation.IsPlaying() == false) {
				m_ensm.Change(EnemyState::MoveState::move);
				m_ensm.StateDamage()->SetDamageFlag(false);
			}
		}
		//�X�e�[�g�}�V���̍X�V
		m_ensm.Update();
		
		if (m_status.GetHp() < 0.01f&&m_isDeath == false) {
			m_isDeath = true;
			m_effectPos = m_position;
			//�G�t�F�N�g�̍Đ��B
			m_playEffectHandle = g_graphicsEngine->GetEffekseerManager()->Play(
				m_effect, m_effectPos.x, m_effectPos.y+100.0f, m_effectPos.z
			);
			//�G�t�F�N�g�̊g�嗦�B
			g_graphicsEngine->GetEffekseerManager()->SetScale(
				m_playEffectHandle,
				20.0f, 20.0f, 20.0f
			);
			m_position.y = -10000.0f;
			m_charaCon.SetPosition(m_position);
			g_gameObjM->FindGO<Game>("Game")->EnemyDeath();
			m_player->EXP(1);
		}
		DamageCut();
		//�d�͉����x
		if (m_charaCon.IsOnGround()) {
			m_moveSpeed.y = 0.0f;
		}
		else {
			m_speedY -= 980.0f * (1.0f / 60.0f);
			m_moveSpeed.y = m_speedY;
		}
		//�L�����R�����g���Ĉړ�����B
		m_position = m_charaCon.Execute(1.0f / 60.0f, m_moveSpeed);

		//�V���h�E�L���X�^�[��o�^�B
		g_graphicsEngine->GetShadowMap()->RegistShadowCaster(&m_model);
		//m_model.SetShadowReciever(true);
		//���[���h�s������߂�B
		m_model.UpdateWorldMatrix(m_position, m_rotation, m_scale);
		//�A�j���[�V�����𗬂��B
		m_animation.Update(1.0f / 30.0f);
	
	}
}

void Enemy::Draw()
{
	//���f����\������B
	m_model.Draw(
		enRenderMode_Normal,
		g_camera3D.GetViewMatrix(),
		g_camera3D.GetProjectionMatrix()
	);
}

void Enemy::PostDraw()
{
	//HP�X�v���C�g�̕\��������B
	HP_Gauge();
}
