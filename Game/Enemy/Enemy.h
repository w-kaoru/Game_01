#pragma once
#include "character/CharacterController.h"
#include "EnStateMachine.h"
class Player;
class BattleHit;
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
	void Attack();
	void Damage();
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
private:
	Player* m_player = nullptr;							//プレイヤー
	//MoveState movestate = idle;
	CVector3 m_position = CVector3::Zero();				//座標。
	CVector3 m_moveSpeed = CVector3::Zero();			//移動速度。
	SkinModel m_model;									//モデル。
	CQuaternion m_rotation = CQuaternion::Identity();	//回転。
	CharacterController m_charaCon;						//キャラクターコントローラーを追加。
	EnStateMachine m_ensm;
	const BattleHit* m_hit;
	int m_currentPointNo = 0;
	float toPlayerLen;
	bool isDead = false;
	int selectModel = 0;
	//エネミーのステイタス
	float m_hp = 0.0f;			//体力
	float m_atk = 0.0f;			//攻撃力
	float m_def = 0.0f;			//防御力
	float m_agi = 0.0f;			//素早さ
};
