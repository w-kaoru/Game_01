#include "stdafx.h"
#include "EnemyBos.h"
#include "Player\Player.h"
#include "../Game.h"
#include "../GameEnd.h"

EnemyBos::EnemyBos():m_enbos_stm(this)
{
}


EnemyBos::~EnemyBos()
{
	//�e�����O����B
	g_graphicsEngine->GetShadowMap()->UnregistShadowCaster(&m_model);
	//�L�����N�^�[�R���g���[���[����������
	m_charaCon.RemoveRigidBoby();
	//�����蔻��������B
	g_hitObject->Delete(m_hit);
	//�@���}�b�v�����[�h�B
	//�t�@�C�������g���āA�e�N�X�`�������[�h���āAShaderResrouceView���쐬����B
	DirectX::CreateDDSTextureFromFileEx(
		g_graphicsEngine->GetD3DDevice(), L"Assets/Assets/sprite/Mutant_normal.dds", 0,
		D3D11_USAGE_DEFAULT, D3D11_BIND_SHADER_RESOURCE, 0, 0,
		false, nullptr, &m_normalMapSRV);
}

bool EnemyBos::Start()
{
	//m_model.Init(L"Assets/modelData/enemy.cmo");

	//�X�e�[�^�X�̐ݒ�
	m_status.SetHp(20.0f);
	m_status.SetAgi(500.0f);
	m_status.SetDef(3.0f);
	m_status.SetAtk(15.0f);
	m_status.StatusUp();
	m_model.Init(L"Assets/modelData/enemy_Bos.cmo");
	//�m�[�}���}�b�v���Z�b�g����B
	m_model.SetNormalMap(m_normalMapSRV);
	//tka�t�@�C���̓ǂݍ��݁B
	//�ҋ@�A�j���[�V����
	m_animationClips[EnemyBosState::AnimationState::AnimIdle].Load(L"Assets/animData/enemy_Bos/enBosidle.tka");
	m_animationClips[EnemyBosState::AnimationState::AnimIdle].SetLoopFlag(true);
	//�����A�j���[�V����
	m_animationClips[EnemyBosState::AnimationState::AnimMove].Load(L"Assets/animData/enemy_Bos/enBosrun.tka");
	m_animationClips[EnemyBosState::AnimationState::AnimMove].SetLoopFlag(true);
	//�U���A�j���[�V����
	m_animationClips[EnemyBosState::AnimationState::AnimAttack].Load(L"Assets/animData/enemy_Bos/enBosattack.tka");
	m_animationClips[EnemyBosState::AnimationState::AnimAttack].SetLoopFlag(false);
	//�_���[�W�A�j���[�V����
	m_animationClips[EnemyBosState::AnimationState::AnimDamage].Load(L"Assets/animData/enemy_Bos/enBosdamage.tka");
	m_animationClips[EnemyBosState::AnimationState::AnimDamage].SetLoopFlag(false);
	//�_���[�W�A�j���[�V����
	m_animationClips[EnemyBosState::AnimationState::AnimDeath].Load(L"Assets/animData/enemy_Bos/enBosdeath.tka");
	m_animationClips[EnemyBosState::AnimationState::AnimDeath].SetLoopFlag(false);
	//�A�j���[�V�����̏������B
	m_animation.Init(
		m_model,								//�A�j���[�V�����𗬂��X�L�����f���B
												//����ŃA�j���[�V�����ƃX�L�����f�����֘A�t�������B
		m_animationClips,						//�A�j���[�V�����N���b�v�̔z��B
		EnemyBosState::AnimationState::AnimNum//�A�j���[�V�����N���b�v�̐��B
	);
	//hp�o�[�̃X�v���C�g�B
	m_hpSprite.Init(L"Assets/sprite/hp_gauge.dds", 40.0f, 10.0f);
	m_position.y = 200.0f;
	//�L�����N�^�[�R���g���[���[�̏������B
	m_charaCon.Init(40.0f, 50.0f, m_position);
	//�G�l�~�[�̃X�e�[�g�}�V���̃X�^�[�g�֐����ĂԁB
	//�X�e�[�g�}�V���̏������B
	m_enbos_stm.Start();
	//�����蔻��̍쐬�B
	m_hit = g_hitObject->Create(
		&m_position, 150.0f,
		[&](float damage) {Damage(damage); },
		Hit::enemy
	);
	//SE
	m_se_damade.Init(L"Assets/sound/se_damage.wav");
	m_se_damade.SetVolume(1.0f);
	return true;
}

