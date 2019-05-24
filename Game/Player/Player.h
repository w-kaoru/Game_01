#pragma once
#include "character/CharacterController.h"
#include "PlayerState.h"
#include "PlStateMachine.h"
#include "CharacterStatus.h"
#include "../graphics/font.h"

class Player :public IGameObject
{
public:
	Player();
	~Player();
	bool Start() override;
	void Update() override;
	void Draw() override;
	void PostDraw() override;

	//void Move();
	//void Jump();
	void Attack();
	void HP_Gauge();
	void Damage(float Enatk);
	//�|�W�V�����̃Z�b�^�[�B
	void SetPosition(CVector3 pos)
	{
		m_position = pos;
	}
	//�|�W�V�����̃Q�b�^�[�B
	CVector3 GetPosition()
	{
		return m_position;
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
	CQuaternion GetRotation()
	{
		return m_rotation;
	}
	//���[�e�[�V�����̃Q�b�^�[�B
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
	void EXP(int exp)
	{
		m_exp += exp;
	}
private:
	SkinModel m_model;									//�X�L�����f���B
	//SkinModel m_model_01;									//�X�L�����f���B
	Animation m_animation;								//�A�j���[�V�����B
	AnimationClip m_animationClips[PlayerState::AnimationState::AnimNum];	//�A�j���[�V�����N���b�v�B
	CVector3 m_position = CVector3::Zero();				//���W�B
	CVector3 m_respawnPosition = CVector3::Zero();		//���X�|�[�����W�B
	CVector3 m_scale = CVector3::One();					//�g�嗦�B
	CVector3 m_moveSpeed = CVector3::Zero();			//�ړ����x�B
	CQuaternion m_rotation = CQuaternion::Identity();	//��]�B
	CharacterController m_charaCon;						//�L�����N�^�[�R���g���[���[��ǉ��B
	PlStateMachine m_stMa;								//�X�e�[�g�}�V��
	CMatrix m_rotMatrix = CMatrix::Identity();
	CVector3 m_forward = CVector3::Zero();		//�O���B
	CVector3 m_attckPos = CVector3::Zero();		//�U���̏ꏊ�B
	Sprite m_hpSprite;		//hp�Q�[�W
	Sprite m_hpFrameSprite;	//hp�Q�[�W�̘g
	float m_HpScaleX = 40.0f;	//HP�̃X�v���C�g�̉���
	float m_HpScaleY = 15.0f;	//HP�̃X�v���C�g�̏c��
	const Hit* m_hit;
	CharacterStatus m_status;	//�v���C���[�̃X�e�C�^�X
	float m_maxHp = 0.0f;
	float m_hpFrame = 0.0f;		//�̗̘͂g
	int m_exp = 0;
	Font m_font;
	CSoundSource m_se;						//SE�B
	ID3D11ShaderResourceView* m_normalMapSRV = nullptr;	//�m�[�}���}�b�v��SRV
	ID3D11ShaderResourceView* m_specularMapSRV = nullptr;	//�m�[�}���}�b�v��SRV
};
