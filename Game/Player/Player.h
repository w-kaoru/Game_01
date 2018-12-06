#pragma once
#include "character/CharacterController.h"
#include "PlayerState.h"
#include "PlStateMachine.h"

class GameCamera;
class ShadowMap;
class Player :public IGameObject
{
public:
	Player();
	~Player();
	bool Start();
	void Move();
	/*void Jump();
	void Attack();*/
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
	void SetShadow(ShadowMap* shadowMap) {
		m_shadowMap = shadowMap;
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
	ShadowMap* m_shadowMap = nullptr;
};
