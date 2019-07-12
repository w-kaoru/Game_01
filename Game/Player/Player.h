#pragma once
#include "character/CharacterController.h"
#include "PlayerState.h"
#include "PlStateMachine.h"
#include "CharacterStatus.h"

class Player :public IGameObject
{
public:
	Player();
	~Player();
	bool Start() override;
	void Update() override;
	void Draw() override;

	void Damage(float damage);
	void DamageCut();
	//�Q�b�^�[�B
	CVector3 GetPosition()
	{
		return m_position;
	}
	CQuaternion GetRotation()
	{
		return m_rotation;
	}
	CVector3 GetForward()
	{
		return m_forward;
	}
	CharacterStatus* GetStatus()
	{
		return &m_status;
	}
	Animation GetAnimation()
	{
		return m_animation;
	}
	CVector3 GetMoveSpeed() {
		return m_moveSpeed;
	}
	bool GetDamageCut()
	{
		return m_damageCut;
	}
	//�Z�b�^�[�B
	void SetPosition(CVector3 pos)
	{
		m_position = pos;
	}
	void SetRotation(CQuaternion rot)
	{
		m_rotation = rot;
	}
	//�A�j���[�V�����̃X�e�[�g�̃Z�b�^�[�B
	void SetAnimation(PlayerState::AnimationState state)
	{
		m_animation.Play(state, 0.2f);
	}
	void SetMoveSpeed(CVector3 ms) {
		m_moveSpeed = ms;
	}
	void EXP(float exp)
	{
		m_save->exp += exp;
	}
	void SetDamageCut(bool damage)
	{
		m_damageCut = damage;
	}


	float GetDCSpan()
	{
		return m_damageCutSpan;
	}
	float GetDCValue()
	{
		return m_damageCutValue;
	}
	CharacterController* GetCharaCon()
	{
		return &m_charaCon;
	}
private:
	SkinModel m_model;									//�X�L�����f���B
	Animation m_animation;								//�A�j���[�V�����B
	AnimationClip m_animationClips[PlayerState::AnimationState::AnimNum];	//�A�j���[�V�����N���b�v�B
	CVector3 m_position = CVector3::Zero();				//���W�B
	CVector3 m_scale = CVector3::One();					//�g�嗦�B
	CVector3 m_moveSpeed = CVector3::Zero();			//�ړ����x�B
	CQuaternion m_rotation = CQuaternion::Identity();	//��]�B
	CharacterController m_charaCon;						//�L�����N�^�[�R���g���[���[��ǉ��B
	PlStateMachine m_stMa;								//�X�e�[�g�}�V��
	CMatrix m_rotMatrix = CMatrix::Identity();
	CVector3 m_forward = CVector3::Zero();		//�O���B
	CVector3 m_attckPos = CVector3::Zero();		//�U���̏ꏊ�B
	const Hit* m_hit;
	CharacterStatus m_status;	//�v���C���[�̃X�e�C�^�X
	float m_maxHp = 0.0f;
	bool m_damageCut = false;
	float m_damageCutSpan = 6.0f;
	float m_damageCutValue = 6.0f;
	CSoundSource m_se;						//SE�B
	ID3D11ShaderResourceView* m_normalMapSRV = nullptr;	//�m�[�}���}�b�v��SRV
	ID3D11ShaderResourceView* m_specularMapSRV = nullptr;	//�m�[�}���}�b�v��SRV
	Save* m_save = nullptr;
};
