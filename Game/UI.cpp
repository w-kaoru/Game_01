#include "stdafx.h"
#include "UI.h"
#include "Player/Player.h"

UI::UI()
{
}


UI::~UI()
{
}

void UI::Destroy()
{
}

bool UI::Start()
{
	//HPの画像の読み込み
	m_ui[UISprite::Controller].Init(L"Assets/sprite/item_controller .dds", m_spriteScaleX, m_spriteScaleY);
	m_ui[UISprite::Sword].Init(L"Assets/sprite/game_ken.dds", m_spriteScaleX, m_spriteScaleY);
	m_ui[UISprite::Shield].Init(L"Assets/sprite/shield.dds", m_spriteScaleX, m_spriteScaleY);
	m_ui[UISprite::Frame].Init(L"Assets/sprite/frame.dds", 1280.0f, 300);
	m_ui[UISprite::Frame].SetAlpha(0.5f);

	//HPの画像の読み込み
	m_ui[UISprite::HPGauge].Init(L"Assets/sprite/hp_gauge.dds", m_HpScaleX, m_HpScaleY);
	m_ui[UISprite::YellowHPGauge].Init(L"Assets/sprite/hp_yellowGauge.dds", m_HpScaleX, m_HpScaleY);
	m_ui[UISprite::RedHPGauge].Init(L"Assets/sprite/redGauge.dds", m_HpScaleX, m_HpScaleY);
	m_ui[UISprite::HPFrame].Init(L"Assets/sprite/GaugeFrame.dds", m_HpScaleX, m_HpScaleY);
	m_ui[UISprite::RedHPGauge].SetAlpha(0.0f);

	m_ui[UISprite::DCGauge].Init(L"Assets/sprite/dc_gauge.dds", m_HpScaleX, m_HpScaleY);
	m_ui[UISprite::DCFrame].Init(L"Assets/sprite/GaugeFrame.dds", m_HpScaleX, m_HpScaleY);

	m_shieldSprite.Init(L"Assets/sprite/shield.dds", 20.0f, 20.0f);

	m_yellowhp = m_spriteScale;
	m_hpFrame = m_spriteScale;
	m_hpGauge = m_spriteScale;
	m_player = g_gameObjM->FindGO<Player>("Player");
	return false;
}

