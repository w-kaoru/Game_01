#include "stdafx.h"
#include "Background.h"


Background::Background()
{
	m_model.Init(L"Assets/modelData/Dungeon.cmo");
	
	m_model.UpdateWorldMatrix(CVector3::Zero(), CQuaternion::Identity(), CVector3::One());

	//Hands-On 2 m_phyStaticObjectを作成する。
	m_phyStaticObject.CreateMeshObject(m_model, CVector3::Zero(), CQuaternion::Identity());
	m_model.SetShadowReciever(true);
}


Background::~Background()
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