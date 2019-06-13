#include "stdafx.h"
#include "Background.h"


Background::Background()
{
	m_model.Init(L"Assets/modelData/Dungeon.cmo");
	
	m_model.UpdateWorldMatrix(m_position, m_rotation, CVector3::One());

	m_phyStaticObject.CreateMeshObject(m_model, m_position, m_rotation);
	m_model.SetShadowReciever(true);
}


Background::~Background()
{
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