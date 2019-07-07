#include "stdafx.h"
#include "Sky.h"
#include "Player/Player.h"

Sky::Sky()
{
}

Sky::~Sky()
{
}

bool Sky::Start()
{
	//スペキュラマップをロード。
	DirectX::CreateDDSTextureFromFileEx(
		g_graphicsEngine->GetD3DDevice(), L"Assets/sprite/skyCubeMap.dds", 0,
		D3D11_USAGE_DEFAULT, D3D11_BIND_SHADER_RESOURCE, 0, 0,
		false, nullptr, &m_cubeMap
	);
	m_model.Init(L"Assets/modelData/sky.cmo", enFbxUpAxisZ);
	m_model.SetCubeMap(m_cubeMap);
	return false;
}

void Sky::Update()
{
	m_model.UpdateWorldMatrix(
		{0.0f,1100.0f,0.0f},
		CQuaternion::Identity(),
		{ 100000.0f,100000.0f,100000.0f });
}

void Sky::Draw()
{
	m_model.Draw(
		enRenderMode_CubeMap,
		g_camera3D.GetViewMatrix(),
		g_camera3D.GetProjectionMatrix()
	);
}
