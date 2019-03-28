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
	void PostDraw();
	//void Move();
	void Search();
	void PlLen();
	void Attack();
	void Damage();
	void HP_Gauge();
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

	void SetAnimation(EnemyState::MoveState state)
	{
		m_animation.Play(state, 0.2f);
	}
	void SetEnemySelect(int s)
	{
		m_selectModel = s;
	}
	float GetPlLen()
	{
		return m_toPlayerLen;
	}
	CVector3 GetMoveSpeed() {
		return m_moveSpeed;
	}
	CVector3 GetPosition() {
		return m_position;
	}
	CQuaternion GetRotation() {
		return m_rotation;
	}
	CVector3 GetForward()
	{
		return m_forward;
	}
private:
	Player* m_player = nullptr;							//プレイヤー
	//MoveState movestate = idle;
	CVector3 m_position = CVector3::Zero();				//座標。
	CVector3 m_moveSpeed = CVector3::Zero();			//移動速度。
	CVector3 m_forward = CVector3::Zero();		//前方。
	SkinModel m_model;									//モデル。
	CQuaternion m_rotation = CQuaternion::Identity();	//回転。
	CharacterController m_charaCon;						//キャラクターコントローラーを追加。
	Animation m_animation;								//アニメーション。
	AnimationClip m_animationClips[EnemyState::num];	//アニメーションクリップ。
	EnStateMachine m_ensm;								//ステートマシン。
	Sprite m_hpSprite;									//hpバー。
	CVector3 m_Sprite_Front = CVector3::AxisZ()*-1;	    //テクスチャの前方向
	CQuaternion m_Sprite_angle = CQuaternion::Identity();	//テクスチャの回転角度
	const BattleHit* m_hit;
	int m_currentPointNo = 0;
	float m_toPlayerLen;		//プレイヤーとの距離
	int m_selectModel = 0;		//モデルの選択
	int m_damageTiming = 0;		//ダメージを受けるタイミング
	int m_AttackTiming = 0;		//攻撃するタイミング
	bool attackFlag = false;
	//エネミーのステイタス
	float m_hp = 0.0f;			//体力
	float m_agi = 0.0f;			//素早さ
};
