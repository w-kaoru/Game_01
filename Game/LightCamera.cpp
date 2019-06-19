#include "stdafx.h"
#include "LightCamera.h"
#include "Player\Player.h"
#include "graphics/SkinModel.h"


LightCamera::LightCamera()
{
}


LightCamera::~LightCamera()
{
}

bool LightCamera::Start()
{
	return false;
}

void LightCamera::Update()
{	
	m_LightCameraPos = {
		m_player->GetPosition().x + 400.0f,
		m_player->GetPosition().y + 400.0f,
		m_player->GetPosition().z + 400.0f
	};
	auto newTarget = m_player->GetPosition();
	//新しい注視点を求める。
	//シャドウマップを更新。
	g_graphicsEngine->GetShadowMap()->UpdateFromLightTarget(
		m_LightCameraPos,
		newTarget
	);
}
