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
	m_LightCameraPos = { 400.0f, 400.0f, 400.0f };
	return false;
}

void LightCamera::Update()
{	
	//新しい注視点を求める。
	auto newTarget = m_player->GetPosition();
	auto pos = m_LightCameraPos + newTarget;
	//シャドウマップを更新。
	g_graphicsEngine->GetShadowMap()->UpdateFromLightTarget(
		pos,
		newTarget
	);
}
