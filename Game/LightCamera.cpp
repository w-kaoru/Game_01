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
	//�V���������_�����߂�B
	//�V���h�E�}�b�v���X�V�B
	g_graphicsEngine->GetShadowMap()->UpdateFromLightTarget(
		m_LightCameraPos,
		newTarget
	);
}
