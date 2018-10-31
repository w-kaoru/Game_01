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
	Player* m_player = nullptr;							//プレイヤー
	MoveState movestate = idle;
	CVector3 m_position = CVector3::Zero();				//座標。
	CVector3 m_moveSpeed = CVector3::Zero();			//移動速度。
	SkinModel m_model;									//モデル。
	CQuaternion m_rotation = CQuaternion::Identity();	//回転。
	CharacterController m_charaCon;						//キャラクターコントローラーを追加。
	int m_currentPointNo = 0;
	float toPlayerLen;
	bool isDead = false;
	int selectModel = 0;
};

