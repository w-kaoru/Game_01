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
	m_sprite.Init(L"Assets/sprite/A.dds", 50.0f, 50.0f);
	return false;
}

void WarpPoint::Update()
{
	if (m_len < 600.0f) {
		m_isWarp = true;
	}
	else {
		m_isWarp = false;
	}
	//スプライトの更新
	m_sprite.Update(
		{ 70.0f, 90.0f, 0.0f },
		CQuaternion::Identity(),
		{ 1.0f, 1.0f, 1.0f }
	);
}

void WarpPoint::Draw()
{
	m_model.Draw(
		enRenderMode_Normal,
		g_camera3D.GetViewMatrix(),
		g_camera3D.GetProjectionMatrix()
	);
	if (m_isWarp) {
		//スプライトを２次元で表示をする。
		m_sprite.Draw(
			g_camera2D.GetViewMatrix(),
			g_camera2D.GetProjectionMatrix()
		);
	}
}