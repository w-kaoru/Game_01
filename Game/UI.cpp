#include "stdafx.h"
#include "UI.h"
#include "Player/Player.h"

UI::UI()
{
}


UI::~UI()
{
}

bool UI::Start()
{
	//HPの画像の読み込み
	m_ui[0].Init(L"Assets/sprite/item_controller .dds", m_spriteScaleX, m_spriteScaleY);
	m_ui[1].Init(L"Assets/sprite/game_ken.dds", m_spriteScaleX, m_spriteScaleY);
	m_ui[2].Init(L"Assets/sprite/shield.dds", m_spriteScaleX, m_spriteScaleY);
	m_ui[3].Init(L"Assets/sprite/frame.dds", 1280.0f, 300);
	m_ui[3].SetAlpha(0.5f);

	//HPの画像の読み込み
	m_hp[HPSprite::HPGauge].Init(L"Assets/sprite/hp_gauge.dds", m_HpScaleX, m_HpScaleY);
	m_hp[HPSprite::YellowHPGauge].Init(L"Assets/sprite/hp_yellowGauge.dds", m_HpScaleX, m_HpScaleY);
	m_hp[HPSprite::RedHPGauge].Init(L"Assets/sprite/redGauge.dds", m_HpScaleX, m_HpScaleY);
	m_hp[HPSprite::HPFrameSprite].Init(L"Assets/sprite/GaugeFrame.dds", m_HpScaleX, m_HpScaleY);
	m_hp[HPSprite::RedHPGauge].SetAlpha(0.0f);

	m_dc[DCSprite::DamageCutSprite].Init(L"Assets/sprite/dc_gauge.dds", m_HpScaleX, m_HpScaleY);
	m_dc[DCSprite::DCFrameSprite].Init(L"Assets/sprite/GaugeFrame.dds", m_HpScaleX, m_HpScaleY);

	m_shieldSprite.Init(L"Assets/sprite/shield.dds", 20.0f, 20.0f);

	m_yellowhp = m_spriteScale;
	m_hpFrame = m_spriteScale;
	m_hpGauge = m_spriteScale;
	m_player = g_gameObjM->FindGO<Player>("Player");
	return false;
}

void UI::Update()
{
	HP_Gauge();
	DamageCut();
	SpriteUI();
}

//HPを表示するスプライトのための関係。
void UI::HP_Gauge()
{
	m_hpGauge = (m_player->GetStatus()->GetHp() / m_player->GetStatus()->GetMaxHp()) * m_spriteScale;
	if (m_hpGauge < m_yellowhp) {
		//ゲージを徐々に減らす
		m_yellowhp -= 1.5f * (1.0f / 60.0f);
	}
	else
	{
		m_yellowhp = m_hpGauge;//m_status.GetHp();
	}

	if (m_hpGauge < 4.0f) {
		if (m_hp[HPSprite::RedHPGauge].GetAlpha() >= 0.99f) {
			m_redGaugeDraw = false;
		}
		if (m_hp[HPSprite::RedHPGauge].GetAlpha() <= 0.01f) {
			m_redGaugeDraw = true;
		}
		if (!m_redGaugeDraw) {
			m_hp[HPSprite::RedHPGauge].DeltaAlpha(-0.05f);
		}
		else {
			m_hp[HPSprite::RedHPGauge].DeltaAlpha(0.05f);
		}
	}
	else
	{
		m_hp[HPSprite::RedHPGauge].SetAlpha(0.0f);
	}
	//スプライトの更新
	m_hp[HPSprite::YellowHPGauge].Update(
		{ -500.0f, 350.0f, 0.0f },
		CQuaternion::Identity(),
		{ m_yellowhp , 1.5f, 1.0f },
		{ 0.0f,1.0f }
	);
	//スプライトの更新
	m_hp[HPSprite::HPGauge].Update(
		{ -500.0f, 350.0f, 0.0f },
		CQuaternion::Identity(),
		{ m_hpGauge, 1.5f, 1.0f },
		{ 0.0f,1.0f }
	);
	//スプライトの更新
	m_hp[HPSprite::RedHPGauge].Update(
		{ -500.0f, 350.0f, 0.0f },
		CQuaternion::Identity(),
		{ m_hpGauge , 1.5f, 1.0f },
		{ 0.0f,1.0f }
	);
	//スプライトの更新
	m_hp[HPSprite::HPFrameSprite].Update(
		{ -500.0f, 350.0f, 0.0f },
		CQuaternion::Identity(),
		{ m_hpFrame , 1.5f, 1.0f },
		{ 0.0f,1.0f }
	);
}


