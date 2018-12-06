#pragma once
#include "character/CharacterController.h"
#include "EnStateMachine.h"
class Player;
class ShadowMap;
class Enemy:public IGameObject
{
public:
	Enemy();
	~Enemy();
	bool Start();
	void Update();
	void Draw();
	//void Move();
	void Search();
	void PlLen();
	void SetPosition(CVector3 pos)
	{
		m_position = pos;
	}
	void SetMoveSpeed(CVector3 ms) {
		m_moveSpeed = ms;
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
	float GetPlLen()
	{
		return toPlayerLen;
	}
	CVector3 GetMoveSpeed() {
		return m_moveSpeed;
	}
	CQuaternion GetRotation() {
		return m_rotation;
	}
	void SetShadow(ShadowMap* shadowMap) {
		m_shadowMap = shadowMap;
	}
private:
	Player* m_player = nullptr;							//�v���C���[
	//MoveState movestate = idle;
	CVector3 m_position = CVector3::Zero();				//���W�B
	CVector3 m_moveSpeed = CVector3::Zero();			//�ړ����x�B
	SkinModel m_model;									//���f���B
	CQuaternion m_rotation = CQuaternion::Identity();	//��]�B
	CharacterController m_charaCon;						//�L�����N�^�[�R���g���[���[��ǉ��B
	EnStateMachine m_ensm;
	int m_currentPointNo = 0;
	float toPlayerLen;
	bool isDead = false;
	int selectModel = 0;

	ShadowMap* m_shadowMap = nullptr;					//�V���h�E�}�b�v�B
};

