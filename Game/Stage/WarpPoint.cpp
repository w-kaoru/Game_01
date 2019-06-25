#include "stdafx.h"
#include "WarpPoint.h"


WarpPoint::WarpPoint()
{
}


WarpPoint::~WarpPoint()
{
}

bool WarpPoint::Start()
{
	m_model.Init(L"Assets/modelData/Warp_point.cmo");
	m_model.UpdateWorldMatrix(m_position, m_rotation, CVector3::One());

	m_phyStaticObject.CreateMeshObject(m_model, m_position, m_rotation);
	m_model.SetShadowReciever(true);
	m_model.SetDirectionLightColor(0, { 0.8f,0.8f,0.8f,1.0f });
	return false;
}

void WarpPoint::Update()
{
	if (m_len < 600.0f) {
		m_isWarp = true;
	}
}

void WarpPoint::Draw()
{
	m_model.Draw(
		enRenderMode_Normal,
		g_camera3D.GetViewMatrix(),
		g_camera3D.GetProjectionMatrix()
	);
}