void UI::HP_GaugeDraw()
{
	//スプライトを２次元で表示をする。
	m_hp[HPSprite::YellowHPGauge].Draw(
		g_camera2D.GetViewMatrix(),
		g_camera2D.GetProjectionMatrix()
	);
	//スプライトを２次元で表示をする。
	m_hp[HPSprite::HPGauge].Draw(
		g_camera2D.GetViewMatrix(),
		g_camera2D.GetProjectionMatrix()
	);
	//スプライトを２次元で表示をする。
	m_hp[HPSprite::RedHPGauge].Draw(
		g_camera2D.GetViewMatrix(),
		g_camera2D.GetProjectionMatrix()
	);
	//スプライトを２次元で表示をする。
	m_hp[HPSprite::HPFrameSprite].Draw(
		g_camera2D.GetViewMatrix(),
		g_camera2D.GetProjectionMatrix()
	);
}


//ダメージカット
void UI::DamageCut()
{
	 
	//スプライトの更新
	m_dc[DCSprite::DamageCutSprite].Update(
		{ -470.0f, 330.0f, 0.0f },
		CQuaternion::Identity(),
		{ m_player->GetDCSpan(), 1.5f, 1.0f },
		{ 0.0f,1.0f }
	);
	//スプライトの更新
	m_dc[DCSprite::DCFrameSprite].Update(
		{ -470.0f, 330.0f, 0.0f },
		CQuaternion::Identity(),
		{ m_player->GetDCValue(), 1.5f, 1.0f },
		{ 0.0f,1.0f }
	);
	//スプライトの更新
	m_shieldSprite.Update(
		{ -485.0f, 322.0f, 0.0f },
		CQuaternion::Identity(),
		{ 1.0f, 1.0f, 1.0f }
	);
}


void UI::DamageCutDraw()
{
	//スプライトを２次元で表示をする。
	m_dc[DCSprite::DamageCutSprite].Draw(
		g_camera2D.GetViewMatrix(),
		g_camera2D.GetProjectionMatrix()
	);
	//スプライトを２次元で表示をする。
	m_dc[DCSprite::DCFrameSprite].Draw(
		g_camera2D.GetViewMatrix(),
		g_camera2D.GetProjectionMatrix()
	);
	//スプライトを２次元で表示をする。
	m_shieldSprite.Draw(
		g_camera2D.GetViewMatrix(),
		g_camera2D.GetProjectionMatrix()
	);
}


void UI::SpriteUI()
{
	m_spriteRot.SetRotationDeg(CVector3::AxisZ(), -5.0f);
	//スプライトの更新
	m_ui[0].Update(
		{ -610.0f, 300.0f, 0.0f },
		CQuaternion::Identity(),
		{ 1.0f, 1.0f, 1.0f },
		{ 0.0f,1.0f }
	);
	//スプライトの更新
	m_ui[1].Update(
		{ -630.0f, 270.0f, 0.0f },
		m_spriteRot,
		{ 0.3f, 0.3f, 1.0f },
		{ 0.0f,1.0f }
	);
	//スプライトの更新
	m_ui[2].Update(
		{ -572.0f, 310.0f, 0.0f },
		CQuaternion::Identity(),
		{ 0.2f, 0.2f, 1.0f },
		{ 0.0f,1.0f }
	);
	//スプライトの更新
	m_ui[3].Update(
		{ -640.0f, 370.0f ,0.0f },
		CQuaternion::Identity(),
		{ 0.5f, 0.2f, 1.0f },
		{ 0.0f,1.0f }
	);
}


//スプライトの描画
void UI::SpriteUIDraw()
{
	//スプライトを２次元で表示をする。
	m_ui[3].Draw(
		g_camera2D.GetViewMatrix(),
		g_camera2D.GetProjectionMatrix()
	);
	//スプライトを２次元で表示をする。
	m_ui[0].Draw(
		g_camera2D.GetViewMatrix(),
		g_camera2D.GetProjectionMatrix()
	);
	//スプライトを２次元で表示をする。
	m_ui[1].Draw(
		g_camera2D.GetViewMatrix(),
		g_camera2D.GetProjectionMatrix()
	);
	m_ui[2].Draw(
		g_camera2D.GetViewMatrix(),
		g_camera2D.GetProjectionMatrix()
	);
}

void UI::PreDraw()
{
}

void UI::Draw()
{
}

void UI::PostDraw()
{
	SpriteUIDraw();
	HP_GaugeDraw();
	DamageCutDraw();
}

void UI::Destroy()
{
}
