#pragma once
#include "character/CharacterController.h"
#include "PlayerState.h"
#include "PlStateMachine.h"
//#include "../BattleController.h"

class GameCamera;
class Player :public IGameObject
{
public:
	Player();
	~Player();
	bool Start();
	void Update();
	void Draw();
	void PostDraw();

	void Move();
	//void Jump();
	void Attack();
	void HP_Gauge();
	void Damage();
	void SetPosition(CVector3 pos)
	{
		m_position = pos;
	}
	CVector3 GetPosition()
	{
		return m_position;
	}
	void SetAnimation(PlayerState::AnimState state)
	{
		m_animation.Play(state, 0.2f);
	}
	CVector3 GetmoveSpeed()
	{
		return m_moveSpeed;
	}
	CVector3 GetForward() 
	{
		return m_forward;
	}
	CQuaternion GetRotation()
	{
		return m_rotation;
	}
	Animation GetAnimation()
	{
		return m_animation;
	}
private:
	SkinModel m_model;									//�X�L�����f���B
	Animation m_animation;								//�A�j���[�V�����B
	AnimationClip m_animationClips[PlayerState::num];	//�A�j���[�V�����N���b�v�B
	CVector3 m_position = CVector3::Zero();				//���W�B
	CVector3 m_respawnPosition = CVector3::Zero();		//���X�|�[�����W�B
	CVector3 m_scale = CVector3::One();					//�g�嗦�B
	CVector3 m_moveSpeed = CVector3::Zero();			//�ړ����x�B
	CQuaternion m_rotation = CQuaternion::Identity();	//��]�B
	CharacterController m_charaCon;						//�L�����N�^�[�R���g���[���[��ǉ��B
	PlStateMachine m_stMa;
	CMatrix m_rotMatrix = CMatrix::Identity();
	CVector3 m_up = CVector3::Zero();
	CVector3 m_forward = CVector3::Zero();		//�O���B
	CVector3 m_right = CVector3::Zero();		//�E�B
	Sprite m_hpSprite;		//hp�Q�[�W
	Sprite m_hpFrameSprite;	//hp�Q�[�W�̘g
	const BattleHit* m_hit;
	//BattleController* m_battle = nullptr;
	bool atk = false;
	int m_damageTiming = 0;		//�_���[�W���󂯂�^�C�~���O
	//�v���C���[�̃X�e�C�^�X
	//int Lv = 0;				//���x��
	float m_hp = 0.0f;			//�̗�
	float m_maxHp = 0.0f;
	float m_hpFrame = 0.0f;		//�̗̘͂g
	float m_agi = 0.0f;			//�f����
};
