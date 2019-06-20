#include "stdafx.h"
#include "Player.h"
#include "PlStateMachine.h"
#include "../Game.h"
#include "../GameEnd.h"


Player::Player() :m_stMa(this)
{
	//cmo�t�@�C���̓ǂݍ��݁B
	m_model.Init(L"Assets/modelData/paladin.cmo", enFbxUpAxisZ);
	//tka�t�@�C���̓ǂݍ��݁B
	//�ҋ@�A�j���[�V����
	m_animationClips[PlayerState::AnimationState::AnimIdle].Load(L"Assets/animData/Player/plidle.tka");
	m_animationClips[PlayerState::AnimationState::AnimIdle].SetLoopFlag(true);

	//����A�j���[�V����
	m_animationClips[PlayerState::AnimationState::AnimRun].Load(L"Assets/animData/Player/plwalk.tka");
	m_animationClips[PlayerState::AnimationState::AnimRun].SetLoopFlag(true);

	//�U���A�j���[�V����
	m_animationClips[PlayerState::AnimationState::AnimAttack_00].Load(L"Assets/animData/Player/plattack.tka");
	m_animationClips[PlayerState::AnimationState::AnimAttack_00].SetLoopFlag(false);
	//�U���A�j���[�V����
	m_animationClips[PlayerState::AnimationState::AnimAttack_01].Load(L"Assets/animData/Player/plattack_01.tka");
	m_animationClips[PlayerState::AnimationState::AnimAttack_01].SetLoopFlag(false);
	//�_���[�W�A�j���[�V����
	m_animationClips[PlayerState::AnimationState::AnimDamage].Load(L"Assets/animData/Player/pldamage.tka");
	m_animationClips[PlayerState::AnimationState::AnimDamage].SetLoopFlag(false);

	//���ʃA�j���[�V����
	m_animationClips[PlayerState::AnimationState::AnimDeath].Load(L"Assets/animData/Player/pldeath.tka");
	m_animationClips[PlayerState::AnimationState::AnimDeath].SetLoopFlag(false);

	//�A�j���[�V�����̏������B
	m_animation.Init(
		m_model,			//�A�j���[�V�����𗬂��X�L�����f���B
							//�A�j���[�V�����ƃX�L�����f���̊֘A�t���������B
		m_animationClips,	//�A�j���[�V�����N���b�v�̔z��B
		PlayerState::AnimationState::AnimNum//�A�j���[�V�����N���b�v�̐��B
	);
	//m_scale *= 0.1f;
	//�@���}�b�v�����[�h�B
	//�t�@�C�������g���āA�e�N�X�`�������[�h���āAShaderResrouceView���쐬����B
	DirectX::CreateDDSTextureFromFileEx(
		g_graphicsEngine->GetD3DDevice(), L"Assets/sprite/Paladin_normal.dds", 0,
		D3D11_USAGE_DEFAULT, D3D11_BIND_SHADER_RESOURCE, 0, 0,
		false, nullptr, &m_normalMapSRV);
	//�X�y�L�����}�b�v�����[�h�B
	DirectX::CreateDDSTextureFromFileEx(
		g_graphicsEngine->GetD3DDevice(), L"Assets/sprite/Paladin_specular.dds", 0,
		D3D11_USAGE_DEFAULT, D3D11_BIND_SHADER_RESOURCE, 0, 0,
		false, nullptr, &m_specularMapSRV);

	m_status.SetStandardHp(60);
	m_status.SetStandardAgi(650.0f);
	m_status.SetStandardDef(1.0f);
	m_status.SetStandardAtk(12.5f);
	m_status.SetMaxLv(100);
	m_status.StatusUp();
}
Player::~Player()
{
	//�e�����O����B
	g_graphicsEngine->GetShadowMap()->UnregistShadowCaster(&m_model);
	//�L�����N�^�[�R���g���[���[����������B
	m_charaCon.RemoveRigidBoby();
	//�����蔻��������B
	g_hitObject->Delete(m_hit);
}
bool Player::Start()
{
	m_status.SetMaxHp(m_status.GetHp());
	//�m�[�}���}�b�v���Z�b�g����B
	m_model.SetNormalMap(m_normalMapSRV);
	m_model.SetSpecularMap(m_specularMapSRV);
	//�|�W�V������������ɂ��Ă����B
	m_position.y = 200.0f;
	//�����̃|�W�V������ݒ肵�ă��X�|�[���n�_�ɂ���B
	m_respawnPosition = m_position;
	//�L�����N�^�[�R���g���[���[�̏������B
	m_charaCon.Init(40.0f, 110.0f, m_position);
	m_stMa.Start();
	m_rotation.SetRotationDeg(CVector3::AxisY(), 180.0f);
	//�����蔻������B
	m_hit = g_hitObject->Create(
		&m_position, 150.0f,
		[&](float damage) {Damage(damage); },
		Hit::player
	);
	//SE
	m_se.Init(L"Assets/sound/se_damage.wav");
	m_se.SetVolume(1.0f);
	m_model.SetShadowReciever(true);
	return true;
}

