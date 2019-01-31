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

void LightCamera::Update()
{	
	//新しい注視点を求める。
	auto newTarget = m_player->GetPosition();
	//シャドウマップを更新。
	g_graphicsEngine->GetShadowMap()->UpdateFromLightTarget(
		{ 1000.0f, 1000.0f, 1000.0f },
		newTarget
	);
}
