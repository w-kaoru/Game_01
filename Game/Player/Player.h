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
	void Damage();
	void SetPosition(CVector3 pos)
	{
		m_position = pos;
	}
	CVector3 GetPosition()
	{
		return m_position;
	}
	void SetAnimation(PlayerState::AnimState state)
	{
		m_animation.Play(state, 0.2f);
	}
	CVector3 GetmoveSpeed()
	{
		return m_moveSpeed;
	}
	CVector3 GetForward() 
	{
		return m_forward;
	}
	CQuaternion GetRotation()
	{
		return m_rotation;
	}
	Animation GetAnimation()
	{
		return m_animation;
	}
private:
	SkinModel m_model;									//スキンモデル。
	Animation m_animation;								//アニメーション。
	AnimationClip m_animationClips[PlayerState::num];	//アニメーションクリップ。
	CVector3 m_position = CVector3::Zero();				//座標。
	CVector3 m_respawnPosition = CVector3::Zero();		//リスポーン座標。
	CVector3 m_scale = CVector3::One();					//拡大率。
	CVector3 m_moveSpeed = CVector3::Zero();			//移動速度。
	CQuaternion m_rotation = CQuaternion::Identity();	//回転。
	CharacterController m_charaCon;						//キャラクターコントローラーを追加。
	PlStateMachine m_stMa;
	CMatrix m_rotMatrix = CMatrix::Identity();
	CVector3 m_up = CVector3::Zero();
	CVector3 m_forward = CVector3::Zero();		//前方。
	CVector3 m_right = CVector3::Zero();		//右。
	Sprite m_hpSprite;		//hpゲージ
	Sprite m_hpFrameSprite;	//hpゲージの枠
	const BattleHit* m_hit;
	//BattleController* m_battle = nullptr;
	bool atk = false;
	int m_damageTiming = 0;		//ダメージを受けるタイミング
	//プレイヤーのステイタス
	//int Lv = 0;				//レベル
	float m_hp = 0.0f;			//体力
	float m_maxHp = 0.0f;
	float m_hpFrame = 0.0f;		//体力の枠
	float m_agi = 0.0f;			//素早さ
};
