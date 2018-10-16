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
	//Hands-On 1 PhysicsStaticObjectÇí«â¡ÅB
	PhysicsStaticObject m_phyStaticObject;
};

