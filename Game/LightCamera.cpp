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
	//�V���������_�����߂�B
	auto newTarget = m_player->GetPosition();
	//�V���h�E�}�b�v���X�V�B
	g_graphicsEngine->GetShadowMap()->UpdateFromLightTarget(
		{ 1000.0f, 1000.0f, 1000.0f },
		newTarget
	);
}
