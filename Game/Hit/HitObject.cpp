#include "stdafx.h"
#include "HitObject.h"


HitObject::HitObject()
{
}


HitObject::~HitObject()
{
}

const Hit * HitObject::Create(const CVector3 * pos, float radius, std::function<void(float damage)> object, Hit::objict_Name name)
{
	auto hit = new Hit;
	hit->Create(pos, radius, object, name);
	m_controller.push_back(hit);
	return hit;
}

bool HitObject::HitTest(CVector3 pos, float radius, float damage, Hit::objict_Name name)
{
	for (auto hitter : m_controller)
	{
		if (hitter->Getobjict_Name() == name) {
			if (hitter->HitTest(pos, radius ,damage))
				return true;
		}
	}
	return false;
}

void HitObject::Delete(const Hit * object)
{
	m_controller.erase(std::find(m_controller.begin(), m_controller.end(), object));
	delete object;
}
