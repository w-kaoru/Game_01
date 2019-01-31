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
		{ 700.0f, 700.0f, 700.0f },
		newTarget
	);
}
