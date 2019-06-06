#pragma once
#include "gameObject/IGameObject.h"
#include "graphics/font.h"
class Player;
class UI:public IGameObject
{
public:
	enum  UISprite
	{
		Controller,
		Sword,
		Shield,
		Frame,
		HPGauge,
		YellowHPGauge,
		RedHPGauge,
		HPFrame,
		DCGauge,
		DCFrame,
		Num
	};
	UI();
	~UI();
	bool Start() override;
	void Update() override;
	void Draw() override;
	void PreDraw() override;
	void PostDraw() override;
	void Destroy() override;
private:
	Player* m_player = nullptr;
	float m_HpScaleX = 40.0f;	//HPのスプライトの横幅
	float m_HpScaleY = 10.0f;	//HPのスプライトの縦幅
	float m_spriteScaleX = 100.0f;	//HPのスプライトの横幅
	float m_spriteScaleY = 100.0f;	//HPのスプライトの縦幅
	float m_yellowhp = 0.0f;
	float m_hpFrame = 0.0f;		//体力の枠
	float m_hpGauge = 0.0f;
	float m_spriteScale = 10.0f;
	bool m_redGaugeDraw = false;
	Sprite m_shieldSprite;
	Sprite m_ui[UISprite::Num];		//UI
	CQuaternion m_spriteRot = CQuaternion::Identity();
	Font m_font;
};