void UI::Update()
{
	{
		m_spriteRot.SetRotationDeg(CVector3::AxisZ(), -5.0f);
		//スプライトの更新
		m_ui[UISprite::Controller].Update(
			{ -610.0f, 300.0f, 0.0f },
			CQuaternion::Identity(),
			{ 1.0f, 1.0f, 1.0f },
			{ 0.0f,1.0f }
		);
		//スプライトの更新
		m_ui[UISprite::Sword].Update(
			{ -630.0f, 270.0f, 0.0f },
			m_spriteRot,
			{ 0.3f, 0.3f, 1.0f },
			{ 0.0f,1.0f }
		);
		//スプライトの更新
		m_ui[UISprite::Shield].Update(
			{ -572.0f, 310.0f, 0.0f },
			CQuaternion::Identity(),
			{ 0.2f, 0.2f, 1.0f },
			{ 0.0f,1.0f }
		);
		//スプライトの更新
		m_ui[UISprite::Frame].Update(
			{ -640.0f, 370.0f ,0.0f },
			CQuaternion::Identity(),
			{ 0.5f, 0.2f, 1.0f },
			{ 0.0f,1.0f }
		);
	}
	{	//HPの処理。
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
			if (m_ui[UISprite::RedHPGauge].GetAlpha() >= 0.99f) {
				m_redGaugeDraw = false;
			}
			if (m_ui[UISprite::RedHPGauge].GetAlpha() <= 0.01f) {
				m_redGaugeDraw = true;
			}
			if (!m_redGaugeDraw) {
				m_ui[UISprite::RedHPGauge].DeltaAlpha(-0.05f);
			}
			else {
				m_ui[UISprite::RedHPGauge].DeltaAlpha(0.05f);
			}
		}
		else
		{
			m_ui[UISprite::RedHPGauge].SetAlpha(0.0f);
		}
		//スプライトの更新
		m_ui[UISprite::YellowHPGauge].Update(
			{ -500.0f, 350.0f, 0.0f },
			CQuaternion::Identity(),
			{ m_yellowhp , 1.5f, 1.0f },
			{ 0.0f,1.0f }
		);
		//スプライトの更新
		m_ui[UISprite::HPGauge].Update(
			{ -500.0f, 350.0f, 0.0f },
			CQuaternion::Identity(),
			{ m_hpGauge, 1.5f, 1.0f },
			{ 0.0f,1.0f }
		);
		//スプライトの更新
		m_ui[UISprite::RedHPGauge].Update(
			{ -500.0f, 350.0f, 0.0f },
			CQuaternion::Identity(),
			{ m_hpGauge , 1.5f, 1.0f },
			{ 0.0f,1.0f }
		);
		//スプライトの更新
		m_ui[UISprite::HPFrame].Update(
			{ -500.0f, 350.0f, 0.0f },
			CQuaternion::Identity(),
			{ m_hpFrame , 1.5f, 1.0f },
			{ 0.0f,1.0f }
		);
	}
	{	//ダメージカットの処理。

		//スプライトの更新
		m_ui[UISprite::DCGauge].Update(
			{ -470.0f, 330.0f, 0.0f },
			CQuaternion::Identity(),
			{ m_player->GetDCSpan(), 1.5f, 1.0f },
			{ 0.0f,1.0f }
		);
		//スプライトの更新
		m_ui[UISprite::DCFrame].Update(
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
}

void UI::PreDraw()
{
}

void UI::Draw()
{
}

void UI::PostDraw()
{
	{
		//スプライトを２次元で表示をする。
		m_ui[UISprite::Frame].Draw(
			g_camera2D.GetViewMatrix(),
			g_camera2D.GetProjectionMatrix()
		);
		//スプライトを２次元で表示をする。
		m_ui[UISprite::Controller].Draw(
			g_camera2D.GetViewMatrix(),
			g_camera2D.GetProjectionMatrix()
		);
		//スプライトを２次元で表示をする。
		m_ui[UISprite::Sword].Draw(
			g_camera2D.GetViewMatrix(),
			g_camera2D.GetProjectionMatrix()
		);
		m_ui[UISprite::Shield].Draw(
			g_camera2D.GetViewMatrix(),
			g_camera2D.GetProjectionMatrix()
		);
	}
	{
		//スプライトを２次元で表示をする。
		m_ui[UISprite::YellowHPGauge].Draw(
			g_camera2D.GetViewMatrix(),
			g_camera2D.GetProjectionMatrix()
		);
		//スプライトを２次元で表示をする。
		m_ui[UISprite::HPGauge].Draw(
			g_camera2D.GetViewMatrix(),
			g_camera2D.GetProjectionMatrix()
		);
		//スプライトを２次元で表示をする。
		m_ui[UISprite::RedHPGauge].Draw(
			g_camera2D.GetViewMatrix(),
			g_camera2D.GetProjectionMatrix()
		);
		//スプライトを２次元で表示をする。
		m_ui[UISprite::HPFrame].Draw(
			g_camera2D.GetViewMatrix(),
			g_camera2D.GetProjectionMatrix()
		);
	}
	{
		m_ui[UISprite::DCGauge].Draw(
			g_camera2D.GetViewMatrix(),
			g_camera2D.GetProjectionMatrix()
		);
		//スプライトを２次元で表示をする。
		m_ui[UISprite::DCFrame].Draw(
			g_camera2D.GetViewMatrix(),
			g_camera2D.GetProjectionMatrix()
		);
		//スプライトを２次元で表示をする。
		m_shieldSprite.Draw(
			g_camera2D.GetViewMatrix(),
			g_camera2D.GetProjectionMatrix()
		);
	}
	{
		m_font.BeginDraw();	//フォントの描画開始。
		if (m_player->GetStatus()->GetLv() < m_player->GetStatus()->GetMaxLv()) {
			wchar_t Seconds[256];
			swprintf_s(Seconds, L"Lv.%d", m_player->GetStatus()->GetLv());
			m_font.Draw(
				Seconds,
				{ -640.0f, 360.0f },
				{ 0.0f,1.0f,0.0f,1.0f },
				0.0f,
				1.0f,
				{ 1.0f, 1.0f }
			);
		}
		else {
			wchar_t Seconds[256];
			swprintf_s(Seconds, L"Lv.Max");
			m_font.Draw(
				Seconds,
				{ -640.0f, 360.0f },
				{ 0.0f,1.0f,0.0f,1.0f },
				0.0f,
				1.0f,
				{ 1.0f, 1.0f }
			);
		}
		m_font.EndDraw();
	}
}

