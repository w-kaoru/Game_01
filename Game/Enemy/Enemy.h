#pragma once
#include "character/CharacterController.h"
#include "EnStateMachine.h"
#include "CharacterStatus.h"
class Player;
class BattleHit;
class Enemy:public IGameObject
{
public:
	enum EnemyType {
		type_skeleton,
		type_troll,
		type_num
	};
	Enemy();
	~Enemy();
	void Destroy() override;
	bool Start();
	void Update();
	void Draw();
	void PostDraw();
	void Search();
	void PlLen();
	void Damage(float damage);
	void DamageCut();
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
	void SetPlayer(Player* pl)
	{
		m_player = pl;
	}
	void SetAnimation(EnemyState::AnimationState state)
	{
		m_animation.Play(state, 0.2f);
	}
	void SetEnemyType(EnemyType type)
	{
		m_type = type;
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
	CharacterStatus* GetStatus()
	{
		return &m_status;
	}
	Animation GetAnimation()
	{
		return m_animation;
	}
private:
	Player* m_player = nullptr;							//プレイヤー
	//MoveState movestate = idle;
	CVector3 m_position = CVector3::Zero();				//座標。
	CVector3 m_moveSpeed = CVector3::Zero();			//移動速度。
	CVector3 m_forward = CVector3::Zero();				//前方。
	SkinModel m_model;									//モデル。
	CQuaternion m_rotation = CQuaternion::Identity();	//回転。
	CVector3 m_scale = CVector3::One();					//拡大率。
	CharacterController m_charaCon;						//キャラクターコントローラーを追加。
	Animation m_animation;								//アニメーション。
	AnimationClip m_animationClips[EnemyState::AnimationState::AnimNum];	//アニメーションクリップ。
	EnStateMachine m_ensm;								//ステートマシン。
	Sprite m_hpSprite;									//hpバー。
	Sprite m_shieldSprite;
	CVector3 m_Sprite_Front = CVector3::AxisZ()*-1;	    //テクスチャの前方向
	CQuaternion m_Sprite_angle = CQuaternion::Identity();	//テクスチャの回転角度
	const Hit* m_hit;
	CSoundSource m_se_damade;							//SE
	EnemyType m_type = type_num;
	float m_toPlayerLen;		//プレイヤーとの距離
	bool m_isDeath = false;
	float m_damageCutSpan = 6.0f;
	float m_damageCutValue = 6.0f;
	bool m_damageCut = false;
	int m_damageCutCounter = 0;
	//エネミーのステイタス
	CharacterStatus m_status;
	int m_atkAnimStart = 10;
	int m_atkHit = 0;
	CVector3 m_effectPos = CVector3::Zero();
	Effekseer::Effect* m_effect = nullptr;
	Effekseer::Handle m_playEffectHandle=-1;
};
