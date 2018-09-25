#pragma once
#include "character/CharacterController.h"

class Player;
class Enemy
{
public:
	Enemy(CVector3 pos, CQuaternion rot, Player* player);
	~Enemy();
	void Update();
	void Draw();
	void Move();
	bool Search();
	void Idle();
	void PlLen();
	void SetPosition(CVector3 pos)
	{
		m_position = pos;
	}
	bool GetEnemydead()
	{
		return isDead;
	}

private:
	enum MoveState {
		idle,
		move
	};
	MoveState movestate = idle;
	CVector3 m_position = CVector3::Zero();				//���W�B
	CVector3 m_moveSpeed = CVector3::Zero();			//�ړ����x�B
	CQuaternion m_rotation = CQuaternion::Identity();	//��]�B
	SkinModel m_model;									//���f���B
	Player* m_player = nullptr;
	CharacterController m_charaCon;						//�L�����N�^�[�R���g���[���[��ǉ��B
	int m_currentPointNo = 0;
	float toPlayerLen;
	bool isDead = false;
};

