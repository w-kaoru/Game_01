#pragma once
#include "character/CharacterController.h"
#include "PlayerState.h"
#include "PlStateMachine.h"
#include "CharacterStatus.h"
#include "../graphics/font.h"

class Player :public IGameObject
{
public:
	Player();
	~Player();
	bool Start() override;
	void Update() override;
	void Draw() override;
	void PostDraw() override;

	//void Move();
	//void Jump();
	void Attack();
	void HP_Gauge();
	void Damage(float Enatk);
	//ポジションのセッター。
	void SetPosition(CVector3 pos)
	{
		m_position = pos;
	}
	//ポジションのゲッター。
	CVector3 GetPosition()
	{
		return m_position;
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
	CVector3 GetMoveSpeed() {
		return m_moveSpeed;
	}
	CQuaternion GetRotation()
	{
		return m_rotation;
	}
	//ローテーションのゲッター。
	void SetRotation(CQuaternion rot)
	{
		m_rotation = rot;
	}
	//アニメーションのステートのセッター。
	void SetAnimation(PlayerState::AnimationState state)
	{
		m_animation.Play(state, 0.2f);
	}
	void SetMoveSpeed(CVector3 ms) {
		m_moveSpeed = ms;
	}
	void EXP(int exp)
	{
		m_exp += exp;
	}
private:
	SkinModel m_model;									//スキンモデル。
	//SkinModel m_model_01;									//スキンモデル。
	Animation m_animation;								//アニメーション。
	AnimationClip m_animationClips[PlayerState::AnimationState::AnimNum];	//アニメーションクリップ。
	CVector3 m_position = CVector3::Zero();				//座標。
	CVector3 m_respawnPosition = CVector3::Zero();		//リスポーン座標。
	CVector3 m_scale = CVector3::One();					//拡大率。
	CVector3 m_moveSpeed = CVector3::Zero();			//移動速度。
	CQuaternion m_rotation = CQuaternion::Identity();	//回転。
	CharacterController m_charaCon;						//キャラクターコントローラーを追加。
	PlStateMachine m_stMa;								//ステートマシン
	CMatrix m_rotMatrix = CMatrix::Identity();
	CVector3 m_forward = CVector3::Zero();		//前方。
	CVector3 m_attckPos = CVector3::Zero();		//攻撃の場所。
	Sprite m_hpSprite;		//hpゲージ
	Sprite m_hpFrameSprite;	//hpゲージの枠
	float m_HpScaleX = 40.0f;	//HPのスプライトの横幅
	float m_HpScaleY = 15.0f;	//HPのスプライトの縦幅
	const Hit* m_hit;
	CharacterStatus m_status;	//プレイヤーのステイタス
	float m_maxHp = 0.0f;
	float m_hpFrame = 0.0f;		//体力の枠
	int m_exp = 0;
	Font m_font;
	CSoundSource m_se;						//SE。
	ID3D11ShaderResourceView* m_normalMapSRV = nullptr;	//ノーマルマップのSRV
	ID3D11ShaderResourceView* m_specularMapSRV = nullptr;	//ノーマルマップのSRV
};
