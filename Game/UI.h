#pragma once
#include "gameObject/IGameObject.h"
class Player;
class UI:public IGameObject
{
public:
	enum HPSprite
	{
		HPGauge,
		YellowHPGauge,
		RedHPGauge,
		HPFrameSprite,
		GaugeNum
	};
	enum DCSprite
	{
		DamageCutSprite,
		DCFrameSprite,
		DCNum
	};
	UI();
	~UI();
	bool Start() override;
	void Update() override;
	void Draw() override;
	void PreDraw() override;
	void PostDraw() override;
	void Destroy() override;
	void HP_Gauge();
	void HP_GaugeDraw();
	void DamageCut();
	void DamageCutDraw();
	void SpriteUI();
	void SpriteUIDraw();
private:
	Player* m_player = nullptr;
	float m_yellowhp = 0.0f;
	float m_hpFrame = 0.0f;		//�̗̘͂g
	float m_hpGauge = 0.0f;
	float m_spriteScale = 10.0f;
	Sprite m_shieldSprite;
	float m_HpScaleX = 40.0f;	//HP�̃X�v���C�g�̉���
	float m_HpScaleY = 10.0f;	//HP�̃X�v���C�g�̏c��
	float m_spriteScaleX = 100.0f;	//HP�̃X�v���C�g�̉���
	float m_spriteScaleY = 100.0f;	//HP�̃X�v���C�g�̏c��
	Sprite m_ui[4];		//UI
	Sprite m_hp[HPSprite::GaugeNum];
	Sprite m_dc[DCSprite::DCNum];
	bool m_redGaugeDraw = false;
	CQuaternion m_spriteRot = CQuaternion::Identity();
};

