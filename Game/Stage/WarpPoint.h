#pragma once
#include "physics/PhysicsStaticObject.h"
class WarpPoint :public IGameObject
{
public:
	WarpPoint();
	~WarpPoint();
	bool Start() override;
	void Update() override;
	void Draw() override;
	void SetPosition(CVector3 pos)
	{
		m_position = pos;
	}
	void SetRotation(CQuaternion rot)
	{
		m_rotation = rot;
	}
	void ToLen(CVector3 topos)
	{
		CVector3 Len = m_position - topos;
		m_len = Len.Length();
	}
	bool GetIsWarp() {
		return m_isWarp;
	}
private:
	SkinModel m_model;
	PhysicsStaticObject m_phyStaticObject;
	CVector3 m_position = CVector3::Zero();
	CQuaternion m_rotation = CQuaternion::Identity();
	Sprite m_sprite;
	float m_len = 0.0f;
	bool m_isWarp = false;
};
