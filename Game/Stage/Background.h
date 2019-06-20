#pragma once
#include "physics/PhysicsStaticObject.h"
class Background:public IGameObject
{
public:
	Background();
	~Background();
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
private:
	SkinModel m_model;
	PhysicsStaticObject m_phyStaticObject;
	CVector3 m_position = CVector3::Zero();
	CQuaternion m_rotation = CQuaternion::Identity();
};

