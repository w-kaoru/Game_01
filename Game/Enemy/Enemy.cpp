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
	g_battleController->Deleteobjict(m_hit);

	m_effect->Release();
	g_graphicsEngine->GetEffekseerManager()->StopEffect(m_playEffectHandle);
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
		m_animationClips[EnemyState::idle].Load(L"Assets/animData/enidle.tka");
		m_animationClips[EnemyState::idle].SetLoopFlag(true);
		//�����A�j���[�V����
		m_animationClips[EnemyState::move].Load(L"Assets/animData/enwalk.tka");
		m_animationClips[EnemyState::move].SetLoopFlag(true);
		//�U���A�j���[�V����
		m_animationClips[EnemyState::attack].Load(L"Assets/animData/enattack.tka");
		m_animationClips[EnemyState::attack].SetLoopFlag(false);
		//�G�l�~�[�̑傫���B
		m_scale *= 2.0f;
		//�U���𓖂Ă�^�C�~���O
		m_atkHit = 45;
		//�G�l�~�[�̊�b�X�e�[�^�X�B
		m_status.SetHp(10.0f);
		m_status.SetAgi(550.0f);
		m_status.SetDef(1.5f);
		m_status.SetAtk(3.0f);
		break;
	case type_troll:
		m_model.Init(L"Assets/modelData/TrollGiant.cmo");
		//tka�t�@�C���̓ǂݍ��݁B
		//�ҋ@�A�j���[�V����
		m_animationClips[EnemyState::idle].Load(L"Assets/animData/troll_anim/troll_idle.tka");
		m_animationClips[EnemyState::idle].SetLoopFlag(true);
		//�����A�j���[�V����
		m_animationClips[EnemyState::move].Load(L"Assets/animData/troll_anim/troll_walk.tka");
		m_animationClips[EnemyState::move].SetLoopFlag(true);
		//�U���A�j���[�V����
		m_animationClips[EnemyState::attack].Load(L"Assets/animData/troll_anim/troll_attack.tka");
		m_animationClips[EnemyState::attack].SetLoopFlag(false);
		m_scale *= 90.0f;
		//�U���𓖂Ă�^�C�~���O
		m_atkHit = 40;
		//�G�l�~�[�̊�b�X�e�[�^�X�B
		m_status.SetHp(15.0f);
		m_status.SetAgi(550.0f);
		m_status.SetDef(2.0f);
		m_status.SetAtk(4.0f);
		break;
	}
	m_status.StatusUp();
	//�A�j���[�V�����̏������B
	m_animation.Init(
		m_model,			//�A�j���[�V�����𗬂��X�L�����f���B
							//����ŃA�j���[�V�����ƃX�L�����f�����֘A�t�������B
		m_animationClips,	//�A�j���[�V�����N���b�v�̔z��B
		4					//�A�j���[�V�����N���b�v�̐��B
	);
	//hp�o�[�̃X�v���C�g�B
	m_hpSprite.Init(L"Assets/sprite/hp_gauge.dds", 100.0f, 10.0f);
	m_position.y = 200.0f;
	//�L�����N�^�[�R���g���[���[�̏������B
	m_charaCon.Init(40.0f, 70.0f, m_position);
	//�G�l�~�[�̃X�e�[�g�}�V���̃X�^�[�g�֐����ĂԁB
	//�X�e�[�g�}�V���̏������B
	m_ensm.Start();
	//�����蔻��̍쐬�B
	m_hit = g_battleController->Create(
		&m_position, 150.0f,
		[&](float damage) {Damage(damage); },
		BattleHit::enemy
	);
	//SE
	m_se_damade.Init(L"Assets/sound/se_damage.wav");
	m_se_damade.SetVolume(1.0f);	
	m_effect = Effekseer::Effect::Create(
		g_graphicsEngine->GetEffekseerManager(),
		(const EFK_CHAR*)L"Assets/effect/test.efk"
	);
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
	//enemyForward��m_moveSpeed�Ƃ̓��ς��v�Z����B
	float d = m_forward.Dot(m_moveSpeed);
	//���ς̌��ʂ�acos�֐��ɓn���āAenemyForward��m_moveSpeed�̂Ȃ��p�����߂�B
	float angle = acos(d);
	//����p����
	//fabsf�͐�Βl�����߂�֐��I
	//�p�x�̓}�C�i�X�����݂��邩��A��Βl�ɂ���B
	if (m_toPlayerLen <= 160.0) {
		//�v���C���[�Ƃ̋��������ȉ���
		//�U���֐�����ׁ[�B
		Attack();
	}
	if (attackFlag == true) {
		//�U���A�j���[�V�������I��������B
		if (m_animation.IsPlaying() == false) {
			attackFlag = false;
			m_AttackTiming = 0;
		}
	}
	else
	{
		//�U���A�j���[�V�������Đ�����ĂȂ���
		if (m_toPlayerLen > 130.0f && m_toPlayerLen < 1200.0f) {
			//�����A�j���[�V�����̍Đ����邽�߂ɃX�e�[�g�̕ύX
			m_ensm.Change(EnemyState::MoveState::move);
		}
		else {
			//�ҋ@�A�j���[�V�����̍Đ����邽�߂ɃX�e�[�g�̕ύX
			m_ensm.Change(EnemyState::MoveState::idle);
		}
	}
}
 //�U��
void Enemy::Attack()
{
	m_AttackTiming++;
	//�U���̊Ԋu
	if (m_AttackTiming == m_atkAnimStart) {
		//�U���A�j���[�V�����̍Đ����邽�߂ɃX�e�[�g�̕ύX
		m_ensm.Change(EnemyState::MoveState::attack);
		attackFlag = true;
	}
	//�U������Ă��炠�������^�C�~���O�ōU���������i�ȈՔŁj
	if (m_AttackTiming == m_atkHit) {
		//���������Ǝv����^�C�~���O��
		CVector3 hit = m_position;
		hit.y += 50.0f;
		hit += m_forward * 50.0f;
		//�U�����q�b�g������B
		g_battleController->Hit(hit, m_status.GetAtk(), BattleHit::player);
		//�U���̊Ԋu��0�ɖ߂��B
		m_AttackTiming = 0;
	}
}
//�_���[�W
void Enemy::Damage(float damage)
{
	m_se_damade.Play(false);
	//�U������������̂�HP���炭�������������
	float hp = m_status.GetHp();
	hp = (hp + m_status.GetDef()) - damage;
	m_status.SetHp(hp);
}

//HP��\������X�v���C�g�̂��߂̊֌W�B
void Enemy::HP_Gauge()
{
	//�|�W�V�����𓪂̏�t�߂ɂ���B
	auto pos = m_position;
	pos.y += 200.0f;
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
	//HP�X�v���C�g�̍X�V
	m_hpSprite.Update(pos, m_Sprite_angle, { m_status.GetHp() / 10, 1.0f, 1.0f });
	//HP�X�v���C�g�̕\��
	m_hpSprite.Draw(
		g_camera3D.GetViewMatrix(),
		g_camera3D.GetProjectionMatrix()
	);
}

void Enemy::Update()
{
	if (m_isDeath == false) {
		m_damageTiming = 0;
		Search();
		//�X�e�[�g�}�V���̍X�V
		m_ensm.Update();
		
		if (m_status.GetHp() <= 0.01f&&m_isDeath == false) {
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
			g_gameObjM->FindGO<Game>()->EnemyDeath();
			m_player->EXP(1);
		}
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
	
	}
	else
	{
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
