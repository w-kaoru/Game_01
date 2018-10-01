#pragma once
#include "character/CharacterController.h"
#include "PlayerState\PlayerState.h"
#include "PlayerState\PlStateMachine.h"

class GameCamera;
class Player
{
public:
	Player();
	~Player();
	void Move();
	/*void Idle();
	void Jump();
	void Attack();*/
	void Update();
	void Draw();
	void SetPosition(CVector3 pos)
	{
		m_position = pos;
	}
	CVector3 GetPosition()
	{
		return m_position;
	}
	void SetAnimation(PlayerState::AniMove state)
	{
		m_animation.Play(state, 0.2);
	}
	CVector3 GetmoveSpeed()
	{
		return m_moveSpeed;
	}
private:
	//enum AniMove {
	//	idle,		//�ҋ@
	//	run,		//����
	//	jump,		//�W�����v
	//	attack,		//�U��
	//	num			//AniMove�̐�	
	//};
	//enum AniMove animove = idle;
	SkinModel m_model;									//�X�L�����f���B
	Animation m_animation;								//�A�j���[�V�����B
	AnimationClip m_animationClips[PlayerState::num];	//�A�j���[�V�����N���b�v�B
	CVector3 m_position = CVector3::Zero();				//���W�B
	CVector3 m_scale = CVector3::One();					//�g�嗦�B
	CVector3 m_moveSpeed = CVector3::Zero();			//�ړ����x�B
	CQuaternion m_rotation = CQuaternion::Identity();	//��]�B
	CharacterController m_charaCon;						//�L�����N�^�[�R���g���[���[��ǉ��B
	PlStateMachine m_stMa;
};
