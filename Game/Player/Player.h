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
	void Move();
	//void Jump();
	void Attack();
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
	CVector3 GetForward() 
	{
		return m_forward;
	}
private:
	SkinModel m_model;									//スキンモデル。
	Animation m_animation;								//アニメーション。
	AnimationClip m_animationClips[PlayerState::num];	//アニメーションクリップ。
	CVector3 m_position = CVector3::Zero();				//座標。
	CVector3 m_scale = CVector3::One();					//拡大率。
	CVector3 m_moveSpeed = CVector3::Zero();			//移動速度。
	CQuaternion m_rotation = CQuaternion::Identity();	//回転。
	CharacterController m_charaCon;						//キャラクターコントローラーを追加。
	PlStateMachine m_stMa;
	CMatrix m_rotMatrix = CMatrix::Identity();
	CVector3 m_up = CVector3::Zero();
	CVector3 m_forward = CVector3::Zero();		//前方。
	CVector3 m_right = CVector3::Zero();		//右。
	//BattleController* m_battle = nullptr;
	bool atk = false;
	//プレイヤーのステイタス
	//int Lv = 0;				//レベル
	float m_hp = 0.0f;			//体力
	float m_atk = 0.0f;			//攻撃力
	float m_def = 0.0f;			//防御力
	float m_agi = 0.0f;			//素早さ
};