void EnemyBos::PlLen()
{
	//�G�l�~�[����v���C���[�ɐL�т�x�N�g�������߂�B
	m_moveSpeed = m_player->GetPosition() - m_position;
	//���K�����s���O�ɁA�v���C���[�܂ł̋��������߂Ă����B
	m_toPlayerLen = m_moveSpeed.Length();
	//���K���I
	m_moveSpeed.Normalize();
}

void EnemyBos::Search()
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
		m_enbos_stm.Change(EnemyBosState::MoveState::attack);
		m_enbos_stm.StateAttack()->SetAttack(60, 25);
	}
	if(m_toPlayerLen > 160.0 &&
		m_enbos_stm.StateAttack()->GetAtkFlag() == false)
	{
		m_enbos_stm.Change(EnemyBosState::MoveState::move);
		m_enbos_stm.StateAttack()->SetTiming(0);
	}
	if (m_enbos_stm.StateAttack()->GetAtkFlag() == true) {
		//�U���A�j���[�V�������I��������B
		if (m_animation.IsPlaying() == false) {
			m_enbos_stm.StateAttack()->SetAtkFlag(false);
			m_enbos_stm.StateAttack()->SetTiming(0);
		}
	}
}

//�_���[�W
void EnemyBos::Damage(float damage)
{
	m_se_damade.Play(false);
	m_enbos_stm.StateDamage()->SetDamage(true);
	//�U������������̂�HP���炭�������������
	float hp = m_status.GetHp();
		hp = (hp + m_status.GetDef()) - damage;
	if (hp <= 0.0f) {
		hp = 0.0f;
	}
	m_status.SetHp(hp);
	if (m_status.GetHp() > 0.0f) {
		m_enbos_stm.Change(EnemyBosState::MoveState::damage);
	}
}

//HP��\������X�v���C�g�̂��߂̊֌W�B
void EnemyBos::HP_Gauge()
{
	//�|�W�V�����𓪂̏�t�߂ɂ���B
	auto pos = m_position;
	pos.y += 170.0f;
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

void EnemyBos::Update()
{
	if (m_status.GetHp() <= 0.0f) {
		m_moveSpeed *= 0.0f;
		m_enbos_stm.Change(EnemyBosState::MoveState::death);
	}
	else {
		if (m_enbos_stm.StateDamage()->GetDamage() == false) {
			Search();
		}
		else {
			if (m_animation.IsPlaying() == false) {
				m_enbos_stm.Change(EnemyBosState::MoveState::move);
				m_enbos_stm.StateDamage()->SetDamage(false);
			}
		}
	}
	//�X�e�[�g�}�V���̍X�V
	m_enbos_stm.Update();
	//�d�͉����x
	m_moveSpeed.y -= 9800.0f * (1.0f / 60.0f);
	//�L�����R�����g���Ĉړ�����B
	m_position = m_charaCon.Execute(1.0f / 60.0f, m_moveSpeed);
	//�V���h�E�L���X�^�[��o�^�B
	g_graphicsEngine->GetShadowMap()->RegistShadowCaster(&m_model);
	//m_model.SetShadowReciever(true);
	//���[���h�s������߂�B
	m_model.UpdateWorldMatrix(m_position, m_rotation, { 1.0f, 1.0f, 1.0f });
	//�A�j���[�V�����𗬂��B
	m_animation.Update(1.0f / 30.0f);
}

void EnemyBos::Draw()
{
	//���f����\������B
	m_model.Draw(
		enRenderMode_Normal,
		g_camera3D.GetViewMatrix(),
		g_camera3D.GetProjectionMatrix()
	);
}

void EnemyBos::PostDraw()
{
	//HP�X�v���C�g�̕\��������B
	HP_Gauge();
}
