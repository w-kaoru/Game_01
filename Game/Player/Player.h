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
	//�A�j���[�V�����̃X�e�[�g�̃Z�b�^�[�B
	void SetAnimation(PlayerState::AnimState state)
	{
		m_animation.Play(state, 0.2f);
	}
private:
	SkinModel m_model;									//�X�L�����f���B
	//SkinModel m_model_01;									//�X�L�����f���B
	Animation m_animation;								//�A�j���[�V�����B
	AnimationClip m_animationClips[PlayerState::num];	//�A�j���[�V�����N���b�v�B
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
	const BattleHit* m_hit;
	//BattleController* m_battle = nullptr;
	bool m_atkAnim = false;
	int m_hitTiming = 0;		//�_���[�W���󂯂�^�C�~���O
	//�v���C���[�̃X�e�C�^�X
	//int Lv = 0;				//���x��
	float m_hp = 0.0f;			//�̗�
	float m_hpFrame = 0.0f;		//�̗̘͂g
	float m_agi = 0.0f;			//�f����
	float m_HpScaleX = 100.0f;	//HP�̃X�v���C�g�̉���
	float m_HpScaleY = 30.0f;	//HP�̃X�v���C�g�̏c��
};
