#pragma once
#include "physics/PhysicsStaticObject.h"
class Background:public IGameObject
{
public:
	Background();
	~Background();
	void Draw();
private:
	SkinModel m_model;
	//Hands-On 1 PhysicsStaticObject��ǉ��B
	PhysicsStaticObject m_phyStaticObject;
};

