#pragma once
#include "character/CharacterController.h"

class Player;
class Enemy:public IGameObject
{
public:
	Enemy();
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
	/*void SetRotation(CQuaternion rot)
	{
		m_rotation = rot;
	}
	void GetPlayer(Player* pl)
	{
		m_player = pl;
	}*/
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
	CVector3 m_position = CVector3::Zero();				//座標。
	CVector3 m_moveSpeed = CVector3::Zero();			//移動速度。
	CQuaternion m_rotation = CQuaternion::Identity();	//回転。
	SkinModel m_model;									//モデル。
	Player* m_player = nullptr;							//プレイヤー
	CharacterController m_charaCon;						//キャラクターコントローラーを追加。
	int m_currentPointNo = 0;
	float toPlayerLen;
	bool isDead = false;
};

