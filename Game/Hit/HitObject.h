#pragma once
#include "Hit.h"
class HitObject :public Noncopyable
{
public:
	HitObject();
	~HitObject();
	const Hit * Create(const CVector3* pos, float radius, std::function<void(float damage)> object, Hit::objict_Name name);
	bool HitTest(CVector3 pos, float radius, float damage, Hit::objict_Name name);
	void Delete(const Hit* object);
private:
	std::list<Hit*> m_controller;
};
extern HitObject* g_hitObject;