#pragma once
#include "character/CharacterController.h"

class Player;
class Enemy:public IGameObject
{
public:
	Enemy();
	~Enemy();
	bool Start();
	void Update();
	void Draw();
	void Move();
	bool Search();
	void PlLen();
	void SetPosition(CVector3 pos)
	{
		m_position = pos;
	}
	void SetRotation(CQuaternion rot)
	{
		m_rotation = rot;
	}
	void GetPlayer(Player* pl)
	{
		m_player = pl;
	}
	bool GetEnemydead()
	{
		return isDead;
	}
	void SetEnemySelect(int s)
	{
		selectModel = s;
	}
private:
	enum MoveState {
		idle,
		move
	};
	Player* m_player = nullptr;							//�v���C���[
	MoveState movestate = idle;
	CVector3 m_position = CVector3::Zero();				//���W�B
	CVector3 m_moveSpeed = CVector3::Zero();			//�ړ����x�B
	SkinModel m_model;									//���f���B
	CQuaternion m_rotation = CQuaternion::Identity();	//��]�B
	CharacterController m_charaCon;						//�L�����N�^�[�R���g���[���[��ǉ��B
	int m_currentPointNo = 0;
	float toPlayerLen;
	bool isDead = false;
	int selectModel = 0;
};

