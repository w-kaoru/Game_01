#pragma once
#include "physics/PhysicsStaticObject.h"
class Dungeon:public IGameObject
{
public:
	Dungeon();
	~Dungeon();
	bool Start() override;
	void Update() override;
	void Draw() override;
	void Destroy() override;
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