//�_���[�W
void Player::Damage(float damage)
{
	//���S�̔���
	if (m_status.GetHp() > 0.0f) {
		m_se.Play(false);
		if (!m_damageCut) {
			m_stMa.StateDamage()->SetDamage(damage);
			m_stMa.Change(PlayerState::MoveState::Damage);
			//�_���[�W���󂯂��̂ŃR���{�������B
			m_stMa.StateAttack()->DamageReceiveIsInit(PlStateAttack::Combo::attack_00);
		}
		else
		{
			//�U������������̂�HP���炭�������������
			float hp = m_status.GetHp();
			float Damage = damage - m_status.GetDef();
			Damage = max(0.0f, Damage);
			hp -= (Damage / 3.0f);
			hp = min(hp, m_status.GetHp());
			hp = max(0.0f, hp);
			m_status.SetHp(hp);
		}
	}
}


void Player::Update()
{
	int lv = m_status.GetLv();
	//�O�����̎擾�B
	m_rotMatrix.MakeRotationFromQuaternion(m_rotation);
	m_forward.x = m_rotMatrix.m[2][0];
	m_forward.y = m_rotMatrix.m[2][1];
	m_forward.z = m_rotMatrix.m[2][2];
	m_forward.Normalize();
	//�o���l
	//���x���A�b�v�̏���
	int exp = lv;
	if (m_exp >= exp) {
		if (lv < m_status.GetMaxLv()) {
			m_status.SetHp(m_status.GetMaxHp());
			m_status.SetLv(lv);
			m_status.LvUp();
			m_status.StatusUp();
			m_exp = 0;
		}
	}
	//�X�e�[�g�}�V���̍X�V�B
	m_stMa.Update();
	//m_model.SetShadowReciever(true);

	//���S�̔���
	if (m_status.GetHp() <= 0.0f) {
		m_stMa.Change(PlayerState::MoveState::Death);
		m_moveSpeed *= 0.0f;
	}
	//�U�����N���������ǂ���
	if (m_stMa.StateDamage()->GetDamageFlag() == false && m_status.GetHp() > 0.0f) {
		if (g_pad[0].IsTrigger(enButtonX) && !m_stMa.StateAttack()->GetHit()) {
			m_stMa.StateAttack()->SetHit(true);
			//m_stMa.StateAttack()->SetAttack(15);
			//�U���X�e�[�g�ɕύX
			m_stMa.Change(PlayerState::MoveState::Attack);
		}
		
		if (!m_stMa.StateAttack()->GetHit()) {
			//�U�����Ă��Ȃ����Ɉړ��Ȃǂ̏����B
			m_stMa.Change(PlayerState::MoveState::Move);
		}
	}
	else {
		if (!m_animation.IsPlaying()) {
			m_stMa.Change(PlayerState::MoveState::Move);
			m_stMa.StateDamage()->SetDamageFlag(false);
		}
	}
	if (g_pad[0].IsTrigger(enButtonY) && m_damageCutSpan >= m_damageCutValue) {
		m_damageCut = true;
	}
	DamageCut();
	//�d��
	m_moveSpeed.y -= 980.0f * (1.0f / 60.0f);
	//�L�����R�����g���Ĉړ�����B
	m_position = m_charaCon.Execute(1.0f / 60.0f, m_moveSpeed);
	//���[���h�s��̍X�V�B
	m_model.UpdateWorldMatrix(m_position, m_rotation, m_scale);
	//�A�j���[�V�����𗬂��B
	m_animation.Update(1.0f / 30.0f);
	//�V���h�E�L���X�^�[��o�^�B
	g_graphicsEngine->GetShadowMap()->RegistShadowCaster(&m_model);

}

void Player::DamageCut()
{
	//�_���[�W�J�b�g�̔���
	if (m_damageCut) {
		//�Q�[�W�����X�Ɍ��炷
		m_damageCutSpan -= 1.4f * (1.0f / 60.0f);
		m_damageCutSpan = max(0.0f, m_damageCutSpan);
		if (m_damageCutSpan <= 0.0f) {
			m_damageCut = false;
		}
	}
	else {
		//�Q�[�W�����X�ɑ��₷
		m_damageCutSpan += 1.0f * (1.0f / 60.0f);
		m_damageCutSpan = min(m_damageCutValue, m_damageCutSpan);
	}

}
void Player::Draw()
{
	//���f����`��B
	m_model.Draw(
		enRenderMode_Normal,
		g_camera3D.GetViewMatrix(), 
		g_camera3D.GetProjectionMatrix()
	);
}

