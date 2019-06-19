#include "stdafx.h"
#include "Background.h"


Background::Background()
{
}


Background::~Background()
{
}

bool Background::Start()
{
	m_model.Init(L"Assets/modelData/Dungeon.cmo");

	//m_model.Init(L"Assets/modelData/ground.cmo");
	m_model.UpdateWorldMatrix(m_position, m_rotation, CVector3::One());

	m_phyStaticObject.CreateMeshObject(m_model, m_position, m_rotation);
	m_model.SetShadowReciever(true);
	m_model.SetDirectionLightColor(0, { 0.8f,0.8f,0.8f,1.0f });
	return false;
}

void Background::Update()
{
}

void Background::Draw()
{
	m_model.Draw(
		enRenderMode_Normal,
		g_camera3D.GetViewMatrix(),
		g_camera3D.GetProjectionMatrix()
	);
}